// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MeleeMonsterBase.h"
#include "Enemy/MeleeAnimInstance.h"
#include "Enemy/MeleeMonsterAIController.h"
AMeleeMonsterBase::AMeleeMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	ComboNum = 0;
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
	Cast<AMeleeMonsterAIController>(GetController())->SetStun(true, 10.f);
	Cast<UMeleeAnimInstance>(MonsterAnim)->PlayComboAttackMontage(ComboNum);
	ComboNum++;
	ComboNum %= 2;
}

void AMeleeMonsterBase::Stun()
{
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
	//중간에 중단되지 않고 몽타주가 끝났다면 stun상태를 품
	if (!bInterrupted)
	{
		Cast<AMeleeMonsterAIController>(GetController())->SetStun(false, 0.f);
	}
	if (Montage->GetName().Contains("Pain"))  // Pain Montage 후 BT실행가능
	{
		Cast<AMeleeMonsterAIController>(GetController())->SetPain(false, 0.f);
	}
	else if (Montage->GetName().Contains("Attack"))// Attack Montage 후 BT실행가능
	{
		Cast<AMeleeMonsterAIController>(GetController())->SetStun(false, 0.f);
	}
	AttackEnd();
}
