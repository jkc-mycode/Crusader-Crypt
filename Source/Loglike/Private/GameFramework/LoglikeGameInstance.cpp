// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/LoglikeGameInstance.h"
#include "UI/StageNodeWidget.h"
#include "Kismet/GameplayStatics.h"

ULoglikeGameInstance::ULoglikeGameInstance()
{
    CurrentStageType = EStageType::E_None;

    FString CharacterDataPath = TEXT("/Script/Engine.DataTable'/Game/Source/CharacterData/ABCharacterData123.ABCharacterData123'");
    static ConstructorHelpers::FObjectFinder<UDataTable> DT_ABCHARACTER(*CharacterDataPath);
    ABCharacterTable = DT_ABCHARACTER.Object;
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

FABCharacterData* ULoglikeGameInstance::GetABCharacterData(int32 Name)
{
    return ABCharacterTable->FindRow<FABCharacterData>(*FString::FromInt(Name), TEXT(""));
}

