// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HNSTPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HACKANDSLASHTEMPLATE_API AHNSTPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void OnPossess(APawn* InPawn) override;
};
