// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGASWeaponBox.h"
#include "AbilitySystemBlueprintLibrary.h"

// Sets default values
AABGASWeaponBox::AABGASWeaponBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AABGASWeaponBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, WeaponEventTag, FGameplayEventData());
}


