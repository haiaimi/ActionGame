// Fill out your copyright notice in the Description page of Project Settings.

#include "GameHUD.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "ActionGameCharacter.h"

void AGameHUD::DrawHUD()
{
	if (!HUDWidget.IsValid() && GEngine)
	{
		if (AActionGameCharacter* CurOwner = Cast<AActionGameCharacter>(GetOwningPawn()))
		{
			SAssignNew(HUDWidget, SHUDWidget)
			.Owner(CurOwner);

			GEngine->GameViewport->AddViewportWidgetContent(
				SNew(SWeakWidget)
				.PossiblyNullContent(HUDWidget.ToSharedRef()),
				1
				);
		}
	}

	Super::DrawHUD();
}
