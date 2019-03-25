// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerStart.h"
#include "Components/CapsuleComponent.h"

void AMyPlayerStart::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetCapsuleComponent()->SetConstraintMode(EDOFMode::YZPlane);
}

void AMyPlayerStart::BeginPlay()
{
	//UCapsuleComponent
	GetCapsuleComponent()->SetConstraintMode(EDOFMode::YZPlane);
	//GetCapsuleComponent()->Constra
	Super::BeginPlay();
}
