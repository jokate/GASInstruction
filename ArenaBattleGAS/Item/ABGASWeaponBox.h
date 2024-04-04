// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABGASItemBox.h"
#include "ABGASWeaponBox.generated.h"

UCLASS()
class ARENABATTLEGAS_API AABGASWeaponBox : public AABGASItemBox
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AABGASWeaponBox();

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	UPROPERTY(EditAnywhere, Category = "GAS", Meta=(Categories = Event))
	FGameplayTag WeaponEventTag;
};
