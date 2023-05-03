// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StageNodeWidget.h"
#include "UI/StageTreeWidget.h"
#include "Components/Image.h" 
#include "Components/Button.h"
#include "Math/Color.h"

UStageNodeWidget::UStageNodeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UStageNodeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StageImage = Cast<UImage>(GetWidgetFromName(TEXT("StageImg")));
	StageBorder = Cast<UImage>(GetWidgetFromName(TEXT("border")));
	StageBtn = Cast<UButton>(GetWidgetFromName(TEXT("Btn")));

	SetStageImage();
	StageBtn->OnClicked.AddDynamic(this, &UStageNodeWidget::NodeButtonCallback);
}


void UStageNodeWidget::SetStageImage()
{
	uint8 TextureNum = (uint8)NodeStateType;
	StageImage->SetBrushFromTexture(NodeImageArr[TextureNum], true);
	//if (Image == nullptr) return;
	//StageImage->SetBrushFromTexture(Image, true);
}

void UStageNodeWidget::SetNodeState(EStageNodeState CurrentState)
{
	//CurrentState = (EStageNodeState)StateNum;
	FLinearColor StageImageColor(1.f,1.f,1.f,1.f);
	FLinearColor StageBorderColor(1.f, 1.f, 1.f, 1.f);
	bool EnableBtn = false;
	switch (CurrentState)
	{
	case EStageNodeState::E_Enable:
		EnableBtn = true;
		break;
	case EStageNodeState::E_Disable:
		StageImageColor.A = 0.15f;
		StageBorderColor.A = 0.15f;
		break;
	case EStageNodeState::E_Select:
		StageBorderColor.B = 0.f;	//Selected Visual
		break;
	}

	StageBorder->SetColorAndOpacity(StageBorderColor);
	StageImage->SetColorAndOpacity(StageImageColor); 
	StageBtn->SetIsEnabled(EnableBtn);
}

void UStageNodeWidget::NodeButtonCallback()
{
	if(ParentWidget == nullptr) return;
	ParentWidget->SelectNode(this);

	ParentWidget->UpdateTree();
	UE_LOG(LogClass, Warning, TEXT("SelectNode: %d"), ParentWidget->TreeLoad.Num());
}
