// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/ABTA_Trace.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Physics/ABCollision.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ArenaBattleGAS.h"

AABTA_Trace::AABTA_Trace()
{
}

void AABTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AABTA_Trace::ConfirmTargetingAndContinue()
{
	//확정적으로 Target을 보는 것.
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle AABTA_Trace::MakeTargetData() const
{
	//이제 실제로 TargetData를 가져올 것임.
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		ABGAS_LOG(LogABGAS, Error, TEXT("ASC NOT FOUND!"));
		return FGameplayAbilityTargetDataHandle();
	}

	const UABCharacterAttributeSet* AttributeSet = ASC->GetSet<UABCharacterAttributeSet>();

	if (!AttributeSet)
	{
		ABGAS_LOG(LogABGAS, Error, TEXT("ABCharacterAttributeSet not Found"));
		return FGameplayAbilityTargetDataHandle();
	}
	
	FHitResult OutHitResult;

	//해당 부분은 이제 차후 Attribute를 이용하여 주입을 할 예정임.
	const float AttackRange = AttributeSet->GetAttackRange();
	const float AttackRadius = AttributeSet->GetAttackRadius();
	
	//Trace를 위한 기초작업 정리.
	FCollisionQueryParams Params(SCENE_QUERY_STAT(UABTA_Trace), false, Character);
	const FVector Forward = Character->GetActorForwardVector();
	const FVector Start = Character->GetActorLocation() + Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + Forward * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

	FGameplayAbilityTargetDataHandle DataHandle;

	if (HitDetected)
	{
		//Hit에 대해서 넣어줄건데 해당 데이터의 경우에는 HitResult만 넣어주면 되기 떄문에 아주 쉬움.
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		//여기서 선언을 하면 사라질 수 있다고 생각하겠지만 SharedPtr이기 때문에 누군가가 홀딩하고 있는 한 사라지지는 않음.
		DataHandle.Add(TargetData);
	}

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = AttackRange * 0.5f;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);
	}
#endif

	return DataHandle;
}
