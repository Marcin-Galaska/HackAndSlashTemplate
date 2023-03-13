// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/GAS/HNSTGameplayAbility.h"

UHNSTGameplayAbility::UHNSTGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	//If your character has any of these tags, they will be unable to cast this ability.
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")));
}

void UHNSTGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if(bActivateAbilityOnGranted)
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
}
