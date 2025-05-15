// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
// publish name:HaoJunDeveloper
// intended year of published:2024 

using System.IO;
using UnrealBuildTool;

public class MysqlConnectorCpp : ModuleRules
{
	public MysqlConnectorCpp(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		PublicSystemLibraryPaths.Add(Path.Combine(ModuleDirectory, "lib"));
		PublicSystemLibraries.Add(Path.Combine(ModuleDirectory, "lib", "DataBaseManager.lib"));
		RuntimeDependencies.Add("$(BinaryOutputDir)/libcrypto-1_1-x64.dll",
			Path.Combine(ModuleDirectory, "lib", "libcrypto-1_1-x64.dll"));
		RuntimeDependencies.Add("$(BinaryOutputDir)/libssl-1_1-x64.dll",
			Path.Combine(ModuleDirectory, "lib", "libssl-1_1-x64.dll"));
		RuntimeDependencies.Add("$(BinaryOutputDir)/DataBaseManager.dll",
			Path.Combine(ModuleDirectory, "lib", "DataBaseManager.dll"));
	}
}