// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MeleeMonsterBase.h"
#include "Enemy/MeleeAnimInstance.h"
#include "Enemy/MeleeMonsterAIController.h"

#include "Components/CapsuleComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AMeleeMonsterBase::AMeleeMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	ComboNum = 0;
	IsAttacking = false;
}

void AMeleeMonsterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MonsterAnim->OnMontageEnded.AddDynamic(this, &AMeleeMonsterBase::OnMontageEnded);
}
void  AMeleeMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AMeleeMonsterBase::ComboAttack()
{
	//Cast<AMeleeMonsterAIController>(GetController())->SetStun(true, 10.f);
	IsAttacking = true;
	Cast<UMeleeAnimInstance>(MonsterAnim)->PlayComboAttackMontage(ComboNum);
	ComboNum++;
	ComboNum %= 2;
}
void AMeleeMonsterBase::PushBack(AActor* PivotActor)
{
	FVector DamageDirection = PivotActor->GetActorLocation() - GetActorLocation();
	DamageDirection.Normalize();
	DamageDirection.Z = 0.f;
	DamageDirection *= -1;

	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	float PushBackSize = 100.f;

	FVector StartLoc = GetActorLocation() + DamageDirection * 50.f;
	FVector EndLoc = StartLoc + DamageDirection * PushBackSize;
	FVector KnockbackPos = EndLoc;
	bool IsExist = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		StartLoc,
		EndLoc,
		50.f,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
		, FLinearColor::Red
		, FLinearColor::Green
		, 3.0f
	);

	if (IsExist)
	{
		KnockbackPos = HitResult.ImpactPoint;
		KnockbackPos.Z = StartLoc.Z;
		FVector ErrorDistance = GetActorLocation() - KnockbackPos;
		ErrorDistance.Normalize();
		KnockbackPos += ErrorDistance * 25.f;
	}
	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), KnockbackPos, GetActorRotation(), false, false, 0.5f, false, EMoveComponentAction::Type::Move, Info);
}
void AMeleeMonsterBase::Stun()
{
	PushBack(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Cast<AMeleeMonsterAIController>(GetController())->SetStun(true, 10.f);
	Cast<UMeleeAnimInstance>(MonsterAnim)->PlayStunMontage();
}

void AMeleeMonsterBase::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//어택 이외의 몽타주가 끝나면 Combo를 초기화

	if (!Montage->GetName().Contains("Attack"))
	{
		ComboNum = 0;
	}

	if (Montage->GetName().Contains("Pain"))  // Pain Montage 후 BT실행가능
	{
		Cast<AMeleeMonsterAIController>(GetController())->SetPain(false, 0.f);
	}
	else if (Montage->GetName().Contains("Attack") )// Attack Montage 후 BT실행가능
	{
		IsAttacking = false;
	}
	else if (Montage->GetName().Contains("Stun"))
	{
		Cast<AMeleeMonsterAIController>(GetController())->SetStun(false, 0.f);
	}
	AttackEnd();
}
