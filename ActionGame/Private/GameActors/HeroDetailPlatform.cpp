// Fill out your copyright notice in the Description page of Project Settings.

#include "GameActors/HeroDetailPlatform.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerInput.h"
#include "HAIAIMIHelper.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AHeroDetailPlatform::AHeroDetailPlatform():
	bCanRotate(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	CharDetection = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CharDetection"));

	RootComponent = PlatformMesh;
	CharacterMesh->SetupAttachment(RootComponent);
	ViewCamera->SetupAttachment(RootComponent);
	CharDetection->SetupAttachment(CharacterMesh);
}

// Called when the game starts or when spawned
void AHeroDetailPlatform::BeginPlay()
{
	Super::BeginPlay();	
	

	//if (InputComponent == nullptr)
	//{
	//	static const FName InputComponentName(TEXT("PlatformInputComponent0"));
	//	InputComponent = NewObject<UInputComponent>(this, InputComponentName);

	//	InputComponent->BindAction(TEXT("DetectPlatform"), EInputEvent::IE_Pressed, this, &AHeroDetailPlatform::MouseClicked);
	//}
	
}

// Called every frame
void AHeroDetailPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bCanRotate)
	{
		if(GetWorld() && GetWorld()->GetFirstPlayerController())
		{
			FVector2D CurMousePos;
			GetWorld()->GetFirstPlayerController()->GetMousePosition(CurMousePos.X, CurMousePos.Y);
			CharacterMesh->SetRelativeRotation(StartRot - 0.5f*FRotator(0.f, CurMousePos.X - StartMousePos.X, 0.f));
		}
	}
}

void AHeroDetailPlatform::PlayMontage(int32 CharIndex, int32 MontageIndex)
{
	if (CharacterMesh && CharInfos.Num() > CharIndex && CharInfos[0].AbilityAnims.Num() > MontageIndex)
	{
		UAnimInstance* AnimInstance = CharacterMesh->GetAnimInstance();
		AnimInstance->Montage_Play(CharInfos[CharIndex].AbilityAnims[MontageIndex]);
	}
}

void AHeroDetailPlatform::SetCharacterMesh(int32 CharIndex, int32 MeshIndex)
{
	if (CharacterMesh && CharInfos.Num() > CharIndex && CharInfos[CharIndex].CharMeshs.Num() > CharIndex)
	{
		CharacterMesh->SetSkeletalMesh(CharInfos[CharIndex].CharMeshs[MeshIndex]);
	}
}

void AHeroDetailPlatform::StartRotate(FVector2D InStartMousePos)
{
	bCanRotate = true;
	StartMousePos = InStartMousePos;
	StartRot = CharacterMesh->GetRelativeTransform().Rotator();
}

void AHeroDetailPlatform::EndRotate()
{
	bCanRotate = false;
}

