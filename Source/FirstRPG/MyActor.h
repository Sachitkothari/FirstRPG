// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyActor.generated.h"

UCLASS()
class FIRSTRPG_API AMyActor : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Taking Damage
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float _damageAmount);

	//Current Health of enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	float health;

	//Being hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	bool hasTakenDamage;

	//Death
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	bool isDead;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
