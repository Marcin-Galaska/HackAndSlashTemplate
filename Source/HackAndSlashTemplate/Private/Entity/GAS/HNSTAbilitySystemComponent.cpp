// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/GAS/HNSTAbilitySystemComponent.h"

void UHNSTAbilitySystemComponent::ReceiveDamage(UHNSTAbilitySystemComponent* Instigator, float UnmitigatedDamage,
	float MitigatedDamage)
{
	ReceivedDamage.Broadcast(Instigator, UnmitigatedDamage, MitigatedDamage);
}
