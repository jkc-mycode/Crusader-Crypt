// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ABSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UABSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UABSaveGame();

	UPROPERTY(EditAnywhere, Category = "Stat")
	int32 Name;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float DefaultDamage;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float DefaultLuck;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float DefaultAttackSpeed;

	UPROPERTY(EditAnywhere, Category = "Stat")
	int32 Token;

};
