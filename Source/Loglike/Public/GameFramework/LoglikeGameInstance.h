// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LoglikeGameInstance.generated.h"

/**
 * 
 */
enum class EStageType : uint8;

UCLASS()
class LOGLIKE_API ULoglikeGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	ULoglikeGameInstance();
	virtual void Init() override;

	TArray<int32> GetSelectedNodeArr();
	void AddSelectedNode(int32 NodeNum);
	void InitSelectedArr();
public:
	EStageType CurrentStageType;
private:
	TArray<int32> SelectedNodeArr;
	
};
