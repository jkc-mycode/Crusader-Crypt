// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_RandomAttack.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UBTService_RandomAttack : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_RandomAttack();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
