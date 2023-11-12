// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultWeapon.h"

// Sets default values
ADefaultWeapon::ADefaultWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Default values for variables
	levelReq = 1;
	baseDamage = 10.0f;
	baseSpeed = 1.0f;
	weaponType = EWeaponType::E_Default;

}

// Called when the game starts or when spawned
void ADefaultWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADefaultWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

