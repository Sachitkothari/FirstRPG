// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "DefaultWeapon.h"
#include "DefaultItem.h"
#include "FirstRPGCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

USTRUCT(BlueprintType)
struct FInventory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float weightLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ADefaultItem*> itemList;
};

UCLASS(config=Game)
class AFirstRPGCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	//Sprinting
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	//Healing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* HealAction;

	//Healing armor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ArmorHealAction;

	//Damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DamageAction;

	//Equiping items
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EquipAction;

	//Fill Stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* StaminaAddAction;

	//Consume Stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* StaminaMinusAction;

	//Zoom in and out
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ZoomAction;

	//Punching
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PunchAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	AFirstRPGCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	//Start and stop sprinting
	void Sprint(const FInputActionValue& Value);
	void StopSprinting(const FInputActionValue& Value);

	//Item equipment
	void EquipItem();

	//Gaining experience points
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void GainExperience(float _expAmount);

	//Healing
	void StartHealing();
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float _healAmount);

	//Damaging
	void StartDamage();
	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(float _damageAmount);

	//Healing shields
	UFUNCTION(BlueprintCallable, Category = "Health")
	void HealArmor(float _healAmount);

	//Stamina addition and removal
	void StartPlusStamina();
	void StartMinusStamina();
	void PlusStamina(float _staminaAmount);
	void MinusStamina(float _staminaAmount);

	//Adding items to inventory
	UFUNCTION(BlueprintCallable, Category = "Item")
	void AddToInventory(ADefaultItem* _item);

	//Zooming in and stopping the zoom
	void ZoomIn();
	void StopZoom();

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	//Punching
	void Punch();

	//Is character currently punshing?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool hasPunched;
	
	//Variable for whether player is sprinting
	bool isSprinting;

	//Variable checking if player has a armor
	bool hasArmor;

	//Variable tracking is the character is overlapping an equipable item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	bool isOverlappingItem;

	//Variable tracking current health of player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float playerHealth;

	//Variable tracking current health of armor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float playerArmor;

	//Variable tracking current stamina of player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float playerStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool isZoomedIn;

	//Player Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int strengthValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int dexterityValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int intellectValue;

	//Player level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int currentLevel;

	//Upgrade points the character has
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int upgradePoints;

	//Experience points the character has
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float experiencePoints;

	//Experience points needed for level up
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float experienceToLevel;

	//The attack speed of the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float attackSpeed;

	//The currently equipped weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	ADefaultWeapon* currentWeapon;

	//The inventry structure for character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FInventory inventory;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:


	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

