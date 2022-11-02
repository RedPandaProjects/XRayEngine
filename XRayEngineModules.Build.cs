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
			PublicAdditionalLibraries.Add(Path.Combine(LibPath,"XrCore"+ Prefix+".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath,"XrEngine" + Prefix + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath,"XrCDB" + Prefix + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath,"XrAPI" + Prefix + ".lib"));

			PublicDelayLoadDLLs.Add("lua51.dll");
			PublicDelayLoadDLLs.Add("OpenAL" + Prefix + ".dll");
			PublicDelayLoadDLLs.Add("XrGameSpy" + Prefix + ".dll");
			PublicDelayLoadDLLs.Add("XrCore" + Prefix + ".dll");
			PublicDelayLoadDLLs.Add("XrXMLParser" + Prefix + ".dll");
			PublicDelayLoadDLLs.Add("XrNetServer" + Prefix + ".dll");
			PublicDelayLoadDLLs.Add("XrCPU_Pipe" + Prefix + ".dll");
			PublicDelayLoadDLLs.Add("XrParticles" + Prefix + ".dll");
			PublicDelayLoadDLLs.Add("XrCDB" + Prefix + ".dll");
			PublicDelayLoadDLLs.Add("XrEngine" + Prefix + ".dll");
			PublicDelayLoadDLLs.Add("XrSound" + Prefix + ".dll");
			PublicDelayLoadDLLs.Add("XrGame" + Prefix + ".dll");
		} 
	}
}
