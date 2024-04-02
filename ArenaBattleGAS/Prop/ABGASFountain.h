// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prop/ABFountain.h"
#include "AbilitySystemInterface.h"
#include "ABGASFountain.generated.h"

UCLASS()
class ARENABATTLEGAS_API AABGASFountain : public AABFountain, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AABGASFountain();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void TimerAction();

public:
	UPROPERTY(VisibleAnywhere, Category = " Movement")
	TObjectPtr<class URotatingMovementComponent> RotatingMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Timer Interval")
	float ActionPeriod;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;
	
	FTimerHandle ActionTimer;
};
