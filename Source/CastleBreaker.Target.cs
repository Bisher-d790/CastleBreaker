using UnrealBuildTool;
using System.Collections.Generic;

public class CastleBreakerTarget : TargetRules
{
	public CastleBreakerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("CastleBreaker");
	}
}
