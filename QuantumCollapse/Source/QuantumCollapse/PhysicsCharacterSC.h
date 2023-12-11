// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameManager.h"
#include "SpawnPoint.h"
#include "PhysicsCharacterSC.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUANTUMCOLLAPSE_API UPhysicsCharacterSC : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPhysicsCharacterSC();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Functions
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	FHitResult RaycastPhysicsChar(UArrowComponent* arrow, float grappleRange, bool ballRaycast);

	UFUNCTION(BlueprintCallable)
	FVector CalculateGrappleForcePhysicsChar(float xInputAxis, float grappleForce);

	UFUNCTION(BlueprintCallable)
	void RopeCollision();

	UFUNCTION(BlueprintCallable)
	void DeathFunction();

	UFUNCTION(BlueprintCallable)
	void RespawnFunction(FVector spawnPos);

	UFUNCTION(BlueprintCallable)
	void RespawnPlayer(UGameManager* gameManager, int respawnDelay);

	UFUNCTION(BlueprintCallable)
	void WallClimbing(float upInput, float climbSpeed, UBoxComponent* box);

	UFUNCTION(BlueprintCallable)
	void Movement(float deltaTime, float moveSpeed);

	UFUNCTION(BlueprintCallable)
	void JumpFunction(float midairSpeed, float jumpHeight, float wallJumpHeight, UBoxComponent* box, UArrowComponent* arrow);

	UFUNCTION(BlueprintCallable)
	void CollisionCheck(UBoxComponent* box);

	UFUNCTION(BlueprintCallable)
	void GrappleCooldown(float cooldownLength);

	UFUNCTION(BlueprintCallable)
	void LungeCooldown(float cooldownLength);

	UFUNCTION(BlueprintCallable)
	void PhysicsPlayerOffScreenRespawn(UGameManager* gameManager, int delay);

	UFUNCTION(BlueprintCallable)
	void ResetSpawnedBool();

	UFUNCTION(BlueprintCallable)
	FVector GenerateSpawnPos(UGameManager* gameManager);

	UFUNCTION(BlueprintCallable)
	bool PauseManager(UGameManager* gameManager);
	
	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector raycastHitPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool hasHitGrapple;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector grappleAnchorPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ropeHasCollided = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool spawned = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int lives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsClimbing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool leftCollision = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool rightCollision = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool upCollision = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool onGround = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector movementDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool hasChangedPoint = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool canClimb = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool jumping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool wallJumping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool livesGamemode = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool grappleCooldownFinished = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool canBeKilled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int invulTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* GMPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D mapBounds;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool inGame = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UILives = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool GMSpawnUpdate = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool GMSpawnPosUpdate = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool paused = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector raycastBallLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpawnPoint* playerSpawning;
};
