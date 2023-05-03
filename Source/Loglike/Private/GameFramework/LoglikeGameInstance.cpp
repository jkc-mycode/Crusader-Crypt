// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/LoglikeGameInstance.h"
#include "UI/StageNodeWidget.h"
#include "Kismet/GameplayStatics.h"

ULoglikeGameInstance::ULoglikeGameInstance()
{
    CurrentStageType = EStageType::E_None;
}

void ULoglikeGameInstance::Init()
{
    Super::Init();
    InitSelectedArr();
}

TArray<int32> ULoglikeGameInstance::GetSelectedNodeArr()
{
    return SelectedNodeArr;
}

void ULoglikeGameInstance::AddSelectedNode(int32 NodeNum)
{
    SelectedNodeArr.AddUnique(NodeNum);
}

void ULoglikeGameInstance::InitSelectedArr()
{
    SelectedNodeArr.Empty();
}
