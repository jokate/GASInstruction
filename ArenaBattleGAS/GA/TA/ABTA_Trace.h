// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "ABTA_Trace.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API AABTA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
public :
	AABTA_Trace();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	void SetShowDebug(bool InShowDebug) { bShowDebug = InShowDebug; } 

	//Confirm 시 물리 판정을 진행시켜야 한다.
	virtual void ConfirmTargetingAndContinue() override;

	
protected :
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

	bool bShowDebug = false;
};
