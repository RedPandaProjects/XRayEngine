// Copyright Epic Games, Inc. All Rights Reserved.
using System;
using System.IO;
using UnrealBuildTool;

public class RedImage: ModuleRules
{
	public RedImage(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Source", "External", "RedImage"));
        string Prefix = "Release";
        if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string LibPath = Path.Combine(ModuleDirectory, "Source","External","RedImage","libraries\\", "x64", Prefix);
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, "ispc_texcomp" + ".lib"));
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "nvtt" + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, "RedImageTool" + ".lib"));

		} 
	}
}
