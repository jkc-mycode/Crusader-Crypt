// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossAIController.h"
#include "Enemy/Boss.h"
#include "Character/LoglikeCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ABossAIController::CheckEntranceKey(TEXT("CheckEntrance"));
const FName ABossAIController::CheckHPKey(TEXT("CheckHP"));
const FName ABossAIController::SkilNumKey(TEXT("SkilNum"));

ABossAIController::ABossAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Script/AIModule.BlackboardData'/Game/ParagonRampage/AI/BB_Boss.BB_Boss'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Script/AIModule.BehaviorTree'/Game/ParagonRampage/AI/BT_Boss.BT_Boss'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ABoss* Boss = Cast<ABoss>(InPawn);
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	ALoglikeCharacter* Player = Cast<ALoglikeCharacter>(this);

	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		Blackboard->SetValueAsBool(IsDeadKey, false);
		Blackboard->SetValueAsBool(IsStunKey, false);
		Blackboard->SetValueAsBool(CheckEntranceKey, true);

		Blackboard->SetValueAsInt(CheckHPKey, Boss->GetHitPoint());
		Blackboard->SetValueAsInt(SkilNumKey, 0);

		Blackboard->SetValueAsFloat(AttackTimeKey, Boss->GetAttackSpeed());

		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		
		

		
		if (Player && Player->GetController()->IsPlayerController())
		{
			Blackboard->SetValueAsObject(TargetKey, Player);
		}

		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s BehaviorTree is not Run"), *(GetOwner()->GetName()));
		}
	}
}

void ABossAIController::PlayedStartEvent(bool Entrance)
{
	Blackboard->SetValueAsBool(CheckEntranceKey, Entrance);
}


void ABossAIController::SetHitPoint(int HitPoint)
{
	Blackboard->SetValueAsInt(CheckHPKey, HitPoint);
}

void ABossAIController::SetSkillPattern(int SkilNum)
{
	Blackboard->SetValueAsInt(SkilNumKey, SkilNum);
}