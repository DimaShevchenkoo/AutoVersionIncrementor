// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AIVR_BlueprintFunctionLibrary.generated.h"

UCLASS()
class AUTOVERSIONINCREMENTERRUNTIME_API UAIVR_BlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static bool IsVersionLike(const FString& Name);

	UFUNCTION(BlueprintCallable)
	static FString GetProjectVersion();
};
