// Copyright Epic Games, Inc. All Rights Reserved.
using System;
using System.IO;
using UnrealBuildTool;

public class XRayEngineModules : ModuleRules
{
	public XRayEngineModules(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory,"Source"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "SDK","Include"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Source", "External","Public"));
        string ConfigureName = "Release";
		if (Target.Configuration== UnrealTargetConfiguration.Debug|| Target.Configuration == UnrealTargetConfiguration.DebugGame)
		{
            ConfigureName = "Debug";
        }
        if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string LibPath = Path.Combine(ModuleDirectory,"Lib", "x64", ConfigureName);
			string BinPath = Path.Combine(ModuleDirectory,"Bin", "x64", ConfigureName);
			PublicAdditionalLibraries.Add(Path.Combine(LibPath,"XrCore_"+ ConfigureName + ".lib"));
            PublicAdditionalLibraries.Add(Path.Combine(LibPath,"XrEngine_" + ConfigureName + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath,"XrCDB_" + ConfigureName + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath,"XrAPI_" + ConfigureName + ".lib"));

		} 
	}
}
