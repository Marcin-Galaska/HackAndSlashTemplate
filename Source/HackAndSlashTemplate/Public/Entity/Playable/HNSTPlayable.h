// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DecalComponent.h"
#include "Entity/HNSTEntity.h"
#include "HNSTPlayable.generated.h"

/**
 * 
 */
UCLASS()
class HACKANDSLASHTEMPLATE_API AHNSTPlayable : public AHNSTEntity
{
private:
	GENERATED_BODY()
protected:
	//If you plan on going with the isometric camera, the camera inputs are obsolete
	//I will however leave them in case you decide to toy with the camera
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Playable|Camera")
		float BaseTurnRate = 45.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Playable|Camera")
		float BaseLookUpRate = 45.0f;

	//Camera transform settings
	UPROPERTY(BlueprintReadOnly, Category = "Playable|Camera")
		float StartingCameraBoomArmLength;
	UPROPERTY(BlueprintReadOnly, Category = "Playable|Camera")
		FVector StartingCameraBoomLocation;
	UPROPERTY(BlueprintReadOnly, Category = "Playable|Camera")
		FRotator StartingCameraBoomRotation;

	//Components
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Playable|Camera")
		class USpringArmComponent* CameraBoom;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Playable|Camera")
		class UCameraComponent* FollowCamera;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Playable|Camera")
		class UDecalComponent* CursorToWorld;

	//Mouse
	void Turn(float Value);
	void LookUp(float Value);
	
	//Gamepad
	void LookUpRate(float Value);
	void TurnRate(float Value);

	//Movement
	void MoveForward(float Value);
	void MoveRight(float Value);

	//Helpers
	bool bASCInputBound = false;
	void BindASCInput();
	void InitializeASC(class AHNSTPlayerState* TempPlayerState);

	FGameplayTag DeadTag;

	virtual void BeginPlay() override;
	virtual void OnRep_PlayerState() override;
public:
	AHNSTPlayable(const class FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaSeconds) override;

	//Getters
	class USpringArmComponent* GetCameraBoom() const;
	class UCameraComponent* GetFollowCamera() const;
	class UDecalComponent* GetCursorToWorld() const;

	UFUNCTION(BlueprintCallable, Category = "Playable|Camera")
		float GetStartingCameraBoomArmLength() const;
	UFUNCTION(BlueprintCallable, Category = "Playable|Camera")
		FVector GetStartingCameraBoomLocation() const;
	UFUNCTION(BlueprintCallable, Category = "Playable|Camera")
		FRotator GetStartingCameraBoomRotation() const;
};
