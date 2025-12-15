// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE5_CppServer : ModuleRules
{
	public UE5_CppServer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Sockets", "Networking" });

		PrivateDependencyModuleNames.AddRange(new string[] { "ProtobufCore" });

		PrivateIncludePaths.AddRange(new string[]
        {
            "UE5_CppServer/",
			"UE5_CppServer/Protobuf/",
			"UE5_CppServer/Network/"
        });
	}
}
