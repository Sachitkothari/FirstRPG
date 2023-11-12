// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	health = 1.00f;
	hasTakenDamage = false;
	isDead = false;
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyActor::TakeDamage(float _damage)
{
	health -= _damage;

	if (health <= 0.0f)
	{
		isDead = true;
	}
	else
	{
		hasTakenDamage = true;
	}
}

