// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RTSMoveSystem : ModuleRules
{
	public RTSMoveSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", "ProceduralMeshComponent" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
		
		PublicDependencyModuleNames.AddRange(new string[] {
			"Slate",      // This module is for FSlateDrawElement
			"SlateCore"   // This module is for FSlateBrush
		});

	}
}
