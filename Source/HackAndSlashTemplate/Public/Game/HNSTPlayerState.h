// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Entity/GAS/HNSTAttributeSet.h"
#include "GameFramework/PlayerState.h"
#include "HNSTPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HACKANDSLASHTEMPLATE_API AHNSTPlayerState : public APlayerState, public IAbilitySystemInterface
{
private:
	GENERATED_BODY()
protected:
	UPROPERTY()
		class UHNSTAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY()
		class UHNSTAttributeSet* AttributeSet;

	FGameplayTag DeadTag;

	//Handles
	FDelegateHandle CurrentHealthChangedHandle;
	FDelegateHandle MaxHealthChangedHandle;
	FDelegateHandle CurrentStaminaChangedHandle;
	FDelegateHandle MaxStaminaChangedHandle;
	FDelegateHandle EntityLevelChangedHandle;
	FDelegateHandle PhysicalDamageChangedHandle;
	FDelegateHandle AbilityDamageChangedHandle;

	//Notifies
	virtual void CurrentHealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void CurrentStaminaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxStaminaChanged(const FOnAttributeChangeData& Data);
	virtual void EntityLevelChanged(const FOnAttributeChangeData& Data);
	virtual void PhysicalDamageChanged(const FOnAttributeChangeData& Data);
	virtual void AbilityDamageChanged(const FOnAttributeChangeData& Data);

	//Called when we either acquire or lose the stun tag.
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	virtual void BeginPlay() override;
	
public:
	AHNSTPlayerState();
	
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	class UHNSTAttributeSet* GetAttributeSet() const;

	UFUNCTION(BlueprintCallable, Category = "Player|HNSTPlayerState")
		bool IsAlive() const;
	UFUNCTION(BlueprintCallable, Category = "Player|HNSTPlayerState|UI")
		void ShowAbilityConfirmCancelText(bool ShowText);

	//Getters
	UFUNCTION(BlueprintCallable, Category = "Player|HNSTPlayerState|Attributes")
		float GetCurrentHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Player|HNSTPlayerState|Attributes")
		float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Player|HNSTPlayerState|Attributes")
		float GetCurrentStamina() const;
	UFUNCTION(BlueprintCallable, Category = "Player|HNSTPlayerState|Attributes")
		float GetMaxStamina() const;
	UFUNCTION(BlueprintCallable, Category = "Player|HNSTPlayerState|Attributes")
		int32 GetEntityLevel() const;
	UFUNCTION(BlueprintCallable, Category = "Player|HNSTPlayerState|Attributes")
		float GetPhysicalDamage() const;
	UFUNCTION(BlueprintCallable, Category = "Player|HNSTPlayerState|Attributes")
		float GetAbilityDamage() const;
};
