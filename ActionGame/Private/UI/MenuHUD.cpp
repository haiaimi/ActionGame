// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MenuHUD.h"
#include "Widgets/SMainMenuWidget.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "HAIAIMIHelper.h"
#include "GameActors/HeroDetailPlatform.h"

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
		.OwnerHUD(MakeWeakObjectPtr(this))
		.OwnerController(MakeWeakObjectPtr(Controller));

		GEngine->GameViewport->AddViewportWidgetContent(
			SNew(SWeakWidget)
			.PossiblyNullContent(MainMenu.ToSharedRef()),
			1
			);
		Controller->SetCurWidget(MainMenu);
	}
}
