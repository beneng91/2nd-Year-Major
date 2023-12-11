// Fill out your copyright notice in the Description page of Project Settings.


#include "Timer.h"

#define PrintString(String) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White,String)

// Sets default values
ATimer::ATimer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATimer::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATimer::CountDown, 3.0f, true, 1.0f);
	
}

// Called every frame
void ATimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATimer::CountDown()
{
	CallTracker--;

	CallTracker == 0 ? PrintString("Finished looping"), GetWorldTimerManager().ClearTimer(TimerHandle) : PrintString("Timer Called");

	PrintString(FString::Printf(TEXT("Calls remaining: %d"), CallTracker));
}

