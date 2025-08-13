// Fill out your copyright notice in the Description page of Project Settings.

#include "AIVR_BlueprintFunctionLibrary.h"

bool UAIVR_BlueprintFunctionLibrary::TryReadVersionTxtAt(const FString& Dir, FString& OutVersion)
{
	const FString Path = FPaths::Combine(Dir, TEXT("version.txt"));
	if (FPaths::FileExists(Path))
	{
		FString Content;
		if (FFileHelper::LoadFileToString(Content, *Path))
		{
			Content.TrimStartAndEndInline();
			if (!Content.IsEmpty())
			{
				OutVersion = Content;
				return true;
			}
		}
	}
	return false;
}

FString UAIVR_BlueprintFunctionLibrary::GetProjectVersion()
{
#if !WITH_EDITOR
	{
		FString Ver;
		if (TryReadVersionTxtAt(FPaths::LaunchDir(), Ver))
		{
			return Ver;
		}
	}

	{
		FString Dir = FPaths::ConvertRelativePathToFull(FPaths::LaunchDir());
		for (int32 Depth = 0; Depth < 3; ++Depth)
		{
			const FString Parent = FPaths::GetPath(Dir);
			if (Parent.IsEmpty() || Parent == Dir) break;
			Dir = Parent;

			FString Ver;
			if (TryReadVersionTxtAt(Dir, Ver))
			{
				return Ver;
			}
		}
	}
#endif

	const FString IniPath = FPaths::Combine(FPaths::ProjectConfigDir(), TEXT("DefaultGame.ini"));
	FString Value;
	if (GConfig->GetString(
			TEXT("/Script/EngineSettings.GeneralProjectSettings"),
			TEXT("ProjectVersion"),
			Value,
			IniPath))
	{
		return Value;
	}

	return TEXT("Unknown");
}
