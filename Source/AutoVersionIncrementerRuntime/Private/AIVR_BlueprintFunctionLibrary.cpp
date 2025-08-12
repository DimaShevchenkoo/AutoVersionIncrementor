// Fill out your copyright notice in the Description page of Project Settings.

#include "AIVR_BlueprintFunctionLibrary.h"

bool UAIVR_BlueprintFunctionLibrary::IsVersionLike(const FString& Name)
{
	TArray<FString> Parts;
	Name.ParseIntoArray(Parts, TEXT("."), true);
	if (Parts.Num() < 2 || Parts.Num() > 5) return false;
	for (const FString& P : Parts)
	{
		if (P.IsEmpty() || !P.IsNumeric()) return false;
	}
	return true;
}

FString UAIVR_BlueprintFunctionLibrary::GetProjectVersion()
{
#if !WITH_EDITOR
	FString Dir = FPaths::ConvertRelativePathToFull(FPaths::LaunchDir());
	for (int32 Depth = 0; Depth < 4 && !Dir.IsEmpty(); ++Depth)
	{
		TArray<FString> Files;
		IFileManager::Get().FindFiles(Files, *(Dir / TEXT("*")), /*Files=*/true, /*Dirs=*/false);
		for (const FString& FileName : Files)
		{
			if (IsVersionLike(FileName))
			{
				return FileName;
			}
		}

		const FString Parent = FPaths::GetPath(Dir);
		if (Parent.IsEmpty() || Parent == Dir) break;
		Dir = Parent;
	}
	
#endif
	
	FString IniPath = FPaths::Combine(FPaths::ProjectConfigDir(), TEXT("DefaultGame.ini"));
	FConfigFile ConfigFile;
	ConfigFile.Read(IniPath);

	const FString SectionName(TEXT("/Script/EngineSettings.GeneralProjectSettings"));
	const FName KeyName(TEXT("ProjectVersion"));

	if (FConfigSection* Section = ConfigFile.Find(SectionName))
	{
		if (FConfigValue* Value = Section->Find(KeyName))
		{
			return Value->GetValue();
		}
	}

	return TEXT("Unknown");
}
