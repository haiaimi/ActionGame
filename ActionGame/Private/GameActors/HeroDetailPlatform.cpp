// Fill out your copyright notice in the Description page of Project Settings.

#include "GameActors/HeroDetailPlatform.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AHeroDetailPlatform::AHeroDetailPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));

	RootComponent = PlatformMesh;
	CharacterMesh->SetupAttachment(RootComponent);
	ViewCamera->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHeroDetailPlatform::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AHeroDetailPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
