// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "ABGASItemBox.generated.h"

UCLASS()
class ARENABATTLEGAS_API AABGASItemBox : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AABGASItemBox();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//액터가 부딪혔을 때를 판정.
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
protected :
	virtual void PostInitializeComponents() override;

	void ApplyEffectToTarget(AActor* Target);

	void InvokeGameplayCue(AActor* Target);
	
protected :
	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> GameplayEffect;

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (Categories=GameplayCue))
	FGameplayTag GameplayCueTag;
};


