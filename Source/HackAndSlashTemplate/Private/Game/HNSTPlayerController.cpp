// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HNSTPlayerController.h"
#include "AbilitySystemComponent.h"
#include "Game/HNSTPlayerState.h"

void AHNSTPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(const AHNSTPlayerState* TempPlayerState = GetPlayerState<AHNSTPlayerState>())
	{
		TempPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerState, InPawn);
	}
}