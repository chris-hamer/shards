// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "GameFramework/HUD.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "TeleClaw.h"
#include "Stick.h"
#include "TwoDimensionalMovementRegion.h"
#include "Checkpoint.h"
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

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere) class UAuyronMovementComponent* MovementComponent;


	//Input functions
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);
	void ControllerYawCamera(float AxisValue);
	void Jump();
	void Unjump();
	void Use();
	void ToggleHelp();
	void CameraFaceForward();
	void CameraUnFaceForward();
	void CameraModeToggle();
	void Warp();
	void Slam();
	void Dash();
	void UnDash();
	void Respawn();
	
	//UFUNCTION()
	//void Hit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void Hit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void Stay(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void UnHit(class AActor * OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	FVector GetPlayerLocation();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	float GetSpeed();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	float GetActualSpeed();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	float GetModelOpacity();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	bool GetIsTurning();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	bool GetIsAiming();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	bool GetIsOnTheGround();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	bool GetHelpEnabled();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	uint8 GetGemAmount();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	UParticleSystemComponent* GetTrailParticlesL();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	UParticleSystemComponent* GetTrailParticlesR();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	bool GetJustWallJumped();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	bool AboutToWarp();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void SetMaterial(int32 index, UMaterialInterface* newmat);
	
	FVector MovementInput;
	FVector CameraInput;

	UPROPERTY(EditAnywhere) USceneComponent* Root;
	UPROPERTY(EditAnywhere) USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere) UCameraComponent* Camera;
	UPROPERTY(EditAnywhere)	USkeletalMeshComponent* PlayerModel;
	UPROPERTY(EditAnywhere)	UStaticMeshComponent* TeleClaw;
	UPROPERTY(EditAnywhere) UCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditAnywhere) UParticleSystemComponent* DashParticles;
	UPROPERTY(EditAnywhere) UParticleSystemComponent* FloatParticles;
	UPROPERTY(EditAnywhere) UParticleSystemComponent* SlamParticles;
	UPROPERTY(EditAnywhere) UParticleSystemComponent* TrailParticlesL;
	UPROPERTY(EditAnywhere) UParticleSystemComponent* TrailParticlesR;
	UPROPERTY(EditAnywhere) UUserWidget* thehud;
	UPROPERTY(EditAnywhere) UPostProcessComponent* PostProcess;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> asd;

	UPROPERTY(EditAnywhere, Category = "Camera") UMaterialInterface* ScreenWarpMatBase;
	UPROPERTY(EditAnywhere, Category = "Camera") UMaterialInterface* HairMatBase;
	UPROPERTY(EditAnywhere, Category = "Camera") UMaterialInterface* BandanaMatBase;
	UPROPERTY(EditAnywhere, Category = "Camera") UMaterialInterface* BodyMatBase;

	/* Rate that the player should accelerate while on the ground. */
	UPROPERTY(EditAnywhere, Category = "Movement") float GroundAccelerationRate;

	/* Rate that the player should accelerate while in the air. */
	UPROPERTY(EditAnywhere, Category = "Movement") float AirAccelerationRate;

	/* The player's maximum horizontal velocity. */
	UPROPERTY(EditAnywhere, Category = "Movement") float MaxVelocity;

	/* The player's maximum vertical velocity. */
	UPROPERTY(EditAnywhere, Category = "Movement") float TerminalVelocity;

	/* Minimum vertical velocity the player needs before drag is actually applied. */
	UPROPERTY(EditAnywhere, Category = "Movement") float DragThreshold;

	/* The player's maximum horizontal velocity. */
	UPROPERTY(EditAnywhere, Category = "Movement") float DashSpeed;

	/* The maximum length of the player's dash in seconds. */
	UPROPERTY(EditAnywhere, Category = "Movement") float DashDuration;

	/* The maximum slope angle in degrees that the player can walk on. */
	UPROPERTY(EditAnywhere, Category = "Movement") float MaxSlope;

	/* The angle in degrees that the player can turn in one second. Only affects the player model, not movement. */
	UPROPERTY(EditAnywhere, Category = "Movement") float TurnRate;

	/* Fraction of the default turn rate that should apply when gliding. */
	UPROPERTY(EditAnywhere, Category = "Movement") float GlideTurnRateMultiplier;

	/* The upward speed at which the player starts their jump. */
	UPROPERTY(EditAnywhere, Category = "Movement") float JumpPower;

	/* The player's initial forward velocity when wall jumping, in multiples of their normal max velocity. */
	UPROPERTY(EditAnywhere, Category = "Movement") float WallJumpMultiplier;

	/* Time after falling off a ledge that the player can still jump. */
	UPROPERTY(EditAnywhere, Category = "Movement") float OffGroundJumpTime;

	/* The magnitude of gravity. */
	UPROPERTY(EditAnywhere, Category = "Movement") float Gravity;

	/* The amount of extra gravity to apply to the player after releasing the jump button. */
	UPROPERTY(EditAnywhere, Category = "Movement") float UnjumpRate;

	/* Minimum angle difference needed for the player model to snap to the direction the player is moving in. */
	UPROPERTY(EditAnywhere, Category = "Movement", AdvancedDisplay) float FacingAngleSnapThreshold;

	/* Whether or not we can use teleport claw. */
	UPROPERTY(EditAnywhere, Category = "Abilities") bool HasTeleport;

	/* Whether or not we can use dash. */
	UPROPERTY(EditAnywhere, Category = "Abilities") bool HasDash;

	/* Whether or not the player can dash infinitely. */
	UPROPERTY(EditAnywhere, Category = "Abilities") bool HasInfiniteDash;

	/* Whether or not the player can keep dashing after jumping. */
	UPROPERTY(EditAnywhere, Category = "Abilities") bool HasDashJump;

	/* Whether or not we can use glide. */
	UPROPERTY(EditAnywhere, Category = "Abilities") bool HasGlide;

	/* Duration of glide in seconds. */
	UPROPERTY(EditAnywhere, Category = "Abilities") float GlideDuration;

	/* Initial upward velocity imparted on player when starting their glide. */
	UPROPERTY(EditAnywhere, Category = "Abilities") float InitialGlideVelocity;

	/* Factor that the gravity should be multiplied by when gliding. */
	UPROPERTY(EditAnywhere, Category = "Abilities") float GlideGravityMultiplier;

	/* Whether or not the player can use slam. */
	UPROPERTY(EditAnywhere, Category = "Abilities") bool HasSlam;

	/* Downward velocity at which the player uses slam. */
	UPROPERTY(EditAnywhere, Category = "Abilities") float SlamVelocity;

	/* The maximum range of your teleporter when aiming. */
	UPROPERTY(EditAnywhere, Category = "Teleporter") float TeleportRangeWhenAiming;

	/* The angle that you can be "off" from a target while aiming but still be able to teleport to it. */
	UPROPERTY(EditAnywhere, Category = "Teleporter") float TeleportAngleToleranceWhenAiming;

	/* The maximum range of your teleporter when not aiming. */
	UPROPERTY(EditAnywhere, Category = "Teleporter") float TeleportRangeWhenNotAiming;

	/* The angle that you can be "off" from a target while not aiming but still be able to teleport to it. */
	UPROPERTY(EditAnywhere, Category = "Teleporter") float TeleportAngleToleranceWhenNotAiming;

	/* The time in seconds it takes the teleport screen warp animation to complete. */
	UPROPERTY(EditAnywhere, Category = "Teleporter") float TeleportAnimationDuration;

	/* The color that the telepads should turn when you're aiming at them. */
	UPROPERTY(EditAnywhere, Category = "Teleporter") FColor TeleportLightColor;

	/* Highest angle the camera is allowed to have. */
	UPROPERTY(EditAnywhere, Category = "Camera") float CameraMaxAngle;

	/* Lowest angle the camera is allowed to have. */
	UPROPERTY(EditAnywhere, Category = "Camera") float CameraMinAngle;
	
	/* Default distance that the camera should be away from the player. */
	UPROPERTY(EditAnywhere, Category = "Camera") float DefaultArmLength;

	/* How long it takes the camera to move to its final position when following the player. */
	UPROPERTY(EditAnywhere, Category = "Camera") float CameraLag;

	/* How long it takes the camera to move to its final position when following the player. */
	UPROPERTY(EditAnywhere, Category = "Camera") float CameraRotationLag;

	/* Factor that the camera lag should be multiplied by when aiming. Setting this to zero disables aiming lag. */
	UPROPERTY(EditAnywhere, Category = "Camera") float AimingLagMultiplier;

	/* Factor that the camera lag should be multiplied by when moving into a camera override region. */
	UPROPERTY(EditAnywhere, Category = "Camera") float OverrideRegionRotationLagMultiplier;

	/* The number of degrees the camera should rotate every second when automatically turning with the player. */
	UPROPERTY(EditAnywhere, Category = "Camera") float CameraAutoTurnFactor;

	/* How long the camera should wait after the last mouse input before reverting to automatic control. */
	UPROPERTY(EditAnywhere, Category = "Camera") float CameraResetTime;

	/* Whether or not the model should fade out if the camera gets close. */
	UPROPERTY(EditAnywhere, Category = "Camera") bool ModelFadeEnabled;

	/* The minumum distance the camera can be from the player model before the player model begines to fade out. */
	UPROPERTY(EditAnywhere, Category = "Camera") float ModelFadeDistance;

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
	bool JustWallJumped;
	bool SlamNextFrame;
	bool JustSlammed;
	bool OnTheGround;
	bool HelpEnabled;

	FVector closecamera;

	UMaterialInstanceDynamic* screenwarpmat;
	UMaterialInstanceDynamic* hairmat;
	UMaterialInstanceDynamic* bandanamat;
	UMaterialInstanceDynamic* bodymat;

	float DefaultGravity;

	bool GlideNextFrame;
	bool IsGliding;
	bool AlreadyUnjumped;
	FVector actualvelocity;
	bool AlreadyGlided;
	float GlideTimer;
	float warptimer;

	AStick* thisguy;

	float warpanimtimer;

	uint8 jumpsLeft;
	bool justDoubleJumped;
	FVector AppliedForce;
	bool WasOnTheGround;
	bool swish;
	bool dash;
	bool dashing;
	bool ztarget;
	bool wasztarget;
	bool movementlocked;
	bool cameralocked;
	bool itshappening;
	bool dunk;
	bool cameramode;
	FVector previousgroundvelocity;
	bool CameraOverrideLookAtPlayer;
	bool CameraLockToPlayerXAxis;
	bool CameraLockToPlayerYAxis;
	bool CameraLockToPlayerZAxis;
	bool MovementAxisLocked;
	float LockedAxisValue;
	FVector previousposition;

	FVector RespawnPoint;
	
	TEnumAsByte<ATwoDimensionalMovementRegion::AxisEnum> LockedMovementAxis;
	FVector CameraOverrideTargetDisplacement;
	FVector CameraOverrideTargetLocation;
	FRotator CameraOverrideTargetRotation;
	FRotator TargetDirection;
	FRotator OldDirection;
	FVector Velocity;
	FVector LastPlatformVelocity;
	FVector2D OldMovementInput;
};