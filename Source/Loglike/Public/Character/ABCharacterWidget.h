// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ABCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UABCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//캐릭터와 연결할 함수
	void BindCharacter(class ALoglikeCharacter* LoglikeCharacter);
	//HP 위젯 업데이트 함수
	void UpdateHPWidget();
	//필요 없을지도...?


protected:
	//AddToViewport 시 호출되는 함수
	virtual void NativeConstruct() override;
	

public:
	//HUD 상의 실제 데미지 텍스트 블럭
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* DamageText;

	//HUD 상의 실제 행운 텍스트 블럭
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* LuckText;


private:
	//캐릭터 클래스 객체
	UPROPERTY()
	ALoglikeCharacter* CurrentCharacter;

	//프로그래스바 클리스 객체
	UPROPERTY()
	class UProgressBar* HPProgressBar;
};
