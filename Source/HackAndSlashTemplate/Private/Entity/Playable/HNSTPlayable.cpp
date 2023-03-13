// Fill out your copyright notice in the Description page of Project Settings.

#include "Entity/Playable/HNSTPlayable.h"
#include <ciso646>
#include "GameplayEffectTypes.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Entity/GAS/HNSTAbilitySystemComponent.h"
#include "Entity/GAS/HNSTAttributeSet.h"
#include "Entity/GAS/HNSTGameplayAbility.h"
#include "Game/HNSTPlayerController.h"
#include "Game/HNSTPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

//Camera positioning inputs in case you drastically change the camera
void AHNSTPlayable::Turn(float Value)
{
	if(IsAlive())
		AddControllerYawInput(Value);
}

void AHNSTPlayable::LookUp(float Value)
{
	if(IsAlive())
		AddControllerPitchInput(Value);
}

void AHNSTPlayable::LookUpRate(float Value)
{
	if(IsAlive())
		AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->DeltaTimeSeconds);
}

void AHNSTPlayable::TurnRate(float Value)
{
	if(IsAlive())
		AddControllerYawInput(Value * BaseTurnRate * GetWorld()->DeltaTimeSeconds);
}

//Movement set to be diagonal relative to world for easier perspective application
void AHNSTPlayable::MoveForward(float Value)
{
	if(Controller and Value != 0.0f)
	{
		//Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//Get forward vector
		const FVector& ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//Get right vector 
		const FVector& RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		const FVector Direction = ForwardDirection - RightDirection;

		//Add input
		AddMovementInput(Direction, Value);
	}
}

void AHNSTPlayable::MoveRight(float Value)
{
	if(Controller and Value != 0.0f)
	{
		//Find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//Get forward vector
		const FVector& ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//Get right vector 
		const FVector& RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		const FVector Direction = ForwardDirection + RightDirection;

		//Add input
		AddMovementInput(Direction, Value);
	}
}

void AHNSTPlayable::BindASCInput()
{
	if(!bASCInputBound and AbilitySystemComponent.IsValid() and IsValid(InputComponent))
	{
		//Additional inputs, like Ability1 or Basic, are automatically bound.
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds
		(
			"Confirm","Cancel", "EHNSTAbilityInputID",
			static_cast<int32>(EHNSTAbilityInputID::Confirm),static_cast<int32>(EHNSTAbilityInputID::Cancel)
		));
		
		bASCInputBound = true;
	}
}

void AHNSTPlayable::InitializeASC(AHNSTPlayerState* TempPlayerState)
{
	//Initialize GAS
	AbilitySystemComponent = Cast<UHNSTAbilitySystemComponent>(TempPlayerState->GetAbilitySystemComponent());
	TempPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(TempPlayerState, this);
	AttributeSet = TempPlayerState->GetAttributeSet();
	
	AbilitySystemComponent->SetTagMapCount(DeadTag, 0);
	
	InitializeAttributes();
	
	SetCurrentHealth(GetMaxHealth());
	SetCurrentStamina(GetMaxStamina());
}

void AHNSTPlayable::BeginPlay()
{
	Super::BeginPlay();

	Controller->SetControlRotation(FRotator(180, 0, 0));

	//Save starting camera settings
	//Useful when you make abilities that change the camera position
	StartingCameraBoomArmLength = CameraBoom->TargetArmLength;
	StartingCameraBoomLocation = CameraBoom->GetRelativeLocation();
	StartingCameraBoomRotation = CameraBoom->GetComponentRotation();
}

//Replication
void AHNSTPlayable::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if(AHNSTPlayerState* TempPlayerState = GetPlayerState<AHNSTPlayerState>())
	{
		InitializeASC(TempPlayerState);
		BindASCInput();
	}
}

AHNSTPlayable::AHNSTPlayable(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	//Setup camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetAbsolute(false, true, false); //Sets rotation to being relative to world
	CameraBoom->bDoCollisionTest = false; //Makes it so the camera boom does not shorten when the camera hits something
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->SetRelativeRotation(FRotator(-45.0f,-45.0f,0.0f)); //Sets the camera in isometric view relative to actor

	//Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	//Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;

	//Setup camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->FieldOfView = 80.0f;
	FollowCamera->bUsePawnControlRotation = false;

	//Setup cursor to world
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

void AHNSTPlayable::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	//Uncomment these when changing camera, e.x. when switching from isometric to third person
	PlayerInputComponent->BindAxis("MoveForward", this, &AHNSTPlayable::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHNSTPlayable::MoveRight);
	//PlayerInputComponent->BindAxis("LookUp", this, &AHNSTPlayable::LookUp);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &AHNSTPlayable::LookUpRate);
	//PlayerInputComponent->BindAxis("Turn", this, &AHNSTPlayable::Turn);
	//PlayerInputComponent->BindAxis("TurnRate", this, &AHNSTPlayable::TurnRate);

	BindASCInput();
}

void AHNSTPlayable::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if(!IsValid(PlayerController))
		return;

	//Updates the cursor transform each frame
	if(CursorToWorld)
	{
		FHitResult TraceHitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
		FVector CursorFV = TraceHitResult.ImpactNormal;
		FRotator CursorR = CursorFV.Rotation();
		CursorToWorld->SetWorldLocation(TraceHitResult.Location);
		CursorToWorld->SetWorldRotation(CursorR);
	}
	
	FVector MouseLocation;
	FVector MouseDirection;

	//Rotates the actor towards the world cursor
	if(PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection))
	{
		FRotator MeshRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CursorToWorld->GetComponentLocation());
		MeshRotation.Pitch = 0;
		MeshRotation.Roll = 0;
		
		GetCharacterMovement()->MoveUpdatedComponent(FVector::ZeroVector, MeshRotation, false);
	}
}

void AHNSTPlayable::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if(AHNSTPlayerState* TempPlayerState = GetPlayerState<AHNSTPlayerState>())
	{
		InitializeASC(TempPlayerState);
		AddStartupEffects();
		GiveAbilities();
	}
}

USpringArmComponent* AHNSTPlayable::GetCameraBoom() const
{
	return CameraBoom;
}

UCameraComponent* AHNSTPlayable::GetFollowCamera() const
{
	return FollowCamera;
}

UDecalComponent* AHNSTPlayable::GetCursorToWorld() const
{
	return CursorToWorld;
}

float AHNSTPlayable::GetStartingCameraBoomArmLength() const
{
	return StartingCameraBoomArmLength;
}

FVector AHNSTPlayable::GetStartingCameraBoomLocation() const
{
	return StartingCameraBoomLocation;
}

FRotator AHNSTPlayable::GetStartingCameraBoomRotation() const
{
	return StartingCameraBoomRotation;
}
