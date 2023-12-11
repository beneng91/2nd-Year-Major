// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

#include <string>

#include "Kismet/GameplayStatics.h"
#include "Engine.h"

// Sets default values for this component's properties
UGameManager::UGameManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGameManager::BeginPlay()
{
	Super::BeginPlay();
	// ...
}


// Called every frame
void UGameManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FVector UGameManager::FindCameraPosition(TArray<FVector> playerPositions)
{
	moveCamera = true;
	FVector camPosition;
	
	float xValue = 0;
	float yValue = 0;
	float zValue = 0;

	// Location of the camera on a 2D plane
	for(int i = 0; i < playerPositions.Num(); i++)
	{
		yValue += playerPositions[i].Y;
		zValue += playerPositions[i].Z;
	}

	yValue = yValue / playerPositions.Num();
	zValue = zValue / playerPositions.Num();
	
	// Distance of the camera from the players
	TArray<float> distances;

	for (int i = 0; i < playerPositions.Num(); i++)
		for (int j = 0; j < playerPositions.Num(); j++)
			distances.Add(FVector::Dist(playerPositions[i], playerPositions[j]));

	distances.Sort();

	xValue = distances[distances.Num() - 1] - (distances[distances.Num() - 1] / 4);

	camPosition = FVector(xValue, yValue, zValue);
	return camPosition;
}

bool UGameManager::CheckCameraPositionValid(FVector camPosition, float xBound, float yBound, UStaticMeshComponent* anchorPoint)
{
	FVector2D camPos = FVector2D(camPosition.Y,
								camPosition.Z);

	if (camPos.X > xBound + anchorPoint->GetComponentLocation().Y ||
		camPos.Y > yBound + anchorPoint->GetComponentLocation().Z ||
		camPos.X < anchorPoint->GetComponentLocation().Y - xBound ||
		camPos.Y < anchorPoint->GetComponentLocation().Z - yBound)
		return false;
	else
		return true;
}

TArray<FVector> UGameManager::CheckPlayerPositionsValid(TArray<FVector> positions, float xBound, float yBound, UStaticMeshComponent* anchorPoint)
{
	TArray<FVector> returnedList;
	for(int i = 0; i < positions.Num(); i++)
	{
		FVector playerPos = positions[i];
		
		if (playerPos.Y > xBound + anchorPoint->GetComponentLocation().Y ||
		playerPos.Z > yBound + anchorPoint->GetComponentLocation().Z ||
		playerPos.Y < anchorPoint->GetComponentLocation().Y - xBound ||
		playerPos.Z < anchorPoint->GetComponentLocation().Z - yBound)
		{
			FVector newPos = positions[i];
			if(playerPos.Y > xBound + anchorPoint->GetComponentLocation().Y)
				newPos.Y = xBound + anchorPoint->GetComponentLocation().Y;
			if(playerPos.Z > yBound + anchorPoint->GetComponentLocation().Z )
				newPos.Z = yBound + anchorPoint->GetComponentLocation().Z;
			if(playerPos.Y < anchorPoint->GetComponentLocation().Y - xBound)
				newPos.Y = anchorPoint->GetComponentLocation().Y - xBound;
			if(playerPos.Z < anchorPoint->GetComponentLocation().Z - yBound)
				newPos.Z = anchorPoint->GetComponentLocation().Z - yBound;

			returnedList.Add(newPos);
		}
		else
			returnedList.Add(playerPos);
	}

	return returnedList;
}

FVector UGameManager::FurthestSpawnPoint(TArray<FVector> playerPositions, TArray<FVector> possibleSpawnPoints)
{
	TArray<FVector> validSpawns;
	TArray<FVector> invalidSpawns;
	for(int i = 0; i < playerPositions.Num(); i++)
	{
		float shortestDist = FVector::Dist(playerPositions[i], possibleSpawnPoints[0]);
		int index = 0;
		for(int j = 0; j < possibleSpawnPoints.Num(); j++)
		{
			float tempDist = FVector::Dist(playerPositions[i], possibleSpawnPoints[j]);

			if(tempDist < shortestDist)
			{
				shortestDist = tempDist;
				index = j;
			}
		}
		invalidSpawns.Add(possibleSpawnPoints[index]);
	}

	for(int i = 0; i < possibleSpawnPoints.Num(); i++)
	{
		bool canAdd = true;
		for(int j = 0; j < invalidSpawns.Num(); j++)
		{
			if(possibleSpawnPoints[i] == invalidSpawns[j])
				canAdd = false;
		}
		if(canAdd)
			validSpawns.Add(possibleSpawnPoints[i]);
	}

	FVector returnedSP;
	
	if(validSpawns.Num() > 0)
		returnedSP = validSpawns[FMath::RandRange(0, validSpawns.Num() - 1)];
		
	else
		returnedSP = possibleSpawnPoints[FMath::RandRange(0, possibleSpawnPoints.Num() - 1)];
	
	return returnedSP;
}

void UGameManager::GameTimer(int gameTime)
{
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UGameManager::LavaRising);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, gameTime, false);
}

void UGameManager::CountDown()
{

}

void UGameManager::LavaRising()
{

}

UPhysicsCharacterSC* UGameManager::WinCondition(TArray<UPhysicsCharacterSC*> playerArray, bool livesGamemode, FName winScreenName)
{
	TArray<UPhysicsCharacterSC*> players = playerArray;

	if(livesGamemode)
	{
		int playersWithLives = 0;
		for(int i = 0; i < playerArray.Num(); i++)
			if(playerArray[i]->lives > 0)
				playersWithLives++;

		if(playersWithLives == 1)
		{
			for(int i = 0; i < players.Num(); i++)
				if(players[i]->lives != 0)
					winner = players[i];
		}
	}
	else
	{
		if(scoreGameFinished)
		{
			players.Sort();
			winner = players[players.Num() - 1];
		}
	}

	return winner;
}

TArray<UPhysicsCharacterSC*> UGameManager::ScoreLosers(TArray<UPhysicsCharacterSC*> players)
{
	TArray<UPhysicsCharacterSC*> losersList = players;
	TArray<UPhysicsCharacterSC*> returnedList;
	losersList.Sort();

	for(int i = 0; i < losersList.Num() - 1; i++)
		returnedList.Add(losersList[i]);
	
	return returnedList;
}

FString UGameManager::TimerText(int minutes, int seconds)
{
	FString minuteText = FString::FromInt(minutes);
	FString secondText = FString::FromInt(seconds);
	if(minutes < 10)
		minuteText = "0" + FString::FromInt(minutes);
		
	if(seconds < 10)
		secondText = "0" + FString::FromInt(seconds);

	FString timerText = "[00:" + minuteText + ":" + secondText + "]";
	return timerText;
}