// Fill out your copyright notice in the Description page of Project Settings.

//**********************************************************************
//프로젝트 설정 맵 & 모드 설정에서 게임 인스턴스 클래스 ABGameInstance 로 변경
//**********************************************************************

#include "Character/ABCharacterStatComponent.h"
#include "GameFramework/LoglikeGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ABPlayerState.h"
#include "GameFramework/ABSaveGame.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	Name = 1;
}

// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	UABSaveGame* ABSaveGame = Cast<UABSaveGame>(UGameplayStatics::CreateSaveGameObject(UABSaveGame::StaticClass()));

	ABSaveGame = Cast<UABSaveGame>(UGameplayStatics::LoadGameFromSlot("Player1", 0));
	if (nullptr == ABSaveGame)
		SetStartName(Name);
	else
	{
		SetMaxHealth(ABSaveGame->MaxHealth);
		SetDefaultDamage(ABSaveGame->DefaultDamage);
		SetDefaultLuck(ABSaveGame->DefaultLuck);
		SetDefaultAttackSpeed(ABSaveGame->DefaultAttackSpeed);
		SetToken(ABSaveGame->Token);
	}
}

//// Called every frame
//void UABCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

void UABCharacterStatComponent::SetStartName(int32 NewName)
{
	auto LoglikeGameInstance = Cast<ULoglikeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (LoglikeGameInstance != nullptr)
	{
		CurrentStatData = LoglikeGameInstance->GetABCharacterData(NewName);
		if (nullptr != CurrentStatData)
		{
			Name = NewName;
			SetMaxHealth(CurrentStatData->DefaultHealth);
			SetDefaultDamage(CurrentStatData->DefaultDamage);
			SetDefaultLuck(CurrentStatData->DefaultLuck);
			SetDefaultAttackSpeed(CurrentStatData->DefaultAttackSpeed);
			SetToken(CurrentStatData->Token);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Name (%d) data doesn't exist"), NewName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"));
	}
}

void UABCharacterStatComponent::SetMaxHealth(float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
}

void UABCharacterStatComponent::SetCurrentHealth(float NewCurrentHealth)
{
	CurrentHealth = NewCurrentHealth;
}

void UABCharacterStatComponent::SetAddHealth(float NewAddHealth)
{
	AddHealth += NewAddHealth;
}

void UABCharacterStatComponent::SetDefaultDamage(float NewDefaultDamage)
{
	DefaultDamage = NewDefaultDamage;
}

void UABCharacterStatComponent::SetAddDamage(float NewAddDamage)
{
	AddDamage += NewAddDamage;
}

void UABCharacterStatComponent::SetDefaultLuck(float NewDefaultLuck)
{
	DefaultLuck = NewDefaultLuck;
}

void UABCharacterStatComponent::SetAddLuck(float NewAddLuck)
{
	AddLuck += NewAddLuck;
}

void UABCharacterStatComponent::SetDefaultAttackSpeed(float NewDefaultAttackSpeed)
{
	DefaultAttackSpeed = NewDefaultAttackSpeed;
}

void UABCharacterStatComponent::SetAddAttackSpeed(float NewAddAttackSpeed)
{
	AddAttackSpeed += NewAddAttackSpeed;
}

void UABCharacterStatComponent::SetToken(int32 NewToken)
{
	Token += NewToken;
}

float UABCharacterStatComponent::GetMaxHealth()
{
	return MaxHealth;
}

float UABCharacterStatComponent::GetHealth()
{
	return MaxHealth + AddHealth;
}

float UABCharacterStatComponent::GetCurrentHealth()
{
	return CurrentHealth;
}

float UABCharacterStatComponent::GetDefaultDamage()
{
	return DefaultDamage;
}

float UABCharacterStatComponent::GetDamage()
{
	return DefaultDamage + AddDamage;
}

float UABCharacterStatComponent::GetDefaultLuck()
{
	return DefaultLuck;
}

float UABCharacterStatComponent::GetLuck()
{
	return DefaultLuck + AddLuck;
}

float UABCharacterStatComponent::GetDefaultAttackSpeed()
{
	return DefaultAttackSpeed;
}

float UABCharacterStatComponent::GetAttackSpeed()
{
	return DefaultAttackSpeed + AddAttackSpeed;
}

int32 UABCharacterStatComponent::GetToken()
{
	return Token;
}
