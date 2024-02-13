﻿#pragma once

#include "NativeGameplayTags.h"

namespace RosGameplayTags
{
	RISEOFSHELTURU_API	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Jump);
	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Zoom);

	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Tactic_Zoom);
	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Tactic_Move_Camera);
	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Tactic_Rotate_Camera);
	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Tactic_SelectTarget);
	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Tactic_NextTurn);

	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Controller_SelectCharacterSlot1);
	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Controller_SelectCharacterSlot2);
	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Controller_SelectCharacterSlot3);
	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Controller_SelectCharacterSlot4);
	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Controller_SwitchCompanionsFollowState);
	
	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Death);
	
	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage);
	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Heal);

	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_GodMode);
	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_UnlimitedHealth);

	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);
	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dying);
	RISEOFSHELTURU_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dead);
}