// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MonsterBase.h"
#include "Enemy/MonsterAnimBase.h"
#include "Enemy/MonsterAIControllerBase.h"
#include "Enemy/MeleeMonsterBase.h"
#include "Enemy/Boss.h"

#include "Character/LoglikeCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DungeonGameMode.h"

#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
AMonsterBase::AMonsterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//particle
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitImpactParticle(TEXT("/Script/Engine.ParticleSystem'/Game/UndeadPack/Blood/FX/P_HitImpact.P_HitImpact'"));
	if (HitImpactParticle.Succeeded())
	{
		HitImpactP = HitImpactParticle.Object;
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> SparkNiagara(TEXT("/Script/Niagara.NiagaraSystem'/Game/UndeadPack/Blood/SparkFX/NE_Sparkle_System.NE_Sparkle_System'"));
	if (SparkNiagara.Succeeded())
	{
		SparkN = SparkNiagara.Object;
	}
	
	//체력
	HealthPoint = 1.f;
	//Mesh불투명도 관련 변수
	MatOpacity = 1.f;
	IsDead = false;
	//AI
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//RVO Avoidance
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 100.f;
}

void AMonsterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;

	MonsterAnim = Cast<UMonsterAnimBase>(GetMesh()->GetAnimInstance());
	//Opacity 변경할 Meshes
	OpacitySkeletalMesh.Add(GetMesh());
}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsDead && (MatOpacity > 0.f))
	{
		ChangeOpacity(DeltaTime);
	}
}

void AMonsterBase::Attack()
{
	MonsterAnim->PlayAttackMontage();
}

float AMonsterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	HealthPoint -= FinalDamage;
	//데미지가 0보다 작아지면 죽음
	if (HealthPoint <= 0.f)
	{
		Dead();
	}

	//blood particle 생성
	if (HitImpactP != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitImpactP, GetActorLocation());
	}
	UE_LOG(LogTemp, Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);
	Cast<AMonsterAIControllerBase>(GetController())->SetPain(true, 10.f);
	MonsterAnim->PlayPainMontage();
	
	return FinalDamage;
}

void AMonsterBase::Dead()
{
	//AI,Animation 죽음처리& Collision 비활성화
	if (Cast<ADungeonGameMode>(UGameplayStatics::GetGameMode(GetWorld())) != nullptr)
	{
		Cast<ADungeonGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->UpdateMonsterNum();
	}
	
	Cast<AMonsterAIControllerBase>(GetController())->SetDead();
	MonsterAnim->SetDead();
	SetActorEnableCollision(false);
	IsDead = true;
}

void AMonsterBase::ChangeOpacity(float DeltaTime)
{
	//Opacity Change
	MatOpacity -= DeltaTime / (3.f);
	MatOpacity = FMath::Clamp(MatOpacity, 0.0f, 1.0f);
	for (auto SKMesh : OpacitySkeletalMesh)
	{
		SKMesh->SetScalarParameterValueOnMaterials(FName("OpacityParam"), MatOpacity);
	}

	//if opacity end -> Destory
	if (MatOpacity == 0.f)
	{
		Destroy();
	}

}

void AMonsterBase::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//LoglikeCharacter면 TakeDamage
	if (OtherActor && Cast<ALoglikeCharacter>(OtherActor) && OtherComp)
	{
		ALoglikeCharacter* LoglikeCharacter = Cast<ALoglikeCharacter>(OtherActor);
		if (LoglikeCharacter->GetIsParrying())
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SparkN, GetActorLocation(), GetActorRotation());
			if (Cast<AMeleeMonsterBase>(this))
			{
				Cast<AMeleeMonsterBase>(this)->Stun();
			}
			if (Cast<ABoss>(this))
			{
				Cast<ABoss>(this)->Stun();
			}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("%s: Hit %s"),*GetName(), *(LoglikeCharacter->GetName()));
			FDamageEvent DamageEvent;
			LoglikeCharacter->TakeDamage(Damage, DamageEvent, GetController(), this);
			AttackEnd();
		}
		
	}
}
