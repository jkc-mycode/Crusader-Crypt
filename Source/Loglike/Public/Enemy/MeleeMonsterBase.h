// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBase.h"
#include "MeleeMonsterBase.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API AMeleeMonsterBase : public AMonsterBase
{
	GENERATED_BODY()
public:
	AMeleeMonsterBase();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	void ComboAttack();
	void Stun();
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
private:
	int32 ComboNum;
};
