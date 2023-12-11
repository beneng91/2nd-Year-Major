// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeAttack.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UCubeAttack::UCubeAttack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	
	// ...
}


// Called when the game starts
void UCubeAttack::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCubeAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

void UCubeAttack::SwordSwing(float Value, float DeltaTime)
{
	CurrentOffset += Value * SwingSpeed * DeltaTime;
}

void UCubeAttack::SetSwingSpeed(float newSwingSpeed)
{
	SwingSpeed = newSwingSpeed;
}



