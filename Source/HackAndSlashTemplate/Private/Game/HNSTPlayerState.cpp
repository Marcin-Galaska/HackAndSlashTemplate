// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HNSTPlayerState.h"
#include "GameplayEffectTypes.h"
#include "Entity/GAS/HNSTAbilitySystemComponent.h"
#include "Entity/GAS/HNSTAttributeSet.h"

void AHNSTPlayerState::CurrentHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("[DEBUG] Current Health Changed. (%s() called)."), *FString(__FUNCTION__));
}

void AHNSTPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("[DEBUG] Max Health Changed. (%s() called)."), *FString(__FUNCTION__));
}

void AHNSTPlayerState::CurrentStaminaChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("[DEBUG] Current Stamina Changed. (%s() called)."), *FString(__FUNCTION__));
}

void AHNSTPlayerState::MaxStaminaChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("[DEBUG] Max Stamina Changed. (%s() called)."), *FString(__FUNCTION__));
}

void AHNSTPlayerState::EntityLevelChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("[DEBUG] Entity Level Changed. (%s() called)."), *FString(__FUNCTION__));
}

void AHNSTPlayerState::PhysicalDamageChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("[DEBUG] Physical Damage Changed. (%s() called)."), *FString(__FUNCTION__));
}

void AHNSTPlayerState::AbilityDamageChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("[DEBUG] Ability Damage Changed. (%s() called)."), *FString(__FUNCTION__));
}

void AHNSTPlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if(NewCount > 0)
	{
		//Cancels normal abilities
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		//Stops uncancellable abilities from cancelling
		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCancelledByStun")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}

void AHNSTPlayerState::BeginPlay()
{
	Super::BeginPlay();

	//Boilerplate
	if(AbilitySystemComponent)
	{
		CurrentHealthChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCurrentHealthAttribute()).AddUObject(this,
			&AHNSTPlayerState::CurrentHealthChanged);
		MaxHealthChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddUObject(this,
			&AHNSTPlayerState::MaxHealthChanged);
		CurrentStaminaChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCurrentStaminaAttribute()).AddUObject(this,
			&AHNSTPlayerState::CurrentStaminaChanged);
		CurrentStaminaChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCurrentStaminaAttribute()).AddUObject(this,
			&AHNSTPlayerState::CurrentStaminaChanged);
		EntityLevelChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetEntityLevelAttribute()).AddUObject(this,
			&AHNSTPlayerState::EntityLevelChanged);
		PhysicalDamageChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetEntityLevelAttribute()).AddUObject(this,
			&AHNSTPlayerState::PhysicalDamageChanged);
		AbilityDamageChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetEntityLevelAttribute()).AddUObject(this,
			&AHNSTPlayerState::AbilityDamageChanged);

		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this,
			&AHNSTPlayerState::StunTagChanged);
	}
}

AHNSTPlayerState::AHNSTPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UHNSTAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); //If you are making a multiplayer project, best set it to Minimal.

	AttributeSet = CreateDefaultSubobject<UHNSTAttributeSet>(TEXT("BaseAttributeSet"));

	NetUpdateFrequency = 100.0f;
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent* AHNSTPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UHNSTAttributeSet* AHNSTPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

bool AHNSTPlayerState::IsAlive() const
{
	return (GetCurrentHealth() > 0.0f) ? true : false;
}

void AHNSTPlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{
	//This function helps you display ability confirmation info on your HUD.
	//Fill it only if you plan on making an user interface.
}

float AHNSTPlayerState::GetCurrentHealth() const
{
	return AttributeSet->GetCurrentHealth();
}

float AHNSTPlayerState::GetMaxHealth() const
{
	return AttributeSet->GetMaxStamina();
}

float AHNSTPlayerState::GetCurrentStamina() const
{
	return AttributeSet->GetCurrentStamina();
}

float AHNSTPlayerState::GetMaxStamina() const
{
	return AttributeSet->GetMaxStamina();
}

int32 AHNSTPlayerState::GetEntityLevel() const
{
	return AttributeSet->GetEntityLevel();
}

float AHNSTPlayerState::GetPhysicalDamage() const
{
	return AttributeSet->GetPhysicalDamage();
}

float AHNSTPlayerState::GetAbilityDamage() const
{
	return AttributeSet->GetAbilityDamage();
}
