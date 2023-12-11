// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/ArrowComponent.h"
#include "QuantumCollapseCharacter.generated.h"

UCLASS(config=Game)
class AQuantumCollapseCharacter : public ACharacter
{
	GENERATED_BODY()

	// Camera boom positioning the camera beside the character

	/** Side view camera */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class USpringArmComponent* CameraBoom;

protected:
	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	AQuantumCollapseCharacter();

	// Camera boom positioning the camera beside the character
	
	/** Returns SideViewCameraComponent subobject **/
	//FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	//FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ropeHasCollided = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector raycastHitPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector tempRaycastHitPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int lives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> ropeCollisionPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool deleteGrapple = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool hasHitGrapple;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool spawned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector grappleAnchorPoint;

	UFUNCTION(BlueprintCallable)
	void Raycast(UArrowComponent* arrow, float grappleRange);

	UFUNCTION(BlueprintCallable)
	FVector RaycastVector(UArrowComponent* arrow, float grappleRange);

	UFUNCTION(BlueprintCallable)
	FVector CalculateGrappleForce(float xInputAxis, float grappleForce);
	
	UFUNCTION(BlueprintCallable)
	FVector LockRotation(FVector rotation)
	{
		return FVector(0, 0, rotation.Z);
	};

	UFUNCTION(BlueprintCallable)
	FVector SetGrappleEndPosition();

	UFUNCTION(BlueprintCallable)
	void ArrowAiming(UArrowComponent* arrow);

	UFUNCTION(BlueprintCallable)
	void LaunchGrappleHook(AActor* hook);

	UFUNCTION(BlueprintCallable)
	float GenerateRopeLength();

	UFUNCTION(BlueprintCallable)
	void SwingingFunction(float ropeLength, FVector raycastPoint, FVector inputDir);

	UFUNCTION(BlueprintCallable)
	void PlayerOffScreenRespawn(FVector spawnPos, float boundsOffset, FVector2D viewportSize);

	UFUNCTION(BlueprintCallable)
	void RespawnPlayer(FVector spawnPos);

	UFUNCTION(BlueprintCallable)
	void ArrowRotations(FVector2D leftStickInput, FVector2D rightStickInput, UArrowComponent* grappleArrow, UArrowComponent* swordArrow);

	UFUNCTION(BlueprintCallable)
	void SetRopeSegments(int32 segments, UCableComponent* rope);

	UFUNCTION(BlueprintCallable)
	void RopeCollision();

	UFUNCTION(BlueprintCallable)
	bool PlayerHasLanded();
};
