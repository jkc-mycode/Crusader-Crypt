// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTService_RandomAttack.h"
#include "Enemy/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_RandomAttack::UBTService_RandomAttack()
{
	NodeName = TEXT("Set Skill Number");
	Interval = 1.0f;
	bNotifyBecomeRelevant = true;

}

void UBTService_RandomAttack::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);\

	UE_LOG(LogTemp, Warning, TEXT("hi"));
	
	int SkillNum = 0;
	
	SkillNum = FMath::RandRange(1, 3);
	UE_LOG(LogTemp, Warning, TEXT("Skill Num: %d"), SkillNum);
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(ABossAIController::SkilNumKey, SkillNum);

	return;
}

void UBTService_RandomAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

}

