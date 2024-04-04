#pragma once

#include "GameplayTagContainer.h"


//태그가 있는지 검사를 할 수 있음.
#define ABTAG_ACTOR_ROTATE FGameplayTag::RequestGameplayTag(FName("Actor.Action.Rotate"))
#define ABTAG_STATE_ISROTATING FGameplayTag::RequestGameplayTag(FName("Actor.State.IsRotating"))
#define ABTAG_DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))
#define ABTAG_Character_ISDEAD FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"))
#define ABTAG_Character_INVINSIBLE FGameplayTag::RequestGameplayTag(FName("Character.State.Invinsible"))

#define GAMEPLAYCUE_CHARACTER_ATTACKHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.AttackHit"))
#define ABTAG_EVENT_CHARACTER_WEAPONEQUIP FGameplayTag::RequestGameplayTag(FName("Event.Character.Weapon.Equip"))
#define ABTAG_EVENT_CHARACTER_WEAPONUNEQUIP FGameplayTag::RequestGameplayTag(FName("Event.Character.Weapon.UnEquip"))
