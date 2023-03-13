// Fill out your copyright notice in the Description page of Project Settings.

#include "Entity/HNSTEntity.h"
#include <ciso646>
#include "GameplayAbilitySpecHandle.h"
#include "Components/CapsuleComponent.h"
#include "Entity/GAS/HNSTAbilitySystemComponent.h"
#include "Entity/GAS/HNSTAttributeSet.h"
#include "Entity/GAS/HNSTGameplayAbility.h"
#include "GameFramework/CharacterMovementComponent.h"

//Sets default values
AHNSTEntity::AHNSTEntity(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	//Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);
	
	bAlwaysRelevant = true;

	//Connect member variables to actual tags
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("State.RemoveOnDeath"));
}

//Called when the game starts or when spawned
void AHNSTEntity::BeginPlay()
{
	Super::BeginPlay();
}

//Gives all the default abilities we set in class defaults to our actor
void AHNSTEntity::GiveAbilities()
{
	//If we don't have role authority, we're not the server and thus shouldn't give the players abilities
	if(GetLocalRole() != ROLE_Authority or !AbilitySystemComponent.IsValid() or AbilitySystemComponent->bAbilitiesGiven)
		return;
	
	for(TSubclassOf<UHNSTGameplayAbility>& Startup_Ability : EntityAbilities)
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Startup_Ability, GetAbilityLevel(Startup_Ability.GetDefaultObject()->AbilityInputID),
			static_cast<int32>(Startup_Ability.GetDefaultObject()->AbilityInputID), this));
	
	AbilitySystemComponent->bAbilitiesGiven = true;
}

//Gives all the default attributes from the attribute set class we set in class defaults
void AHNSTEntity::InitializeAttributes()
{
	if(!AbilitySystemComponent.IsValid())
		return;

	if(!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("[DEBUG] %s() missing DefaultAttributes for %s. Please fill in the character blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	
	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetEntityLevel(), EffectContext);
	if(NewHandle.IsValid())
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
}

//Gives all the startup effects we set in class defaults
void AHNSTEntity::AddStartupEffects()
{
	if(GetLocalRole() != ROLE_Authority or !AbilitySystemComponent.IsValid() or AbilitySystemComponent->bStartupEffectsApplied)
		return;

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for(TSubclassOf<UGameplayEffect> effect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(effect, GetEntityLevel(), EffectContext);
		if(NewHandle.IsValid())
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}

	AbilitySystemComponent->bStartupEffectsApplied = true;
}

void AHNSTEntity::SetCurrentHealth(const float& NewCurrentHealth)
{
	if(AttributeSet.IsValid())
		AttributeSet->SetCurrentHealth(NewCurrentHealth);
}

void AHNSTEntity::SetMaxHealth(const float& NewMaxHealth)
{
	if(AttributeSet.IsValid())
		AttributeSet->SetMaxHealth(NewMaxHealth);
}

void AHNSTEntity::SetCurrentStamina(const float& NewCurrentStamina)
{
	if(AttributeSet.IsValid())
		AttributeSet->SetCurrentStamina(NewCurrentStamina);
}

void AHNSTEntity::SetMaxStamina(const float& NewMaxStamina)
{
	if(AttributeSet.IsValid())
		AttributeSet->SetMaxStamina(NewMaxStamina);
}

void AHNSTEntity::SetEntityLevel(const float& NewLevel)
{
	if(AttributeSet.IsValid())
		AttributeSet->SetEntityLevel(NewLevel);
}

void AHNSTEntity::SetPhysicalDamage(const float& NewPhysicalDamage)
{
	if(AttributeSet.IsValid())
		AttributeSet->SetPhysicalDamage(NewPhysicalDamage);
}

void AHNSTEntity::SetAbilityDamage(const float& NewAbilityDamage)
{
	if(AttributeSet.IsValid())
		AttributeSet->SetAbilityDamage(NewAbilityDamage);
}

UAbilitySystemComponent* AHNSTEntity::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

//Checks whether our health is above zero
bool AHNSTEntity::IsAlive() const
{
	return (GetCurrentHealth() > 0.0f) ? true : false;
}

//This is something that you can implement once you learn GAS, now let's simply hardcode it
int32 AHNSTEntity::GetAbilityLevel(EHNSTAbilityInputID ID) const
{
	return 1;
}

//Removes all abilities 
void AHNSTEntity::RemoveAbilities()
{
	if(GetLocalRole() != ROLE_Authority or !AbilitySystemComponent.IsValid() or !AbilitySystemComponent->bAbilitiesGiven)
		return;
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for(const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if(Spec.SourceObject == this and EntityAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Emplace(Spec.Handle);
		}
	}
	for(auto i=0; i<AbilitiesToRemove.Num(); ++i)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}
	AbilitySystemComponent->bAbilitiesGiven = false;
}

void AHNSTEntity::Die()
{
	RemoveAbilities();
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale=0;
	GetCharacterMovement()->Velocity = FVector(0);
	
	OnDied.Broadcast(this);

	if(AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAbilities();
		FGameplayTagContainer EffectsTagsToRemove;
		EffectsTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectsTagsToRemove);
		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	if(DeathMontage)
		PlayAnimMontage(DeathMontage);
	else
		FinishDying();
}

void AHNSTEntity::FinishDying()
{
	Destroy();
}

float AHNSTEntity::GetCurrentHealth() const
{
	if(AttributeSet.IsValid())
		return AttributeSet->GetCurrentHealth();
	return 0.0f;
}

float AHNSTEntity::GetMaxHealth() const
{
	if(AttributeSet.IsValid())
		return AttributeSet->GetMaxHealth();
	return 0.0f;
}

float AHNSTEntity::GetCurrentStamina() const
{
	if(AttributeSet.IsValid())
		return AttributeSet->GetCurrentStamina();
	return 0.0f;
}

float AHNSTEntity::GetMaxStamina() const
{
	if(AttributeSet.IsValid())
		return AttributeSet->GetMaxStamina();
	return 0.0f;
}

float AHNSTEntity::GetEntityLevel() const
{
	if(AttributeSet.IsValid())
		return AttributeSet->GetEntityLevel();
	return 0.0f;
}

float AHNSTEntity::GetPhysicalDamage() const
{
	if(AttributeSet.IsValid())
		return AttributeSet->GetPhysicalDamage();
	return 0.0f;
}

float AHNSTEntity::GetAbilityDamage() const
{
	if(AttributeSet.IsValid())
		return AttributeSet->GetAbilityDamage();
	return 0.0f;
}

//Called every frame
void AHNSTEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Called to bind functionality to input
void AHNSTEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

