// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAT_JumpAndWaitForLanding.h"
#include "GameFramework/Character.h"

UABAT_JumpAndWaitForLanding::UABAT_JumpAndWaitForLanding()
{
}

UABAT_JumpAndWaitForLanding* UABAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	UABAT_JumpAndWaitForLanding* NewTask = NewAbilityTask<UABAT_JumpAndWaitForLanding>(OwningAbility);
	return NewTask;
}

void UABAT_JumpAndWaitForLanding::Activate()
{
	Super::Activate();

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.AddDynamic(this, &UABAT_JumpAndWaitForLanding::OnLandedCallback);
	Character->Jump();
	
	//언제 점프가 끝날지 모르기 때문에, Waiting 형태로 만들어 준다.
	SetWaitingOnAvatar();
}

void UABAT_JumpAndWaitForLanding::OnDestroy(bool bInOwnerFinished)
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.RemoveDynamic(this, &UABAT_JumpAndWaitForLanding::OnLandedCallback);
	Super::OnDestroy(bInOwnerFinished);
}

void UABAT_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& Hit)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}
}
