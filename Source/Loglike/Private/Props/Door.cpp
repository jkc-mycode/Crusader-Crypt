// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Door.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	DoorL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Door"));
	DoorR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Door"));
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CharacterCollider"));
	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Detail Text"));

	SetRootComponent(SphereComponent);
	DoorL->SetupAttachment(RootComponent);
	DoorR->SetupAttachment(RootComponent);
	TextComponent->SetupAttachment(RootComponent);
}

void ADoor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	TextComponent->SetVisibility(false);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoor::CharacterOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ADoor::CharacterOverlapEnd);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ADoor::CharacterOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TextComponent->SetVisibility(true);
}
void ADoor::CharacterOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TextComponent -> SetVisibility(false);
}
