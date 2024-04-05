// Fill out your copyright notice in the Description page of Project Settings.


#include "ABTA_SphereMultiTrace.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ArenaBattleGAS.h"
#include "GameFramework/Character.h"
#include "Physics/ABCollision.h"
#include "DrawDebugHelpers.h"
#include "Attribute/ABCharacterSkillAttributeSet.h"
// Sets default values
AABTA_SphereMultiTrace::AABTA_SphereMultiTrace()
{
}

FGameplayAbilityTargetDataHandle AABTA_SphereMultiTrace::MakeTargetData() const
{
	//이제 실제로 TargetData를 가져올 것임.
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		ABGAS_LOG(LogABGAS, Error, TEXT("ASC NOT FOUND!"));
		return FGameplayAbilityTargetDataHandle();
	}
	
	const UABCharacterSkillAttributeSet* SkillAttributeSet = ASC->GetSet<UABCharacterSkillAttributeSet>();

	if (!SkillAttributeSet)
	{
		ABGAS_LOG(LogABGAS, Error, TEXT("Attribute NOT FOUND!"));
		return FGameplayAbilityTargetDataHandle();
	}
	//구체와 겹친 정보를 넣기 위해 다음과 같은 구조체 배열을 넣는다.
	TArray<FOverlapResult> Overlaps;
	const float SkillRange = SkillAttributeSet->GetSkillRange();
	
	FVector Origin = Character->GetActorLocation();

	FCollisionQueryParams Params(SCENE_QUERY_STAT(AABTA_SphereMultiTrace), false, Character);
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity,
		CCHANNEL_ABACTION, FCollisionShape::MakeSphere(SkillRange), Params);

	//Target Data Handle에 넣어줘야 함.
	//다수의 Actor을 넣는 것에 대해 적합한 데이터가 있음.
	TArray<TWeakObjectPtr<AActor>> HitActors;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.OverlapObjectHandle.FetchActor<AActor>();

		if (HitActor && !HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);
		}
	}
	
	FGameplayAbilityTargetData_ActorArray* ActorsData = new FGameplayAbilityTargetData_ActorArray();

	ActorsData->SetActors(HitActors);

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FColor DrawColor = HitActors.Num() > 0 ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), Origin, SkillRange, 16, DrawColor, false, 5.0f);
	}
#endif
	
	
	return FGameplayAbilityTargetDataHandle(ActorsData);
}

