// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsCharacterSC.h"

#include <string>

#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPhysicsCharacterSC::UPhysicsCharacterSC()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPhysicsCharacterSC::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPhysicsCharacterSC::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FHitResult UPhysicsCharacterSC::RaycastPhysicsChar(UArrowComponent* arrow, float grappleRange, bool ballRaycast)
{
	FHitResult OutHit;
	hasHitGrapple = false;

	// Start gets the front of the player
	FVector Start = this->GetOwner()->GetActorLocation();

	//Getting the forward vector and shooting a beam that will be 1000 units long ig?
	FVector End = (arrow->GetForwardVector() * grappleRange) + Start;
	//FVector End = ArrowComponent->GetForwardVector() * 1000.0f + Start;

	//FVector Swing = GetActorUpVector() * 1000.0f + Start;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this->GetOwner());

	//raycast debug line
	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	//GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);


	// This part is incase you wanted the raycast to destroy objects in front of it, needs to be adjusted but this works

	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);
	if (isHit && !ballRaycast)
	{
		hasHitGrapple = true;
		raycastHitPoint = OutHit.Location;
		grappleAnchorPoint = raycastHitPoint;
	}
	else if(isHit && ballRaycast)
	{
		raycastBallLocation = OutHit.Location;
	}

	return OutHit;
}

FVector UPhysicsCharacterSC::CalculateGrappleForcePhysicsChar(float xInputAxis, float grappleForce)
{
	FVector upDir = grappleAnchorPoint - this->GetOwner()->GetActorLocation();
	FVector rightDir = (this->GetOwner()->GetActorRightVector() * xInputAxis) * -0.7;
	FVector directionVector = upDir + rightDir;
	directionVector.Normalize();
	FVector directionForce = directionVector * grappleForce;
	directionForce.X = 0;
	return directionForce;
}

void UPhysicsCharacterSC::RopeCollision()
{
	// Get the character's location and forward vector
	FVector StartLocation = this->GetOwner()->GetActorLocation();
	FVector ForwardVector = this->GetOwner()->GetActorForwardVector();

	// Calculate the end location of the raycast
	FVector EndLocation = raycastHitPoint;

	// Perform a line trace to check for objects hit by the raycast
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this->GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams))
	{
		// Get the hit location and normal
		FVector HitLocation = HitResult.Location;
		FVector HitNormal = HitResult.ImpactNormal;

		// Calculate the new direction by reflecting the ray around the hit normal
		FVector NewDirection = FVector::VectorPlaneProject(ForwardVector, HitNormal).GetSafeNormal();

		// Create a new end location for the raycast line to simulate the wrap-around effect
		EndLocation = HitLocation + (NewDirection * 5000);

		// Debug draw the original and new raycast lines
		//DrawDebugLine(GetWorld(), StartLocation, HitResult.Location, FColor::Red, false, 1, 0, 3);

		ropeHasCollided = true;

		// Perform another line trace to see if there's an object at the new end location
		if (GetWorld()->LineTraceSingleByChannel(HitResult, HitLocation, EndLocation, ECC_Visibility, CollisionParams))
		{
		}
	}
	else
	{
		//DrawDebugLine(GetWorld(), HitResult.Location, EndLocation, FColor::Green, false, 1, 0, 3);
		ropeHasCollided = false;
	}

	if (ropeHasCollided && !hasChangedPoint)
	{
		grappleAnchorPoint = HitResult.Location;
		hasChangedPoint = true;
	}
	else if (!ropeHasCollided && hasChangedPoint)
	{
		grappleAnchorPoint = raycastHitPoint;
		hasChangedPoint = false;
	}
}
void UPhysicsCharacterSC::DeathFunction()
{
	GMSpawnUpdate = true;
	if (lives != 0 && livesGamemode)
		lives -= 1;
	
	spawned = false;
	canBeKilled = false;
	this->GetOwner()->SetActorHiddenInGame(true);
	this->GetOwner()->SetActorLocation(FVector(this->GetOwner()->GetActorLocation().X, FMath::RandRange(9999, 99999), FMath::RandRange(9999, 99999)));
}

void UPhysicsCharacterSC::RespawnFunction(FVector spawnPos)
{
	GMSpawnPosUpdate = true;
	if((livesGamemode && lives > 0) || !livesGamemode)
	{
		this->GetOwner()->SetActorLocation(spawnPos);
		this->GetOwner()->SetActorHiddenInGame(false);
		spawned = true;
	}
	
	FTimerDelegate TimerDel = FTimerDelegate::CreateUObject(this, &UPhysicsCharacterSC::ResetSpawnedBool);
	FTimerHandle TimerHand;
	GetWorld()->GetTimerManager().SetTimer(TimerHand, TimerDel, invulTime, false);
}

void UPhysicsCharacterSC::ResetSpawnedBool()
{
	canBeKilled = true;
	playerSpawning->spawning = false;
}


void UPhysicsCharacterSC::RespawnPlayer(UGameManager* gameManager, int respawnDelay)
{
	if(canBeKilled)
	{
		// Death
		UILives = true;
		DeathFunction();

		// Delay
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UPhysicsCharacterSC::RespawnFunction, GenerateSpawnPos(gameManager));
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, respawnDelay, false);
	}
}

FVector UPhysicsCharacterSC::GenerateSpawnPos(UGameManager* gameManager)
{
	TArray<USpawnPoint*> sp = gameManager->spawnPoints;
	TArray<UPhysicsCharacterSC*> players;

	for(int i = 0 ; i < gameManager->physicsPlayers.Num(); i++)
		if(gameManager->physicsPlayers[i] != this && gameManager->physicsPlayers[i]->spawned)
			players.Add(gameManager->physicsPlayers[i]);
	
	FVector avgPlayerPoint;

	for(int i = 0; i < players.Num(); i++)
		avgPlayerPoint += players[i]->GetOwner()->GetActorLocation();

	avgPlayerPoint = avgPlayerPoint / players.Num();
	int index = 0;
	float distance = FVector::Dist(avgPlayerPoint, sp[0]->GetOwner()->GetActorLocation());
	
	for(int i = 1; i < sp.Num(); i++)
	{
		if(sp[index]->SPoccupied && !sp[i]->SPoccupied)
		{
			index = i;
			distance = FVector::Dist(avgPlayerPoint, sp[i]->GetOwner()->GetActorLocation());
		}
		
		if(distance < FVector::Dist(avgPlayerPoint, sp[i]->GetOwner()->GetActorLocation()) && !sp[i]->SPoccupied)
		{
			distance = FVector::Dist(avgPlayerPoint, sp[i]->GetOwner()->GetActorLocation());
			index = i;
		}
	}

	playerSpawning = sp[index];
	playerSpawning->SPoccupied = true;
	playerSpawning->spawning = true;
	return playerSpawning->GetOwner()->GetActorLocation();
}


void UPhysicsCharacterSC::WallClimbing(float upInput, float climbSpeed, UBoxComponent* box)
{
	if (leftCollision || rightCollision)
		IsClimbing = true;

	if (IsClimbing)
	{
		float upDirection = FMath::Clamp(upInput, 1.0f, -1.0f);
		movementDirection.Z = upDirection;
		box->SetEnableGravity(false);
		box->SetPhysicsLinearVelocity(FVector(0,0,0));
		FVector newPos = FVector(this->GetOwner()->GetActorLocation().X, this->GetOwner()->GetActorLocation().Y,
								(upInput * climbSpeed) + this->GetOwner()->GetActorLocation().Z);
		this->GetOwner()->SetActorLocation(newPos);
	}
	else
	{
		box->SetEnableGravity(true);
	}
}

void UPhysicsCharacterSC::Movement(float deltaTime, float moveSpeed)
{
	if (!movementDirection.IsZero())
	{
		if (leftCollision || rightCollision)
		{
			if((movementDirection.Y < -0.1f && rightCollision) || (movementDirection.Y > 0 && leftCollision))
				this->GetOwner()->SetActorLocation(((movementDirection * deltaTime) * moveSpeed) + this->GetOwner()->GetActorLocation());
		}
		else
			this->GetOwner()->SetActorLocation(((movementDirection * deltaTime) * moveSpeed) + this->GetOwner()->GetActorLocation());
	}
}

void UPhysicsCharacterSC::CollisionCheck(UBoxComponent* box)
{
	FHitResult hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this->GetOwner());
	FVector playerPos = this->GetOwner()->GetActorLocation();

	rightCollision = GetWorld()->LineTraceSingleByChannel(hit, playerPos, playerPos + (box->GetRightVector() + -75.0f), ECC_Visibility, CollisionParams);
	leftCollision = GetWorld()->LineTraceSingleByChannel(hit, playerPos, playerPos + (box->GetRightVector() + 75.0f), ECC_Visibility, CollisionParams);
	upCollision = GetWorld()->LineTraceSingleByChannel(hit, playerPos, playerPos + (box->GetUpVector() + 75.0f), ECC_Visibility, CollisionParams);
	onGround = GetWorld()->LineTraceSingleByChannel(hit, playerPos, playerPos + (box->GetUpVector() + -100.0f), ECC_Visibility, CollisionParams);
}

void UPhysicsCharacterSC::JumpFunction(float midairSpeed, float jumpHeight, float wallJumpHeight, UBoxComponent* box, UArrowComponent* arrow)
{
	if (onGround)
	{
		jumping = true;
		float jumpY = (midairSpeed * -1.0f) * movementDirection.Y;
		box->AddImpulse(FVector(0, jumpY, jumpHeight), "", true);
	}
	else
	{
		if (canClimb && IsClimbing)
		{
			box->SetEnableGravity(true);
			FVector arrowForward = arrow->GetForwardVector();
			float jumpY = arrowForward.Y * wallJumpHeight;
			float jumpZ = arrowForward.Z * wallJumpHeight;
			box->AddImpulse(FVector(0, jumpY, jumpZ), "", true);
			wallJumping = true;
			jumping = true;
		}
	}
}

void UPhysicsCharacterSC::GrappleCooldown(float cooldownLength)
{
	grappleCooldownFinished = false;
	FTimerHandle GrappleTimer;
	FTimerDelegate Delegate; 
	Delegate.BindLambda([&]
		{
			grappleCooldownFinished = true;
		});

	GetWorld()->GetTimerManager().SetTimer(GrappleTimer, Delegate, cooldownLength, false);
}

void UPhysicsCharacterSC::LungeCooldown(float cooldownLength)
{

}

void UPhysicsCharacterSC::PhysicsPlayerOffScreenRespawn(UGameManager* gameManager, int delay)
{
	if(inGame)
	{ 
		FVector2D playerPos2D = FVector2D(this->GetOwner()->GetActorLocation().Y, this->GetOwner()->GetActorLocation().Z);
		FVector2D GMPos2D = FVector2D(GMPos->GetComponentLocation().Y, GMPos->GetComponentLocation().Z);
		if(playerPos2D.X > GMPos2D.X + mapBounds.X ||
		playerPos2D.Y > GMPos2D.Y + mapBounds.Y ||
		playerPos2D.X < GMPos2D.X - mapBounds.X ||
		playerPos2D.Y < GMPos2D.Y - mapBounds.Y)
		{
			if(spawned)
				canBeKilled = true;
			
			RespawnPlayer(gameManager, delay);
		}
	}
}

bool UPhysicsCharacterSC::PauseManager(UGameManager* gameManager)
{
	TArray<UPhysicsCharacterSC*> players;

	for(int i = 0 ; i < gameManager->physicsPlayers.Num(); i++)
		if(gameManager->physicsPlayers[i] != this)
			players.Add(gameManager->physicsPlayers[i]);

	for(int i = 0; i < players.Num(); i++)
		if(players[i]->paused)
			return false;

	paused = true;
	return true;
}
