// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Auyron.generated.h"

UCLASS()
class AAuyron : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAuyron();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere) class UAuyronMovementComponent* MovementComponent;

	//Input functions
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);
	void Jump();
	void Unjump();
	void CameraFaceForward();

	FVector2D MovementInput;
	FVector2D CameraInput;

	UPROPERTY(EditAnywhere) USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere) UCameraComponent* Camera;
	UPROPERTY(EditAnywhere)	UStaticMeshComponent* PlayerModel;
	UPROPERTY(EditAnywhere) UCapsuleComponent* CapsuleComponent;

	/* Rate that the player should accelerate. */
	UPROPERTY(EditAnywhere, Category = "Movement") float AccelerationRate;

	/* Rate that the player should decelerate while on the ground. */
	UPROPERTY(EditAnywhere, Category = "Movement") float GroundDeceleration;

	/* Rate that the player should decelerate while in the air. */
	UPROPERTY(EditAnywhere, Category = "Movement") float AirDeceleration;
	
	/* The player's maximum horizontal velocity. */
	UPROPERTY(EditAnywhere, Category = "Movement") float MaxVelocity;

	/* The maximum slope angle in degrees that the player can walk on. */
	UPROPERTY(EditAnywhere, Category = "Movement") float MaxSlope;

	/* The angle in degrees that the player can turn in one second. Only affects the player model, not movement. */
	UPROPERTY(EditAnywhere, Category = "Movement") float TurnRate;

	/* The upward speed at which the player starts their jump. */
	UPROPERTY(EditAnywhere, Category = "Movement") float JumpPower;

	/* The upward speed at which the player starts their jump. */
	UPROPERTY(EditAnywhere, Category = "Movement") float OffGroundJumpTime;

	/* The magnitude of gravity. */
	UPROPERTY(EditAnywhere, Category = "Movement") float Gravity;

	/* The amount of extra gravity to apply to the player after releasing the jump button. */
	UPROPERTY(EditAnywhere, Category = "Movement") float UnjumpRate;

	/* Minimum angle difference needed for the player model to snap to the direction the player is moving in. */
	UPROPERTY(EditAnywhere, Category = "Movement", AdvancedDisplay) float FacingAngleSnapThreshold;

	/* Highest angle the camera is allowed to have. */
	UPROPERTY(EditAnywhere, Category = "Camera") float CameraMaxAngle;

	/* Lowest angle the camera is allowed to have. */
	UPROPERTY(EditAnywhere, Category = "Camera") float CameraMinAngle;
	
	/* Default distance that the camera should be away from the player. */
	UPROPERTY(EditAnywhere, Category = "Camera") float DefaultArmLength;

	/* How long it takes the camera to move to its final position when following the player. */
	UPROPERTY(EditAnywhere, Category = "Camera") float CameraLag;

	/* How rapidly the camera should turn with the player. */
	UPROPERTY(EditAnywhere, Category = "Camera") float CameraAutoTurnFactor;

	/* How long the camera should wait after the last mouse input before reverting to automatic control. */
	UPROPERTY(EditAnywhere, Category = "Camera") float CameraResetTime;

private:
	float TimeSinceLastRealignment;
	float HowLong;
	float TimeSinceLastMouseInput;
	bool JumpNextFrame;
	bool HoldingJump;
	bool JustJumped;
	bool OnTheGround;
	bool WasOnTheGround;
	FRotator TargetDirection;
	FRotator OldDirection;
	FVector Velocity;
	FVector2D OldMovementInput;
};