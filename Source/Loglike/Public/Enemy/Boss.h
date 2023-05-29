// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/MonsterBase.h"
#include "Boss.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnRangedAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnChargedAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnJumpAttackEndDelegate);

UCLASS()
class LOGLIKE_API ABoss : public AMonsterBase
{
	GENERATED_BODY()

public:
	ABoss();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void AttackStart() override;
	virtual void AttackEnd() override;

	void ChargedAttackStart(int32 FistIndex);
	void JumpAttackStart();
	void JumpAttackEnd();
	void SpawnEnemies();

	/**Boss Monster Default Attack Func*/
	void Attack();
	/**Boss Monster Charged Attack Func*/
	void ChargedAttack();
	/**Boss Monster Jump Attack Func*/
	void JumpAttack();
	/**Boss Monster Ranged Attack Func*/
	void RangedAttack();
	void RangedAttackStart();

	void PushBack(AActor* PivotActor);
	void Stun();

	void CheckHitPoint();
	void ReadyToSkill(bool enable);
	void AttackCount();
	
	void SetHPbar();

	float GetHitPoint() { return HealthPoint; };
	float GetAttackSpeed() { return AttackSpeed; };
	float GetHitNum() { return HitNum; };
	bool GetIsAttacking() { return IsAttacking; }	

public:
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "projectile")
	TSubclassOf<class AProjectile> Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemies", Meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<class AMonsterBase>> Enemies;

	FOnRangedAttackEndDelegate RangedAttackIsEnded;
	FOnChargedAttackEndDelegate ChargedAttackIsEnded;
	FOnJumpAttackEndDelegate JumpAttackIsEnded;

protected:
	TSubclassOf<UUserWidget> BossHpWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = Widget)
	class UBossHPBar* BossHpWidget;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fist", Meta = (AllowPrivateAccess = true))
	class UBoxComponent* LeftBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fist", Meta = (AllowPrivateAccess = true))
	class UBoxComponent* RightBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemies", Meta = (AllowPrivateAccess = true))
	TArray<class AMonsterBase*> SpawnedEnemies;

	float AttackDamageMin;
	float AttackDamageMax;
	float ChargedAttackDamage;
	float JumpAttackDamage;

	int32 ComboNum;
	int32 HitNum;
	bool IsAttacking;
	float CheckHP;
};
