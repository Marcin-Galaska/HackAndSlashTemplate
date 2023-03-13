// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "HNSTAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class HACKANDSLASHTEMPLATE_API UHNSTAssetManager : public UAssetManager
{
private:
	GENERATED_BODY()
public:
    virtual void StartInitialLoading() override;
};
