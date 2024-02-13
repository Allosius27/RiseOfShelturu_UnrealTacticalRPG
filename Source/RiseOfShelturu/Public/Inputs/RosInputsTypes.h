#pragma once
#include "GameplayTagContainer.h"
#include "UObject/ObjectMacros.h"

#include "RosInputsTypes.generated.h"

class UInputAction;
/**
 * FRosInputAction
 *
 */
USTRUCT(BlueprintType)
struct FRosInputAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};
