// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "EditorUtilityWidgetComponents.h"
#include "AVI_PackageAndIncrementEditorUtilityWidget.generated.h"

UCLASS()
class AUTOVERSIONINCREMENTER_API UAVI_PackageAndIncrementEditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void OnChoosePathButtonPressed();

	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditorUtilityButton> ChoosePathButton;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UEditorUtilityEditableTextBox> PackagePathTextBox;
};
