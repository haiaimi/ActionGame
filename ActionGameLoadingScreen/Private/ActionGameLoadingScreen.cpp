#include "ActionGameLoadingScreen.h"

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "MoviePlayer.h"
#include "SThrobber.h"

struct FActionGameLoadingScreenBrush : public FSlateDynamicImageBrush, public FGCObject
{
	FActionGameLoadingScreenBrush( const FName InTextureName, const FVector2D& InImageSize, class UTexture2D* InTexture)
		: FSlateDynamicImageBrush( InTexture, InImageSize, InTextureName)
	{
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector)
	{
		UObject* Temp = GetResourceObject();
		Collector.AddReferencedObject(Temp);
	}
};

class SActionGameLoadingScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SActionGameLoadingScreen) {}
	SLATE_END_ARGS()

	
	void Construct(const FArguments& InArgs)
	{
		static const FName LoadingScreenName(TEXT("/Game/UI/Image/LoadingScreen.LoadingScreen"));
		UObject* LoadingTexture = LoadObject<UObject>( NULL, *LoadingScreenName.ToString() );
		LoadingScreenBrush = MakeShareable( new FActionGameLoadingScreenBrush(LoadingScreenName, FVector2D(1920,1080),(UTexture2D*)LoadingTexture) );

		ChildSlot
		[
			SNew(SOverlay)
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.Image(LoadingScreenBrush.Get())
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
	TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;
};

class FActionGameLoadingScreenModule : public IActionGameLoadingScreenModule
{
public:
	virtual void StartupModule() override
	{
		//force load for cooker reference
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
