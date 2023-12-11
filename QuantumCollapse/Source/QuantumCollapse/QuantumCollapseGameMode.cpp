// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuantumCollapseGameMode.h"
#include "QuantumCollapseCharacter.h"
#include "UObject/ConstructorHelpers.h"

AQuantumCollapseGameMode::AQuantumCollapseGameMode()
{
	PlayerCharacterClass = AQuantumCollapseCharacter::StaticClass();

	UniqueGamepadCount = 0;
	bIsSpawningPlayers = false;
}

void AQuantumCollapseGameMode::BeginPlay()
{
	Super::BeginPlay();

	if(bIsSpawningPlayers)
		SpawnPlayers();
}

void AQuantumCollapseGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AQuantumCollapseGameMode::SpawnPlayers()
{
	bIsSpawningPlayers = false;
}

void AQuantumCollapseGameMode::ResetButtonPressed()
{
	GamepadToControllerMap.Empty();
	UniqueGamepadCount = 0;
}

void AQuantumCollapseGameMode::HandleGamepadButtonPressed(APlayerController* Controller)
{
	if(!GamepadToControllerMap.Contains(Controller->GetLocalPlayer()->GetControllerId()))
	{
		GamepadToControllerMap.Add(Controller->GetLocalPlayer()->GetControllerId(), Controller);

		UniqueGamepadCount++;
	}
}

void AQuantumCollapseGameMode::HandleGamepadButtonRelease(APlayerController* Controller)
{
	if(GamepadToControllerMap.Contains(Controller->GetLocalPlayer()->GetControllerId()))
	{
		GamepadToControllerMap.Remove(Controller->GetLocalPlayer()->GetControllerId());

		UniqueGamepadCount--;
	}
}
