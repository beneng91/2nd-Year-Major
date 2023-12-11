// Fill out your copyright notice in the Description page of Project Settings.


#include "GamepadMove.h"

#include <PhysXInterfaceWrapperCore.h>
#include <Kismet/GameplayStatics.h>

#include "QuantumCollapseCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values for this component's properties
UGamepadMove::UGamepadMove()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGamepadMove::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGamepadMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this,0);

	if(PlayerController)
		return;

	float moveVal = PlayerController->GetInputAnalogKeyState(EKeys::Gamepad_RightY);

	float moveSpeed = 100.0f;
//	FVector NewLocation = G() + FVector(moveVal * moveSpeed * DeltaTime, 0.0f, 0.0f);
	
	// ...
}

