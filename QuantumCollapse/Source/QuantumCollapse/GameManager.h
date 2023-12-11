// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool moveCamera = true;

	UFUNCTION(BlueprintCallable)
	FVector FurthestSpawnPoint(TArray<FVector> playerPositions, TArray<FVector> possibleSpawnPoints);
};
