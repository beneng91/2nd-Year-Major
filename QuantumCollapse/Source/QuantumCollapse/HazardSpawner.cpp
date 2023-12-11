// Fill out your copyright notice in the Description page of Project Settings.


#include "HazardSpawner.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UHazardSpawner::UHazardSpawner()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHazardSpawner::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHazardSpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FVector UHazardSpawner::GetHazardSpawn(bool random, UBoxComponent* bounds, FVector setSpawn)
{
	FVector returnedPos = setSpawn;
	if (random)
	{
		FVector extents = FVector(bounds->GetScaledBoxExtent());
		returnedPos = UKismetMathLibrary::RandomPointInBoundingBox(setSpawn, extents);
	}
	returnedPos.X = setSpawn.X;
	return returnedPos;
}

