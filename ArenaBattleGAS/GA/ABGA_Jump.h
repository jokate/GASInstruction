// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ABGA_Jump.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABGA_Jump : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UABGA_Jump();

public :
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	//어빌리티를 실행할 수 있는 지 없는지에 대해서 여부를 묻는 함수.
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

protected :
	UFUNCTION()
	virtual void OnLandedCallback();
};
