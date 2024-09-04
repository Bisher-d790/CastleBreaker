using UnrealBuildTool;
using System.Collections.Generic;

public class CastleBreakerEditorTarget : TargetRules
{
	public CastleBreakerEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("CastleBreaker");
	}
}
