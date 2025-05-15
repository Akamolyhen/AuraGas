// Copyright Epic Games, Inc. All Rights Reserved.
// Publish Name:HaoJunDeveloper
// Intended Year of Published:2024 
using System.IO;
using UnrealBuildTool;

public class MySQLConnectorTools : ModuleRules
{
	public MySQLConnectorTools(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
                
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				  "CoreUObject",
				  "Engine",
				  "InputCore", 
				  "MysqlConnectorCpp",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"MysqlConnectorCpp"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
		
		PublicDefinitions.Add("REPLACEMENT_OPERATOR_NEW_AND_DELETE=");
	}
}
