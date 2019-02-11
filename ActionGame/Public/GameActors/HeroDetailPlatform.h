// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionGameType.h"
#include "HeroDetailPlatform.generated.h"

UCLASS()
class ACTIONGAME_API AHeroDetailPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHeroDetailPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PlayMontage(int32 CharIndex, int32 MontageIndex);

	void SetCharacterMesh(int32 CharIndex, int32 MeshIndex);

	void StartRotate(FVector2D StartMousePos);

	void EndRotate();

protected:
	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* CharacterMesh;

	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PlatformMesh;

	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CharDetection;

	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* ViewCamera;

public:
	/**所有游戏人物的信息*/
	UPROPERTY(EditDefaultsOnly)
	TArray<struct FCharacterInfo> CharInfos;

private:
	bool bCanRotate;

	FVector2D StartMousePos;

	FRotator StartRot;
};
