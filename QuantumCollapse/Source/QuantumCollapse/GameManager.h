// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "Containers/UnrealString.h"
#include "PhysicsCharacterSC.h"
#include "SpawnPoint.h"
#include "GameManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUANTUMCOLLAPSE_API UGameManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGameManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	FVector FindCameraPosition(TArray<FVector> playerPositions);

	UFUNCTION(BlueprintCallable)
	bool CheckCameraPositionValid(FVector camPosition, float xBound, float yBound, UStaticMeshComponent* anchorPoint);

	UFUNCTION(BlueprintCallable)
	TArray<FVector> CheckPlayerPositionsValid(TArray<FVector> positions, float xBound, float yBound, UStaticMeshComponent* anchorPoint);

	UFUNCTION(BlueprintCallable)
	TArray<UPhysicsCharacterSC*> ScoreLosers(TArray<UPhysicsCharacterSC*> players);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool moveCamera = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool scoreGameFinished = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPhysicsCharacterSC* winner;

	UFUNCTION(BlueprintCallable)
	FVector FurthestSpawnPoint(TArray<FVector> playerPositions, TArray<FVector> possibleSpawnPoints);

	UFUNCTION(BlueprintCallable)
	UPhysicsCharacterSC* WinCondition(TArray<UPhysicsCharacterSC*> playerArray, bool livesGamemode, FName winScreenName);
	
	UFUNCTION(BlueprintCallable)
	FString TimerText(int minutes, int seconds);

	UFUNCTION(BlueprintCallable)
	void GameTimer(int gameTime);

	int Minutes = 1;
	int Seconds = 10;

	UFUNCTION(BlueprintCallable)
	void CountDown();

	UFUNCTION(BlueprintCallable)
	void LavaRising();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USpawnPoint*> spawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UPhysicsCharacterSC*> physicsPlayers;
};
