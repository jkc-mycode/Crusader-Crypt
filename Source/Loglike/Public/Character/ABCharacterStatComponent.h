// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOGLIKE_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//void SetNewLevel(int NewLevel);
	void SetStartName(int32 NewName);
	void SetMaxHealth(float NewMaxHealth);
	void SetCurrentHealth(float NewCurrentHealth);
	void SetAddHealth(float NewAddHealth);
	void SetDefaultDamage(float NewDefaultDamage);
	void SetAddDamage(float NewAddDamage);
	void SetDefaultLuck(float NewDefaultLuck);
	void SetAddLuck(float NewAddLuck);
	void SetDefaultAttackSpeed(float NewDefaultAttackSpeed);
	void SetAddAttackSpeed(float NewAddAttackSpeed);
	void SetToken(int32 NewToken);

	
	float GetMaxHealth();
	float GetHealth();
	float GetCurrentHealth();
	float GetDefaultDamage();
	float GetDamage();
	float GetDefaultLuck();
	float GetLuck();
	float GetDefaultAttackSpeed();
	float GetAttackSpeed();
	int32 GetToken();

	FOnHPIsZeroDelegate OnHPIsZero;
		
private:
	struct FABCharacterData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Name;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float MaxHealth; // == DefaultHealth

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AddHealth;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHealth;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float DefaultDamage;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AddDamage;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float DefaultLuck;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AddLuck;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float DefaultAttackSpeed;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AddAttackSpeed;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Token;

};
