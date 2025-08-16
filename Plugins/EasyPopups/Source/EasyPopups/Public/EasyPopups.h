/*

Copyright © 2019 Mashmashu Studio.
All rights reserved.

*/

#pragma once

#include "Modules/ModuleManager.h"

class FEasyPopupsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
