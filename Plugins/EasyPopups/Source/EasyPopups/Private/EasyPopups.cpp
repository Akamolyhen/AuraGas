/*

Copyright © 2019 Mashmashu Studio.
All rights reserved.

*/


#include "EasyPopups.h"

#define LOCTEXT_NAMESPACE "FEasyPopupsModule"

void FEasyPopupsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FEasyPopupsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEasyPopupsModule, EasyPopups)
