// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGC_AttackHit.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
UABGC_AttackHit::UABGC_AttackHit()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionRef(TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));

	if (ExplosionRef.Object)
	{
		ParticleSystem = ExplosionRef.Object;
	}
}


bool UABGC_AttackHit::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	//FGameplayCueParameter의 경우 많은 정보를 담고 있는데,
	//명령을 내리는 애가 어떤 값을 내리는지 관해서는 사전에 조율하는 것이 좋음.
	
	//일반적으로 HitResult는 Paramters의 EffectContext에 저장이 된 채로 옴.
	//근데 이제 이거를 넣어서 보내는 것이 일반적인지라 있을 수도 있고 없을 수도 있음.
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();

	if (HitResult)
	{
		UGameplayStatics::SpawnEmitterAtLocation(MyTarget, ParticleSystem, HitResult->ImpactPoint, FRotator::ZeroRotator, true);
	}

	return false;
}
