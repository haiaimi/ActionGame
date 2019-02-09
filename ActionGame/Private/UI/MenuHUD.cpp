// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MenuHUD.h"
#include "Widgets/SMainMenuWidget.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "UI/SHeroDetailWidget.h"
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

void AMenuHUD::ShowCharacterDetail(TWeakObjectPtr<AHeroDetailPlatform> DetailPlatform /*= nullptr*/)
{
	if(!HeroDetail.IsValid() && GEngine && DetailPlatform.IsValid())
	{
		AMenuPlayerController* Controller = Cast<AMenuPlayerController>(GetOwningPlayerController());
		SAssignNew(HeroDetail, SHeroDetailWidget)
			.PreWidget(MainMenu)
			.OwnerHUD(MakeWeakObjectPtr(this))
			.DetailPlatform(DetailPlatform);

		GEngine->GameViewport->AddViewportWidgetContent(
			SAssignNew(MainMenuContainer, SWeakWidget)
			.PossiblyNullContent(HeroDetail.ToSharedRef()),
			1
			);
		Controller->SetCurWidget(HeroDetail);
	}
}

void AMenuHUD::RemoveDetailWidget()
{
	if(HeroDetail.IsValid() && GEngine)
	{
		AMenuPlayerController* Controller = Cast<AMenuPlayerController>(GetOwningPlayerController());
		GEngine->GameViewport->RemoveViewportWidgetContent(MainMenuContainer.ToSharedRef());
		HeroDetail.Reset();
		Controller->SetCurWidget(MainMenu);
	}
}
