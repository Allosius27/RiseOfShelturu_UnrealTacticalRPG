#include "RosGameplayTags.h"

#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"
#include "System/GameInstances/RosGameManager.h"

namespace RosGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (mouse) input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Jump, "InputTag.Jump", "Jump input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Zoom, "InputTag.Zoom", "Zoom input.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Tactic_Zoom, "InputTag.Tactic.Zoom", "Tactic Zoom input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Tactic_Move_Camera, "InputTag.Tactic.Move.Camera", "Tactic Move Camera input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Tactic_Rotate_Camera, "InputTag.Tactic.Rotate.Camera", "Tactic Rotate Camera input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Tactic_SelectTarget, "InputTag.Tactic.SelectTarget", "Tactic SelectTarget input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Tactic_NextTurn, "InputTag.Tactic.NextTurn", "Tactic NextTurn input.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Controller_SelectCharacterSlot1, "InputTag.Controller.SelectCharacterSlot1", "Select Character Slot1 input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Controller_SelectCharacterSlot2, "InputTag.Controller.SelectCharacterSlot2", "Select Character Slot2 input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Controller_SelectCharacterSlot3, "InputTag.Controller.SelectCharacterSlot3", "Select Character Slot3 input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Controller_SelectCharacterSlot4, "InputTag.Controller.SelectCharacterSlot4", "Select Character Slot4 input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Controller_SwitchCompanionsFollowState, "InputTag.Controller.SwitchCompanionsFollowState", "Switch Companions Follow State input.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Death, "GameplayEvent.Death", "Event that fires on death. This event only fires on the server.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Damage, "SetByCaller.Damage", "SetByCaller tag used by damage gameplay effects.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Heal, "SetByCaller.Heal", "SetByCaller tag used by healing gameplay effects.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cheat_GodMode, "Cheat.GodMode", "GodMode cheat is active on the owner.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cheat_UnlimitedHealth, "Cheat.UnlimitedHealth", "UnlimitedHealth cheat is active on the owner.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death, "Status.Death", "Target has the death status.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death_Dying, "Status.Death.Dying", "Target has begun the death process.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death_Dead, "Status.Death.Dead", "Target has finished the death process.");

	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString)
	{
		const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
		FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

		if (!Tag.IsValid() && bMatchPartialString)
		{
			FGameplayTagContainer AllTags;
			Manager.RequestAllGameplayTags(AllTags, true);

			for (const FGameplayTag& TestTag : AllTags)
			{
				if (TestTag.ToString().Contains(TagString))
				{
					UE_LOG(LogRos, Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."), *TagString, *TestTag.ToString());
					Tag = TestTag;
					break;
				}
			}
		}

		return Tag;
	}
}
