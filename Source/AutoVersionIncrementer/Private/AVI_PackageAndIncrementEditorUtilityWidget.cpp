// Fill out your copyright notice in the Description page of Project Settings.

#include "AVI_PackageAndIncrementEditorUtilityWidget.h"
#include "DesktopPlatformModule.h"

void UAVI_PackageAndIncrementEditorUtilityWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ChoosePathButton->OnClicked.AddDynamic(this, &ThisClass::OnChoosePathButtonPressed);
}

void UAVI_PackageAndIncrementEditorUtilityWidget::OnChoosePathButtonPressed()
{
	if (IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get())
	{
		const void* ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);

		FString OutFolderName;
		const FString Title = TEXT("Select a folder for packaging");
		const FString DefaultPath = FPaths::ProjectDir();

		const bool bOpened = DesktopPlatform->OpenDirectoryDialog(
			ParentWindowHandle,
			Title,
			DefaultPath,
			OutFolderName
		);

		if (bOpened)
		{
			PackagePathTextBox->SetText(FText::FromString(OutFolderName));
		}
	}
}