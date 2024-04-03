#pragma once

#include "GameplayTagContainer.h"


//태그가 있는지 검사를 할 수 있음.
#define ABTAG_ACTOR_ROTATE FGameplayTag::RequestGameplayTag(FName("Actor.Action.Rotate"))
#define ABTAG_STATE_ISROTATING FGameplayTag::RequestGameplayTag(FName("Actor.State.IsRotating"))
#define ABTAG_DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))
