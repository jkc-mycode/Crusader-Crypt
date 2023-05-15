// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoglikeGameMode.h"
#include "DungeonGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStageClearDelegate);
/**
 * 
 */
UCLASS()
class LOGLIKE_API ADungeonGameMode : public ALoglikeGameMode
{
	GENERATED_BODY()
public:
	ADungeonGameMode();
	virtual void BeginPlay() override;
	void EnableNextStageWidget();

	void UpdateMonsterNum();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GateUI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UStageTreeWidget> NextStageWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GateUI", meta = (AllowPrivateAccess = "true"))
	class UStageTreeWidget* NextStageWidgetObject;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterGen", meta = (AllowPrivateAccess = "true"))
	TArray<class AMonsterGenerator*> MonsterGenerator;
	TArray<TTuple<int8, int8>> MonsterGroupPairs;
	TTuple<int8, int8> MonsterGroupPair;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterGen", meta = (AllowPrivateAccess = "true"))
	int32 MonsterNum;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterGen", meta = (AllowPrivateAccess = "true"))
	int32 Wave;
public:
	FOnStageClearDelegate OnStageClear;
};
