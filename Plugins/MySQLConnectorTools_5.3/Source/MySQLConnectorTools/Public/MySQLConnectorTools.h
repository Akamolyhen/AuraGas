// Copyright Epic Games, Inc. All Rights Reserved.
// Publish Name:HaoJunDeveloper
// Intended Year of Published:2024 
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FMySQLConnectorToolsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
