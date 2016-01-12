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

USTRUCT()
struct FMovementPhysics
{
	GENERATED_USTRUCT_BODY()

	/* Rate that the player should accelerate while on the ground. */
	UPROPERTY(EditAnywhere) float GroundAccelerationRate;

	/* Rate that the player should accelerate while in the air. */
	UPROPERTY(EditAnywhere) float AirAccelerationRate;

	/* The player's maximum horizontal velocity. */
	UPROPERTY(EditAnywhere) float MaxVelocity;

	/* The player's maximum vertical velocity. */
	UPROPERTY(EditAnywhere) float TerminalVelocity;

	/* Minimum vertical velocity the player needs before drag is actually applied. */
	UPROPERTY(EditAnywhere) float DragThreshold;

	/* The magnitude of gravity. */
	UPROPERTY(EditAnywhere) float Gravity;

	/* The maximum slope angle in degrees that the player can walk on. */
	UPROPERTY(EditAnywhere) float MaxSlope;
};

USTRUCT()
struct FMovementJumping
{
	GENERATED_USTRUCT_BODY()

	/* The upward speed at which the player starts their jump. */
	UPROPERTY(EditAnywhere) float JumpPower;

	/* The amount of extra gravity to apply to the player after releasing the jump button. */
	UPROPERTY(EditAnywhere) float UnjumpRate;

	/* Time after falling off a ledge that the player can still jump. */
	UPROPERTY(EditAnywhere) float OffGroundJumpTime;

	/* The player's initial forward velocity when wall jumping, in multiples of their normal max velocity. */
	UPROPERTY(EditAnywhere) float WallJumpMultiplier;
};

USTRUCT()
struct FMovementTurning
{
	GENERATED_USTRUCT_BODY()

	/* The angle in degrees that the player can turn in one second. Only affects the player model, not movement. */
	UPROPERTY(EditAnywhere) float TurnRate;

	/* Minimum angle difference needed for the player model to snap to the direction the player is moving in. */
	UPROPERTY(EditAnywhere, AdvancedDisplay) float FacingAngleSnapThreshold;
};

USTRUCT()
struct FAbilitiesTeleport
{
	GENERATED_USTRUCT_BODY()

	/* Whether or not we can use teleport claw. */
	UPROPERTY(EditAnywhere) bool HasTeleport;

	/* The maximum range of your teleporter when aiming. */
	UPROPERTY(EditAnywhere) float TeleportRangeWhenAiming;

	/* The angle that you can be "off" from a target while aiming but still be able to teleport to it. */
	UPROPERTY(EditAnywhere) float TeleportAngleToleranceWhenAiming;

	/* The maximum range of your teleporter when not aiming. */
	UPROPERTY(EditAnywhere) float TeleportRangeWhenNotAiming;

	/* The angle that you can be "off" from a target while not aiming but still be able to teleport to it. */
	UPROPERTY(EditAnywhere) float TeleportAngleToleranceWhenNotAiming;

	/* The time in seconds it takes the teleport screen warp animation to complete. */
	UPROPERTY(EditAnywhere) float TeleportAnimationDuration;

	/* The field of view immediately after teleporting. This will lerp back to the default FOV. */
	UPROPERTY(EditAnywhere) float TeleportFOV;

	/* The exponent applied to the FOV lerp. Values greater than 1 make the restoring animation focus more on the initial part. */
	UPROPERTY(EditAnywhere) float TeleportAnimationPowerFactor;

	/* Time in seconds after teleporting when the teleport animation switches to warping in to warping out. */
	UPROPERTY(EditAnywhere) float TeleportAnimationRestoreThreshold;

	/* The color that the telepads should turn when you're aiming at them. */
	UPROPERTY(EditAnywhere) FColor TeleportLightColor;
};

USTRUCT()
struct FAbilitiesDash
{
	GENERATED_USTRUCT_BODY()

	/* Whether or not we can use dash. */
	UPROPERTY(EditAnywhere) bool HasDash;

	/* Whether or not the player can dash infinitely. */
	UPROPERTY(EditAnywhere) bool HasInfiniteDash;

	/* Whether or not the player can keep dashing after jumping. */
	UPROPERTY(EditAnywhere) bool HasDashJump;

	/* Whether or not the player can use dash wall jump. */
	UPROPERTY(EditAnywhere) bool HasDashWallJump;

	/* The player's maximum horizontal velocity. */
	UPROPERTY(EditAnywhere) float DashSpeed;

	/* The maximum length of the player's dash in seconds. */
	UPROPERTY(EditAnywhere) float DashDuration;

	/* The player's initial forward velocity when dash wall jumping, in multiples of their normal max velocity. */
	UPROPERTY(EditAnywhere) float DashWallJumpMultiplier;
};


USTRUCT()
struct FAbilitiesGlide
{
	GENERATED_USTRUCT_BODY()

	/* Whether or not we can use glide. */
	UPROPERTY(EditAnywhere) bool HasGlide;

	/* Duration of glide in seconds. */
	UPROPERTY(EditAnywhere) float GlideDuration;

	/* Initial upward velocity imparted on player when starting their glide. */
	UPROPERTY(EditAnywhere) float InitialGlideVelocity;

	/* Factor that the gravity should be multiplied by when gliding. */
	UPROPERTY(EditAnywhere) float GlideGravityMultiplier;

	/* Fraction of the default turn rate that should apply when gliding. */
	UPROPERTY(EditAnywhere) float GlideTurnRateMultiplier;
};


USTRUCT()
struct FAbilitiesSlam
{
	GENERATED_USTRUCT_BODY()

	/* Whether or not the player can use slam. */
	UPROPERTY(EditAnywhere) bool HasSlam;

	/* Downward velocity at which the player uses slam. */
	UPROPERTY(EditAnywhere) float SlamVelocity;
};

USTRUCT()
struct FCameraLag
{
	GENERATED_USTRUCT_BODY()

	/* The camera's movement lag when following the player. */
	UPROPERTY(EditAnywhere) float CameraLag;

	/* The camera's rotation lag when following the player. */
	UPROPERTY(EditAnywhere) float CameraRotationLag;

	/* Factor that the camera lag should be multiplied by when aiming. Setting this to zero disables aiming lag. */
	UPROPERTY(EditAnywhere) float AimingLagMultiplier;

	/* Factor that the camera lag should be multiplied by when moving into a camera override region. */
	UPROPERTY(EditAnywhere) float OverrideRegionRotationLagMultiplier;
};

USTRUCT()
struct FCameraAutoTurn
{
	GENERATED_USTRUCT_BODY()

	/* The number of degrees the camera should rotate every second when automatically turning with the player. */
	UPROPERTY(EditAnywhere) float CameraAutoTurnFactor;

	/* How long the camera should wait after the last mouse input before reverting to automatic control. */
	UPROPERTY(EditAnywhere) float CameraResetTime;
};

USTRUCT()
struct FCameraModelFade
{

	GENERATED_USTRUCT_BODY()

	/* Whether or not the model should fade out if the camera gets close. */
	UPROPERTY(EditAnywhere) bool ModelFadeEnabled;

	/* The minumum distance the camera can be from the player model before the player model begines to fade out. */
	UPROPERTY(EditAnywhere) float ModelFadeDistance;
};

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

	//Input functions
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);
	void ControllerYawCamera(float AxisValue);
	void Pause();
	void Unpause();
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

	UPROPERTY(EditAnywhere, Category = "Components") USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = "Components") UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, Category = "Components") USkeletalMeshComponent* PlayerModel;
	UPROPERTY(EditAnywhere, Category = "Components") UStaticMeshComponent* TeleClaw;
	UPROPERTY(EditAnywhere, Category = "Components") UCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditAnywhere, Category = "Components") UParticleSystemComponent* DashParticles;
	UPROPERTY(EditAnywhere, Category = "Components") UParticleSystemComponent* FloatParticles;
	UPROPERTY(EditAnywhere, Category = "Components") UParticleSystemComponent* SlamParticles;
	UPROPERTY(EditAnywhere, Category = "Components") UParticleSystemComponent* SlamTrail;
	UPROPERTY(EditAnywhere, Category = "Components") UParticleSystemComponent* TrailParticlesL;
	UPROPERTY(EditAnywhere, Category = "Components") UParticleSystemComponent* TrailParticlesR;
	UPROPERTY(EditAnywhere, Category = "Components") UPostProcessComponent* PostProcess;
	UPROPERTY(EditAnywhere, Category = "Components") class UAuyronMovementComponent* MovementComponent;

	//UPROPERTY(EditAnywhere, Category = "UI") TSubclassOf<class UUserWidget> Hud;

	UPROPERTY(EditAnywhere, Category = "Materials") UMaterialInterface* ScreenWarpMatBase;
	UPROPERTY(EditAnywhere, Category = "Materials") UMaterialInterface* HairMatBase;
	UPROPERTY(EditAnywhere, Category = "Materials") UMaterialInterface* BandanaMatBase;
	UPROPERTY(EditAnywhere, Category = "Materials") UMaterialInterface* BodyMatBase;

	UPROPERTY(EditAnywhere, Category = "Movement") FMovementPhysics PhysicsSettings;
	UPROPERTY(EditAnywhere, Category = "Movement") FMovementJumping JumpSettings;
	UPROPERTY(EditAnywhere, Category = "Movement") FMovementTurning TurnSettings;

	UPROPERTY(EditAnywhere, Category = "Abilities") FAbilitiesTeleport TeleportSettings;
	UPROPERTY(EditAnywhere, Category = "Abilities") FAbilitiesDash DashSettings;
	UPROPERTY(EditAnywhere, Category = "Abilities") FAbilitiesGlide GlideSettings;
	UPROPERTY(EditAnywhere, Category = "Abilities") FAbilitiesSlam SlamSettings;

	/* Highest angle the camera is allowed to have. */
	UPROPERTY(EditAnywhere, Category = "Camera") float CameraMaxAngle;

	/* Lowest angle the camera is allowed to have. */
	UPROPERTY(EditAnywhere, Category = "Camera") float CameraMinAngle;

	/* Default distance that the camera should be away from the player. */
	UPROPERTY(EditAnywhere, Category = "Camera") float DefaultArmLength;

	UPROPERTY(EditAnywhere, Category = "Camera") FCameraLag CameraLagSettings;
	UPROPERTY(EditAnywhere, Category = "Camera") FCameraAutoTurn CameraAutoTurnSettings;
	UPROPERTY(EditAnywhere, Category = "Camera") FCameraModelFade CameraModelFadeSettings;

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
	bool AlreadySlammed;
	bool OnTheGround;
	bool HelpEnabled;

	UUserWidget* thehud;

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
	bool dashnextframe;
	bool holdingdash;
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
	
	float defaultfov;
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