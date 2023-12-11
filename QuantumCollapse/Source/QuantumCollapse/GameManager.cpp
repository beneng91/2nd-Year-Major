// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

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

	xValue = distances[distances.Num() - 1];

	camPosition = FVector(xValue, yValue, zValue);
	return camPosition;
}

bool UGameManager::CheckCameraPositionValid(FVector camPosition, float xBound, float yBound, UStaticMeshComponent* anchorPoint)
{
	FVector2D camPos = FVector2D(camPosition.Y,
								camPosition.Z);

	if (camPos.X > xBound + anchorPoint->GetComponentLocation().Y ||
		camPos.Y > yBound + anchorPoint->GetComponentLocation().Z ||
		camPos.X < -xBound + anchorPoint->GetComponentLocation().Y ||
		camPos.Y < -yBound + anchorPoint->GetComponentLocation().Z)
		return false;
	else
		return true;
}

FVector UGameManager::FurthestSpawnPoint(TArray<FVector> playerPositions, TArray<FVector> possibleSpawnPoints)
{
	FVector returnedPos = possibleSpawnPoints[0];

	float xValue = 0;
	float yValue = 0;
	float zValue = 0;

	for (int i = 0; i < playerPositions.Num(); i++)
	{
		xValue += playerPositions[i].X;
		yValue += playerPositions[i].Y;
		zValue += playerPositions[i].Z;
	}

	xValue = xValue / playerPositions.Num();
	yValue = yValue / playerPositions.Num();
	zValue = zValue / playerPositions.Num();

	FVector playersAvgPoint = FVector(xValue, yValue, zValue);

	for (int i = 1; i < possibleSpawnPoints.Num(); i++)
	{
		float dist1 = FVector::Dist(playersAvgPoint, possibleSpawnPoints[i]);
		float dist2 = FVector::Dist(playersAvgPoint, possibleSpawnPoints[i - 1]);

		if (dist1 > dist2)
			returnedPos = possibleSpawnPoints[i];
		else
			returnedPos = possibleSpawnPoints[i - 1];
	}

	return returnedPos;
}
