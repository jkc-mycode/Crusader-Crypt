// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/MonsterAIControllerBase.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API ABossAIController : public AMonsterAIControllerBase
{
	GENERATED_BODY()
	
public:
	ABossAIController();
	virtual void OnPossess(APawn* InPawn) override;

	void PlayedStartEvent(bool Entrance);
	void SetHitPoint(int HitPoint);
	void SetSkillPattern(int SkilNum);

public:
	static const FName CheckEntranceKey;
	static const FName CheckHPKey;
	static const FName SkilNumKey;
};
