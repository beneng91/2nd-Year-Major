// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "QuantumCollapseGameMode.generated.h"

UCLASS(minimalapi)
class AQuantumCollapseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AQuantumCollapseGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:

	TMap<int32, APlayerController*> GamepadToControllerMap;

	int32 UniqueGamepadCount;

	bool bIsSpawningPlayers;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	TSubclassOf<class AQuantumCollapseCharacter> PlayerCharacterClass;

	void SpawnPlayers();
	void ResetButtonPressed();

public:
	void HandleGamepadButtonPressed(APlayerController* Controller);
	void HandleGamepadButtonRelease(APlayerController* Controller);
};



