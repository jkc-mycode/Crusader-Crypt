// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/MonsterBase.h"
#include "Boss.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API ABoss : public AMonsterBase
{
	GENERATED_BODY()

public:
	ABoss();
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
public:
	virtual void AttackStart() override;
	virtual void AttackEnd() override;
	void ChargedAttackStart(int32 FistIndex);
	void JumpAttackStart();
	void JumpAttackEnd();
	/**Boss Monster Default Attack Func*/
	void Attack();
	/**Boss Monster Charged Attack Func*/
	void ChargedAttack();
	/**Boss Monster Jump Attack Func*/
	void JumpAttack();
	/**Boss Monster Ranged Attack Func*/
	void RangedAttack();
	void RangedAttackStart();
	
	float GetHitPoint() { return HealthPoint; };
	float GetAttackSpeed() { return AttackSpeed; };
	
public:
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "projectile")
	TSubclassOf<class AProjectile> Projectile;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Fist", Meta = (AllowPrivateAccess = true))
	class UBoxComponent* LeftBox;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Fist", Meta = (AllowPrivateAccess = true))
	class UBoxComponent* RightBox;

private:
	float AttackDamageMin;
	float AttackDamageMax;
	float ChargedAttackDamage;
	float JumpAttackDamage;
private:
	int32 ComboNum;
	bool Shooted;
};
