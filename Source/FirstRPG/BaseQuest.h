// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DefaultItem.h"
#include "MyActor.h"
#include "BaseQuest.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EQuestReward : uint8
{
	E_Default		UMETA(DisplayName = "DEFAULT"),
	E_Experience	UMETA(DisplayName = "EXPERIENCE"),
	E_Item			UMETA(DisplayName = "ITEM")
};

UENUM(BlueprintType)
enum class EClearCondition : uint8
{
	E_Default		UMETA(DisplayName = "DEFAULT"),
	E_Slay			UMETA(DisplayName = "SLAY"),
	E_Collect		UMETA(DisplayName = "COLLECT"),
	E_Travel		UMETA(DisplayName = "TRAVEL")
};

USTRUCT(BlueprintType)
struct FReward
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestReward rewardType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ADefaultItem> item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float experience;
};

USTRUCT(BlueprintType)
struct FObjective
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EClearCondition clearType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMyActor> enemyToSlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ADefaultItem> itemToCollect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int numRequired;
};


UCLASS(BlueprintType)
class FIRSTRPG_API UBaseQuest : public UObject
{
	GENERATED_BODY()
	
	UBaseQuest();

public:
	UFUNCTION(BlueprintCallable)
	void SetQuestDetails(FString _name, FString _description);

	UFUNCTION(BlueprintCallable)
	void SetUpObjective(int _objectiveNum, TSubclassOf<AMyActor> _enemy, TSubclassOf<ADefaultItem> _item, FString _description, int _numRequired);

	UFUNCTION(BlueprintCallable)
	void SetNumObjectives(int _numObjectives);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FReward reward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FObjective> objectives;
};
