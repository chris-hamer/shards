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
#include "DialogueCut.h"
#include "NPC.h"
#include "CameraOverrideRegion.h"
#include "TwoDimensionalMovementRegion.h"
#include "Checkpoint.h"
#include "Auyron.generated.h"

USTRUCT()
struct FMovementPhysics
{
	GENERATED_USTRUCT_BODY()

	/* Rate that the player should accelerate while on the ground. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float GroundAccelerationRate;

	/* Rate that the player should accelerate while in the air. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float AirAccelerationRate;

	/* The player's maximum horizontal velocity. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float MaxVelocity;

	/* If the player's velocity falls below this value, it snaps to zero. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float MinVelocity;

	/* The player's maximum vertical velocity. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TerminalVelocity;

	/* Minimum vertical velocity the player needs before drag is actually applied. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float DragThreshold;

	/* The magnitude of gravity. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Gravity;

	/* The maximum slope angle in degrees that the player can walk on. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float MaxSlope;

	/* The time in seconds that you can walk on a too-steep slope before sliding down. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float SlopeSlideTime;

	/* Controls how strongly the player pushes physics objects. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float PushForceFactor;

	/* Exponent applied to the pushing force when the player is going faster than their normal max speed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float HighVelocityForceExponent;
};

USTRUCT()
struct FMovementJumping
{
	GENERATED_USTRUCT_BODY()

	/* The upward speed at which the player starts their jump. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float JumpPower;

	/* The amount of extra gravity to apply to the player after releasing the jump button. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float UnjumpRate;

	/* Time after falling off a ledge that the player can still jump. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CoyoteJumpTime;

	/* Whether or not the player has wall jump. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool HasWallJump;

	/* The player's initial forward velocity when wall jumping, in multiples of their normal max velocity. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float WallJumpMultiplier;
};

USTRUCT()
struct FMovementTurning
{
	GENERATED_USTRUCT_BODY()

	/* The angle in degrees that the player can turn in one second. Only affects the player model, not movement. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TurnRate;

	/* Minimum angle difference needed for the player model to snap to the direction the player is moving in. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay) float FacingAngleSnapThreshold;
};

USTRUCT()
struct FAbilitiesTeleport
{
	GENERATED_USTRUCT_BODY()

	/* Whether or not we can use teleport claw. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool HasTeleport;

	/* The maximum range of your teleporter when aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TeleportRangeWhenAiming;

	/* The angle that you can be "off" from a target while aiming but still be able to teleport to it. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TeleportAngleToleranceWhenAiming;

	/* The maximum range of your teleporter when not aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TeleportRangeWhenNotAiming;

	/* The angle that you can be "off" from a target while not aiming but still be able to teleport to it. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TeleportAngleToleranceWhenNotAiming;

	/* The time in seconds it takes the teleport screen warp animation to complete. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TeleportAnimationDuration;

	/* The field of view immediately after teleporting. This will lerp back to the default FOV. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TeleportFOV;

	/* The exponent applied to the FOV lerp. Values greater than 1 make the restoring animation focus more on the initial part. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TeleportAnimationPowerFactor;

	/* Time in seconds after teleporting when the teleport animation switches to warping in to warping out. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TeleportAnimationRestoreThreshold;

	/* The color that the telepads should turn when you're aiming at them. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FColor TeleportLightColor;
};

USTRUCT()
struct FAbilitiesDash
{
	GENERATED_USTRUCT_BODY()

	/* Whether or not we can use dash. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool HasDash;

	/* Whether or not the player can dash infinitely. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool HasInfiniteDash;

	/* Whether or not the player can control their direction while dashing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool HasDashControl;

	/* Whether or not the player can use dash wall jump. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool HasDashWallJump;

	/* Whether or not the player can keep dashing after jumping. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool HasDashJump;

	/* The player's maximum horizontal velocity. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float DashSpeed;

	/* The maximum length of the player's dash in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float DashDuration;

	/* The player's initial forward velocity when dash wall jumping, in multiples of their normal max velocity. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float DashWallJumpMultiplier;

	/* Whether or not we can use air dash. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool HasAirDash;
};


USTRUCT()
struct FAbilitiesGlide
{
	GENERATED_USTRUCT_BODY()

	/* Whether or not we can use glide. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool HasGlide;

	/* Duration of glide in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float GlideDuration;

	/* Initial upward velocity imparted on player when starting their glide. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float InitialGlideVelocity;

	/* Factor that the gravity should be multiplied by when gliding. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float GlideGravityMultiplier;

	/* Fraction of the default turn rate that should apply when gliding. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float GlideTurnRateMultiplier;

	/* Number of glide sounds that should play per second when gliding. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float GlideSoundsPerSecond;

	/* Lowest pitch multiplier for glide sounds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float GlideSoundPitchMin;

	/* Highest pitch multiplier for glide sounds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float GlideSoundPitchMax;
};


USTRUCT()
struct FAbilitiesSlam
{
	GENERATED_USTRUCT_BODY()

	/* Whether or not the player can use slam. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool HasSlam;

	/* Downward velocity at which the player uses slam. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float SlamVelocity;
};

USTRUCT()
struct FCameraLag
{
	GENERATED_USTRUCT_BODY()

	/* The camera's movement lag when following the player. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CameraLag;

	/* The camera's rotation lag when following the player. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CameraRotationLag;

	/* Factor that the camera lag should be multiplied by when aiming. Setting this to zero disables aiming lag. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float AimingLagMultiplier;

	/* Factor that the camera lag should be multiplied by when moving into a camera override region. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float OverrideRegionRotationLagMultiplier;

	/* Factor that the camera lag should be multiplied when in dialogue. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float DialogueLagMultiplier;
};

USTRUCT()
struct FCameraAutoTurn
{
	GENERATED_USTRUCT_BODY()

	/* The number of degrees the camera should rotate every second when automatically turning with the player. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CameraAutoTurnFactor;

	/* How long the camera should wait after the last mouse input before reverting to automatic control. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CameraResetTime;

	/* What pitch the camera should move to if under automatic control. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CameraDefaultPitch;

	/* The rate at which the pitch should go to its default value. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CameraDefaultPitchRate;
};

USTRUCT()
struct FCameraModelFade
{

	GENERATED_USTRUCT_BODY()

	/* Whether or not the model should fade out if the camera gets close. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool ModelFadeEnabled;

	/* The minumum distance the camera can be from the player model before the player model begines to fade out. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float ModelFadeDistance;
};

USTRUCT()
struct FCelShader
{

	GENERATED_USTRUCT_BODY()

	/* Sets the darkest color that the cel shader will render (as a fraction of the original image's color). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float LightMin;

	/* Sets the brightest color that the cel shader will render (as a fraction of the original image's color). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float LightMax;

	/* Number that the reconstruced light map will be multiplied by before being mapped to the gradient. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float MultiplicativeLightBias;

	/* Number added to reconstruced light map before being mapped to the gradient. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float AdditiveLightBias;

	/* Color used for white in the cel shader. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FLinearColor Tint;
};

UENUM() enum AimType {
	HOLD			UMETA(DisplayName = "Hold"),
	TOGGLE			UMETA(DisplayName = "Toggle")
};

UENUM() enum AxisType {
	STANDARD		UMETA(DisplayName = "Standard"),
	INVERTED		UMETA(DisplayName = "Inverted")
};

UCLASS() class AAuyron : public APawn
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

	class PlayerState
	{
	public:
		PlayerState() {};
		virtual void Tick(AAuyron* Player, float DeltaTime);
		virtual void Tick2(AAuyron* Player, float DeltaTime);
		virtual void PhysicsStuff(AAuyron* Player, float DeltaTime);
		virtual void CameraStuff(AAuyron* Player, float DeltaTime);
		virtual void FaceTargetDirection(AAuyron* Player, float DeltaTime);
	};

	class NormalState : public PlayerState
	{
	public:
		NormalState() {};
		virtual void Tick(AAuyron* Player, float DeltaTime);
	};
	
	class DialogueState : public PlayerState
	{
	public:
		DialogueState() {};
		virtual void Tick(AAuyron* Player, float DeltaTime);
	};

	class ClimbingState : public PlayerState
	{
	public:
		ClimbingState() {};
		virtual void Tick(AAuyron* Player, float DeltaTime);
	};

	class TeleportingState : public PlayerState
	{
	public:
		TeleportingState() {};
		virtual void Tick(AAuyron* Player, float DeltaTime);
	};

	class AimingState : public PlayerState
	{
	public:
		AimingState() {};
		virtual void Tick(AAuyron* Player, float DeltaTime);
	};

	//Input functions
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);
	void ControllerPitchCamera(float AxisValue);
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
	void Attack();
	void CameraZoomIn();
	void CameraZoomOut();
	void Respawn();
	void HereWeGo();
	void MoveIt();
	void FadeInMusic();
	void whywhy();
	void StopClimbing();

	void FlattenVelocity();
	
	UFUNCTION()
	void Hit(class UPrimitiveComponent* thisguy, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	//UFUNCTION()
	//void Stay(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void UnHit(class UPrimitiveComponent* thisguy, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	FVector GetPlayerLocation();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	float GetSpeed();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	FVector GetPlayerVelocity();
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
	bool GetIsDashing();
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
	bool GetIsInDialogue();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	float GetWarpTimerCompleted();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	FString GetDialogueText();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	float GetDialogueWidth();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	bool AboutToWarp();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void SetAimStyle(FString Style);
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	FString GetAimStyle();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void SetXAxisStyle(FString Style);
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	FString GetXAxisStyle();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void SetXAxisAimingStyle(FString Style);
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	FString GetXAxisAimingStyle();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void SetYAxisStyle(FString Style);
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	FString GetYAxisStyle();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void SetYAxisAimingStyle(FString Style);
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	FString GetYAxisAimingStyle();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void SetEnableTeleportAnimation(bool ShouldEnable);
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	bool GetEnableTeleportAnimation();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void HandlePause();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void BlockInput();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void ResumeInput();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void SetLightMin(float newvalue);
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void SetLightMax(float newvalue);
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void SetAdditiveLightBias(float newvalue);
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void SetMultiplicativeLightBias(float newvalue);
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void SetTint(FLinearColor newvalue);
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void SetMaterial(int32 index, UMaterialInterface* newmat);
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	USkeletalMeshComponent* GetMesh();
	
	
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	bool HasTeleport();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void SetHasTeleport(bool has);
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	bool HasDash();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void SetHasDash(bool has);
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	bool HasWallJump();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void SetHasWallJump(bool has);
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	bool HasGlide();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void SetHasGlide(bool has);
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	bool HasSlam();
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void SetHasSlam(bool has);

	
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	void SetStillScrolling(bool b);
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	bool GetSkipText();

	
	UFUNCTION(BlueprintCallable, Category = "Auyron Interface")
	bool GetIsClimbing();

	FVector MovementInput;
	FVector CameraInput;
	FVector CameraMouseInput;
	FVector CameraControllerInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") USkeletalMeshComponent* PlayerModel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UStaticMeshComponent* TeleClaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UStaticMeshComponent* BootsR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UStaticMeshComponent* BootsL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UStaticMeshComponent* Belt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UStaticMeshComponent* Bracelet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UStaticMeshComponent* Wings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UStaticMeshComponent* TheAbyss;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UParticleSystemComponent* DashParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UParticleSystemComponent* FloatParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UParticleSystemComponent* SlamParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UParticleSystemComponent* SlamTrail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UParticleSystemComponent* TrailParticlesL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UParticleSystemComponent* TrailParticlesR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UParticleSystemComponent* grassparticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UPostProcessComponent* PostProcess;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UDecalComponent* DropShadow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") class UAuyronMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials") UMaterialInterface* HairMatBase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials") UMaterialInterface* BandanaMatBase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials") UMaterialInterface* BodyMatBase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials") UMaterialInterface* TestTeleEffectBase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials") UMaterialInterface* TeleportRiftMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials") UTextureRenderTarget2D* TeleportRiftRenderTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials") UMaterialInterface* CelShaderMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials") UMaterialInterface* DrawBehindMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundCue* JumpSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundCue* DashSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundCue* CollectSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundCue* WarpSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundCue* DunkSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundCue* DunkHitSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundCue* WingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") FMovementPhysics PhysicsSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") FMovementJumping JumpSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") FMovementTurning TurnSettings;

	/* I really hope I don't need to write a tooltip for what this is. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") float AttackRange;

	/* Should be obvious as well. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") TEnumAsByte<AimType> AimStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") TEnumAsByte<AxisType> XAxisStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") TEnumAsByte<AxisType> XAxisAimingStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") TEnumAsByte<AxisType> YAxisStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") TEnumAsByte<AxisType> YAxisAimingStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") FAbilitiesTeleport TeleportSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") FAbilitiesDash DashSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") FAbilitiesGlide GlideSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") FAbilitiesSlam SlamSettings;

	/* Highest angle the camera is allowed to have. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") float CameraMaxAngle;

	/* Lowest angle the camera is allowed to have. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") float CameraMinAngle;

	/* Default distance that the camera should be away from the player. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") float DefaultArmLength;

	/* Minimum distance from the player that the camera can be zoomed in to. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") float MinimumArmLength;

	/* Maximum distance from the player that the camera can be zoomed out to. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") float MaximumArmLength;

	/* Number (from 0 to 1) that controlls how fast the camera moves to its new zoom level. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") float CameraZoomRate;

	/* Distance between each camera zoom level. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") float CameraZoomStep;

	/* Number to multiply the camera lag factor by for each zoom level closer than the default one. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") float CameraLagZoomScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") FCameraLag CameraLagSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") FCameraAutoTurn CameraAutoTurnSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") FCameraModelFade CameraModelFadeSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") FCelShader CelShaderSettings;

	PlayerState* CurrentState;
	NormalState Normal;
	DialogueState Dialogue;
	ClimbingState Climbing;
	TeleportingState Teleporting;
	AimingState Aiming;

	bool ActivateNextFrame;
	int GemCount;
	FVector Right;
	FVector Forward;
	float TimeSinceLastRealignment;
	float HowLong;
	float dashtimer;
	float TimeSinceLastMouseInput;
	float TimeSinceLastMovementInputReleased;
	//float AutoCameraCurrentPitch;
	float TeleportRange;
	float TeleportAngleTolerance;
	bool JumpPressed;
	bool HoldingJump;
	bool JustJumped;
	bool JustWallJumped;
	bool RidingWall;
	bool lastd;
	bool SlamNextFrame;
	bool JustSlammed;
	bool AlreadySlammed;
	bool AlreadyAirDash;
	bool OnTheGround;
	bool HelpEnabled;

	FScriptDelegate onbeginoverlapdelegate;
	FScriptDelegate onendoverlapdelegate;
	FScriptDelegate onhitdelegate;

	AStick* closeststick;

	FVector StoredWallNormal;

	ACameraOverrideRegion* currentoverrideregion;

	UUserWidget* thehud;

	FVector pushvelocity;

	FVector closecamera;

	FTransform lastcamerabeforedialogue;

	AAmbientSound* currentmusic;

	UPhysicalMaterial* physmat;

	ASceneCaptureCube* CaptureCube;
	ASceneCapture2D* Capture2D;

	FTimerHandle WarpAnimationTimer;
	FTimerHandle PreWarpTimer;
	FTimerHandle PreWarpTimer2;
	FTimerHandle MusicChangeTimer;
	FTimerHandle WHY;
	FTimerHandle climbtimer;

	UMaterialInstanceDynamic* hairmat;
	UMaterialInstanceDynamic* bandanamat;
	UMaterialInstanceDynamic* bodymat;
	UMaterialInstanceDynamic* teletestmat;
	UMaterialInstanceDynamic* outlinemat;
	UMaterialInstanceDynamic* celshadermat;

	UMaterialInterface* bluewings;

	float DefaultGravity;

	float lastdt;
	float SpeedRelativeToGround;

	bool GlideNextFrame;
	bool IsGliding;
	bool AlreadyUnjumped;
	FVector actualvelocity;
	bool AlreadyGlided;
	float GlideTimer;
	float warptimer;
	float timesinceoverrideenter;

	bool justlandedcameraflag;

	bool IsInDialogue;
	ADialogueCut* CurrentCut;
	ANPC* CurrentNPC;
	FTransform DialogueCameraTransform;
	FString CurrentLine;
	float CurrentTextWidth;

	AStick* thisguy;

	bool justteleported;
	bool justswished;

	bool stillscrolling;
	bool onsandship;
	bool skiptext;

	float dt;

	float warpanimtimer;

	FVector climblocation;
	FVector ledgegroundvelocity;
	float ledgeangularfrequency;
	UStaticMeshComponent* grabbedledge;

	FVector warphere;
	FVector warpvel;

	bool lel;

	bool AttackPressed;

	bool currentdialoguebpactivated;

	uint8 jumpsLeft;
	bool justDoubleJumped;
	FVector AppliedForce;
	bool WasOnTheGround;
	bool swish;
	bool DashNextFrame;
	bool holdingdash;
	bool dashing;
	bool airdashing;
	bool ztarget;
	bool wasztarget;
	bool movementlocked;
	bool cameralocked;
	bool InCameraOverrideRegion;
	bool itshappening;
	bool dunk;
	bool cameramode;
	FVector previousgroundvelocity;
	bool MovementAxisLocked;
	float LockedAxisValue;
	FVector previousposition;
	FVector previousvelocity;

	FVector RespawnPoint;

	bool blockedbyblueprint;

	bool WasInCameraOverrideRegion;

	bool ingrass;
	bool grassactive;

	bool ishanging;
	bool isclimbing;

	float ActualDefaultArmLength;
	float ActualDefaultCameraLag;
	float ActualTeleportAnimationDuration;
	float TargetDefaultArmLength;
	float BackupDefaultArmLength;
	
	float defaultfov;
	TEnumAsByte<MovementRegionLockedAxis::Axis> LockedMovementAxis;
	USplineComponent* newspline;
	FVector CameraOverrideTargetDisplacement;
	FVector CameraOverrideTargetOffset;
	FVector CameraOverrideTargetLocation;
	FRotator CameraOverrideTargetRotation;
	FRotator TargetDirection;
	FRotator OldDirection;
	FVector Velocity;
	FVector LastPlatformVelocity;
	FVector2D OldMovementInput;
};