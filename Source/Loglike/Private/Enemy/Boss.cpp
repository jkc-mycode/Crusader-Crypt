// Fill out your copyright notice in the Description page of Project Settings. 


#include "Enemy/Boss.h"
#include "Enemy/BossAnimInstance.h"
#include "Enemy/MonsterAIControllerBase.h"
#include "Enemy/BossAIController.h"
#include "Enemy/MeleeMonsterAIController.h"
#include "Enemy/Projectile.h"

#include "Character/LoglikeCharacter.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

#include "Engine/DamageEvents.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

ABoss::ABoss()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARMBASE(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonRampage/Characters/Heroes/Rampage/Meshes/Rampage.Rampage'"));
	if (SK_CHARMBASE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARMBASE.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AB_BOSS(TEXT("/Script/Engine.AnimBlueprint'/Game/ParagonRampage/Characters/Heroes/Rampage/Animations/UsedAsset/AnimBP_Boss.AnimBP_Boss_C'"));
	if (AB_BOSS.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AB_BOSS.Class);
	}

	//BoxComponent
	LeftBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFistBox"));
	RightBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFistBox"));
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	LeftBox->AttachToComponent(GetMesh(), AttachmentRules, "LeftFist");
	RightBox->AttachToComponent(GetMesh(), AttachmentRules, "RightFist");
	LeftBox->SetCollisionProfileName(TEXT("OverlapAll"));
	RightBox->SetCollisionProfileName(TEXT("OverlapAll"));

	// init
	HealthPoint = 50.f;  // temp stat (400.f)
	MovementSpeed = 400.f;
	AttackSpeed = 2.f;

	//Combo Attack
	ComboNum = 0;
	//Damage
	ChargedAttackDamage = 10.f;
	JumpAttackDamage = 10.f;
	AttackDamageMin = 3.f;
	AttackDamageMax = 5.f;

	// AI
	// AIControllerClass = ABossAIController::StaticClass();
}

void ABoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//Anim
	MonsterAnim->OnMontageEnded.AddDynamic(this, &ABoss::OnMontageEnded);
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	//MeleeAttackInit
	LeftBox->OnComponentBeginOverlap.AddDynamic(this, &AMonsterBase::OnOverlapBegin);
	RightBox->OnComponentBeginOverlap.AddDynamic(this, &AMonsterBase::OnOverlapBegin);
	AttackEnd();

	// Attack Test
	// JumpAttack();
	//RangedAttack();
}

void ABoss::AttackStart()
{
	LeftBox->SetGenerateOverlapEvents(true);
	RightBox->SetGenerateOverlapEvents(true);
}

void ABoss::AttackEnd()
{
	LeftBox->SetGenerateOverlapEvents(false);
	RightBox->SetGenerateOverlapEvents(false);
}

void ABoss::Attack()
{
	Damage = FMath::RandRange(AttackDamageMin, AttackDamageMax);
	Cast<UBossAnimInstance>(MonsterAnim)->PlayComboAttackMontage(ComboNum);
	ComboNum++;
	ComboNum %= 3;
}

void ABoss::ChargedAttack()
{
	Cast<UBossAnimInstance>(MonsterAnim)->PlayChargedAttackMontage();
}

void ABoss::JumpAttack()
{
	Cast<UBossAnimInstance>(MonsterAnim)->PlayJumpAttackMontage();
	Jump();
}

void ABoss::RangedAttack()
{
	Cast<UBossAnimInstance>(MonsterAnim)->PlayRangedAttackMontage();
}

void ABoss::ChargedAttackStart(int32 FistIndex)
{
	FVector FistLoc;
	switch (FistIndex)
	{
	case 1:
		FistLoc = GetMesh()->GetSocketLocation(FName("LeftFist"));
		break;
	case 0:
		FistLoc = GetMesh()->GetSocketLocation(FName("RightFist"));
		break;
	}

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		FistLoc,
		FistLoc+ 50.f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(50.f),
		Params
	);
	
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	DrawDebugCapsule(
		GetWorld(), 
		FistLoc,
		50.f,
		50.f,
		FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
		DrawColor, 
		false, 
		3.f);
	if (bResult)
	{
		if (HitResult.GetActor()->IsValidLowLevel())
		{
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(ChargedAttackDamage, DamageEvent, GetController(), this);
		}
	}
}

void ABoss::JumpAttackStart()
{
	APawn* Target = UGameplayStatics::GetPlayerPawn(this, 0);
	FVector TargetLoc = Target->GetActorLocation();

	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(
		GetCapsuleComponent(),
		TargetLoc,
		(Target->GetActorLocation() - GetActorLocation()).Rotation(),
		true,
		true,
		0.35f,
		false,
		EMoveComponentAction::Type::Move,
		Info);
}

void ABoss::JumpAttackEnd()
{
	FVector TargetLoc = GetMesh()->GetSocketLocation(FName("LeftFist"))+GetActorRightVector()*75.f;
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		TargetLoc,
		TargetLoc + 100.f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(100.f),
		Params
	);

	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	DrawDebugCapsule(
		GetWorld(),
		TargetLoc,
		100.f,
		100.f,
		FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
		DrawColor,
		false,
		3.f);
	if (bResult)
	{
		if (HitResult.GetActor()->IsValidLowLevel())
		{
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(ChargedAttackDamage, DamageEvent, GetController(), this);
		}
	}
}

void ABoss::RangedAttackStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Boss Ranged"));

	// �ϵ��ڵ��� �� �� ��ġ�� ������ ,,
	TSubclassOf<AProjectile> Rock_0 = StaticLoadClass(AProjectile::StaticClass(), NULL, TEXT("/Script/Engine.Blueprint'/Game/ParagonRampage/Blueprints/BP_BossProjectile.BP_BossProjectile_C'"));
	TSubclassOf<AProjectile> Rock_1 = StaticLoadClass(AProjectile::StaticClass(), NULL, TEXT("/Script/Engine.Blueprint'/Game/ParagonRampage/Blueprints/BP_BossProjectile.BP_BossProjectile_C'"));
	TSubclassOf<AProjectile> Rock_2 = StaticLoadClass(AProjectile::StaticClass(), NULL, TEXT("/Script/Engine.Blueprint'/Game/ParagonRampage/Blueprints/BP_BossProjectile.BP_BossProjectile_C'"));
	TSubclassOf<AProjectile> Rock_3 = StaticLoadClass(AProjectile::StaticClass(), NULL, TEXT("/Script/Engine.Blueprint'/Game/ParagonRampage/Blueprints/BP_BossProjectile.BP_BossProjectile_C'"));
	TSubclassOf<AProjectile> Rock_4 = StaticLoadClass(AProjectile::StaticClass(), NULL, TEXT("/Script/Engine.Blueprint'/Game/ParagonRampage/Blueprints/BP_BossProjectile.BP_BossProjectile_C'"));

	TArray<TSubclassOf<AProjectile>> Rocks;

	Rocks.Emplace(Rock_0);
	Rocks.Emplace(Rock_1);
	Rocks.Emplace(Rock_2);
	Rocks.Emplace(Rock_3);
	Rocks.Emplace(Rock_4);

	if (Projectile)
	{
		const FRotator ActorRotation = GetActorRotation();
		const FVector SpawnLocation = GetActorLocation() + ActorRotation.RotateVector(FVector(0.f, 0.f, 88.f));
		const FRotator SpawnRotation = FRotator(0.f, ActorRotation.Yaw + 80.f, 0.f);
		float dist = 33.f;
		int index = 1;

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (ensure(Rocks.Num() > 0))
			{
				for (auto& Rock : Rocks)
				{
					World->SpawnActor<AProjectile>(Rock, SpawnLocation, FRotator(0.f, SpawnRotation.Yaw - (dist * index++), 0.f));
				}
				
			}
		}
	}
}

void ABoss::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//Attack test
	
	/*if (Montage->GetName().Contains("Attack"))
	{
		ChargedAttack();
	}*/
}
