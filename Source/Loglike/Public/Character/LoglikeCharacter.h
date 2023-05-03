// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "LoglikeCharacter.generated.h"


//HP 체크용 델리게이트
//DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
//HP 변화 체크용 델리게이트
//DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS(config = Game, meta = (BlueprintSpawnableComponent))
class ALoglikeCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;


public:
	ALoglikeCharacter();

	/*UPROPERTY(VisibleAnywhere, Category = "Weapon")
	USkeletalMeshComponent* Weapon;*/

	//virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;


protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//공격 애님 실행 함수
	void Attack();
	//닷지 애님 실행 함수
	void Dodge();
	//패링 애님 실행 함수
	void Parrying();
	//받는 데미지 처리 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	//HUD에서의 HP Bar 비율을 계산하는 함수
	float GetHPRatio();

	//각 HP 델리게이트(델리게이트는 바인딩된 함수들을 동시에 실행 시켜줌)
	//FOnHPIsZeroDelegate OnHPIsZero;
	//FOnHPChangedDelegate OnHPChanged;

	//블루프린트에서 현재 체력을 가져갈 때 사용할 함수
	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetHealth();
	//블루프린트에서 캐릭터 공격력을 가져갈 때 사용할 함수
	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetCharacterDamage();
	//블루프린트에서 캐릭터 행운을 가져갈 때 사용할 함수
	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetCharacterLuck();
	//블루프린트에서 캐릭터 행운을 가져갈 때 사용할 함수
	UFUNCTION(BlueprintPure, Category = "Stat")
	int GetCharacterToken();


private:
	//공격 몽타주 끝 체크 함수
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	//콤보 공격 시작 상태 체크 함수
	void AttackStartComboState();
	//콤보 공격 끝 상태 체크 함수
	void AttackEndComboState();
	//공격 콜리전을 켜서 공격했는지 체크하는 함수
	void AttackCheck();

	//닷지 몽타주 끝 체크 함수
	UFUNCTION()
	void OnDodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	//패링 몽타주 끝 체크 함수
	UFUNCTION()
	void OnParryingMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	//닷지용 딜레이 함수
	void Delay1();
	//패링용 딜레이 함수
	void Delay2();


public:
	//캐릭터 데미지 변수
	float CharacterDamage;
	//캐릭터 행운 변수
	float CharacterLuck;
	//캐릭터 토근 변수
	int CharacterToken;
	//캐릭터 죽음 체크(체력 0 체크)
	bool IsDead;

	//HUD 클래스 객체
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UUserWidget> HUDWidgetClass;
	//위젯 클래스 객체
	UPROPERTY(EditAnywhere, Category = "Widget")
	class UABCharacterWidget* CurrentWidget;


private:
	//애님 클래스 객체
	UPROPERTY()
	class UABAnimInstance* ABAnim;
	//닷지 타이머 핸들러
	UPROPERTY()
	FTimerHandle Dodge_Timer;
	//패링 타이머 핸들러
	UPROPERTY()
	FTimerHandle Parrying_Timer;

	//공격 체크
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool IsAttacking;
	//콤보 가능 여부 체크
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;
	//콤보 가능하도록 입력이 들어오는지 체크(클릭이 연속으로 들어오는지 체크)
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;
	//현재 콤보 수
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;
	//최대 콤보 수
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;
	
	//닷지 체크
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Dodge", Meta = (AllowPrivateAccess = true))
	bool IsDodge = false;
	//닷지 스피드
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Dodge", Meta = (AllowPrivateAccess = true))
	float DodgeSpeed = 500.0f;
	//닷지 딜레이 유무 체크
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Dodge", Meta = (AllowPrivateAccess = true))
	bool DodgeDelay = false;
	
	//패링 동작 유무 체크
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Parrying", Meta = (AllowPrivateAccess = true))
	bool IsParrying = false;
	//패링 딜레이 유무 체크
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Parrying", Meta = (AllowPrivateAccess = true))
	bool ParryingDelay = false;
	
	//캐릭터 공격 콜리전 범위(앞뒤)
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRange;
	//캐릭터 공격 콜리전 범위(양옆)
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRadius;
	
	//맞았는지 유무 체크
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Hit", Meta = (AllowPrivateAccess = true))
	bool IsHit = false;
	//최대 체력
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess = true))
	float MaxHealth;
	//현재 체력
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess = true))
	float CurrentHealth;
	//맞았을 때 파티클 효과
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitParticle", meta = (AllowPrivateAccess = true))
	UParticleSystem* HitImpactP;
};

