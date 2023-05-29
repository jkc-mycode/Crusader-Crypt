#include "UI/StageTreeWidget.h"
#include "UI/StageNodeWidget.h"
#include "GameFramework/LoglikeGameInstance.h"
#include "Kismet/GameplayStatics.h"

UStageTreeWidget::UStageTreeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//연결 입력
	TreeConnection.Emplace(0, 1);
	TreeConnection.Emplace(0, 2);

	TreeConnection.Emplace(1, 3);
	TreeConnection.Emplace(2, 4);
	TreeConnection.Emplace(2, 5);

	TreeConnection.Emplace(3, 6);
	TreeConnection.Emplace(4, 7);
	TreeConnection.Emplace(5, 8);

	TreeConnection.Emplace(6, 9);
	TreeConnection.Emplace(6, 10);
	TreeConnection.Emplace(7, 10);
	TreeConnection.Emplace(8, 10);

	TreeConnection.Emplace(9, 11);
	TreeConnection.Emplace(10, 12);
	TreeConnection.Emplace(10, 13);

	TreeConnection.Emplace(11, 14);
	TreeConnection.Emplace(12, 15);
	TreeConnection.Emplace(13, 15);

	TreeConnection.Emplace(14, 16);
	TreeConnection.Emplace(15, 16);

	NodeNum = 17;
}

void UStageTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	for (int i = 0; i < NodeNum; i++)
	{
		FString NodeName = FString::Printf(TEXT("StageNode%d"), i);
		UStageNodeWidget* Node = Cast<UStageNodeWidget>(GetWidgetFromName(FName(*NodeName)));
		if (Node != nullptr)
		{
			StageTree.Add(Node);
			NodeNumMap.Add(Node, i);
			//Node->SetNodeState(1);
			Node->ParentWidget = this;
		}
	}

	for (auto tuple : TreeConnection)
	{
		StageTree[tuple.Key]->NextStageNodes.Add(tuple.Value);
	}
	UpdateTree();
	
}

void UStageTreeWidget::UpdateTree()
{
	TArray<int32> SelectedArr = Cast<ULoglikeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetSelectedNodeArr();
	//Tree Load Update
	for (auto StageNode : StageTree)
	{
		StageNode->SetNodeState(EStageNodeState::E_Disable);
	}
	for (auto SelectNode : SelectedArr)
	{
		StageTree[SelectNode]->SetNodeState(EStageNodeState::E_Select);
	}
	if (SelectedArr.Num() == 0)
	{
		StageTree[0]->SetNodeState(EStageNodeState::E_Enable);
	}
	else
	{
		//Next Node UI Change
		for (auto j : StageTree[SelectedArr[SelectedArr.Num() - 1]]->NextStageNodes)
		{
			StageTree[j]->SetNodeState(EStageNodeState::E_Enable);
		}
	}
	
}

void UStageTreeWidget::SelectNode(UStageNodeWidget* NodeWidget)
{
	int32 Num = (int32)NodeNumMap[NodeWidget];
	
	Cast<ULoglikeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->AddSelectedNode(Num);	//Add Node
	Cast<ULoglikeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->CurrentStageType = NodeWidget->NodeStateType;
	TransformStage(NodeWidget->NodeStateType == EStageType::E_Boss);	//Transform Stage

	TransformStage();	//Transform Stage
	//TreeLoad.AddUnique(Num);

}


void UStageTreeWidget::FadeAnimationPlay(bool IsIn)
{
	if (IsIn)
	{
		SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(Fade);
	}
	else
	{
		PlayAnimation(Fade, 0.f, 1, EUMGSequencePlayMode::Reverse);
	}
		
}

void UStageTreeWidget::TransformStage(bool IsBossStage)
{
	if (IsBossStage)
	{
		UGameplayStatics::OpenLevel(this, FName("Dungeon-Boss"));
		return;
	}
	int8 RandomStageNum = FMath::RandRange(1, 4);
	FName DungeonName = FName(*FString::Printf(TEXT("Dungeon-%d"), RandomStageNum));
	UGameplayStatics::OpenLevel(this, DungeonName);
	

void UStageTreeWidget::TransformStage()
{
	int8 RandomStageNum = FMath::RandRange(1, 4);
	//FName DungeonName = FName(*FString::Printf(TEXT("Dungeon-%d"), RandomStageNum));
	//UGameplayStatics::OpenLevel(this, DungeonName);
	UGameplayStatics::OpenLevel(this, FName("MonsterTestLevel"));	//TestCode

}
