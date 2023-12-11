// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuantumCollapseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "CubeAttack.h"
#include "Components/ArrowComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "CableComponent.h"
#include "Components/SceneComponent.h"

AQuantumCollapseCharacter::AQuantumCollapseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	//CameraBoom->bDoCollisionTest = false;
	//CameraBoom->TargetArmLength = 500.f;
	//CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	//CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

	// Create a camera and attach to boom
	//SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	//SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
	

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}


//////////////////////////////////////////////////////////////////////////
// Input

void AQuantumCollapseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AQuantumCollapseCharacter::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AQuantumCollapseCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AQuantumCollapseCharacter::TouchStopped);
}

void AQuantumCollapseCharacter::Raycast(UArrowComponent* arrow, float grappleRange) 
{
	FHitResult OutHit;
	hasHitGrapple = false;
	
	// Start gets the front of the player
	FVector Start = GetActorLocation();

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
	if(isHit)
	{
		hasHitGrapple = true;
		raycastHitPoint = OutHit.Location;
	}
}

FVector AQuantumCollapseCharacter::RaycastVector(UArrowComponent* arrow, float grappleRange)
{
	FHitResult OutHit;
	hasHitGrapple = false;

	// Start gets the front of the player
	FVector Start = GetActorLocation();

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
	if (isHit)
	{
		hasHitGrapple = true;
		return OutHit.Location;
	}
	else
		return FVector(0, 0, 0);
}

void AQuantumCollapseCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

void AQuantumCollapseCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void AQuantumCollapseCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

FVector AQuantumCollapseCharacter::CalculateGrappleForce(float xInputAxis, float grappleForce)
{
	FVector upDir = grappleAnchorPoint - GetActorLocation();
	FVector rightDir = (GetActorRightVector() * xInputAxis) * -0.7;
	FVector directionVector = upDir + rightDir;
	directionVector.Normalize();
	FVector directionForce = directionVector * grappleForce;
	return directionForce;
}

FVector AQuantumCollapseCharacter::SetGrappleEndPosition()
{
	return raycastHitPoint;
}

void AQuantumCollapseCharacter::ArrowAiming(UArrowComponent* arrow)
{
	
}

void AQuantumCollapseCharacter::LaunchGrappleHook(AActor* hook)
{
	hook->DisableComponentsSimulatePhysics();
}

float AQuantumCollapseCharacter::GenerateRopeLength()
{
	return FVector::Dist(GetActorLocation(), raycastHitPoint);
}

void AQuantumCollapseCharacter::SwingingFunction(float ropeLength, FVector raycastPoint, FVector inputDir)
{
	FVector newLocation;
	float distance = FVector::Dist(GetActorLocation(), raycastPoint);

	if (distance > ropeLength)
	{
		FVector restrictPlayer = GetActorLocation() - raycastPoint;
		restrictPlayer *= ropeLength / distance;
		newLocation = raycastPoint + restrictPlayer + inputDir;
		SetActorLocation(newLocation, true);
	}
}

void AQuantumCollapseCharacter::PlayerOffScreenRespawn(FVector spawnPos, float boundsOffset, FVector2D viewportSize)
{
	const APlayerController* const PlayerController = Cast<const APlayerController>(GetController());

	FVector2D ScreenLocation;
	PlayerController->ProjectWorldLocationToScreen(GetActorLocation(), ScreenLocation);
	
	int viewportWidth = viewportSize.X;
	int viewportHeight = viewportSize.Y;

	int screenX = (int)ScreenLocation.X;
	int screenY = (int)ScreenLocation.Y;

	if (screenX > boundsOffset && 
		screenY > boundsOffset &&
		screenX < viewportWidth - boundsOffset && 
		screenY < viewportHeight - boundsOffset)
	{
		// If player is in the bounds of the screen
	}
	else
	{
		// If player goes outside the view of the camera
		RespawnPlayer(spawnPos);
	}
}

void AQuantumCollapseCharacter::RespawnPlayer(FVector spawnPos)
{
	SetActorLocation(spawnPos);
	deleteGrapple = true;
	if (lives != 0)
		lives -= 1;
	spawned = true;
}

void AQuantumCollapseCharacter::ArrowRotations(FVector2D leftStickInput, FVector2D rightStickInput, UArrowComponent* grappleArrow, UArrowComponent* swordArrow)
{
	float grappleDirection;
	float swordDirection;

	if (leftStickInput.X != 0 || leftStickInput.Y != 0)
		grappleDirection = UKismetMathLibrary::Atan2(leftStickInput.X, leftStickInput.Y);

	if (rightStickInput.X != 0 || rightStickInput.Y != 0)
		swordDirection = UKismetMathLibrary::Atan2(rightStickInput.X, rightStickInput.Y);

	if (swordDirection > 0)
		grappleArrow->SetWorldRotation(FRotator(0, (grappleDirection + 180 * -1.0f), 0));
	else
		grappleArrow->SetWorldRotation(FRotator(0, grappleDirection, 0));

	swordArrow->SetWorldRotation(FRotator(0, swordDirection, -90));
}

void AQuantumCollapseCharacter::SetRopeSegments(int32 segments, UCableComponent* rope)
{
	rope->NumSegments = segments;
}

void AQuantumCollapseCharacter::RopeCollision()
{
	// Get the character's location and forward vector
	FVector StartLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();

	// Calculate the end location of the raycast
	FVector EndLocation = raycastHitPoint;

	// Perform a line trace to check for objects hit by the raycast
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

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
		//DrawDebugLine(GetWorld(), HitResult.Location, EndLocation, FColor::Green, false, 1, 0, 3);

		if (ropeHasCollided)
		{
			grappleAnchorPoint = HitResult.Location;
			ropeHasCollided = false;
		}
		// Perform another line trace to see if there's an object at the new end location
		if (GetWorld()->LineTraceSingleByChannel(HitResult, HitLocation, EndLocation, ECC_Visibility, CollisionParams))
		{
		}
	}
	else
	{
		ropeHasCollided = true;
		grappleAnchorPoint = raycastHitPoint;
	}
}

bool AQuantumCollapseCharacter::PlayerHasLanded()
{
	// Get the character's location and forward vector
	FVector StartLocation = GetActorLocation();
	FVector DownwardVector = -GetActorUpVector();

	// Calculate the end location of the raycast
	FVector EndLocation = DownwardVector + 10;

	// Perform a line trace to check for objects hit by the raycast
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1, 0, 3);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility))
		return true;
	else
		return false;
}