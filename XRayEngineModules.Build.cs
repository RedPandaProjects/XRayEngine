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
		if (Target.Configuration== UnrealTargetConfiguration.Debug || Target.Configuration == UnrealTargetConfiguration.DebugGame)
		{
            ConfigureName = "Debug";
        }

		if (Target.Configuration == UnrealTargetConfiguration.Shipping ||Target.Configuration == UnrealTargetConfiguration.Test)
		{
			ConfigureName = "Shipping";
		}

		string LibPath = Path.Combine(ModuleDirectory,"Lib", "x64", ConfigureName);
		string SDKLibPath = Path.Combine(ModuleDirectory,"SDK","Lib", "x64");
        if (Target.Platform == UnrealTargetPlatform.Win64)
		{
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "XrCore_" + ConfigureName + ".lib"));
            PublicAdditionalLibraries.Add(Path.Combine(LibPath,"XrEngine_" + ConfigureName + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath,"XrCDB_" + ConfigureName + ".lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath,"XrAPI_" + ConfigureName + ".lib"));

		}

        if (Target.Configuration== UnrealTargetConfiguration.Shipping)
        {
	        string ThirdPartyLibPath = Path.Combine(ModuleDirectory,"Lib", "x64", "Release");
	        PublicAdditionalLibraries.Add(Path.Combine(LibPath,"XrGameSOC_" + ConfigureName + ".lib"));
	        PublicAdditionalLibraries.Add(Path.Combine(LibPath,"XrNetServer_" + ConfigureName + ".lib"));
	        PublicAdditionalLibraries.Add(Path.Combine(LibPath,"XrPhysics_" + ConfigureName + ".lib"));
	        PublicAdditionalLibraries.Add(Path.Combine(LibPath,"XrXMLParser_" + ConfigureName + ".lib"));
	        PublicAdditionalLibraries.Add(Path.Combine(LibPath,"Luabind_" + ConfigureName + ".lib"));
	        PublicAdditionalLibraries.Add(Path.Combine(LibPath,"Crypto_" + ConfigureName + ".lib"));
	        PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyLibPath,"Ode_Release" + ".lib"));
	        PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyLibPath,"Ode_Release" + ".lib"));
	        PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyLibPath,"Ode_Release" + ".lib"));
	        PublicAdditionalLibraries.Add(Path.Combine(SDKLibPath,"lua51.lib" ));
        }
	}
}
