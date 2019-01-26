// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <SlateSound.h>

/**
 * 
 */
class ACTIONGAME_API HAIAIMIHelper
{
public:
	static void Debug_ScreenMessage(FString&& InString, float ShowTime = 1.f, FColor FontColor = FColor::Blue);

	static void Debug_LogMessage(FString&& InString);

	static float AdaptSubAngle(const float InAngle1, const float InAngle2);

	static float AdaptAngle(const float InAngle);

	static FVector2D ConvertToNormalCoord(FVector2D Pos);

	/**用于切换关卡是播放音乐*/
	template< class UserClass >	
	FORCEINLINE static FTimerHandle PlaySoundAndCall(UWorld* World, const FSlateSound& Sound, UserClass* inObj, typename TBaseDelegate<void>::TUObjectMethodDelegate< UserClass >::FMethodPtr inMethod)
	{
		FSlateApplication::Get().PlaySound(Sound);

		FTimerHandle Result;
		const float SoundDuration = FMath::Max(FSlateApplication::Get().GetSoundDuration(Sound), 0.1f);
		World->GetTimerManager().SetTimer(Result, FTimerDelegate::CreateUObject(inObj, inMethod), SoundDuration, false);

		return Result;
	}

	static void SaveScore(uint32 newScore);

	static TArray<uint32> LoadScores();

	/* 从文件加载信息，是加载到字符串里
	* @Param FileName     配置文件名
	* @Param OutInfo      读取输出的信息
	*/
	static bool LoadStringFromFile(const FString& FileName, FString& OutInfo);

	/** 准备Json读取，就是事先从文件读取文件，并构建Json对象用以读取文件,所以尽量一个一个文件的读，否则会浪费性能
	* @Param FileName Json文件名称
	*/
	static void PrepareJson(const FString& FileName);

	static void GetNumberFromJson(const FString& FileName, const FString& Key, const int32 Index, float& OutNumber);

	static int32 GetEnemyCounts();
public:
	static FString RelativePath;

	static TArray<TSharedPtr<class FJsonValue>> JsonParser;
};
