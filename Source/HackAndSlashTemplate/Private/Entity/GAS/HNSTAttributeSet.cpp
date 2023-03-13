// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/GAS/HNSTAttributeSet.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

void UHNSTAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UHNSTAttributeSet, CurrentHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHNSTAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHNSTAttributeSet, CurrentStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHNSTAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHNSTAttributeSet, EntityLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHNSTAttributeSet, PhysicalDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHNSTAttributeSet, AbilityDamage, COND_None, REPNOTIFY_Always);
}

void UHNSTAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHNSTAttributeSet, CurrentHealth, OldCurrentHealth);
}

void UHNSTAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHNSTAttributeSet, MaxHealth, OldMaxHealth);
}

void UHNSTAttributeSet::OnRep_CurrentStamina(const FGameplayAttributeData& OldCurrentStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHNSTAttributeSet, CurrentStamina, OldCurrentStamina);
}

void UHNSTAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHNSTAttributeSet, MaxStamina, OldMaxStamina);
}

void UHNSTAttributeSet::OnRep_EntityLevel(const FGameplayAttributeData& OldEntityLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHNSTAttributeSet, EntityLevel, OldEntityLevel);
}

void UHNSTAttributeSet::OnRep_PhysicalDamage(const FGameplayAttributeData& OldPhysicalDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHNSTAttributeSet, PhysicalDamage, OldPhysicalDamage);
}

void UHNSTAttributeSet::OnRep_AbilityDamage(const FGameplayAttributeData& OldAbilityDamage)

{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHNSTAttributeSet, AbilityDamage, OldAbilityDamage);
}
