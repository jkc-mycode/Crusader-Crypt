// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/MonsterBase.h"
#include "Projectile.generated.h"

UCLASS()
class LOGLIKE_API AProjectile : public AMonsterBase
{
	GENERATED_BODY() 
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UPROPERTY(EditDefaultsOnly, Category = projectile)
		float myInitialSpeed;

	UPROPERTY(EditDefaultsOnly, Category = projectile)
		float myMaxSpeed;

	UPROPERTY(EditDefaultsOnly, Category = projectile)
		FName ProjectileName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = projectile)
		class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = projectile)
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = projectile)
		UParticleSystemComponent* Particle;

	//UFUNCTION()
	//	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FORCEINLINE class USphereComponent* GetProjectileCollision() const { return CollisionComp; }
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
