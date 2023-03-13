// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "HackAndSlashTemplate/HackAndSlashTemplate.h"
#include "HNSTGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class HACKANDSLASHTEMPLATE_API UHNSTGameplayAbility : public UGameplayAbility
{
private:
	GENERATED_BODY()
public:
	UHNSTGameplayAbility();

	//Which input should this ability use.
	//Property which you HAVE TO set when making a derived blueprint class.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EHNSTAbilityInputID AbilityInputID = EHNSTAbilityInputID::None;

	//Whether this ability should be immediately activated when granted (like stun or knockback).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Ability")
	bool bActivateAbilityOnGranted = false;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
