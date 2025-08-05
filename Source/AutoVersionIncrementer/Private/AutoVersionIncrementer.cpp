// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutoVersionIncrementer.h"
#include "EditorAssetLibrary.h"
#include "EditorUtilityObject.h"
#include "LevelEditor.h"
#include "ToolMenu.h"

#define LOCTEXT_NAMESPACE "FAutoVersionIncrementerModule"

void FAutoVersionIncrementerModule::RegisterButton()
{
	const FString Path = TEXT("/AutoVersionIncrementer/EUO_AutoVersion.EUO_AutoVersion");
	UObject* Asset = UEditorAssetLibrary::LoadAsset(Path);

	if (!Asset)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load EUO asset at %s"), *Path);
		return;
	}
	UBlueprint* BP = Cast<UBlueprint>(Asset);
	if (!BP || !BP->GeneratedClass->IsChildOf(UEditorUtilityObject::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("Asset is not an Editor Utility Object Blueprint: %s"), *Asset->GetName());
		return;
	}

	UEditorUtilityObject* Instance = NewObject<UEditorUtilityObject>(GetTransientPackage(), BP->GeneratedClass);
	Instance->AddToRoot();
	Instance->Run();
}

void FAutoVersionIncrementerModule::StartupModule()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.OnTabManagerChanged().AddRaw(this, &FAutoVersionIncrementerModule::RegisterButton);
}

void FAutoVersionIncrementerModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAutoVersionIncrementerModule, AutoVersionIncrementer)
