// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Timer.generated.h"

UCLASS()
class QUANTUMCOLLAPSE_API ATimer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	int32 CallTracker = 3;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CountDown();
	FTimerHandle TimerHandle;

};
