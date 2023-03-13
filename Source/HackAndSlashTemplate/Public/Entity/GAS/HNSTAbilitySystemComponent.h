// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "HNSTAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UHNSTAbilitySystemComponent*, Instigator, float, UnmitigatedDamage, float, MitigatedDamage);

/**
 * 
 */
UCLASS()
class HACKANDSLASHTEMPLATE_API UHNSTAbilitySystemComponent : public UAbilitySystemComponent
{
private:
	GENERATED_BODY()
public:
	bool bAbilitiesGiven = false;
	bool bStartupEffectsApplied = false;

	FReceivedDamageDelegate ReceivedDamage;
	
	virtual void ReceiveDamage(UHNSTAbilitySystemComponent* Instigator, float UnmitigatedDamage, float MitigatedDamage);
};
