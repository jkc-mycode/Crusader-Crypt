// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/LoglikeCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/ABAnimInstance.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/WidgetComponent.h"
#include "Character/ABCharacterWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Character/ABWeapon.h"
#include "Particles/ParticleSystem.h"
#include "Props/Door.h"
#include "Character/ABCharacterStatComponent.h"
#include "GameFramework/LoglikeGameInstance.h"
#include "Character/ABPlayerState.h"


//////////////////////////////////////////////////////////////////////////
// ALoglikeCharacter

ALoglikeCharacter::ALoglikeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	//CameraBoom->bDoCollisionTest = false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	//공격중인지 판별하는 변수
	IsAttacking = false;
	//공격 최대 콤보 수
	MaxCombo = 3;
	//공격 콤보 초기화
	AttackEndComboState();
	//콜리전 채널 등 기본 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("LoglikeCharacter"));
	
	//캐릭터 공격 콜리전 범위(양옆)
	AttackRadius = 50.0f;
	//캐릭터 공격 콜리전 범위(앞뒤)
	AttackRange = 100.0f;

	//캐릭터 스텟 클래스 객체 설정
	CharacterStat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CHARACTERSTAT"));

	//캐릭터 죽음 유무
	IsDead = false;

	// Tick 처리 가능 + 시작하자마자 Tick 처리를 원할 때
	PrimaryActorTick.bCanEverTick = true;

	//사용할 HP Bar HUD 클래스 객체 생성
	static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarObj(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_HUD.UI_HUD_C'"));
	HUDWidgetClass = HealthBarObj.Class;

	//맞았을 때 파티클 효과 가져오기
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitImpactParticle(TEXT("/Script/Engine.ParticleSystem'/Game/UndeadPack/Blood/FX/P_HitImpact_2.P_HitImpact_2'"));
	if (HitImpactParticle.Succeeded())
		HitImpactP = HitImpactParticle.Object;

	AssetIndex = 4;
	//SetActorHiddenInGame(true);

}


void ALoglikeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	auto CurWeapon = GetWorld()->SpawnActor<AABWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	SetWeapon(CurWeapon);

	//캐릭터의 상태 변경
	//SetCharacterState(ECharacterState::LOADING);

	//생성한 위젯 클래스 객체 확인
	if (HUDWidgetClass != nullptr)
	{
		//위젯 객체 생성
		CurrentWidget = CreateWidget<UABCharacterWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}


//////////////////////////////////////////////////////////////////////////////
// Input

//캐릭터 기본 입력을 관리하는 함수
void ALoglikeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALoglikeCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALoglikeCharacter::Look);

		EnhancedInputComponent->BindAction(InteractDoorActon, ETriggerEvent::Triggered, this, &ALoglikeCharacter::DetectDoor);
	}

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ALoglikeCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Dodge"), EInputEvent::IE_Pressed, this, &ALoglikeCharacter::Dodge);
	PlayerInputComponent->BindAction(TEXT("Parrying"), EInputEvent::IE_Pressed, this, &ALoglikeCharacter::Parrying);
	//PlayerInputComponent->BindAction(TEXT("Inventory"), EInputEvent::IE_Pressed, this, &ALoglikeCharacter::Inventory);
	//PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &ALoglikeCharacter::Interact);
}

//void ALoglikeCharacter::Inventory()
//{
//	UE_LOG(LogTemp, Warning, TEXT("sdfsdfsdfsdf"));
//}
//void ALoglikeCharacter::Interact()
//{
//
//}

//캐릭터의 이동 함수
void ALoglikeCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && !IsAttacking && !IsParrying)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

//캐릭터가 보는 방향 체크하는 함수
void ALoglikeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


//컴포넌트 초기화
void ALoglikeCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());

	ABAnim->OnMontageEnded.AddDynamic(this, &ALoglikeCharacter::OnAttackMontageEnded);
	ABAnim->OnMontageEnded.AddDynamic(this, &ALoglikeCharacter::OnDodgeMontageEnded);
	ABAnim->OnMontageEnded.AddDynamic(this, &ALoglikeCharacter::OnParryingMontageEnded);

	ABAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		CanNextCombo = false;
		if (IsComboInputOn)
		{
			AttackStartComboState();
			ABAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});
	ABAnim->OnAttackHitCheck.AddUObject(this, &ALoglikeCharacter::AttackCheck);

	//캐릭터 현재 체력 세팅(스텟 컴포넌트에서 가져옴)
	CurrentHealth = CharacterStat->GetMaxHealth();
	//캐릭터 공격력 세팅(스텟 컴포넌트에서 가져옴)
	CharacterDamage = CharacterStat->GetDamage();
	//캐릭터 행운 세팅(스텟 컴포넌트에서 가져옴)
	CharacterLuck = CharacterStat->GetLuck();
	//캐릭터 토큰 세팅(스텟 컴포넌트에서 가져옴)
	CharacterToken = CharacterStat->GetToken();
}

//캐릭터 스테이스 상태 설정 함수
void ALoglikeCharacter::SetCharacterState(ECharacterState NewState)
{
	CurrentState = NewState;
	switch (CurrentState)
	{
		case ECharacterState::LOADING: 
		{
			break;
		}
		case ECharacterState::READY:
		{
			break;
		}
		case ECharacterState::DEAD:
		{
			break;
		}
	}
}

//캐릭터 상태 반환 함수
ECharacterState ALoglikeCharacter::GetCharacterState() const
{
	return ECharacterState();
}

//공격 애님 실행 함수
void ALoglikeCharacter::Attack()
{
	if (IsDodge) return;
	if (IsAttacking)
	{
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		AttackStartComboState();
		ABAnim->PlayAttackMontage();
		ABAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

//공격 몽타주 끝 체크 함수
void ALoglikeCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	AttackEndComboState();
}

//콤보 공격 시작 상태 체크 함수
void ALoglikeCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

//콤보 공격 끝 상태 체크 함수
void ALoglikeCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

//닷지 애님 실행 함수
void ALoglikeCharacter::Dodge()
{
	if (!IsDodge && !DodgeDelay) {
		ABAnim->PlayDodgeMontage();
		IsDodge = true;
		DodgeDelay = true;
		GetWorld()->GetTimerManager().SetTimer(Dodge_Timer, this, &ALoglikeCharacter::Delay1, 2.0f, false);
	}
}

//닷지용 딜레이 함수
void ALoglikeCharacter::Delay1()
{
	DodgeDelay = false;
}

//닷지 몽타주 끝 체크 함수
void ALoglikeCharacter::OnDodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsDodge = false;
}

//패링 애님 실행 함수
void ALoglikeCharacter::Parrying()
{
	if (!IsParrying && !ParryingDelay) {
		ABAnim->PlayParryingMontage();
		IsParrying = true;
		ParryingDelay = true;
		GetWorld()->GetTimerManager().SetTimer(Parrying_Timer, this, &ALoglikeCharacter::Delay2, 2.0f, false); // ������
	}
}

//패링용 딜레이 함수
void ALoglikeCharacter::Delay2()
{
	ParryingDelay = false;
}

void ALoglikeCharacter::DetectDoor()
{
	FVector StartLoc = GetActorLocation();
	FVector EndLoc = StartLoc + GetActorForwardVector() * 1000.f;
	FHitResult HitResult;
	TArray<AActor*> IgnoreActors;
	bool Result = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		StartLoc,
		EndLoc,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		HitResult,
		true
		, FLinearColor::Red
		, FLinearColor::Blue
		, 5.0f
	);
	if (Result)
	{
		ADoor* Gate = Cast<ADoor>(HitResult.GetActor());
		if (::IsValid(Gate))
		{
			Gate->ShowNextStageWidget();
		}
	}
}

bool ALoglikeCharacter::GetIsParrying()
{
	return IsParrying;
}

//패링 몽타주 끝 체크 함수
void ALoglikeCharacter::OnParryingMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsParrying = false;
}

//공격 콜리전을 켜서 공격했는지 체크하는 함수
void ALoglikeCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 0.5f;

	//콜리전이 눈에 보이도록 켜주는 함수
	DrawDebugCapsule
	(
		GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);

#endif
	//콜리전에 들어갔는지 체크
	if (bResult)
	{
		if (HitResult.GetActor()->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor Name : %s"), *HitResult.GetActor()->GetName());
			//데미지를 캐릭터에 적용시키는 함수 실행(기본 함수)
			UGameplayStatics::ApplyDamage(HitResult.GetActor(), CharacterDamage, GetController(), this, UDamageType::StaticClass());
		}
	}
}

//받는 데미지 처리 함수
float ALoglikeCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//최종 계산된 데미지를 담을 변수
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);
	
	//구르기 중일 때는 피해x
	if(!IsDodge || !IsParrying)
		CurrentHealth -= FinalDamage;

	//델리게이트에 바인딩된 함수를 호출
	//OnHPChanged.Broadcast();

	//blood particle 생성
	if (HitImpactP != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitImpactP, GetActorLocation());
	}

	//나중에 지울 것!!!
	//============================================
	CharacterStat->SetAddDamage(2.0);
	CharacterDamage = CharacterStat->GetDamage();

	CharacterStat->SetAddLuck(0.3);
	CharacterLuck = CharacterStat->GetLuck();

	CharacterStat->SetToken(2);
	CharacterToken = CharacterStat->GetToken();
	//============================================

	UE_LOG(LogTemp, Warning, TEXT("Character Current Health : %f "), CurrentHealth);
	if (CurrentHealth <= 0.0f)
	{
		ABAnim->SetDeadAnim();//죽는 애니메이션 실행하도록 함수
		SetActorEnableCollision(false); //죽을 때는 콜리전 해제
		IsDead = true;
	}
	return FinalDamage;
}

////HUD에서의 HP Bar 비율을 계산하여 반환하는 함수
//float ALoglikeCharacter::GetHPRatio()
//{
//	return (CurrentHealth < KINDA_SMALL_NUMBER ? 0.0f : (CurrentHealth / CharacterStat->GetMaxHealth()));
//}

//무기 장착이 가능한지를 반환하는 함수
bool ALoglikeCharacter::CanSetWeapon()
{
	return (nullptr == CurrentWeapon);
}
//무기 장착 함수
void ALoglikeCharacter::SetWeapon(AABWeapon* NewWeapon)
{
	//무기 소켓 장착
	FName WeaponSocket(TEXT("RightHand_Weapon"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		CurrentWeapon = NewWeapon;
	}
}

//블루프린트에서 현재 체력을 가져갈 때 사용할 함수(비율을 반환)
float ALoglikeCharacter::GetHealth()
{
	return (CurrentHealth < KINDA_SMALL_NUMBER ? 0.0f : (CurrentHealth / CharacterStat->GetMaxHealth()));
}

//블루프린트에서 캐릭터의 공격력을 가져갈 때 사용할 함수
float ALoglikeCharacter::GetCharacterDamage()
{
	return CharacterDamage;
}

float ALoglikeCharacter::GetCharacterLuck()
{
	return CharacterLuck;
}

int ALoglikeCharacter::GetCharacterToken()
{
	return CharacterToken;
}
