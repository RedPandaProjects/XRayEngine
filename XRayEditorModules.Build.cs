// Copyright Epic Games, Inc. All Rights Reserved.
using System;
using System.IO;
using UnrealBuildTool;

public class XRayEditorModules : ModuleRules
{
	public XRayEditorModules(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Source", "Editors"));
        Type = ModuleType.External;
        string ConfigureName = "Release";
        if (Target.Configuration == UnrealTargetConfiguration.Debug || Target.Configuration == UnrealTargetConfiguration.DebugGame)
        {
            ConfigureName = "Debug";
        }
        if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string LibPath = Path.Combine(ModuleDirectory,"Lib", "x64", ConfigureName);
			string BinPath = Path.Combine(ModuleDirectory,"Bin", "x64", ConfigureName);
			PublicAdditionalLibraries.Add(Path.Combine(LibPath,"XrECore_"+ ConfigureName + ".lib"));
		} 
	}
}
