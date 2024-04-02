// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ABAT_JumpAndWaitForLanding.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpAndWaitForLandingDelegate);
/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABAT_JumpAndWaitForLanding : public UAbilityTask
{
	GENERATED_BODY()

public :
	UABAT_JumpAndWaitForLanding();

	//지금은 변수 설정이 따로 있는 것이 아니기 떄문에 인자가 없기는 하지만 사실, 여기서 인자를 설정하는 단계를 거치면 된다.
	UFUNCTION(BlueprintCallable, Category= "Ability | Tasks", meta = (DisplayName = "JumpAndWaitForLanding",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UABAT_JumpAndWaitForLanding* CreateTask(UGameplayAbility* OwningAbility);

	//발동 될 때 호출이 될 함수
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

	UPROPERTY(BlueprintAssignable)
	FJumpAndWaitForLandingDelegate OnComplete;

protected :
	UFUNCTION()
	void OnLandedCallback(const FHitResult& Hit);
};
