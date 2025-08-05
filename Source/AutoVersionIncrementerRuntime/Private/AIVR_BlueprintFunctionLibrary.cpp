// Fill out your copyright notice in the Description page of Project Settings.

#include "AIVR_BlueprintFunctionLibrary.h"

FString UAIVR_BlueprintFunctionLibrary::GetProjectVersion()
{
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
