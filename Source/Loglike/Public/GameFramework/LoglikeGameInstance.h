// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "LoglikeGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FABCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FABCharacterData() : Name(1), DefaultHealth(100.0f), DefaultDamage(7.0f), DefaultLuck(0.0f), DefaultAttackSpeed(0.0f), Token(0), WeaponKey(false), ArtiffectKey(false) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float DefaultHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float DefaultDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float DefaultLuck;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float DefaultAttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Token;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		bool WeaponKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		bool ArtiffectKey;
};
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

	FABCharacterData* GetABCharacterData(int32 Name);
public:
	EStageType CurrentStageType;
private:
	TArray<int32> SelectedNodeArr;
	
	UPROPERTY()
	class UDataTable* ABCharacterTable;
};
