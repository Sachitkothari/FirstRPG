// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstRPGCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AFirstRPGCharacter

AFirstRPGCharacter::AFirstRPGCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	isSprinting = false;

	playerHealth = 1.00f;
	playerStamina = 1.00f;

	isOverlappingItem = false;
	hasArmor = true;

	playerArmor = 1.00f;

	isZoomedIn = false;

	hasPunched = false;

	currentLevel = 1;
	upgradePoints = 5;

	strengthValue = 1;
	dexterityValue = 1;
	intellectValue = 1;

	experiencePoints = 0.0f;
	experienceToLevel = 2000.0f;

	attackSpeed = 1.0f;
}

void AFirstRPGCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFirstRPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AFirstRPGCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AFirstRPGCharacter::StopSprinting);

		//Heal & damage
		EnhancedInputComponent->BindAction(HealAction, ETriggerEvent::Started, this, &AFirstRPGCharacter::StartHealing);
		EnhancedInputComponent->BindAction(DamageAction, ETriggerEvent::Started, this, &AFirstRPGCharacter::StartDamage);

		//Heal armor
		//EnhancedInputComponent->BindAction(ArmorHealAction, ETriggerEvent::Started, this, &AFirstRPGCharacter::HealArmor);

		//Equip an item
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &AFirstRPGCharacter::EquipItem);

		//Punching
		EnhancedInputComponent->BindAction(PunchAction, ETriggerEvent::Started, this, &AFirstRPGCharacter::Punch);

		//Stamina gain and consumption
		EnhancedInputComponent->BindAction(StaminaAddAction, ETriggerEvent::Started, this, &AFirstRPGCharacter::StartPlusStamina);
		EnhancedInputComponent->BindAction(StaminaMinusAction, ETriggerEvent::Started, this, &AFirstRPGCharacter::StartMinusStamina);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFirstRPGCharacter::Move);

		//Zoom in and out
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &AFirstRPGCharacter::ZoomIn);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &AFirstRPGCharacter::StopZoom);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFirstRPGCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AFirstRPGCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AFirstRPGCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFirstRPGCharacter::Sprint(const FInputActionValue& Value)
{
	isSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = 1500.0f;
}

void AFirstRPGCharacter::StopSprinting(const FInputActionValue& Value)
{
	isSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void AFirstRPGCharacter::StartDamage() 
{
	TakeDamage(0.02f);
}

void AFirstRPGCharacter::StartHealing()
{
	Heal(0.02f);
}

void AFirstRPGCharacter::HealArmor(float _healAmount)
{
	playerArmor += _healAmount;
	hasArmor = true;
	if (playerArmor > 1.00f) {
		playerArmor = 1.00f;
	}
}

void AFirstRPGCharacter::TakeDamage(float _damageAmount)
{
	if (hasArmor) 
	{
		playerArmor -= _damageAmount;
		if (playerArmor < 0.00f) 
		{
			playerHealth += playerArmor;
			playerArmor = 0.00f;
			hasArmor = false;
		}
	}
	else 
	{
		playerHealth -= _damageAmount;
		if (playerHealth < 0.00f) {
			playerHealth = 0.00f;
		}
	}
	
}

void AFirstRPGCharacter::Heal(float _healAmount)
{
	playerHealth += _healAmount;
	if (playerHealth > 1.00f) {
		playerHealth = 1.00f;
	}
}

void AFirstRPGCharacter::StartPlusStamina()
{
	PlusStamina(0.02f);
}

void AFirstRPGCharacter::StartMinusStamina()
{
	MinusStamina(0.02f);
}

void AFirstRPGCharacter::MinusStamina(float _staminaAmount)
{
	playerStamina -= _staminaAmount;
	if (playerStamina < 0.00f) {
		playerStamina = 0.00f;
	}
}

void AFirstRPGCharacter::PlusStamina(float _staminaAmount)
{
	playerStamina += _staminaAmount;
	if (playerStamina > 1.00f) {
		playerStamina = 1.00f;
	}
}

void AFirstRPGCharacter::EquipItem() 
{
	if (isOverlappingItem) {
		UE_LOG(LogTemp, Warning, TEXT("We picked up an item"));
	}
}

void AFirstRPGCharacter::ZoomIn()
{
	if (auto thirdPersonCamera = GetCameraBoom())
	{
		thirdPersonCamera->TargetArmLength = 150.0f;
		thirdPersonCamera->TargetOffset = FVector(0.0f, 80.0f, 70.0f);

		if (auto characterMovement = GetCharacterMovement())
		{
			characterMovement->MaxWalkSpeed = 300.0f;
		}

		isZoomedIn = true;
	}
}

void AFirstRPGCharacter::StopZoom()
{
	if (auto thirdPersonCamera = GetCameraBoom())
	{
		thirdPersonCamera->TargetArmLength = 300.0f;
		thirdPersonCamera->TargetOffset = FVector(0.0f, 0.0f, 0.0f);
		if (auto characterMovement = GetCharacterMovement())
		{
			characterMovement->MaxWalkSpeed = 600.0f;
		}

		isZoomedIn = false;
	}
}

void AFirstRPGCharacter::GainExperience(float _expAmount)
{
	experiencePoints += _expAmount;

	if (experiencePoints >= experienceToLevel)
	{
		experiencePoints -= experienceToLevel;
		experienceToLevel += 500.0f;
		currentLevel++;
	}
}

void AFirstRPGCharacter::Punch()
{
	hasPunched = true;
}

void AFirstRPGCharacter::AddToInventory(ADefaultItem* _item)
{
	inventory.itemList.Add(_item);
}