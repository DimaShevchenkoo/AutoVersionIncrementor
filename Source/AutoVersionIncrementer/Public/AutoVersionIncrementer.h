// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FAutoVersionIncrementerModule : public IModuleInterface
{
public:
	void RegisterButton();

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
