#include "Common/HAIAIMIHelper.h"
#include <Engine/Engine.h>
#include "ActionGame.h"
#include <Engine/GameViewportClient.h>
#include <Kismet/GameplayStatics.h>
#include "Paths.h"
#include "FileHelper.h"
#include "JsonSerializer.h"
#include "JsonValue.h"
#include "Culture.h"

FString HAIAIMIHelper::RelativePath = FString(TEXT("LevelConfig"));
TArray<TSharedPtr<FJsonValue>> HAIAIMIHelper::JsonParser = {};

void HAIAIMIHelper::Debug_ScreenMessage(FString&& InString, float ShowTime, FColor FontColor)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, ShowTime, FontColor, InString);
}

void HAIAIMIHelper::Debug_LogMessage(FString&& InString)
{
	UE_LOG(LogHM, Log, TEXT("%s"), *InString);
}

float HAIAIMIHelper::AdaptSubAngle(const float InAngle1, const float InAngle2)
{
	const float TempSub = InAngle1 - InAngle2;

	if (TempSub > 180.f)
		return 360.f - TempSub;
	else if (TempSub < -180.f)
		return 360.f + TempSub;
	else return TempSub;
}

float HAIAIMIHelper::AdaptAngle(const float InAngle)
{
	if (InAngle > 180.f)
		return 360.f - InAngle;
	else if (InAngle < -180.f)
		return 360.f + InAngle;
	else return InAngle;
}

float HAIAIMIHelper::GetDegreesBetweenActors(AActor* Aim, AActor* Self)
{
	const FVector ToAimDir = (Aim->GetActorLocation() - Self->GetActorLocation()).GetSafeNormal2D();
	const FVector ForwardDir = Self->GetActorRotation().Vector();
	return FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ToAimDir, ForwardDir)));
}

FVector2D HAIAIMIHelper::ConvertToNormalCoord(FVector2D Pos)
{
	FVector2D Res = Pos;
	if (GEngine && GEngine->GameViewport)
	{
		FVector2D Size;
		GEngine->GameViewport->GetViewportSize(Size);
		//Res.X = Pos.X * 1920.f / Size.X;
		Res.Y = Pos.Y * 1080.f / Size.Y;
		const float NormalWidth = Size.Y*(16.f / 9.f);
		Res.X = Pos.X * 1920.f / NormalWidth;
	}
	
	return Res;
}

float HAIAIMIHelper::GetMaxWidth()
{
	float  MaxWidth = 1920.f;
	FVector2D Res;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(Res);
		MaxWidth = (Res.X / Res.Y)*1920.f;
	}
	return MaxWidth;
}

FString HAIAIMIHelper::GetLocalization()
{
	return FInternationalization::Get().GetCurrentCulture().Get().GetName();
}

void HAIAIMIHelper::ChangeLocalization(FString target)
{
	FInternationalization::Get().SetCurrentCulture(target);
}

void HAIAIMIHelper::SaveScore(uint32 newScore)
{
	/*UScoreSaveGame* CurSaveGame = NewObject<UScoreSaveGame>();
	CurSaveGame->AllScores = HAIAIMIHelper::LoadScores();
	TArray<uint32>& Temp = CurSaveGame->AllScores;

	for (int32 i = 0; i < 10; ++i)
	{
		if (newScore > Temp[i])
		{
			Temp.Insert(newScore, i);
			Temp.SetNum(10);
			break;
		}
	}
	UGameplayStatics::SaveGameToSlot(CurSaveGame, TEXT("AllScores"), 0);*/
}

TArray<uint32>  HAIAIMIHelper::LoadScores()
{
	TArray<uint32> Res;
	/*Res.Init(0, 10);
	if (USaveGame* Scores = UGameplayStatics::LoadGameFromSlot(TEXT("AllScores"), 0))
	{
		UScoreSaveGame* MyScores = Cast<UScoreSaveGame>(Scores);
		if (MyScores->AllScores.Num() == 0)
			return Res;
		else
			return MyScores->AllScores;
	}*/

	return Res;
}

bool HAIAIMIHelper::LoadStringFromFile(const FString& FileName, FString& OutInfo)
{
	if (FileName.IsEmpty())return false;

	FString AbsolutePath = FPaths::ProjectContentDir() + RelativePath + "/" + FileName;
	if (FPaths::FileExists(AbsolutePath))
	{
		if (FFileHelper::LoadFileToString(OutInfo, *AbsolutePath))
			return true;      //加载成功
		else
			Debug_LogMessage(TEXT("未找到文件"));
	}

	return false;
}

void HAIAIMIHelper::PrepareJson(const FString& FileName)
{
	FString JsonContent;
	//CurParseFile = FileName;
	if (LoadStringFromFile(FileName, JsonContent))
	{
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonContent);       //创建Json读取实例类

		if (FJsonSerializer::Deserialize(JsonReader, JsonParser))    //反序列化
			Debug_LogMessage(TEXT("读取成功"));
		else
			Debug_LogMessage(TEXT("读取失败"));
	}
}

void HAIAIMIHelper::GetNumberFromJson(const FString& FileName, const FString& Key, const int32 Index, float& OutNumber)
{
	if (JsonParser.Num() <= 0)
		PrepareJson(FileName);
	if (JsonParser.Num() > 0)
	{
		if (JsonParser.Num() > Index)
			OutNumber = JsonParser[Index]->AsObject()->GetNumberField(Key);
	}
	else
		Debug_LogMessage(TEXT("请重新读取文件"));
}

int32 HAIAIMIHelper::GetEnemyCounts()
{
	return JsonParser.Num();
}
