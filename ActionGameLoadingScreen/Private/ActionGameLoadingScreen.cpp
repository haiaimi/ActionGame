#include "ActionGameLoadingScreen.h"

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "MoviePlayer.h"
#include "SThrobber.h"
#include "DeferredCleanupSlateBrush.h"

class SActionGameLoadingScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SActionGameLoadingScreen) {}
	SLATE_END_ARGS()

	
	void Construct(const FArguments& InArgs)
	{
		static const FName LoadingScreenName(TEXT("/Game/UI/Image/LoadingScreen.LoadingScreen"));
		UObject* LoadingTexture = LoadObject<UObject>( NULL, *LoadingScreenName.ToString() );
		FSlateBrush* Brush = new FSlateBrush();
		Brush->SetResourceObject(LoadingTexture);
		LoadingScreenBrush = FDeferredCleanupSlateBrush::CreateBrush(*Brush);

		ChildSlot
		[
			SNew(SOverlay)
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.Image(LoadingScreenBrush->GetSlateBrush())
			]
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.VAlign(VAlign_Bottom)
				.HAlign(HAlign_Right)
				.Padding(FMargin(10.0f))
				[
					SNew(SThrobber)
					.Visibility(this, &SActionGameLoadingScreen::GetLoadIndicatorVisibility)
				]
			]
		];
	}

private:
	EVisibility GetLoadIndicatorVisibility() const
	{
		bool Vis =  GetMoviePlayer()->IsLoadingFinished();
		return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Collapsed : EVisibility::Visible;
	}
	
	/** loading screen image brush */
	TSharedPtr<FDeferredCleanupSlateBrush> LoadingScreenBrush;
};

class FActionGameLoadingScreenModule : public IActionGameLoadingScreenModule
{
public:
	virtual void StartupModule() override
	{
		LoadObject<UObject>(NULL, TEXT("/Game/UI/Image/LoadingScreen.LoadingScreen") );

		if (IsMoviePlayerEnabled())
		{
			CreateScreen();
		}
	}
	
	virtual bool IsGameModule() const override
	{
		return true;
	}

	virtual void StartInGameLoadingScreen() override
	{
		CreateScreen();
	}

	virtual void CreateScreen()
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.MinimumLoadingScreenDisplayTime = 0.f;
		LoadingScreen.WidgetLoadingScreen = SNew(SActionGameLoadingScreen);
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}

};

IMPLEMENT_GAME_MODULE(FActionGameLoadingScreenModule, ActionGameLoadingScreen);
