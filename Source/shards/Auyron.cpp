// DON'T TELL ME WHERE TO FILL OUT MY COPYRIGHT NOTICE YOU'RE NOT MY REAL DAD.

#include "shards.h"
#include "Gem.h"
#include "Auyron.h"
#include "AuyronMovementComponent.h"
#include "CameraOverrideRegion.h"
#include "EngineUtils.h" 
#include "Stick.h"  
#include "Checkpoint.h"
#include "TeleClaw.h"
#include "MusicRegion.h"
#include "TwoDimensionalMovementRegion.h"
#include "ForceRegion.h"
#include "WarpCrystal.h"
#include "Switch.h"

// Sets default values
AAuyron::AAuyron()
{	
	// These should work.
	PhysicsSettings.GroundAccelerationRate = 5500.0f;
	PhysicsSettings.AirAccelerationRate = 500.0f;
	PhysicsSettings.MaxVelocity = 500.0f;
	PhysicsSettings.TerminalVelocity = 2000.0f;
	PhysicsSettings.Gravity = 900.0f;
	PhysicsSettings.MaxSlope = 45.0f;

	JumpSettings.JumpPower = 450.0f;
	JumpSettings.WallJumpMultiplier = 1.0f;
	JumpSettings.OffGroundJumpTime = 0.04f;
	JumpSettings.UnjumpRate = 0.33f;

	TurnSettings.TurnRate = 720.0f;
	TurnSettings.FacingAngleSnapThreshold = 5.0f;

	TeleportSettings.TeleportRangeWhenAiming = 4000.0f;
	TeleportSettings.TeleportAngleToleranceWhenAiming = 5.0f;
	TeleportSettings.TeleportRangeWhenNotAiming = 900.0f;
	TeleportSettings.TeleportAngleToleranceWhenNotAiming = 70.0f;
	TeleportSettings.TeleportAnimationDuration = 0.4f;
	TeleportSettings.TeleportFOV = 160.0f;
	TeleportSettings.TeleportAnimationPowerFactor = 4.0f;
	TeleportSettings.TeleportAnimationRestoreThreshold = 0.15f;
	TeleportSettings.TeleportLightColor = FColor(0x336FE6FF);

	DashSettings.HasDash = false;
	DashSettings.HasInfiniteDash = false;
	DashSettings.HasDashJump = false;
	DashSettings.HasDashWallJump = false;
	DashSettings.DashSpeed = 1500.0f;
	DashSettings.DashDuration = 0.25f;
	DashSettings.DashWallJumpMultiplier = 3.0f;

	GlideSettings.HasGlide = false;
	GlideSettings.GlideTurnRateMultiplier = 1.0f;
	GlideSettings.GlideDuration = 2.0f;
	GlideSettings.InitialGlideVelocity = 100.0f;
	GlideSettings.GlideGravityMultiplier = 50.0f;

	SlamSettings.HasSlam = false;
	SlamSettings.SlamVelocity = 1000.0f;

	CameraMaxAngle = 85.0f;
	CameraMinAngle = -85.0f;
	DefaultArmLength = 1000.0f;

	CameraLagSettings.CameraLag = 3.0f;
	CameraLagSettings.CameraRotationLag = 10.0f;
	CameraLagSettings.AimingLagMultiplier = 0.0f;
	CameraLagSettings.OverrideRegionRotationLagMultiplier = 0.75f;

	CameraAutoTurnSettings.CameraAutoTurnFactor = 60.0f;
	CameraAutoTurnSettings.CameraResetTime = 1.0f;

	CameraModelFadeSettings.ModelFadeEnabled = false;
	CameraModelFadeSettings.ModelFadeDistance = 250.0f;

	HelpEnabled = false;

	// Just in case.
	TargetDirection = FRotator::ZeroRotator;
	Velocity = FVector::ZeroVector;

	// Oh you better believe we're ticking every frame.
	PrimaryActorTick.bCanEverTick = true;

	// I wanted to be a cylinder, but no, we gotta be a capsule.
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	CapsuleComponent->InitCapsuleSize(45.0f, 90.0f);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuyron::Hit);
	CapsuleComponent->OnComponentHit.AddDynamic(this, &AAuyron::Stay);
	CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &AAuyron::UnHit);
	CapsuleComponent->SetSimulatePhysics(true);
	CapsuleComponent->SetEnableGravity(false);
	CapsuleComponent->SetLinearDamping(10.0f);
	CapsuleComponent->AttachTo(RootComponent);
	SetActorEnableCollision(true);

	// It you.
	PlayerModel = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VisualRepresentation"));
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMeshObj(TEXT("/Game/Models/Characters/Auyron/Auyron"));
	PlayerModel->SetSkeletalMesh(PlayerMeshObj.Object);
	PlayerModel->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	PlayerModel->AttachTo(CapsuleComponent);

	// Use a spring arm so the camera can be all like swoosh.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 60.0f), FRotator(-30.0f, 0.0f, 0.0f));
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = CameraLagSettings.CameraLag;
	SpringArm->CameraRotationLagSpeed = CameraLagSettings.CameraRotationLag;
	SpringArm->CameraLagMaxDistance = 1000.0f;
	SpringArm->AttachTo(CapsuleComponent);

	// Camera so the casuals can "see what they're doing" or whatever.
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);

	// May god have mercy on your GPU.
	DashParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Dash Particles"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> dp(TEXT("/Game/Particles/DashParticles"));
	DashParticles->SetTemplate(dp.Object);
	DashParticles->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	DashParticles->AttachTo(PlayerModel);

	FloatParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Float Particles"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> fp(TEXT("/Game/Particles/FloatParticles"));
	FloatParticles->SetTemplate(fp.Object);
	FloatParticles->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	FloatParticles->AttachTo(PlayerModel);

	SlamParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Slam Particles"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> sp(TEXT("/Game/Particles/SlamParticles"));
	SlamParticles->SetTemplate(sp.Object);
	SlamParticles->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	SlamParticles->AttachTo(PlayerModel);

	SlamTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Slam Trail"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> st(TEXT("/Game/Particles/SlamTrail"));
	SlamTrail->SetTemplate(st.Object);
	SlamTrail->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	SlamTrail->AttachTo(PlayerModel);

	TrailParticlesL = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particles L"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> tpl(TEXT("/Game/Particles/TrailParticles"));
	TrailParticlesL->SetTemplate(tpl.Object);
	TrailParticlesL->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	TrailParticlesL->AttachTo(PlayerModel);

	TrailParticlesR = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particles R"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> tpr(TEXT("/Game/Particles/TrailParticles"));
	TrailParticlesR->SetTemplate(tpr.Object);
	TrailParticlesR->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	TrailParticlesR->AttachTo(PlayerModel);

	// Get the instance of the TeleClaw weapon.
	TeleClaw = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleClaw"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> tc(TEXT("/Game/Models/Weapons/TeleClaw"));
	TeleClaw->SetStaticMesh(tc.Object);

	// ASSUMING DIRECT CONTROL.
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Apparently we need some newfangled "MovementComponent".
	MovementComponent = CreateDefaultSubobject<UAuyronMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CapsuleComponent;

	// BLAST PROCESSING.
	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	PostProcess->AttachTo(RootComponent);

	// NINTENDON'T DO 16 BIT.
	const ConstructorHelpers::FObjectFinder<UMaterialInterface> sw(TEXT("/Game/bestmaterial"));
	ScreenWarpMatBase = sw.Object;

	const ConstructorHelpers::FObjectFinder<UMaterialInterface> hair(TEXT("/Game/Textures/Characters/Auyron/Hair"));
	HairMatBase = hair.Object;

	const ConstructorHelpers::FObjectFinder<UMaterialInterface> bandana(TEXT("/Game/Textures/Characters/Auyron/Headband"));
	BandanaMatBase = bandana.Object;

	const ConstructorHelpers::FObjectFinder<UMaterialInterface> body(TEXT("/Game/Textures/Characters/Auyron/protag-UVs_Mat"));
	BodyMatBase = body.Object;

}

void AAuyron::Respawn() {
	SetActorLocation(RespawnPoint);
}

void AAuyron::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	hairmat = UMaterialInstanceDynamic::Create(HairMatBase, this);
	bandanamat = UMaterialInstanceDynamic::Create(BandanaMatBase, this);
	bodymat = UMaterialInstanceDynamic::Create(BodyMatBase, this);

	PlayerModel->SetMaterial(0, bodymat);
	PlayerModel->SetMaterial(1, hairmat);
	PlayerModel->SetMaterial(2, bandanamat);

	screenwarpmat = UMaterialInstanceDynamic::Create(ScreenWarpMatBase, this);
	PostProcess->bUnbound = true;
	PostProcess->AddOrUpdateBlendable(screenwarpmat);
}

void AAuyron::UnHit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {
		// We just exited a camera override region.
		if (OtherActor->IsA(ACameraOverrideRegion::StaticClass())) {
			// Set the camera back to normal, but only if the region we just
			// exited was affecting the camera in the first place.
			if (CameraOverrideLookAtPlayer == ((ACameraOverrideRegion*)OtherActor)->LookAtPlayer &&
				CameraOverrideTargetDisplacement == ((ACameraOverrideRegion*)OtherActor)->TargetCamera->GetComponentLocation() &&
				CameraOverrideTargetRotation == ((ACameraOverrideRegion*)OtherActor)->TargetCamera->GetComponentRotation()) {
				cameralocked = false;
				CameraLockToPlayerXAxis = false;
				CameraLockToPlayerYAxis = false;
				CameraLockToPlayerZAxis = false;
				CameraOverrideLookAtPlayer = false;
				CameraOverrideTargetDisplacement = FVector::ZeroVector;
				CameraOverrideTargetRotation = FRotator::ZeroRotator;
			}
		}
		if (OtherActor->IsA(AForceRegion::StaticClass())) {
			AppliedForce -= ((AForceRegion*)OtherActor)->Direction * ((AForceRegion*)OtherActor)->Magnitude;
		}
		if (OtherActor->IsA(ATwoDimensionalMovementRegion::StaticClass())) {
			MovementAxisLocked = false;
		}
		
		if (OtherActor->IsA(AMusicRegion::StaticClass())) {
			((AMusicRegion*)OtherActor)->Music->FadeOut(2.0f,0.0f);
		}
	}
}

void AAuyron::Stay(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {
		if (OtherActor->IsA(ACameraOverrideRegion::StaticClass())) {
			cameralocked = true;
			CameraLockToPlayerXAxis = ((ACameraOverrideRegion*)OtherActor)->LockToPlayerXAxis;
			CameraLockToPlayerYAxis = ((ACameraOverrideRegion*)OtherActor)->LockToPlayerYAxis;
			CameraLockToPlayerZAxis = ((ACameraOverrideRegion*)OtherActor)->LockToPlayerZAxis;
			CameraOverrideLookAtPlayer = ((ACameraOverrideRegion*)OtherActor)->LookAtPlayer;
			CameraOverrideTargetDisplacement = ((ACameraOverrideRegion*)OtherActor)->TargetCamera->GetComponentLocation();
			CameraOverrideTargetRotation = ((ACameraOverrideRegion*)OtherActor)->TargetCamera->GetComponentRotation();
		}
		if (OtherActor->IsA(ATwoDimensionalMovementRegion::StaticClass())) {
			MovementAxisLocked = true;
			LockedAxisValue = ((ATwoDimensionalMovementRegion*)OtherActor)->LockedCoordinate;
			LockedMovementAxis = ((ATwoDimensionalMovementRegion*)OtherActor)->LockedAxis;
		}
	}
}
void AAuyron::Hit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Stop hitting yourself.
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		// We just picked up a gem.
		if (OtherActor->IsA(AGem::StaticClass()))
		{
			OtherActor->Destroy();
			GemCount++;
		}

		// We just entered a camera override region.
		if (OtherActor->IsA(ACameraOverrideRegion::StaticClass())) {
			// Lock the camera and place it at the position and rotation given by the region.
			cameralocked = true;
			CameraLockToPlayerXAxis = ((ACameraOverrideRegion*)OtherActor)->LockToPlayerXAxis;
			CameraLockToPlayerYAxis = ((ACameraOverrideRegion*)OtherActor)->LockToPlayerYAxis;
			CameraLockToPlayerZAxis = ((ACameraOverrideRegion*)OtherActor)->LockToPlayerZAxis;
			CameraOverrideLookAtPlayer = ((ACameraOverrideRegion*)OtherActor)->LookAtPlayer;
			CameraOverrideTargetDisplacement = ((ACameraOverrideRegion*)OtherActor)->TargetCamera->GetComponentLocation();
			CameraOverrideTargetRotation = ((ACameraOverrideRegion*)OtherActor)->TargetCamera->GetComponentRotation();
		}
		if (OtherActor->IsA(AForceRegion::StaticClass())) {
			AppliedForce += ((AForceRegion*)OtherActor)->Direction * ((AForceRegion*)OtherActor)->Magnitude;
		}
		if (OtherActor->IsA(ATwoDimensionalMovementRegion::StaticClass())) {
			MovementAxisLocked = true;
			LockedAxisValue = ((ATwoDimensionalMovementRegion*)OtherActor)->LockedCoordinate;
			LockedMovementAxis = ((ATwoDimensionalMovementRegion*)OtherActor)->LockedAxis;
		}

		if (OtherActor->IsA(ACheckpoint::StaticClass())) {
			RespawnPoint = ((ACheckpoint*)OtherActor)->RespawnPoint->GetComponentLocation();
		}

		if (OtherActor->IsA(AMusicRegion::StaticClass())) {
			((AMusicRegion*)OtherActor)->Music->FadeIn(2.0f);
		}

		if (OtherActor->IsA(AWarpCrystal::StaticClass())) {
			SetActorLocation(((AWarpCrystal*)OtherActor)->WarpExit->GetComponentLocation());
		}
	}
}

// GAME START.
void AAuyron::BeginPlay()
{
	Super::BeginPlay();

	// Set the spring arm's length.
	SpringArm->TargetArmLength = DefaultArmLength;

	// Set the max slope and max off ground time for the movement component.
	MovementComponent->maxslope = PhysicsSettings.MaxSlope;
	MovementComponent->MaxOffGroundTime = JumpSettings.OffGroundJumpTime;

	// Point gravuty downwards.
	PhysicsSettings.Gravity = -PhysicsSettings.Gravity;
	DefaultGravity = PhysicsSettings.Gravity;

	// Initialize gem count.
	GemCount = 0;

	defaultfov = Camera->FieldOfView;

	warpanimtimer = -1.0f;
	screenwarpmat->SetScalarParameterValue(TEXT("Wooshiness"), 0.0f);

	previousposition = GetActorLocation();
	closecamera = GetActorLocation();
	RespawnPoint = GetActorLocation();

	// Sets the player's "true" facing direction to whatever
	// the model's facing direction is in the editor.
	TargetDirection = PlayerModel->GetComponentRotation();

	TeleClaw->AttachTo(PlayerModel, "RightHand", EAttachLocation::SnapToTargetIncludingScale);

	((APlayerController*)GetController())->SetAudioListenerOverride(PlayerModel, FVector::ZeroVector, FRotator::ZeroRotator);

	// Start with the particles off.
	DashParticles->bAutoActivate = false;
	DashParticles->DeactivateSystem();
	FloatParticles->bAutoActivate = false;
	FloatParticles->DeactivateSystem();
	SlamParticles->bAutoActivate = false;
	SlamParticles->DeactivateSystem();
	SlamTrail->bAutoActivate = false;
	SlamTrail->DeactivateSystem();

	//if (Hud) {
		// Create the widget and store it.
		//thehud = CreateWidget<UUserWidget>(GetWorld(), Hud);

		// now you can use the widget directly since you have a referance for it.
		// Extra check to  make sure the pointer holds the widget.
		//if (thehud)
		//{
			//let add it to the view port
			//thehud->AddToViewport();
		//}
	//}
}

// Called every frame UNLIKE UNITY MIRITE?
void AAuyron::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// This isn't Doom.
	MovementInput = MovementInput.GetClampedToMaxSize(1.0f);

	// Used for situations where the player's movement may be impeded and for debug info.
	actualvelocity = (GetActorLocation() - previousposition) / DeltaTime;

	// If there is a sharp change in the velocity of the platform that the player is
	// standing on, immediately snap the player's velocity to match it. 
	if (!MovementComponent->groundvelocity.IsNearlyZero() && !JumpNextFrame && FMath::Abs((FVector::VectorPlaneProject(MovementComponent->groundvelocity,FVector::UpVector) - FVector::VectorPlaneProject(previousgroundvelocity,FVector::UpVector)).Size()) > 200.0f) {
		Velocity = MovementComponent->groundvelocity + FVector(0.0f, 0.0f, -100.0f);
	}

	// Set our frame of reference for future calculations to be that of the surface we're standing on.
	Velocity -= MovementComponent->groundvelocity;

	// Update timing values.
	TimeSinceLastMouseInput += DeltaTime;

	// Reset temporary booleans.
	JustJumped = false;
	JustWallJumped = false;
	SpringArm->bEnableCameraLag = true;
	bool cameralocktemp = cameralocked;

	// Set and range and angle to the "short range" values.
	float TeleportRange = TeleportSettings.TeleportRangeWhenNotAiming;
	float TeleportAngleTolerance = TeleportSettings.TeleportAngleToleranceWhenNotAiming;

	// Temporarily unlock the camera if the player is using the free camera mode.
	if (cameramode) {
		cameralocked = false;
	}

	// Move the camera in response to mouse movement.
	{

		// Handle camera movement when the camera is controllable.
		if (!cameralocked || ztarget) {

			// Reset camera input timer if the camera controls were touched.
			if (CameraInput.X != 0.0f || CameraInput.Y != 0.0f) {
				TimeSinceLastMouseInput = 0.0f;
			}

			// Temporary variable to hold the camera's new rotation.
			FRotator NewRotation = SpringArm->GetComponentRotation();

			// Move the camera's yaw in response to the x input of the mouse/stick.
			NewRotation.Yaw = NewRotation.Yaw + CameraInput.X;

			// The camera should only turn with the player if the mouse hasn't been touched recently.
			if (TimeSinceLastMouseInput > CameraAutoTurnSettings.CameraResetTime && !ztarget && !movementlocked) {
				NewRotation.Yaw += FMath::Pow(FMath::Abs(MovementInput.X),1.0f) * (Camera->GetRightVector().GetSafeNormal() | FVector::VectorPlaneProject(actualvelocity,FVector::UpVector)/ PhysicsSettings.MaxVelocity) * DeltaTime * CameraAutoTurnSettings.CameraAutoTurnFactor;
			}

			// Set the rotation of the camera.
			SpringArm->SetWorldRotation(NewRotation);

			// Move the camera's pitch in response to the y input of the mouse/stick.
			NewRotation = SpringArm->GetComponentRotation();
			NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -CameraMaxAngle, -CameraMinAngle);
			SpringArm->CameraRotationLagSpeed = CameraLagSettings.CameraRotationLag;
			SpringArm->SetWorldRotation(NewRotation);

		} else {

			// If the camera is locked, put it at the override region's
			// target location and rotation.
			SpringArm->CameraRotationLagSpeed = CameraLagSettings.CameraRotationLag*CameraLagSettings.OverrideRegionRotationLagMultiplier;
			SpringArm->SetWorldRotation(CameraOverrideTargetRotation);
			if (!CameraLockToPlayerXAxis&&!CameraLockToPlayerYAxis&&!CameraLockToPlayerZAxis) {
				SpringArm->SetWorldLocation(CameraOverrideTargetDisplacement);
			} else {
				SpringArm->SetWorldLocation(FVector(
					(CameraLockToPlayerXAxis ? GetActorLocation().X : CameraOverrideTargetDisplacement.X),
					(CameraLockToPlayerYAxis ? GetActorLocation().Y : CameraOverrideTargetDisplacement.Y),
					(CameraLockToPlayerZAxis ? GetActorLocation().Z : CameraOverrideTargetDisplacement.Z)));
			}

			// Face the camera towards the player if the region specifies to do so.
			if (CameraOverrideLookAtPlayer) {
				SpringArm->SetRelativeRotation((-SpringArm->RelativeLocation).Rotation());
			}
		}

		// The player is aiming.
		if (ztarget&&!dashing) {

			// Set teleport range and angle to the "long range" values.
			TeleportRange = TeleportSettings.TeleportRangeWhenAiming;
			TeleportAngleTolerance = TeleportSettings.TeleportAngleToleranceWhenAiming;

			// Find the direction the player and camera should be facing.
			FRotator NewRotation = SpringArm->RelativeRotation;

			// Update the player model's target facing direction.
			TargetDirection.Yaw = NewRotation.Yaw;

			// If the player just started aiming...
			if (!wasztarget) {

				// ...orient the new rotation to be level with the xy axis...
				NewRotation.Pitch = 0.0f;

				// ...and face the camera in direction that the player is facing...
				if (MovementInput.IsNearlyZero()) {
					NewRotation.Yaw = PlayerModel->GetComponentRotation().Yaw;
				} else {
					// ...unless the player is holding a direction,
					// in which case face that direction.
					NewRotation.Yaw = SpringArm->GetComponentRotation().Yaw +
						(MovementInput.X >= 0 ? 1 : -1) * FMath::RadiansToDegrees(FMath::Acos(MovementInput.GetSafeNormal() | FVector(0, 1, 0)));
				}
			}

			// Face the player and camera to the new rotation.
			PlayerModel->RelativeRotation.Yaw = NewRotation.Yaw;
			SpringArm->TargetArmLength = 300.0f;
			SpringArm->CameraLagSpeed = CameraLagSettings.CameraLag * CameraLagSettings.AimingLagMultiplier;
			SpringArm->CameraRotationLagSpeed = CameraLagSettings.CameraRotationLag * CameraLagSettings.AimingLagMultiplier;
			SpringArm->SetRelativeRotation(NewRotation);

			// Get the camera's right and forward vectors and transform them from world to realtive vectors.
			FVector Right = FVector::VectorPlaneProject(CapsuleComponent->GetComponentRotation().RotateVector(Camera->GetRightVector()), FVector::UpVector);
			FVector Forward = FVector::VectorPlaneProject((CapsuleComponent->GetComponentRotation() * -1.0f).RotateVector(Camera->GetForwardVector()), FVector::UpVector);

			// Offset the spring arm (and therefore the camera) a bit so the player model
			// isn't blocking the screen when we're trying to aim.
			FVector base = FVector(0.0f, 100.0f, 100.0f);
			base = (base.X*Forward + base.Y*Right + base.Z*FVector::UpVector);
			SpringArm->SetRelativeLocation(base);

			// Lock the player's movement inputs.
			movementlocked = true;

		} else {

			// Return the spring arm (and camera) to its original location.
			if (!cameralocked) {
				SpringArm->TargetArmLength = DefaultArmLength;
				SpringArm->CameraLagSpeed = CameraLagSettings.CameraLag;
				SpringArm->CameraRotationLagSpeed = CameraLagSettings.CameraRotationLag;
				SpringArm->SetRelativeLocation(FVector::ZeroVector);
				
				// If we just stopped aiming, reset the camera's rotation as well.
				if (wasztarget) {
					SpringArm->SetRelativeRotation(FRotator(-30.0f, SpringArm->GetComponentRotation().Yaw, 0.0f));
				}
			}

			// Re-enable the player's movement inputs.
			movementlocked = false;
		}

		//PlayerModel->param

		// Restore camera's old locked state.
		cameralocked = cameralocktemp;
	}

	// Move the player in response to movement inputs.
	{
		{
			// Use the player as the source for the teleport raycast...
			FVector source = GetActorLocation();
			FVector forward = PlayerModel->GetForwardVector();

			// ...unless they're aiming, in which case use the camera.
			if (ztarget) {
				source = Camera->GetComponentLocation();
				forward = Camera->GetForwardVector();
			}

			AStick* closest = NULL;
			float biggestdot = -1.0f;

			// Iterate over each TelePad and cast a ray.
			for (TActorIterator<AStick> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
				if (ActorItr->GetClass()->GetName() == "Stick") {

					// Assume it's not being targeted, and set it to the default
					// color and brightness.
					ActorItr->PointLight->LightColor = FColor::White;
					ActorItr->PointLight->Intensity = 1385.76f;
					ActorItr->PointLight->UpdateColorAndBrightness();

					// Get displacement vector from the player/camera to the TelePad.
					FVector displacement = ActorItr->GetActorLocation() - source;

					// Get the dot product between the displacement and the source.
					float dot = displacement.GetSafeNormal() | forward.GetSafeNormal();

					// Set trace parameters. I have no idea what these do but the raycast doesn't work
					// if I don't put these here.
					FHitResult f;
					FCollisionObjectQueryParams TraceParams(ECollisionChannel::ECC_WorldStatic);
					TraceParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
					FCollisionQueryParams asdf = FCollisionQueryParams();

					// Don't want the ray to collide with the player model now do we?
					asdf.AddIgnoredActor(this);

					// Figure out if the ray is blocked by an object.
					bool blocked = GetWorld()->LineTraceSingleByObjectType(f, source, ActorItr->GetActorLocation(), TraceParams, asdf);

					// If the trace hit a telepad and it's closer to where we're aiming
					// at than any other TelePad, set it as the "closest" one.
					if (f.GetActor() != nullptr && f.GetActor()->GetClass() != nullptr && f.GetActor()->GetClass()->GetName() == "Stick" && dot > biggestdot && blocked && displacement.Size() < TeleportRange) {
						closest = *ActorItr;
						biggestdot = dot;
					}
				}
			}

			// Set the color of the "closest" TelePad as long as it's in range
			// and within a certain angle tolerance and teleport he player to it
			// if they are trying to teleport.
			if (closest != nullptr &&
				TeleportSettings.HasTeleport &&
				FMath::RadiansToDegrees(FMath::Acos(biggestdot)) < TeleportAngleTolerance &&
				(closest->GetActorLocation() - GetActorLocation()).Size() < TeleportRange) {

				AStick* s = closest;

				// Set the target TelePad's color and brightness so that it stands out.
				s->PointLight->LightColor = TeleportSettings.TeleportLightColor;
				s->PointLight->Intensity = 16.0*1385.76f;
				s->PointLight->UpdateColorAndBrightness();

				// Teleport the player to the TelePad if they're trying to teleport
				// and impart the post teleport velocity on them.
				if (swish) {
					if (s != nullptr) {
						// Move the player to the telepad's position and give them the perscribed velocity.
						SetActorLocation(s->gohere);
						Velocity = s->PostTeleportVelocity;
						ztarget = false;

						// Tell the animation blueprint that we're about to teleport.
						itshappening = true;

						// If we were aiming, reset the camera's rotation.
						if (wasztarget) {
							SpringArm->CameraLagSpeed = 0.0f;
							SpringArm->SetRelativeRotation(FRotator(-30.0f, SpringArm->GetComponentRotation().Yaw, 0.0f));
						}

						// Reset OnTheGround and glide variables.
						OnTheGround = false;
						WasOnTheGround = false;
						dunk = false;
						if (IsGliding) {
							AlreadyGlided = true;
							IsGliding = false;
						}
					}
				}
			}
			// Reset the "player wants to teleport" variable.
			swish = false;

			ASwitch* closestswitch = NULL;
			biggestdot = -1.0f;

			// Iterate over each Switch and cast a ray.
			for (TActorIterator<ASwitch> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
				if (ActorItr->GetClass()->GetName() == "Switch") {

					// Get displacement vector from the player/camera to the switch.
					FVector displacement = ActorItr->GetActorLocation() - source;

					// Get the dot product between the displacement and the source.
					float dot = displacement.GetSafeNormal() | forward.GetSafeNormal();

					// Set trace parameters. I have no idea what these do but
					// the raycast doesn't work if I don't put these here.
					FHitResult f;
					FCollisionObjectQueryParams TraceParams(ECC_Visibility);
					TraceParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
					FCollisionQueryParams asdf = FCollisionQueryParams();

					// Don't want the ray to collide with the player model now do we?
					asdf.AddIgnoredActor(this);

					// Figure out if the ray is blocked by an object.
					bool blocked = GetWorld()->LineTraceSingleByObjectType(f, source, ActorItr->GetActorLocation(), TraceParams, asdf);

					// If the trace hit a switch and it's closer to where we're aiming
					// at than any other switch, set it as the "closest" one.
					if (f.GetActor() != nullptr && f.GetActor()->GetClass() != nullptr && f.GetActor()->GetClass()->GetName() == "Switch" && dot > biggestdot && blocked && displacement.Size() < TeleportRange) {
						closestswitch = *ActorItr;
						biggestdot = dot;
					}
				}

				// Activate the "closest" switch as long as it's in range
				// and within a certain angle tolerance.
				if (closestswitch != nullptr &&
					FMath::RadiansToDegrees(FMath::Acos(biggestdot)) < TeleportAngleTolerance &&
					(closestswitch->GetActorLocation() - GetActorLocation()).Size() < closestswitch->MaxDistance) {
					if (ShouldActivate) {
						closestswitch->Activate();
						ShouldActivate = false;
					}
				}
			}
		}

		// Handle the screen warp animation.
		if (warpanimtimer >= 0.0f) {
			if (warpanimtimer < TeleportSettings.TeleportAnimationRestoreThreshold) {
				Camera->FieldOfView = FMath::Lerp(TeleportSettings.TeleportFOV, defaultfov, FMath::Pow((TeleportSettings.TeleportAnimationRestoreThreshold - warpanimtimer) / TeleportSettings.TeleportAnimationRestoreThreshold, TeleportSettings.TeleportAnimationPowerFactor));
			} else {
				Camera->FieldOfView = FMath::Lerp(TeleportSettings.TeleportFOV, defaultfov, FMath::Pow((warpanimtimer - TeleportSettings.TeleportAnimationRestoreThreshold) / (TeleportSettings.TeleportAnimationDuration - TeleportSettings.TeleportAnimationRestoreThreshold), TeleportSettings.TeleportAnimationPowerFactor));
			}
			warpanimtimer += DeltaTime;
			if (warpanimtimer >= TeleportSettings.TeleportAnimationDuration) {
				warpanimtimer = -1.0f;
			}
		}
		
		// Start the warp animation timer if the player just warped.
		if (itshappening) {
			itshappening = false;
			warpanimtimer = 0.0f;
		}

		// Make the player model translucent if the camera gets too close.
		float opacity = FMath::Clamp((Camera->GetComponentLocation() - GetActorLocation()).Size() / CameraModelFadeSettings.ModelFadeDistance, 0.0f, 1.0f);
		hairmat->SetScalarParameterValue(TEXT("fade"), (CameraModelFadeSettings.ModelFadeEnabled ? opacity : 1.0f));
		bandanamat->SetScalarParameterValue(TEXT("fade"), (CameraModelFadeSettings.ModelFadeEnabled ? opacity : 1.0f));
		bodymat->SetScalarParameterValue(TEXT("fade"), (CameraModelFadeSettings.ModelFadeEnabled ? opacity : 1.0f));

		// Lock the player's movement inputs if they're dashing.
		if (dashing) {
			movementlocked = true;
		}

		// If you can't move, don't move.
		if (movementlocked) {
			MovementInput = FVector::ZeroVector;

			// Lock jumping and gliding as well unless we're dashing.
			if (!dashing) {
				JumpNextFrame = false;
				GlideNextFrame = false;
			}
		}

		// The directions of "Right" and "Forward" depend on the direction that the camera's facing.
		FVector Right = Camera->GetRightVector();
		Right.Z = 0.0f;
		Right = Right.GetSafeNormal();
		FVector Forward = Camera->GetForwardVector();
		Forward.Z = 0.0f;
		Forward = Forward.GetSafeNormal();

		// Set up acceleration vector using the movement inputs.
		FVector AdjustedInput = MovementInput.GetClampedToSize(0.0f, 1.0f);
		FVector Acceleration = FVector::ZeroVector;
		Acceleration = (Right*AdjustedInput.X + Forward*AdjustedInput.Y)*(OnTheGround ? PhysicsSettings.GroundAccelerationRate : PhysicsSettings.AirAccelerationRate);

		// If the platform we're standing on is accelerating, add that acceleration to the player's acceleration,
		// but only if the player didn't just jump onto or off of the platform, and the platform didn't just
		// quickly and immediately change directions.
		if (!(previousgroundvelocity.IsNearlyZero() && !MovementComponent->groundvelocity.IsNearlyZero()) &&
			!(!previousgroundvelocity.IsNearlyZero() && MovementComponent->groundvelocity.IsNearlyZero()) &&
			(MovementComponent->groundvelocity - previousgroundvelocity).Size() / DeltaTime < 1000.0f) {
			Acceleration += (MovementComponent->groundvelocity - previousgroundvelocity) / DeltaTime;
		}

		// Decreases the effect of deceleration when the player is moving near max speed
		// and increases it when they are going slow. This makes decelerating feel much smoother.
		float slowfactor = FMath::Clamp(FMath::Lerp(FMath::Sqrt(Velocity.Size() / PhysicsSettings.MaxVelocity), FMath::Square(Velocity.Size() / PhysicsSettings.MaxVelocity), Velocity.Size() / PhysicsSettings.MaxVelocity), 0.0f, 1.0f);

		// Apply a deceleration that scales with the player's velocity
		// in such a way that it limits it to MaxVelocity.
		Acceleration -= (FVector::VectorPlaneProject(Velocity, FVector::UpVector)) * (OnTheGround ? PhysicsSettings.GroundAccelerationRate / PhysicsSettings.MaxVelocity : PhysicsSettings.AirAccelerationRate / PhysicsSettings.MaxVelocity) * slowfactor;

		// Ask the movement component if we're on the ground and apply gravity if we aren't.
		OnTheGround = MovementComponent->onground;
		if (!OnTheGround) {
			Acceleration += FVector(0.0f, 0.0f, PhysicsSettings.Gravity);
		} else if (WasOnTheGround) {
			// Push the player into the ground a bit so we still get collisions every frame.
			// Also keeps player from falling off slopes while running down them.
			if (MovementComponent->Floor.Normal.Z > 0.0f) {
				Velocity.Z = -100.0f;
			}
		}

		// Apply drag.
		if (!OnTheGround && FMath::Abs(Velocity.Z) > JumpSettings.JumpPower) {
			Acceleration -= FVector::UpVector * -PhysicsSettings.Gravity * FMath::Pow(Velocity.Z / PhysicsSettings.TerminalVelocity, 2.0f) * FMath::Sign(Velocity.Z);
		}

		Acceleration += AppliedForce;
		if (AppliedForce.Z > 0.0f) {
			AlreadyUnjumped = true;
			if(OnTheGround) {
				Velocity.Z += 100.0f;
			}
		}

		// You done bonked yer head on that there ceiling.
		if (MovementComponent->Floor.Normal.Z < -0.6f) {
			Velocity.Z = 0.0f;
		}

		// This is a 2D platformer now.
		if (MovementAxisLocked) {
			FVector newpos(GetActorLocation());
			switch (LockedMovementAxis) {
				case ATwoDimensionalMovementRegion::XAXIS:
					newpos.X = LockedAxisValue;
					Velocity.X = 0.0f;
					MovementInput.Y = 0.0f;
					AdjustedInput.Y = 0.0f;
					break;
				case ATwoDimensionalMovementRegion::YAXIS:
					newpos.Y = LockedAxisValue;
					Velocity.Y = 0.0f;
					MovementInput.Y = 0.0f;
					AdjustedInput.Y = 0.0f;
					break;
				case ATwoDimensionalMovementRegion::ZAXIS:
					newpos.Z = LockedAxisValue;
					Velocity.Z = 0.0f;
					break;
			}
			SetActorLocation(newpos);
		}

		// Physiiiiicccss.
		Velocity += Acceleration * DeltaTime;

		// This ain't Megaman X, kiddo.
		if (dashing&&!DashSettings.HasDashJump&&(!OnTheGround||JumpNextFrame)) {
			DashParticles->DeactivateSystem();
			dashing = false;
			dashtimer = 0.0f;
		}

		// Handle jumping.
		if (JumpNextFrame) {

			// Jump while taking the floor's angle and vertical movement into account.
			Velocity.Z = 0.5f * (MovementComponent->groundvelocity.Z >= 0.0f ? MovementComponent->groundverticalvelocity : -MovementComponent->groundvelocity.Z);
			Velocity += JumpSettings.JumpPower * (MovementComponent->offGroundTime > 0 ? FVector::UpVector : MovementComponent->Floor.Normal).GetSafeNormal();
			JumpNextFrame = false;
			WasOnTheGround = false;
			JustJumped = true;

			// OK this might be Megaman X a litle.
			FVector wnproject = FVector::VectorPlaneProject(MovementComponent->wallnormal, FVector::UpVector);
			if (!OnTheGround && wnproject.Size() > 0.95f) {
				// No cheating.
				if (IsGliding||AlreadyGlided) {
					AlreadyGlided = true;
					IsGliding = false;
				}

				// Set player velocity to be away from the wall.
				FVector temp = Velocity;
				float multi = JumpSettings.WallJumpMultiplier;
				if (DashSettings.HasDashWallJump&&holdingdash) {
					multi = DashSettings.DashWallJumpMultiplier;
				}
				Velocity = (MovementComponent->wallnormal.GetSafeNormal()*PhysicsSettings.MaxVelocity*multi + FVector::VectorPlaneProject(Velocity, MovementComponent->wallnormal.GetSafeNormal()));
				Velocity.Z = temp.Z;
				JustWallJumped = true;

				// Make the player face away from the wall we just jumped off of.
				FRotator newmodelrotation = PlayerModel->GetComponentRotation();
				newmodelrotation.Yaw = (MovementComponent->wallnormal.GetSafeNormal() + FVector::VectorPlaneProject(Velocity, MovementComponent->wallnormal.GetSafeNormal()).GetSafeNormal() + FVector::VectorPlaneProject((Right*AdjustedInput.X + Forward*AdjustedInput.Y).GetSafeNormal(), MovementComponent->wallnormal.GetSafeNormal())).Rotation().Yaw;
				PlayerModel->SetWorldRotation(newmodelrotation);
				TargetDirection = newmodelrotation;
			}
		}

		// Start gliding.
		if (GlideNextFrame&&!AlreadyGlided&&!dunk) {
			Velocity.Z = GlideSettings.InitialGlideVelocity;
			IsGliding = true;
			AlreadyGlided = true;
			GlideNextFrame = false;
			FloatParticles->ActivateSystem();
		}

		// Handle gliding.
		if (IsGliding) {
			GlideTimer += DeltaTime;
			PhysicsSettings.Gravity = DefaultGravity / GlideSettings.GlideGravityMultiplier;
		} else {
			FloatParticles->DeactivateSystem();
			GlideTimer = 0.0f;
			PhysicsSettings.Gravity = DefaultGravity;
		}

		// Stop gliding.
		if (GlideTimer > GlideSettings.GlideDuration || OnTheGround || !HoldingJump) {
			IsGliding = false;
		}

		if (OnTheGround) {
			AlreadyGlided = false;
			AlreadyUnjumped = false;
		}

		if (warptimer >= 0.0f) {
			warptimer += DeltaTime;
		}

		// Apply a downward force if the player lets go of jump while still moving upwards.
		// This allows for variable jump heights.
		if (!HoldingJump && Velocity.Z > 0 && !AlreadyUnjumped) {
			Velocity += FVector::UpVector * PhysicsSettings.Gravity * JumpSettings.UnjumpRate * (Velocity.Z / JumpSettings.JumpPower);
			AlreadyUnjumped = true;
		}

		// Make the player start dashing in response to input.
		if (dashnextframe) {
			dashnextframe = false;
			if (OnTheGround) {
				DashParticles->ActivateSystem();
				dashing = true;
			}
		}

		// Handle dashing.
		if (dashing) {

			// Set the player's horizontal velocity while preserving their vertical velocity.
			float z = Velocity.Z;
			Velocity = TargetDirection.Vector().GetSafeNormal()*DashSettings.DashSpeed;
			Velocity.Z = z;

			// Tick up the dash timer.
			if (!DashSettings.HasInfiniteDash) {
				dashtimer += DeltaTime;
			}

			// Stop dashing if the player was dashing for too long.
			if (dashtimer > DashSettings.DashDuration) {
				dashing = false;
				DashParticles->DeactivateSystem();
				dashtimer = 0.0f;
			}
		}

		// COME ON AND SLAM
		if (SlamNextFrame&&!dunk) {
			SlamNextFrame = false;
			if (!OnTheGround) {
				Velocity -= SlamSettings.SlamVelocity * FVector::UpVector;
				dunk = true;
				SlamTrail->ActivateSystem();
			}
		}

		// AND WELCOME TO THE JAM
		if (dunk) {
			if (OnTheGround) {
				dunk = false;
				SlamTrail->DeactivateSystem();
				SlamParticles->ActivateSystem();
			}
		}

		if (ShouldActivate) {
			ShouldActivate = false;
		}

		// How the hell did you end up down there?
		if (GetActorLocation().Z < -2000.0f) {
			SpringArm->bEnableCameraLag = false;
			Respawn();
		}

		// Put Velocity back in the reference frame of the stationary world.
		Velocity += MovementComponent->groundvelocity;

		// Tad's Shame
		//WasOnTheGround = !WasOnTheGround && (MovementComponent->offGroundTime < OffGroundJumpTime ? false : OnTheGround);

		// Store current on the ground state into WasOnTheGround.
		WasOnTheGround = OnTheGround;

		// Store current velocity into previousgroundvelocity.
		previousgroundvelocity = MovementComponent->groundvelocity;

		previousposition = GetActorLocation();

		// Store current aiming state into wasaiming.
		wasztarget = ztarget;

		// And now we get to actually move.
		MovementComponent->AddInputVector(Velocity * DeltaTime);
	}

	// Handle rotating the player model in response to player input.
	{

		// If the player is standing on a moving platform, they should rotate along with it.
		if (MovementComponent->platformangularfrequency != 0.0f) {
			FQuat platformrotation = FQuat(FVector::UpVector, -MovementComponent->platformspindir * MovementComponent->platformangularfrequency * DeltaTime);
			PlayerModel->AddLocalRotation(platformrotation);
			TargetDirection.Yaw += platformrotation.Rotator().Yaw;
		}

		// If we're trying to move, take the camera's orientation into account to figure
		// out the direction we want to face.
		if (!ztarget && ( OnTheGround || IsGliding) && !JustJumped) {
			// I'ma tell ya not even Unity was stupid enough to use -180 -> 180 for rotations.
			int8 reflect = (MovementInput.X >= 0 ? 1 : -1);

			// (but I forgive you)
			if (!MovementInput.IsNearlyZero() && !ztarget) {
				TargetDirection.Yaw = SpringArm->GetComponentRotation().Yaw +
					reflect * FMath::RadiansToDegrees(FMath::Acos(MovementInput.GetSafeNormal() | FVector(0, 1, 0)));
				if (MovementAxisLocked) {
					FVector tempvector(TargetDirection.Vector());
					switch (LockedMovementAxis) {
						case ATwoDimensionalMovementRegion::XAXIS:
							tempvector.X = 0.0f;
							TargetDirection = tempvector.Rotation();
							break;
						case ATwoDimensionalMovementRegion::YAXIS:
							tempvector.Y = 0.0f;
							TargetDirection = tempvector.Rotation();
							break;
					}
				}
			}
		}

		// Turn to face the target direction, but do it all smooth like.
		{
			// Horrible quaternion voodoo. Viewer discretion is advised.
			// I'm honestly still not quite sure what I did.
			FQuat test = FQuat::FindBetween(PlayerModel->GetComponentRotation().Vector(), TargetDirection.Vector());
			float angle = 0.0f;

			// We just need the angle between the vectors, not the axis (which should point upwards anyway).
			FVector dummy;
			test.ToAxisAndAngle(dummy, angle);

			// Turn faster if we're dashing.
			float multiply = (dashing ? 3.0f : 1.0f);

			// Snap to the target angle if we're close enough, otherwise just keep turning.
			if (!ztarget) {
				test = FQuat(dummy, FMath::DegreesToRadians(multiply*(IsGliding ? 1.0f : GlideSettings.GlideTurnRateMultiplier)*TurnSettings.TurnRate)*DeltaTime);
				float angle2 = 0.0f;
				test.ToAxisAndAngle(dummy, angle2);
				if (FMath::Abs(angle2) > FMath::Abs(angle)) {
					PlayerModel->SetWorldRotation(TargetDirection);
				} else {
					PlayerModel->AddLocalRotation(test);
				}
			}
		}
		// Like what even ARE quaternions anyway?
	}
}

// Bind buttons and axes to input handling functions.
void AAuyron::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->bBlockInput = true;
	InputComponent->Priority = 1000;
	InputComponent->BindAxis("MoveX", this, &AAuyron::MoveRight);
	InputComponent->BindAxis("MoveY", this, &AAuyron::MoveForward);
	InputComponent->BindAxis("CameraPitch", this, &AAuyron::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &AAuyron::YawCamera);
	InputComponent->BindAxis("ControllerCameraYaw", this, &AAuyron::ControllerYawCamera);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AAuyron::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &AAuyron::Unjump);
	InputComponent->BindAction("Use", IE_Pressed, this, &AAuyron::Use);
	InputComponent->BindAction("ToggleHelp", IE_Pressed, this, &AAuyron::ToggleHelp);
	InputComponent->BindAction("CameraFaceForward", IE_Pressed, this, &AAuyron::CameraFaceForward);
	InputComponent->BindAction("CameraFaceForward", IE_Released, this, &AAuyron::CameraUnFaceForward);
	InputComponent->BindAction("CameraMode", IE_Pressed, this, &AAuyron::CameraModeToggle);
	InputComponent->BindAction("Warp", IE_Pressed, this, &AAuyron::Warp);
	InputComponent->BindAction("Slam", IE_Pressed, this, &AAuyron::Slam);
	InputComponent->BindAction("Dash", IE_Pressed, this, &AAuyron::Dash);
	InputComponent->BindAction("Dash", IE_Released, this, &AAuyron::UnDash);
	InputComponent->BindAction("Pause", IE_Pressed, this, &AAuyron::Pause);
	InputComponent->BindAction("Unpause", IE_Pressed, this, &AAuyron::Unpause);
}

// Can you believe the tutorial wanted me to use Y for horizontal movement
void AAuyron::MoveRight(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

// and X for vertical? I mean who does that?
void AAuyron::MoveForward(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void AAuyron::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void AAuyron::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void AAuyron::ControllerYawCamera(float AxisValue)
{
	if (ztarget) {
		AxisValue *= -1;
	}
	CameraInput.X += AxisValue;
}

void AAuyron::Pause()
{
	//UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void AAuyron::Unpause()
{
	//UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void AAuyron::Jump()
{
	// We can jump if:
	//   1. We're on the ground.
	//   2. We haven't been off the ground for very long.
	//   3. We are pushing into a wall (wall jump).
	FVector wnproject = FVector::VectorPlaneProject(MovementComponent->wallnormal, FVector::UpVector);
	if (OnTheGround || MovementComponent->offGroundTime < JumpSettings.OffGroundJumpTime || (!MovementComponent->wallnormal.IsNearlyZero() && wnproject.Size()>0.9f)) {
		JumpNextFrame = true;
	} else {
		if (GlideSettings.HasGlide&&!AlreadyGlided) {
			GlideNextFrame = true;
		}
	}
	HoldingJump = true;
}

// I wish you could unjump in real life.
void AAuyron::Unjump()
{
	HoldingJump = false;
}

void AAuyron::Use()
{
	ShouldActivate = true;
}

// HEY LINK TALK TO ME USING Z TARGETING
void AAuyron::CameraFaceForward()
{
	if (!dashing&&!MovementAxisLocked) {
		ztarget = true;
	}
}

void AAuyron::CameraUnFaceForward()
{
	if (!dashing) {
		ztarget = false;
	}
}

void AAuyron::CameraModeToggle()
{
	cameramode = !cameramode;
}

// swish
void AAuyron::Warp()
{
	if (TeleportSettings.HasTeleport) {
		swish = true;
	}
}

void AAuyron::Slam()
{
	if (SlamSettings.HasSlam&&!OnTheGround) {
		SlamNextFrame = true;
	}
}

// I DON'T NEED YOUR HELP
void AAuyron::ToggleHelp() {
	HelpEnabled = !HelpEnabled;
}

// woosh
void AAuyron::Dash()
{
	if (!dashing&&!ztarget&&DashSettings.HasDash) {
		dashnextframe = true;
		holdingdash = true;
	}
}

// unwoosh
void AAuyron::UnDash()
{
	holdingdash = false;
	if (dashing) {
		dashtimer = DashSettings.DashDuration + 100.0f;
	}
}

// Getter functions used by the animation blueprints.
float AAuyron::GetSpeed()
{
	return (FVector::VectorPlaneProject(Velocity - MovementComponent->groundvelocity - AppliedForce, FVector::UpVector)).Size();
}

float AAuyron::GetActualSpeed()
{
	FVector temp = Velocity;
	if (OnTheGround) {
		temp.Z += 100.0f;
	}
	return temp.Size();
}

float AAuyron::GetModelOpacity()
{
	float dist = (Camera->GetComponentLocation() - GetActorLocation()).Size();
	return FMath::Clamp(dist / CameraModelFadeSettings.ModelFadeDistance,0.0f,1.0f);
}

bool AAuyron::GetIsTurning() 
{
	FQuat test = FQuat::FindBetween(PlayerModel->GetComponentRotation().Vector(), TargetDirection.Vector());
	float angle = 0.0f;
	FVector dummy;
	test.ToAxisAndAngle(dummy, angle);
	return (FMath::Abs(angle) > FMath::DegreesToRadians(TurnSettings.FacingAngleSnapThreshold));
}

bool AAuyron::GetIsAiming()
{
	return ztarget;
}

bool AAuyron::GetIsOnTheGround()
{
	return OnTheGround;
}

bool AAuyron::GetHelpEnabled()
{
	return HelpEnabled;
}

uint8 AAuyron::GetGemAmount()
{
	return GemCount;
}

FVector AAuyron::GetPlayerLocation() {
	return GetActorLocation();
}

bool AAuyron::AboutToWarp() {
	bool isitreally = itshappening;
	itshappening = false;
	return isitreally;
}

void AAuyron::SetMaterial(int32 index, UMaterialInterface * newmat)
{
	PlayerModel->SetMaterial(index,newmat);
}

UParticleSystemComponent* AAuyron::GetTrailParticlesL() {
	return TrailParticlesL;
}

UParticleSystemComponent* AAuyron::GetTrailParticlesR() {
	return TrailParticlesR;
}

bool AAuyron::GetJustWallJumped()
{
	return JustWallJumped;
}
