// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageTreeWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UStageTreeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UStageTreeWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	void UpdateTree();
	void SelectNode(class UStageNodeWidget* NodeWidget);
	void FadeAnimationPlay(bool IsIn);
	void TransformStage(bool IsBossStage);

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "NextStage")
	TArray<class UStageNodeWidget*> StageTree;
	TArray<TTuple<int32, int32>> TreeConnection;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "NextStage")
	TMap<class UStageNodeWidget*, int32> NodeNumMap;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* Fade;
	int32 NodeNum = 7;
};
