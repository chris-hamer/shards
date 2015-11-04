// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "GameFramework/HUD.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
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
	void Use();
	void CameraFaceForward();
	void CameraUnFaceForward();
	void CameraModeToggle();
	void Warp();
	void Dash();
	void UnDash();

	//UFUNCTION()
	//void Hit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void Hit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void Stay(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void UnHit(class AActor * OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "GO FUCK YOURSELF")
	float GetSpeed();
	UFUNCTION(BlueprintCallable, Category = "GO FUCK YOURSELF")
	bool GetIsTurning();
	UFUNCTION(BlueprintCallable, Category = "GO FUCK YOURSELF")
	bool GetIsAiming();
	UFUNCTION(BlueprintCallable, Category = "GO FUCK YOURSELF")
	bool GetIsOnTheGround();
	//UFUNCTION(BlueprintCallable, Category = "GO FUCK YOURSELF")
	//int GetGemCount();
	

	FVector MovementInput;
	FVector CameraInput;

	UPROPERTY(EditAnywhere) USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere) USpringArmComponent* SidewaysSpringArm;
	UPROPERTY(EditAnywhere) UCameraComponent* Camera;
	UPROPERTY(EditAnywhere)	USkeletalMeshComponent* PlayerModel;
	UPROPERTY(EditAnywhere)	UStaticMeshComponent* TeleClaw;
	UPROPERTY(EditAnywhere) UCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditAnywhere) UParticleSystemComponent* DashParticles;
	UPROPERTY(EditAnywhere) UUserWidget* thehud;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> asd;

	/* Rate that the player should accelerate while on the ground. */
	UPROPERTY(EditAnywhere, Category = "Movement") float GroundAccelerationRate;

	/* Rate that the player should accelerate while in the air. */
	UPROPERTY(EditAnywhere, Category = "Movement") float AirAccelerationRate;

	/* Rate that the player should decelerate while on the ground. */
	UPROPERTY(EditAnywhere, Category = "Movement") float GroundDeceleration;

	/* Rate that the player should decelerate while in the air. */
	UPROPERTY(EditAnywhere, Category = "Movement") float AirDeceleration;

	/* The player's maximum horizontal velocity. */
	UPROPERTY(EditAnywhere, Category = "Movement") float MaxVelocity;

	/* The player's maximum horizontal velocity. */
	UPROPERTY(EditAnywhere, Category = "Movement") float DashSpeed;

	/* The player's maximum horizontal velocity. */
	UPROPERTY(EditAnywhere, Category = "Movement") float DashDuration;

	/* The maximum slope angle in degrees that the player can walk on. */
	UPROPERTY(EditAnywhere, Category = "Movement") float MaxSlope;

	/* The angle in degrees that the player can turn in one second. Only affects the player model, not movement. */
	UPROPERTY(EditAnywhere, Category = "Movement") float TurnRate;

	/* The upward speed at which the player starts their jump. */
	UPROPERTY(EditAnywhere, Category = "Movement") float JumpPower;

	/* The upward speed at which the player double jumps. */
	UPROPERTY(EditAnywhere, Category = "Movement") float DoubleJumpPower;

	/* Time after falling off a ledge that the player can still jump. */
	UPROPERTY(EditAnywhere, Category = "Movement") float OffGroundJumpTime;

	/* The magnitude of gravity. */
	UPROPERTY(EditAnywhere, Category = "Movement") float Gravity;

	/* The amount of extra gravity to apply to the player after releasing the jump button. */
	UPROPERTY(EditAnywhere, Category = "Movement") float UnjumpRate;

	/* Minimum angle difference needed for the player model to snap to the direction the player is moving in. */
	UPROPERTY(EditAnywhere, Category = "Movement", AdvancedDisplay) float FacingAngleSnapThreshold;

	/* Whether or not we can use teleport claw */
	UPROPERTY(EditAnywhere, Category = "Abilities") bool HasTeleport;

	/* Whether or not we can do short glide */
	UPROPERTY(EditAnywhere, Category = "Abilities") bool HasGlide;

	/* Duration of glide. */
	UPROPERTY(EditAnywhere, Category = "Abilities") float GlideDuration;

	/* The maximum range of your teleporter. */
	UPROPERTY(EditAnywhere, Category = "Teleporter") float TeleportRange;

	/* The angle that you can be "off" from a target but still be able to teleport to it. */
	UPROPERTY(EditAnywhere, Category = "Teleporter") float TeleportAngleTolerance;

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

	bool ShouldActivate;
	int GemCount;

private:
	float TimeSinceLastRealignment;
	float HowLong;
	float dashtimer;
	float TimeSinceLastMouseInput;
	bool JumpNextFrame;
	bool HoldingJump;
	bool JustJumped;
	bool OnTheGround;
	uint8 jumpsLeft;
	bool justDoubleJumped;
	bool WasOnTheGround;
	bool swish;
	bool dash;
	bool dashing;
	bool ztarget;
	bool wasztarget;
	bool movementlocked;
	bool cameralocked;
	bool cameramode;
	bool CameraOverrideLookAtPlayer;
	FVector CameraOverrideTargetDisplacement;
	FVector CameraOverrideTargetLocation;
	FRotator CameraOverrideTargetRotation;
	FRotator TargetDirection;
	FRotator OldDirection;
	FVector Velocity;
	FVector LastPlatformVelocity;
	FVector2D OldMovementInput;
};