// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MenuHUD.h"
#include "Widgets/SMainMenuWidget.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "UI/SHeroDetailWidget.h"
#include "HAIAIMIHelper.h"

AMenuHUD::AMenuHUD():
	MainMenu(nullptr)
{

}

void AMenuHUD::DrawHUD()
{
	Super::DrawHUD();

	if(!MainMenu.IsValid() && GEngine)
	{
		AMenuPlayerController* Controller = Cast<AMenuPlayerController>(GetOwningPlayerController());
		SAssignNew(MainMenu, SMainMenuWidget)
		.OwnerHUD(this)
		.OwnerController(Controller);

		GEngine->GameViewport->AddViewportWidgetContent(
			SNew(SWeakWidget)
			.PossiblyNullContent(MainMenu.ToSharedRef()),
			0
			);
	}
}

void AMenuHUD::ShowCharacterDetail()
{
	if(!HeroDetail.IsValid() && GEngine)
	{
		SAssignNew(HeroDetail, SHeroDetailWidget);

		GEngine->GameViewport->AddViewportWidgetContent(
			SNew(SWeakWidget)
			.PossiblyNullContent(HeroDetail.ToSharedRef()),
			0
			);
	}
}
