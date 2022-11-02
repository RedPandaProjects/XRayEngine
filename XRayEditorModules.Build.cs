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
        string Prefix = "_Release";
		bool bOnlyDebug = true;
		if (Target.Configuration== UnrealTargetConfiguration.Debug||bOnlyDebug)
		{
            Prefix = "_Debug";
        }
        if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string LibPath = Path.Combine(ModuleDirectory,"Lib", "x64","Debug");
			string BinPath = Path.Combine(ModuleDirectory,"Bin", "x64","Debug");
			PublicAdditionalLibraries.Add(Path.Combine(LibPath,"XrECore"+ Prefix+".lib"));
		} 
	}
}
