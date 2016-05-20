// DON'T TELL ME WHERE TO FILL OUT MY COPYRIGHT NOTICE YOU'RE NOT MY REAL DAD.

#include "shards.h"
#include "Gem.h"
#include "Auyron.h"
#include "AuyronMovementComponent.h"
#include "CameraOverrideRegion.h"
#include "MovingPlatform.h"
#include "RotatingPlatform.h"
#include "EngineUtils.h" 
#include "Stick.h"
#include "KillZone.h"
#include "Checkpoint.h"
#include "DestructibleBox.h"
#include "TeleClaw.h"
#include "EquipmentPickup.h"
#include "MusicRegion.h"
#include "DialogueCut.h"
#include "NPC.h"
#include "TwoDimensionalMovementRegion.h"
#include "ForceRegion.h"
#include "WarpCrystal.h"
#include "Switch.h"

// Sets default values
AAuyron::AAuyron()
{	
	// These should work.//
	PhysicsSettings.GroundAccelerationRate = 80.0f;
	PhysicsSettings.AirAccelerationRate = 15.0f;
	PhysicsSettings.MaxVelocity = 650.0f;
	PhysicsSettings.MinVelocity = 10.0f;
	PhysicsSettings.TerminalVelocity = 2000.0f;
	PhysicsSettings.Gravity = 1960.0f;
	PhysicsSettings.MaxSlope = 70.0f;
	PhysicsSettings.SlopeSlideTime = 0.15f;
	PhysicsSettings.PushForceFactor = 50.0f;
	PhysicsSettings.HighVelocityForceExponent = 2.5f;

	AttackRange = 300.0f;

	XAxisStyle = STANDARD;
	XAxisAimingStyle = STANDARD;
	YAxisStyle = STANDARD;
	YAxisAimingStyle = STANDARD;

	JumpSettings.JumpPower = 880.0f;
	JumpSettings.WallJumpMultiplier = 1.0f;
	JumpSettings.CoyoteJumpTime = 0.1f;
	JumpSettings.UnjumpRate = 0.33f;

	TurnSettings.TurnRate = 720.0f;
	TurnSettings.FacingAngleSnapThreshold = 5.0f;

	TeleportSettings.TeleportRangeWhenAiming = 4000.0f;
	TeleportSettings.TeleportAngleToleranceWhenAiming = 5.0f;
	TeleportSettings.TeleportRangeWhenNotAiming = 0.0f;
	TeleportSettings.TeleportAngleToleranceWhenNotAiming = 70.0f;
	TeleportSettings.TeleportAnimationDuration = 0.5f;
	TeleportSettings.TeleportAnimationPowerFactor = 4.0f;
	TeleportSettings.TeleportLightColor = FColor(0x336FE6FF);

	DashSettings.HasDash = false;
	DashSettings.HasInfiniteDash = false;
	DashSettings.HasDashControl = false;
	DashSettings.HasDashWallJump = false;
	DashSettings.HasDashJump = false;
	DashSettings.DashSpeed = 1500.0f;
	DashSettings.DashDuration = 0.5f;
	DashSettings.DashWallJumpMultiplier = 3.0f;

	GlideSettings.HasGlide = false;
	GlideSettings.GlideTurnRateMultiplier = 1.0f;
	GlideSettings.GlideDuration = 0.5f;
	GlideSettings.InitialGlideVelocity = 200.0f;
	GlideSettings.GlideSoundsPerSecond = 8.0f;
	GlideSettings.GlideSoundPitchMin = 0.996f;
	GlideSettings.GlideSoundPitchMax = 1.004f;

	SlamSettings.HasSlam = false;
	SlamSettings.SlamVelocity = 1500.0f;

	CameraMaxAngle = 85.0f;
	CameraMinAngle = -85.0f;
	DefaultArmLength = 1000.0f;
	MinimumArmLength = 600.0f;
	MaximumArmLength = 1400.0f;
	CameraZoomRate = 0.05f;
	CameraZoomStep = 200.0f;
	CameraLagZoomScale = 2.0f;

	CameraLagSettings.CameraLag = 3.0f;
	CameraLagSettings.CameraRotationLag = 10.0f;
	CameraLagSettings.AimingLagMultiplier = 0.0f;
	CameraLagSettings.OverrideRegionRotationLagMultiplier = 0.75f;
	CameraLagSettings.DialogueLagMultiplier = 3.0f;

	CameraAutoTurnSettings.CameraAutoTurnFactor = 60.0f;
	CameraAutoTurnSettings.CameraResetTime = 1.0f;
	CameraAutoTurnSettings.CameraDefaultPitch = -30.0f;
	CameraAutoTurnSettings.CameraDefaultPitchRate = 0.1f;

	CameraModelFadeSettings.ModelFadeEnabled = false;
	CameraModelFadeSettings.ModelFadeDistance = 250.0f;

	HelpEnabled = false;

	// Just in case.
	TargetDirection = FRotator::ZeroRotator;
	Velocity = FVector::ZeroVector;

	// Oh you better believe we're ticking every frame.
	PrimaryActorTick.bCanEverTick = true;

	// *I* wanted to be a cylinder, but no, we gotta be a *capsule*.
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	CapsuleComponent->InitCapsuleSize(45.0f, 90.0f);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	onbeginoverlapdelegate.BindUFunction(this, "Hit");
	CapsuleComponent->OnComponentBeginOverlap.Add(onbeginoverlapdelegate);
	onendoverlapdelegate.BindUFunction(this, "UnHit");
	CapsuleComponent->OnComponentEndOverlap.Add(onendoverlapdelegate);
	onhitdelegate.BindUFunction(this, "Stay");
	CapsuleComponent->OnComponentHit.Add(onhitdelegate);
	//CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuyron::Hit);
	//CapsuleComponent->OnComponentHit.AddDynamic(this, &AAuyron::Stay);
	//CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &AAuyron::UnHit);
	CapsuleComponent->SetSimulatePhysics(true);
	CapsuleComponent->SetEnableGravity(false);
	CapsuleComponent->SetLinearDamping(0.0f);
	CapsuleComponent->BodyInstance.bLockRotation = true;
	CapsuleComponent->BodyInstance.bLockXRotation = true;
	CapsuleComponent->BodyInstance.bLockYRotation = true;
	CapsuleComponent->BodyInstance.bLockZRotation = true;
	CapsuleComponent->BodyInstance.PositionSolverIterationCount = 16;
	CapsuleComponent->BodyInstance.VelocitySolverIterationCount = 16;
	CapsuleComponent->bShouldUpdatePhysicsVolume = true;
	CapsuleComponent->AttachTo(RootComponent);
	//CapsuleComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);//4.12
	SetActorEnableCollision(true);

	// It you.
	PlayerModel = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerModel"));
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMeshObj(TEXT("/Game/Models/Characters/Auyron/Auyron2"));
	PlayerModel->SetSkeletalMesh(PlayerMeshObj.Object);
	PlayerModel->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	PlayerModel->bRenderCustomDepth = true;
	PlayerModel->SetSimulatePhysics(false);
	PlayerModel->bReceivesDecals = false;
	PlayerModel->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PlayerModel->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	PlayerModel->SetCollisionResponseToChannel(ECC_PhysicsBody, ECollisionResponse::ECR_Block);
	PlayerModel->SetCollisionObjectType(ECC_PhysicsBody);
	PlayerModel->SetSimulatePhysics(false);
	PlayerModel->AttachTo(CapsuleComponent);
	//PlayerModel->AttachToComponent(CapsuleComponent, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	//static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimationBlueprint(TEXT("/Game/Animations/Characters/Auyron/Anim_Auyron"));
	//PlayerModel->SetAnimInstanceClass(AnimationBlueprint.Object->GetAnimBlueprintGeneratedClass());
	
	// Use a spring arm so the camera can be all like swoosh.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(-30.0f, 0.0f, 0.0f));
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = CameraLagSettings.CameraLag;
	SpringArm->CameraRotationLagSpeed = CameraLagSettings.CameraRotationLag;
	SpringArm->CameraLagMaxDistance = 1000.0f;
	SpringArm->ProbeSize = 20.0f;
	SpringArm->AttachTo(CapsuleComponent);
	//SpringArm->AttachToComponent(CapsuleComponent, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	// Camera so the casuals can "see what they're doing" or whatever.
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	//Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);//4.12

	// May god have mercy on your GPU.
	DashParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Dash Particles"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> dp(TEXT("/Game/Particles/DashParticles"));
	DashParticles->SetTemplate(dp.Object);
	DashParticles->bAutoActivate = false;
	DashParticles->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	DashParticles->AttachTo(PlayerModel);
	//DashParticles->AttachToComponent(PlayerModel, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	FloatParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Float Particles"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> fp(TEXT("/Game/Particles/FloatParticles"));
	FloatParticles->SetTemplate(fp.Object);
	FloatParticles->bAutoActivate = false;
	FloatParticles->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	FloatParticles->AttachTo(PlayerModel);
	//FloatParticles->AttachToComponent(PlayerModel, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	SlamParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Slam Particles"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> sp(TEXT("/Game/Particles/SlamParticles"));
	SlamParticles->SetTemplate(sp.Object);
	SlamParticles->bAutoActivate = false;
	SlamParticles->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	SlamParticles->AttachTo(PlayerModel);
	//SlamParticles->AttachToComponent(PlayerModel, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	SlamTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Slam Trail"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> st(TEXT("/Game/Particles/SlamTrail"));
	SlamTrail->SetTemplate(st.Object);
	SlamTrail->bAutoActivate = false;
	SlamTrail->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	SlamTrail->AttachTo(PlayerModel);
	//SlamTrail->AttachToComponent(PlayerModel, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	TrailParticlesL = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particles L"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> tpl(TEXT("/Game/Particles/TrailParticles"));
	TrailParticlesL->SetTemplate(tpl.Object);
	TrailParticlesL->bAutoActivate = false;
	TrailParticlesL->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	TrailParticlesL->AttachTo(PlayerModel);
	//TrailParticlesL->AttachToComponent(PlayerModel, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	TrailParticlesR = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particles R"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> tpr(TEXT("/Game/Particles/TrailParticles"));
	TrailParticlesR->SetTemplate(tpr.Object);
	TrailParticlesR->bAutoActivate = false;
	TrailParticlesR->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	TrailParticlesR->AttachTo(PlayerModel);
	//TrailParticlesR->AttachToComponent(PlayerModel, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	grassparticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Grass Particles"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> gpr(TEXT("/Game/grassparticles"));
	grassparticles->SetTemplate(gpr.Object);
	grassparticles->bAutoActivate = false;
	grassparticles->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	grassparticles->AttachTo(PlayerModel);

	DropShadow = CreateDefaultSubobject<UDecalComponent>(TEXT("Drop Shadow"));
	const ConstructorHelpers::FObjectFinder<UMaterialInterface> dsmat(TEXT("/Game/Textures/Effects/dropshadow"));
	DropShadow->SetMaterial(0,dsmat.Object);
	DropShadow->DecalSize = FVector(1000.0f, 45.0f, 45.0f);
	DropShadow->DecalSize = FVector(1000.0f, 90.0f, 90.0f);
	DropShadow->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	DropShadow->SetRelativeLocation(FVector(0.0f, 0.0f, -950.0f));
	DropShadow->SetRelativeLocation(FVector(0.0f, 0.0f, -850.0f));
	DropShadow->AttachTo(PlayerModel);

	// Deferring application of physical material because Unreal crashes
	// for no reason if you try to apply it in the contrsuctor.
	const ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PlayerPhysMat(TEXT("/Game/Textures/Characters/Auyron/PlayerPhysMaterial"));
	physmat = PlayerPhysMat.Object;

	// Get the instance of the TeleClaw weapon.
	TeleClaw = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleClaw"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> tc(TEXT("/Game/Models/Weapons/TeleClaw"));
	TeleClaw->SetStaticMesh(tc.Object);
	TeleClaw->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TeleClaw->bRenderCustomDepth = true;
	TeleClaw->bReceivesDecals = false;

	// WHAT ARE THOSE?
	BootsR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BootsR"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> bootsmodel(TEXT("/Game/Models/Weapons/Boots"));
	BootsR->SetStaticMesh(bootsmodel.Object);
	BootsR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BootsR->bRenderCustomDepth = true;
	BootsR->bReceivesDecals = false;

	BootsL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BootsL"));
	BootsL->SetStaticMesh(bootsmodel.Object);
	BootsL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BootsL->bRenderCustomDepth = true;
	BootsL->bReceivesDecals = false;

	// WWE CHAMPIONSHIP BELT
	Belt = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Belt"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> beltmodel(TEXT("/Game/Models/Weapons/Belt"));
	Belt->SetStaticMesh(beltmodel.Object);
	Belt->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Belt->bRenderCustomDepth = true;
	Belt->bReceivesDecals = false;

	Bracelet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bracelet"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> braceletmodel(TEXT("/Game/Models/Weapons/Bracelet"));
	Bracelet->SetStaticMesh(braceletmodel.Object);
	Bracelet->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Bracelet->bRenderCustomDepth = true;
	Bracelet->bReceivesDecals = false;

	Wings = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wings"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> wingsmodel(TEXT("/Game/Models/Weapons/Wings"));
	Wings->SetStaticMesh(wingsmodel.Object);
	Wings->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Wings->bRenderCustomDepth = true;
	Wings->bReceivesDecals = false;

	// It gazes also into you.
	TheAbyss = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TheAbyss"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> ab(TEXT("/Game/Models/Weapons/cutidea1"));
	TheAbyss->SetStaticMesh(ab.Object);
	TheAbyss->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TheAbyss->SetWorldScale3D(FVector(0.2f, 1.6f, 1.0f));
	TheAbyss->SetRelativeLocation(FVector(185.0f, 0.0f, 120.0f));
	TheAbyss->SetRelativeRotation(FRotator(0.0f, 5.0f, 0.0f));
	TheAbyss->AttachTo(PlayerModel);
	//TheAbyss->AttachToComponent(PlayerModel, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	TeleClaw->SetVisibility(TeleportSettings.HasTeleport);
	BootsR->SetVisibility(DashSettings.HasDash);
	BootsL->SetVisibility(DashSettings.HasDash);
	Bracelet->SetVisibility(SlamSettings.HasSlam);
	Belt->SetVisibility(false);
	Wings->SetVisibility(GlideSettings.HasGlide);
	TheAbyss->SetVisibility(false);

	// ASSUMING DIRECT CONTROL.
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Apparently we need some newfangled "MovementComponent".
	MovementComponent = CreateDefaultSubobject<UAuyronMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CapsuleComponent;
	
	// BLAST PROCESSING.
	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	PostProcess->AttachTo(RootComponent);
	//PostProcess->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	// NINTENDON'T DO 16 BIT.
	const ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> riftmatfile(TEXT("/Game/Textures/Effects/RiftRTT"));
	TeleportRiftRenderTarget = riftmatfile.Object;

	const ConstructorHelpers::FObjectFinder<UMaterialInterface> hair(TEXT("/Game/Textures/Characters/Auyron/Hair"));
	HairMatBase = hair.Object;

	const ConstructorHelpers::FObjectFinder<UMaterialInterface> bandana(TEXT("/Game/Textures/Characters/Auyron/Headband"));
	BandanaMatBase = bandana.Object;

	const ConstructorHelpers::FObjectFinder<UMaterialInterface> body(TEXT("/Game/Textures/Characters/Auyron/protag-UVs_Mat"));
	BodyMatBase = body.Object;

	// Might need this later.
	const ConstructorHelpers::FObjectFinder<UMaterialInterface> bw(TEXT("/Game/Textures/Weapons/wing_blue"));
	bluewings = bw.Object;

	const ConstructorHelpers::FObjectFinder<UMaterialInterface> riftmat(TEXT("/Game/Textures/Effects/RiftMat"));
	TeleportRiftMaterial = riftmat.Object;

	const ConstructorHelpers::FObjectFinder<UMaterialInterface> riftmat2(TEXT("/Game/Textures/Effects/drawbehind"));
	TestTeleEffectBase = riftmat2.Object;

	const ConstructorHelpers::FObjectFinder<UMaterialInterface> celshade(TEXT("/Game/Textures/Effects/celshader"));
	CelShaderMaterial = celshade.Object;

	const ConstructorHelpers::FObjectFinder<USoundCue> jsound(TEXT("/Game/Sound/SoundEffects/jump"));
	JumpSound = jsound.Object;

	const ConstructorHelpers::FObjectFinder<USoundCue> dsound(TEXT("/Game/Sound/SoundEffects/dash"));
	DashSound= dsound.Object;

	const ConstructorHelpers::FObjectFinder<USoundCue> csound(TEXT("/Game/Sound/SoundEffects/gempickup"));
	CollectSound = csound.Object;

	const ConstructorHelpers::FObjectFinder<USoundCue> wrpsound(TEXT("/Game/Sound/SoundEffects/warpsound"));
	WarpSound = wrpsound.Object;

	const ConstructorHelpers::FObjectFinder<USoundCue> dksound(TEXT("/Game/Sound/SoundEffects/dunk"));
	DunkSound = dksound.Object;

	const ConstructorHelpers::FObjectFinder<USoundCue> dhksound(TEXT("/Game/Sound/SoundEffects/dunkhit"));
	DunkHitSound = dhksound.Object;

	const ConstructorHelpers::FObjectFinder<USoundCue> wsound(TEXT("/Game/Sound/SoundEffects/wing"));
	WingSound = wsound.Object;

}

void AAuyron::Respawn() {

	CapsuleComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
	justteleported = true;
	SetActorLocation(RespawnPoint, false,NULL,ETeleportType::TeleportPhysics);
}

void AAuyron::StopClimbing() {
	isclimbing = false;
	//CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	grabbedledge = nullptr;
	CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
	//SetActorLocation(GetActorLocation() + PlayerModel->GetForwardVector()*75.0f + 15.9f*FVector::UpVector, false, NULL, ETeleportType::TeleportPhysics);//56.582f  ,false,nullptr,ETeleportType::TeleportPhysics
	//SetActorLocation(climblocation, false, NULL, ETeleportType::TeleportPhysics);//56.582f  ,false,nullptr,ETeleportType::TeleportPhysics//
	PhysicsSettings.Gravity = DefaultGravity;
}

void AAuyron::HereWeGo()
{

	TheAbyss->SetVisibility(false);
	cameralocked = false;

	// Move the player to the telepad's position and give them the perscribed velocity.
	FVector temp = GetActorLocation();
	//SpringArm->CameraLagSpeed = 0.0f;
	//SpringArm->CameraRotationLagSpeed = 0.0f;
	//SpringArm->SetWorldRotation((closeststick->gohere - temp).Rotation());
	//SpringArm->TargetArmLength = (temp - closeststick->gohere).Size();
	CapsuleComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
	CapsuleComponent->AddImpulse(closeststick->PostTeleportVelocity,NAME_None,true);
	justteleported = true;
	justswished = true;
	AlreadyUnjumped = true;

	// If we were aiming, reset the camera's rotation.
	//if (wasztarget) {
		//SpringArm->SetRelativeRotation(FRotator(-30.0f, SpringArm->GetComponentRotation().Yaw, 0.0f));
	//}

	// Reset OnTheGround and glide variables.
	OnTheGround = false;
	MovementComponent->onground = false;
	WasOnTheGround = false;
	if (IsGliding) {
		AlreadyGlided = true;
		IsGliding = false;
	}

	PhysicsSettings.Gravity = DefaultGravity;

	GetWorld()->GetTimerManager().ClearTimer(PreWarpTimer);
}

void AAuyron::FlattenVelocity()
{
	FVector temp = CapsuleComponent->GetPhysicsLinearVelocity();
	temp.Z = 0.0f;
	CapsuleComponent->SetPhysicsLinearVelocity(temp);
}

void AAuyron::FadeInMusic() {
	if (currentmusic != nullptr) {
		currentmusic->FadeIn(2.0f);
	}
}

void AAuyron::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	hairmat = UMaterialInstanceDynamic::Create(HairMatBase, this);
	bandanamat = UMaterialInstanceDynamic::Create(BandanaMatBase, this);
	teletestmat = UMaterialInstanceDynamic::Create(TestTeleEffectBase, this);
	celshadermat = UMaterialInstanceDynamic::Create(CelShaderMaterial, this);

	Camera->AddOrUpdateBlendable(teletestmat);
	Camera->AddOrUpdateBlendable(celshadermat);

	PlayerModel->SetMaterial(0, bodymat);
	PlayerModel->SetMaterial(1, hairmat);
	PlayerModel->SetMaterial(2, bandanamat);

	CapsuleComponent->SetPhysMaterialOverride(physmat);

	PostProcess->bUnbound = true;
}

void AAuyron::UnHit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {
		// We just exited a camera override region.
		if (OtherActor->IsA(ACameraOverrideRegion::StaticClass())) {
			// Set the camera back to normal, but only if the region we just
			// exited was affecting the camera in the first place.
			if (currentoverrideregion == ((ACameraOverrideRegion*)OtherActor)) {
				InCameraOverrideRegion = false;
			}
		}

		if (OtherComp->IsA(UInstancedStaticMeshComponent::StaticClass())) {
			ingrass = false;
		}

		if (OtherActor->IsA(AForceRegion::StaticClass())) {
			AppliedForce -= ((AForceRegion*)OtherActor)->Direction * ((AForceRegion*)OtherActor)->Magnitude;
		}

		if (OtherActor->IsA(ATwoDimensionalMovementRegion::StaticClass())) {
			MovementAxisLocked = false;
		}
		
		if (OtherActor->IsA(AMusicRegion::StaticClass())) {
			((AMusicRegion*)OtherActor)->MusicActor->FadeOut(2.0f,0.0f);
		}
	}
}

void AAuyron::Stay(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {
		// Get out mah way!
		if (OtherComp->IsSimulatingPhysics()) {
			float scalefactor = FVector::VectorPlaneProject(CapsuleComponent->GetPhysicsLinearVelocity(),FVector::UpVector).Size() / PhysicsSettings.MaxVelocity;
			scalefactor = (scalefactor > 1.0f ? FMath::Pow(scalefactor, PhysicsSettings.HighVelocityForceExponent) : 1.0f);
			OtherComp->AddImpulse((OtherComp->GetComponentLocation() - GetActorLocation()).GetSafeNormal()*scalefactor*PhysicsSettings.PushForceFactor, NAME_None, true);
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
			((AGem*)OtherActor)->GetCollected(this);
			GemCount++;
			UGameplayStatics::PlaySound2D(this, CollectSound);
		}
		if (OtherComp->IsA(UInstancedStaticMeshComponent::StaticClass()) ) {
			ingrass = true;
		}
		if (OtherActor->IsA(AEquipmentPickup::StaticClass()))
		{
			if (((AEquipmentPickup*)OtherActor)->Name == "TeleClaw") {
				TeleportSettings.HasTeleport = true;
			}
			if (((AEquipmentPickup*)OtherActor)->Name == "WallJump") {
				JumpSettings.HasWallJump = true;
			}
			if (((AEquipmentPickup*)OtherActor)->Name == "Dash") {
				DashSettings.HasDash = true;
			}
			if (((AEquipmentPickup*)OtherActor)->Name == "Slam") {
				SlamSettings.HasSlam = true;
			}
			if (((AEquipmentPickup*)OtherActor)->Name == "Glide") {
				GlideSettings.HasGlide = true;
			}
			if (((AEquipmentPickup*)OtherActor)->Name == "Glide2") {
				GlideSettings.HasGlide = true;
				GlideSettings.GlideDuration *= 4.0f;
				Wings->SetMaterial(0, bluewings);
			}
			OtherActor->Destroy();
		}

		// We just entered a camera override region.
		if (OtherActor->IsA(ACameraOverrideRegion::StaticClass())) {
			InCameraOverrideRegion = true;
			currentoverrideregion = ((ACameraOverrideRegion*)OtherActor);
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
			GetWorldTimerManager().ClearTimer(MusicChangeTimer);
			if (currentmusic != nullptr) {
				currentmusic->FadeOut(2.0f, 0.0f);
			}
			currentmusic = ((AMusicRegion*)OtherActor)->MusicActor;
			GetWorldTimerManager().SetTimer(MusicChangeTimer, this, &AAuyron::FadeInMusic, 2.0f);
		}

		if (OtherActor->IsA(AWarpCrystal::StaticClass())) {
			SetActorLocation(((AWarpCrystal*)OtherActor)->WarpExit->GetComponentLocation());
			justteleported = true;
		}

		if (OtherActor->IsA(AKillZone::StaticClass())) {
			Respawn();
			justteleported = true;
		}
	}
}

// GAME START.
void AAuyron::BeginPlay()
{
	Super::BeginPlay();

	// Set the spring arm's length.
	SpringArm->TargetArmLength = DefaultArmLength;
	ActualDefaultArmLength = DefaultArmLength;
	TargetDefaultArmLength = DefaultArmLength;
	ActualDefaultCameraLag = CameraLagSettings.CameraLag;
	ActualTeleportAnimationDuration = TeleportSettings.TeleportAnimationDuration;

	// Set the max slope and max off ground time for the movement component.
	MovementComponent->minnormalz = FMath::Cos(FMath::DegreesToRadians(PhysicsSettings.MaxSlope));
	MovementComponent->timerlimit = PhysicsSettings.SlopeSlideTime;
	MovementComponent->MaxOffGroundTime = JumpSettings.CoyoteJumpTime;

	// Point gravity downwards.
	PhysicsSettings.Gravity = -PhysicsSettings.Gravity;
	DefaultGravity = PhysicsSettings.Gravity;

	// Initialize gem count.
	GemCount = 0;

	defaultfov = Camera->FieldOfView;
	GetWorld()->GetTimerManager().ClearTimer(WarpAnimationTimer);

	// >_>
	CapsuleComponent->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
	FRotator springarmrotation = SpringArm->GetComponentRotation();
	springarmrotation.Roll = 0.0f;
	SpringArm->SetWorldRotation(springarmrotation);

	previousposition = GetActorLocation();
	closecamera = GetActorLocation();
	RespawnPoint = GetActorLocation();
	TheAbyss->SetVisibility(false);
	TheAbyss->SetMaterial(0, TeleportRiftMaterial);

	Capture2D = GetWorld()->SpawnActor<ASceneCapture2D>();
	Capture2D->GetCaptureComponent2D()->bCaptureEveryFrame = true;
	Capture2D->GetCaptureComponent2D()->TextureTarget = TeleportRiftRenderTarget;
	Capture2D->GetCaptureComponent2D()->HideActorComponents(this);
	Capture2D->GetCaptureComponent2D()->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
	Capture2D->GetCaptureComponent2D()->PostProcessSettings.AddBlendable(celshadermat,1.0f);

	Capture2D->GetCaptureComponent2D()->PostProcessSettings.bOverride_BloomIntensity = true;
	Capture2D->GetCaptureComponent2D()->PostProcessSettings.bOverride_BloomThreshold = true;
	Capture2D->GetCaptureComponent2D()->PostProcessSettings.bOverride_BloomSizeScale = true;
	Capture2D->GetCaptureComponent2D()->PostProcessSettings.BloomIntensity = 2.0f;
	Capture2D->GetCaptureComponent2D()->PostProcessSettings.BloomThreshold = -1.0f;
	Capture2D->GetCaptureComponent2D()->PostProcessSettings.BloomSizeScale = 2.0f;

	Camera->PostProcessSettings.bOverride_BloomIntensity = true;
	Camera->PostProcessSettings.bOverride_BloomThreshold = true;
	Camera->PostProcessSettings.bOverride_BloomSizeScale = true;
	Camera->PostProcessSettings.BloomIntensity = 1.0f;
	Camera->PostProcessSettings.BloomThreshold = -1.0f;
	Camera->PostProcessSettings.BloomSizeScale = 1.0f;

	// Sets the player's "true" facing direction to whatever
	// the model's facing direction is in the editor.
	TargetDirection = PlayerModel->GetComponentRotation();
	
	//TeleClaw->AttachToComponent(PlayerModel, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "RightHand");
	//BootsR->AttachToComponent(PlayerModel, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WHATARETHOSE_R");
	//BootsL->AttachToComponent(PlayerModel, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WHATARETHOSE_L");
	//Bracelet->AttachToComponent(PlayerModel, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Bracelet");
	//Belt->AttachToComponent(PlayerModel, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Belt");
	//Wings->AttachToComponent(PlayerModel, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Wings");

	TeleClaw->AttachTo(PlayerModel, "RightHand");
	BootsR->AttachTo(PlayerModel, "WHATARETHOSE_R");
	BootsL->AttachTo(PlayerModel, "WHATARETHOSE_L");
	Bracelet->AttachTo(PlayerModel, "Bracelet");
	Belt->AttachTo(PlayerModel, "Belt");
	Wings->AttachTo(PlayerModel,  "Wings");

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
	grassparticles->bAutoActivate = false;
	grassparticles->DeactivateSystem();
	grassparticles->bAutoDestroy = false;
	grassparticles->SecondsBeforeInactive = 0.0f;
}

void AAuyron::whywhy() {
	Capture2D->GetCaptureComponent2D()->bCaptureEveryFrame = false;
}

// Called every frame UNLIKE UNITY MIRITE?
void AAuyron::Tick(float DeltaTime)
{

	// 3x^2 - 2x^3

	Super::Tick(DeltaTime);

	// The definitions of "Right" and "Forward" depend on the direction that the camera's facing.
	FVector Right = Camera->GetRightVector();
	Right.Z = 0.0f;
	Right = Right.GetSafeNormal();
	FVector Forward = Camera->GetForwardVector();
	Forward.Z = 0.0f;
	Forward = Forward.GetSafeNormal();

	Capture2D->GetCaptureComponent2D()->Deactivate();

	if (MovementInput.IsNearlyZero()) {
		TimeSinceLastMovementInputReleased = 0.0f;
	} else {
		TimeSinceLastMovementInputReleased += DeltaTime;
	}

	if (FMath::Abs(MovementInput.X) > 0.9f) {
		MovementInput.X = FMath::Sign(MovementInput.X);
	}
	if (FMath::Abs(MovementInput.Y) > 0.9f) {
		MovementInput.Y = FMath::Sign(MovementInput.Y);
	}

	FRootMotionMovementParams RootMotion = PlayerModel->ConsumeRootMotion();

	if (RootMotion.bHasRootMotion)
	{
		FTransform WorldRootMotion = PlayerModel->ConvertLocalRootMotionToWorld(RootMotion.RootMotionTransform);
		CapsuleComponent->SetWorldLocation(CapsuleComponent->GetComponentLocation() + WorldRootMotion.GetTranslation(),false,NULL,ETeleportType::TeleportPhysics);
	}

	// This isn't Doom.
	MovementInput = MovementInput.GetClampedToMaxSize(1.0f);

	// Now how did you get down there?
	if (GetActorLocation().Z < -20000.0f) {
		Respawn();
	}

	// We're in the middle of the warp animation.
	if (GetWorld()->GetTimerManager().GetTimerElapsed(PreWarpTimer) >= 0.0f) {
		movementlocked = true;
		cameralocked = true;
	}
	
	if (isclimbing) {
		movementlocked = true;
	}

	if (ingrass && OnTheGround) {
		if (!grassactive) {
			grassparticles->ActivateSystem();
		}
		grassactive = true;
	} else {
		grassactive = false;
		grassparticles->DeactivateSystem();
	}

	JustWallJumped = false;

	teletestmat->SetScalarParameterValue("t", GetWorld()->GetTimerManager().GetTimerElapsed(PreWarpTimer)/TeleportSettings.TeleportAnimationDuration);
	
	// A blueprint is overriding player input.
	if (blockedbyblueprint) {
		movementlocked = true;
		cameralocked = true;
	}

	// Going somewhere?
	if (movementlocked) {
		MovementInput = FVector::ZeroVector;

		// Lock jumping and gliding as well unless we're dashing.
		if (!dashing&&!IsInDialogue) {
			JumpNextFrame = false;
			GlideNextFrame = false;
		}
	}

	// Nullify camera input if camera is locked.
	if (cameralocked) {
		CameraInput = FVector::ZeroVector;
	}

	// Nullify movement input if dashing with no dash control.
	if (dashing&&!DashSettings.HasDashControl) {
		MovementInput = FVector::ZeroVector;
	}

	// Update OnTheGround state.
	OnTheGround = MovementComponent->onground;

	//DropShadow->bVisible = !OnTheGround;

	// Temporarily remove camera lag if player is teleported by some
	// mechanism other than the TeleClaw.
	if (justteleported&&!justswished) {
		SpringArm->CameraLagMaxDistance = 1.0f;
	} else {
		SpringArm->CameraLagMaxDistance = 0.0f;
	}
	
	// Allow for smooth camera zooming.
	DefaultArmLength = FMath::Lerp(DefaultArmLength, TargetDefaultArmLength, CameraZoomRate);

	// If there is a sharp change in the velocity of the platform that the player is
	// standing on, immediately snap the player's velocity to match it. 
	if (!MovementComponent->groundvelocity.IsNearlyZero() && !JumpNextFrame && FMath::Abs((FVector::VectorPlaneProject(MovementComponent->groundvelocity,FVector::UpVector) - FVector::VectorPlaneProject(previousgroundvelocity,FVector::UpVector)).Size()) > 200.0f) {
		CapsuleComponent->SetPhysicsLinearVelocity(MovementComponent->groundvelocity);
	}

	// Inform the MovementComponent of our velocity.
	MovementComponent->PlayerVelocity = CapsuleComponent->GetPhysicsLinearVelocity();
	MovementComponent->forceregiondirection = AppliedForce;
	MovementComponent->isclimbing = isclimbing;

	// Set our frame of reference for future calculations to be that of the surface we're standing on.
	CapsuleComponent->SetPhysicsLinearVelocity(CapsuleComponent->GetPhysicsLinearVelocity() - MovementComponent->groundvelocity); // - (MovementComponent->groundvelocity + pushvelocity)
	
	// Snap velocity to zero if it's really small.
	if (FVector::VectorPlaneProject(CapsuleComponent->GetPhysicsLinearVelocity(),FVector::UpVector).Size() < PhysicsSettings.MinVelocity && !dashing && !JumpNextFrame && MovementInput.IsNearlyZero()) {
		float tempz = CapsuleComponent->GetPhysicsLinearVelocity().Z;
		CapsuleComponent->SetPhysicsLinearVelocity(tempz*FVector::UpVector);
	}

	// Make the equipment we have visible on the player model.
	TeleClaw->SetVisibility(TeleportSettings.HasTeleport);
	BootsR->SetVisibility(DashSettings.HasDash);
	BootsL->SetVisibility(DashSettings.HasDash);
	Bracelet->SetVisibility(SlamSettings.HasSlam);
	Belt->SetVisibility(JumpSettings.HasWallJump);
	Wings->SetVisibility(GlideSettings.HasGlide);

	// Handle player abilities.
	{

		{
			FCollisionShape LedgeFinderShape = FCollisionShape::MakeSphere(50.0f);// (55.0f, 90.0f);
			FCollisionObjectQueryParams QueryParams;
			QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);//
			QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
			QueryParams.AddObjectTypesToQuery(ECC_Destructible);
			FCollisionQueryParams Params;
			FHitResult ShapeTraceResult;
			FVector head = GetActorLocation() + 20.0f*FVector::UpVector;
			GetWorld()->SweepSingleByChannel(ShapeTraceResult, head - 20.0f*PlayerModel->GetForwardVector(), head + 50.0f*PlayerModel->GetForwardVector(), FQuat::Identity, ECC_Visibility, LedgeFinderShape, Params);

			if (!OnTheGround && CapsuleComponent->GetPhysicsLinearVelocity().Z<0.0f && ShapeTraceResult.IsValidBlockingHit()) {
				FVector WallNormal;
				FVector TraceHit;
				WallNormal = FVector::VectorPlaneProject(ShapeTraceResult.ImpactNormal, FVector::UpVector);
				TraceHit = ShapeTraceResult.ImpactPoint;
				FHitResult LedgeTraceResult;
				GetWorld()->SweepSingleByChannel(LedgeTraceResult, TraceHit + 200.0f*FVector::UpVector, TraceHit, FQuat::Identity, ECC_Visibility, LedgeFinderShape, Params);
				FHitResult ceilhit;
				FCollisionShape CeilFinderShape = FCollisionShape::MakeSphere(20.0f);// (55.0f, 90.0f);
				GetWorld()->SweepSingleByChannel(ceilhit, GetActorLocation() + 65.0f*FVector::UpVector, GetActorLocation() + 200.0f*FVector::UpVector, FQuat::Identity, ECC_Visibility, CeilFinderShape);
				bool underceiling = ceilhit.GetActor() != nullptr && ceilhit.GetComponent() != nullptr && ceilhit.GetComponent()->IsA(UStaticMeshComponent::StaticClass());
				if (LedgeTraceResult.IsValidBlockingHit()) {
					if (LedgeTraceResult.GetActor() != nullptr && LedgeTraceResult.GetComponent() != nullptr && LedgeTraceResult.GetComponent()->IsA(UStaticMeshComponent::StaticClass())) {
						grabbedledge = ((UStaticMeshComponent*)LedgeTraceResult.GetComponent());
						// The motion of a point on a rigid body is the combination of its motion about the center of mass...
						FVector angvel = FMath::DegreesToRadians(grabbedledge->GetPhysicsAngularVelocity());
						FVector rr = LedgeTraceResult.ImpactPoint - (grabbedledge->GetComponentLocation());
						FVector rvel = FVector::CrossProduct(angvel, rr);

						// ...and the motion of the center of mass itself.
						FVector cmvel = (grabbedledge->GetPhysicsLinearVelocity());

						ledgegroundvelocity = rvel + cmvel;
						ledgeangularfrequency = -angvel.Z;
					} else {
						// BSP a shit
						//ledgegroundvelocity = FVector::ZeroVector;

						ledgegroundvelocity = FVector::ZeroVector;
						ledgeangularfrequency = 0.0f;
					}

				}

				if (!isclimbing && !underceiling && !dunk && LedgeTraceResult.IsValidBlockingHit() && LedgeTraceResult.ImpactNormal.Z>0.85f) {
					FVector LedgeTop = FVector(TraceHit.X, TraceHit.Y, LedgeTraceResult.ImpactPoint.Z);
					FVector NewPlayerLocation = LedgeTop + 45.0f*WallNormal;
					if (true) {
						IsGliding = false;
						dashing = false;
						dunk = false;
						JumpNextFrame = false; 
						TargetDirection = (-WallNormal).Rotation();
						PlayerModel->SetWorldRotation(TargetDirection);
						SetActorLocation(NewPlayerLocation-52.0f*FVector::UpVector);
						ActivateNextFrame = false;
						climblocation = LedgeTop;
						isclimbing = true;
						PhysicsSettings.Gravity = 0.0f;
						CapsuleComponent->SetPhysicsLinearVelocity(ledgegroundvelocity);
						CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
						CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
						CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
						CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
						GetWorld()->GetTimerManager().SetTimer(climbtimer, this, &AAuyron::StopClimbing, 1.667f/2.0f);
					}
				}
			}

			if (isclimbing) {
				FVector WallNormal;
				FVector TraceHit;
				WallNormal = FVector::VectorPlaneProject(ShapeTraceResult.ImpactNormal,FVector::UpVector);
				TraceHit = ShapeTraceResult.ImpactPoint;
				FHitResult LedgeTraceResult;
				GetWorld()->SweepSingleByChannel(LedgeTraceResult, head + 50.0f*PlayerModel->GetForwardVector() + 200.0f*FVector::UpVector, head + 50.0f*PlayerModel->GetForwardVector() - 200.0f*FVector::UpVector, FQuat::Identity, ECC_Visibility, LedgeFinderShape, Params);

				if (LedgeTraceResult.GetActor() != nullptr && LedgeTraceResult.GetComponent() != nullptr) {
					if (LedgeTraceResult.GetComponent()->IsA(UStaticMeshComponent::StaticClass())) {
						grabbedledge = ((UStaticMeshComponent*)LedgeTraceResult.GetComponent());
						// The motion of a point on a rigid body is the combination of its motion about the center of mass...
						FVector angvel = FMath::DegreesToRadians(grabbedledge->GetPhysicsAngularVelocity());
						FVector rr = LedgeTraceResult.ImpactPoint - (grabbedledge->GetComponentLocation());
						FVector rvel = FVector::CrossProduct(angvel, rr);

						// ...and the motion of the center of mass itself.
						FVector cmvel = (grabbedledge->GetPhysicsLinearVelocity());

						ledgegroundvelocity = rvel + cmvel;
						ledgeangularfrequency = -angvel.Z;
					} else {
						// BSP a shit
						//ledgegroundvelocity = FVector::ZeroVector;

						ledgegroundvelocity = FVector::ZeroVector;
						ledgeangularfrequency = 0.0f;
					}

				}
				CapsuleComponent->SetPhysicsLinearVelocity(ledgegroundvelocity);
				MovementComponent->platformangularfrequency = ledgeangularfrequency;
			}
		}

		// Determine if the player is rubbing up against a wall and get the wall normal if they are.
		{
			FCollisionShape WallJumpCapsuleShape = FCollisionShape::MakeCapsule(55.0f, 90.0f);
			FCollisionObjectQueryParams QueryParams;
			QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);//
			QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
			QueryParams.AddObjectTypesToQuery(ECC_Destructible);

			if (!GetWorld()->OverlapAnyTestByObjectType(GetActorLocation() - FVector::UpVector, FQuat::Identity, QueryParams, WallJumpCapsuleShape)) {
				RidingWall = false;
				StoredWallNormal = FVector::ZeroVector;
			}

			FCollisionQueryParams Params;

			// Telepads don't count.
			for (TActorIterator<AStick> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
				Params.AddIgnoredActor(ActorItr.operator->());
			}

			// Neither do destructibles.
			for (TActorIterator<ADestructibleBox> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
				// When they're broken, that is.
				if (ActorItr->fadetimer >= 0.0f) {
					Params.AddIgnoredActor(ActorItr.operator->());
				}
			}

			FHitResult ShapeTraceResult;
			GetWorld()->SweepSingleByChannel(ShapeTraceResult, GetActorLocation() + 10.0f*FVector::UpVector, GetActorLocation() - 10.0f*FVector::UpVector, FQuat::Identity, ECC_Visibility, WallJumpCapsuleShape, Params); //100

			if (!OnTheGround && FVector::VectorPlaneProject(ShapeTraceResult.Normal, FVector::UpVector).Size() > 0.95f) {
				if (ShapeTraceResult.GetActor() == nullptr ||
					(ShapeTraceResult.GetActor()->GetClass() != AMovingPlatform::StaticClass() &&
					ShapeTraceResult.GetActor()->GetClass() != ARotatingPlatform::StaticClass())) {
					RidingWall = true;
					StoredWallNormal = FVector::VectorPlaneProject(ShapeTraceResult.Normal, FVector::UpVector);
				}
			}
		}

		// Handle jumping.
		if (JumpNextFrame && !IsInDialogue) {
			bool wouldhavebeenotg = OnTheGround;
			if (OnTheGround || (StoredWallNormal.Size() > 0.95f && JumpSettings.HasWallJump) || MovementComponent->offGroundTime < JumpSettings.CoyoteJumpTime) {

				// Jump while taking the floor's angle and vertical movement into account.
				CapsuleComponent->AddImpulse((JumpSettings.JumpPower) * FVector::UpVector, NAME_None, true);
				
				WasOnTheGround = false;
				OnTheGround = false;
				MovementComponent->onground = false;
				MovementComponent->PlayerVelocity = CapsuleComponent->GetPhysicsLinearVelocity();
				AlreadyUnjumped = false;
				JustJumped = true;
				MovementComponent->justjumped = true;

				UGameplayStatics::PlaySound2D(this, JumpSound);

				if (StoredWallNormal.Size() > 0.3f && !wouldhavebeenotg && JumpSettings.HasWallJump) {
					// No cheating.
					if (IsGliding || AlreadyGlided) {
						AlreadyGlided = true;
						IsGliding = false;
					}

					// Set player velocity to be away from the wall.
					float multi = JumpSettings.WallJumpMultiplier;
					if (DashSettings.HasDashWallJump&&holdingdash) {
						multi = DashSettings.DashWallJumpMultiplier;
					}

					FlattenVelocity();
					CapsuleComponent->AddImpulse((StoredWallNormal.GetSafeNormal()*PhysicsSettings.MaxVelocity*multi), NAME_None, true);
					GlideNextFrame = false;
					JustWallJumped = true;

					// Make the player face away from the wall we just jumped off of.
					FRotator newmodelrotation = PlayerModel->GetComponentRotation();
					FVector tangentalvelocity = FVector::VectorPlaneProject(CapsuleComponent->GetPhysicsLinearVelocity(), StoredWallNormal.GetSafeNormal());
					if (tangentalvelocity.Size() < PhysicsSettings.MaxVelocity/2.0f) {
						tangentalvelocity = FVector::ZeroVector;
					}

					newmodelrotation.Yaw = ((tangentalvelocity.IsNearlyZero() ? 1.0f : 0.75f)*StoredWallNormal.GetSafeNormal() + 0.25f*tangentalvelocity.GetSafeNormal()).GetSafeNormal().Rotation().Yaw;// = (StoredWallNormal.GetSafeNormal() + FVector::VectorPlaneProject(CapsuleComponent->GetPhysicsLinearVelocity(), StoredWallNormal.GetSafeNormal()).GetSafeNormal() + FVector::VectorPlaneProject((Right*MovementInput.X + Forward*MovementInput.Y).GetSafeNormal(), StoredWallNormal.GetSafeNormal())).Rotation().Yaw;
					PlayerModel->SetWorldRotation(newmodelrotation);
					TargetDirection = newmodelrotation;//
				}
			}
		}

		// Apply a downward force if the player lets go of jump while still moving upwards.
		// This allows for variable jump heights.
		if (!HoldingJump && CapsuleComponent->GetPhysicsLinearVelocity().Z > 0 && !AlreadyUnjumped && !IsGliding) {
			CapsuleComponent->AddImpulse(FVector::UpVector * PhysicsSettings.Gravity * JumpSettings.UnjumpRate * (CapsuleComponent->GetPhysicsLinearVelocity().Z / JumpSettings.JumpPower), NAME_None, true);
			AlreadyUnjumped = true;
		}

		// >_>
		if (MovementComponent->DistanceFromImpact < 25.0f && MovementComponent->overground) {
			GlideNextFrame = false;
		}

		// Start gliding.
		if (GlideNextFrame&&!AlreadyGlided&&!dunk&&GlideSettings.HasGlide&&!WasOnTheGround) {
			if (AppliedForce.Z <= 0.0f) {
				FlattenVelocity();
				CapsuleComponent->AddImpulse(FVector::UpVector * GlideSettings.InitialGlideVelocity, NAME_None, true);
			}
			IsGliding = true;
			AlreadyGlided = true;
			GlideNextFrame = false;
			FloatParticles->ActivateSystem();
		}

		// Handle gliding.
		if (IsGliding&&!WasOnTheGround) {
			GlideTimer += DeltaTime;
			//FlattenVelocity();
			if (FMath::Abs(GlideTimer*GlideSettings.GlideSoundsPerSecond - FMath::FloorToInt(GlideTimer*GlideSettings.GlideSoundsPerSecond)) < DeltaTime*GlideSettings.GlideSoundsPerSecond) {
				WingSound->PitchMultiplier = FMath::FRandRange(GlideSettings.GlideSoundPitchMin, GlideSettings.GlideSoundPitchMax);
				UGameplayStatics::PlaySound2D(this, WingSound);
				//WingSound->PitchMultiplier = 1.0f;
			}
			Wings->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
			PhysicsSettings.Gravity = DefaultGravity * GlideSettings.GlideGravityMultiplier;
			if (AppliedForce.Z <= 0.0f) {
				//CapsuleComponent->AddForce(FVector::UpVector * PhysicsSettings.Gravity, NAME_None, true);
			} else {
				PhysicsSettings.Gravity = DefaultGravity;
			}
		} else {
			FloatParticles->DeactivateSystem();
			GlideTimer = 0.0f;
			Wings->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
			if (GetWorld()->GetTimerManager().GetTimerElapsed(PreWarpTimer) < 0.0f) {
				PhysicsSettings.Gravity = DefaultGravity;
			}
		}

		// Stop gliding.
		if (GlideTimer > GlideSettings.GlideDuration || OnTheGround || !HoldingJump) {
			IsGliding = false;
		}

		// Reset glide and unjump variables if the player is on the ground.
		if (OnTheGround) {
			AlreadyGlided = false;
			AlreadyUnjumped = false;
		}

		// Make the player start dashing in response to input.
		if (DashNextFrame && DashSettings.HasDash && !movementlocked) {
			DashNextFrame = false;
			if (OnTheGround && !MovementComponent->toosteep) {
				DashParticles->ActivateSystem();
				dashing = true;
				UGameplayStatics::PlaySound2D(this, DashSound);
			}
		}

		// Handle dashing.
		if (dashing) {

			// Set the player's horizontal velocity while preserving their vertical velocity.
			CapsuleComponent->SetPhysicsLinearVelocity(TargetDirection.Vector().GetSafeNormal()*DashSettings.DashSpeed+ CapsuleComponent->GetPhysicsLinearVelocity().Z*FVector::UpVector, NAME_None);

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

		// This ain't Megaman X, kiddo.
		if (dashing&&!DashSettings.HasDashJump&&(!OnTheGround||JustJumped)) {
			DashParticles->DeactivateSystem();
			dashing = false;
			dashtimer = 0.0f;
		}

		// COME ON AND SLAM
		if (dunk) {
			if (OnTheGround || CapsuleComponent->GetPhysicsLinearVelocity().Z >= -SlamSettings.SlamVelocity / 2.0f || AppliedForce.Z<0.0f) {
				dunk = false;
				SlamTrail->DeactivateSystem();
				if (OnTheGround) {
					SlamParticles->ActivateSystem();
					SlamParticles->ActivateSystem();
					UGameplayStatics::PlaySound2D(this, DunkHitSound);
				}
			}
		}

		// AND WELCOME TO THE JAM
		if (SlamNextFrame&&SlamSettings.HasSlam) {
			IsGliding = false;
			SlamNextFrame = false;
			if (!OnTheGround && AppliedForce.Z <= 0.0f && !dunk && !ztarget) {
				CapsuleComponent->AddImpulse(-SlamSettings.SlamVelocity*FVector::UpVector, NAME_None, true);
				dunk = true;
				SlamTrail->ActivateSystem();
				UGameplayStatics::PlaySound2D(this, DunkSound);
			}
		}

		// Looks like we're talking to someone.
		if (IsInDialogue) {

			// Don't be rude.
			movementlocked = true;
			MovementInput = FVector::ZeroVector;
			CapsuleComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
			FVector displacement = (GetActorLocation() - CurrentNPC->GetActorLocation());
			TargetDirection = (-displacement).Rotation();

			// Gotta go all smooth like with the camera.
			SpringArm->CameraLagSpeed = ActualDefaultCameraLag * CameraLagSettings.DialogueLagMultiplier * !CurrentCut->InstantTransition;
			SpringArm->CameraRotationLagSpeed = ActualDefaultCameraLag * CameraLagSettings.DialogueLagMultiplier * !CurrentCut->InstantTransition;
			SpringArm->SetWorldTransform(CurrentCut->Camera->GetComponentTransform());
			SpringArm->SetWorldRotation(FRotator(SpringArm->GetComponentRotation().Pitch, SpringArm->GetComponentRotation().Yaw, 0.0f));
			SpringArm->bDoCollisionTest = false;

			// Activates the attached blueprint (if the cut has one).
			if (CurrentCut->BlueprintToExecute != nullptr && !currentdialoguebpactivated) {
				currentdialoguebpactivated = true;
				FOutputDeviceNull dummy;
				CurrentCut->BlueprintToExecute->CallFunctionByNameWithArguments(TEXT("Activate"), dummy, NULL, true);
			}
		}

		// Advances text.
		if (IsInDialogue && ((JumpNextFrame && CurrentCut->CutDuration <= 0.0f) || CurrentCut->cuttimer > CurrentCut->CutDuration)) {
			currentdialoguebpactivated = false;
			if ((JumpNextFrame && CurrentCut->CutDuration <= 0.0f) && stillscrolling) {
				skiptext = true;
			}
			if (!stillscrolling) {
				if (CurrentCut->Next != nullptr) {
					CurrentCut->cuttimer = -1.0f;
					CurrentCut = CurrentCut->Next;
					stillscrolling = true;
					FVector displacement = (GetActorLocation() - CurrentNPC->GetActorLocation());
					TargetDirection = (-displacement).Rotation();
					FRotator rot = displacement.Rotation();
					rot.Pitch = 0.0f;
					rot.Roll = 0.0f;
					CurrentNPC->SetActorRotation(rot);
					TArray<TCHAR> escape = TArray<TCHAR>();
					escape.Add('\n');
					CurrentLine = CurrentCut->DialogueText.ReplaceEscapedCharWithChar(&escape);
					if (CurrentCut->CutDuration > 0.0f) {
						CurrentCut->cuttimer = 0.0f;
					}
				} else {
					IsInDialogue = false;
					movementlocked = false;
					SpringArm->CameraLagSpeed = 0.0f;
					SpringArm->CameraRotationLagSpeed = 0.0f;
					SpringArm->SetWorldTransform(lastcamerabeforedialogue);
					SpringArm->SetRelativeRotation(FRotator(-30.0f, SpringArm->GetComponentRotation().Yaw, 0.0f));
					SpringArm->CameraLagSpeed = CameraLagSettings.CameraLag;
					SpringArm->CameraRotationLagSpeed = CameraLagSettings.CameraRotationLag;

				}
			}
			JumpNextFrame = false;
		}

		if (!stillscrolling) {
			skiptext = false;
		}

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
				ActorItr->PointLight->Intensity = 0.0f; 1750.0f;
				ActorItr->PointLight->UpdateColorAndBrightness();
				ActorItr->asfd = false;
				//closest->mat->SetScalarParameterValue("On", 0.0f);

				// Get displacement vector from the player/camera to the TelePad.
				FVector displacement = ActorItr->GetActorLocation() - source;

				// Get the dot product between the displacement and the source.
				float dot = displacement.GetSafeNormal() | forward.GetSafeNormal();

				// Set trace parameters.
				FHitResult f;
				FCollisionObjectQueryParams TraceParams(ECollisionChannel::ECC_Visibility);
				TraceParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
				TraceParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
				FCollisionQueryParams QueryParams = FCollisionQueryParams();

				// Don't want the ray to collide with the player model now do we?
				QueryParams.AddIgnoredActor(this);

				// Figure out if the ray is blocked by an object.
				bool blocked = GetWorld()->LineTraceSingleByObjectType(f, source, ActorItr->GetActorLocation(), TraceParams, QueryParams);

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

			// Set the target TelePad's color and brightness so that it stands out.
			closest->PointLight->LightColor = TeleportSettings.TeleportLightColor;
			closest->PointLight->Intensity = 32.0f;
			closest->PointLight->UpdateColorAndBrightness();

			closest->asfd = true;
			//closest->mat->SetScalarParameterValue("On", 1.0f);

			// Teleport the player to the TelePad if they're trying to teleport
			// and impart the post teleport velocity on them.
			if (swish) {
				if (closest != nullptr) {

					IsGliding = false;
					ztarget = false;

					closeststick = closest;
					warphere = closest->gohere;
					warpvel = closest->PostTeleportVelocity;
					BackupDefaultArmLength = TargetDefaultArmLength;

					UGameplayStatics::PlaySound2D(this, WarpSound);

					// Start the teleport animation timer.
					if (InCameraOverrideRegion) {
						HereWeGo();
					} else {
						GetWorld()->GetTimerManager().SetTimer(PreWarpTimer, this, &AAuyron::HereWeGo, TeleportSettings.TeleportAnimationDuration);
						TheAbyss->SetRelativeRotation(FRotator(0.0f, 5.0f, FMath::RandRange(-5.0f, 5.0f)));
						//TheAbyss->SetVisibility(true);

						FRotator NewRotation = SpringArm->GetComponentRotation();
						NewRotation.Yaw = PlayerModel->GetComponentRotation().Yaw;
						if (wasztarget) {
							NewRotation.Pitch = -30.0f;
						}

						//SpringArm->CameraLagSpeed = CameraLagSettings.CameraLag*4.0f;
						//SpringArm->CameraRotationLagSpeed = CameraLagSettings.CameraRotationLag*4.0f;

						//TargetDefaultArmLength = 600.0f;
						//SpringArm->TargetArmLength = TargetDefaultArmLength;
						//SpringArm->SetRelativeRotation(NewRotation);

						// Offset the spring arm (and therefore the camera) a bit so the player model
						// isn't blocking the screen when we're trying to aim.
						//FVector base = FVector(0.0f, 100.0f, 100.0f);
						//base = (base.X*Forward + base.Y*Right + base.Z*FVector::UpVector);
						//SpringArm->SetRelativeLocation(base);

						FlattenVelocity();
						PhysicsSettings.Gravity = 0.0f;
						//Capture2D->SetActorLocation(warphere - (warphere - GetActorLocation()).GetSafeNormal()*DefaultArmLength*4.0f);
						FRotator asdfasdf = FRotator::ZeroRotator;
						asdfasdf.Yaw = (warphere - GetActorLocation()).Rotation().Yaw;
						//Capture2D->SetActorLocation(warphere);
						//Capture2D->SetActorRotation(SpringArm->GetComponentRotation());
						//Capture2D->AddActorLocalOffset(-BackupDefaultArmLength*FVector::ForwardVector);
						Capture2D->SetActorTransform(Camera->GetComponentTransform());
						Capture2D->AddActorLocalOffset(FVector::ZeroVector);
						Capture2D->AddActorLocalOffset(FVector::ZeroVector);
						Capture2D->AddActorLocalOffset(FVector::ZeroVector);
						Capture2D->AddActorLocalOffset(FVector::ZeroVector);
						Capture2D->GetCaptureComponent2D()->UpdateContent();
						Capture2D->GetCaptureComponent2D()->Activate();
						//GetWorld()->GetTimerManager().SetTimer(WHY, this, &AAuyron::whywhy, 0.25f);
						lel = true;

						TargetDefaultArmLength = BackupDefaultArmLength;
						DefaultArmLength = TargetDefaultArmLength;
						SpringArm->TargetArmLength = TargetDefaultArmLength;

						teletestmat->SetScalarParameterValue("t",0.1f);

						SpringArm->CameraLagSpeed = 0.0f; 1.0f / 128.0f;
						SpringArm->CameraRotationLagSpeed = 0.0f; 1.0f / 128.0f;
						SetActorLocation(warphere, false, nullptr, ETeleportType::TeleportPhysics);
						SpringArm->SetRelativeLocation(FVector::ZeroVector);// = DefaultArmLength;
					}
				}
			}
		}

		ADestructibleBox* currentbox = NULL;
		biggestdot = -1.0f;

		// Iterate over each DestructibleBox and cast a ray.
		for (TActorIterator<ADestructibleBox> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
			if (ActorItr->IsA(ADestructibleBox::StaticClass())) {

				if (ActorItr->fadetimer != -1.0f) {
					continue;
				}

				// Get displacement vector from the player/camera to the DestructibleBox.
				FVector displacement = ActorItr->GetActorLocation() - source;

				// Get the dot product between the displacement and the source.
				float dot = displacement.GetSafeNormal() | forward.GetSafeNormal();

				// Set trace parameters. I have no idea what these do but the raycast doesn't work
				// if I don't put these here.
				FCollisionObjectQueryParams TraceParams(ECollisionChannel::ECC_Destructible);
				FCollisionQueryParams QueryParams = FCollisionQueryParams();
				QueryParams.AddIgnoredActor(this);
				FHitResult f;

				// Don't want the ray to collide with the player model now do we?
				QueryParams.AddIgnoredActor(this);

				// Figure out if the ray is blocked by an object.
				bool blocked = GetWorld()->LineTraceSingleByObjectType(f, source, ActorItr->GetActorLocation(), TraceParams, QueryParams);

				// If the trace hit a DestructibleBox and it's closer to where we're aiming
				// at than any other DestructibleBox, set it as the "closest" one.
				if (f.GetActor() != nullptr && f.GetActor()->GetClass() != nullptr && f.GetActor()->IsA(ADestructibleBox::StaticClass()) && blocked && displacement.Size() < AttackRange && dot > 0.65f) {
					currentbox = (ADestructibleBox*)(f.GetActor());
					if (currentbox!=nullptr&&AttackPressed) {
						currentbox->BeginFadeout();
					}
				}
			}
		}

		AttackPressed = false;

		// Reset the "player wants to teleport" variable.
		swish = false;

		ASwitch* closestswitch = NULL;
		biggestdot = -1.0f;

		// Iterate over each Switch and cast a ray.
		for (TActorIterator<ASwitch> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
			if (ActorItr->IsA(ASwitch::StaticClass())) {

				// Get displacement vector from the player/camera to the switch.
				FVector displacement = ActorItr->GetActorLocation() - source;

				// Get the dot product between the displacement and the source.
				float dot = displacement.GetSafeNormal() | forward.GetSafeNormal();

				// Set trace parameters. I have no idea what these do but
				// the raycast doesn't work if I don't put these here.
				FHitResult f;
				FCollisionObjectQueryParams TraceParams(ECC_Visibility);
				TraceParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
				TraceParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
				FCollisionQueryParams QueryParams = FCollisionQueryParams();

				// Don't want the ray to collide with the player model now do we?
				QueryParams.AddIgnoredActor(this);

				// Figure out if the ray is blocked by an object.
				bool blocked = GetWorld()->LineTraceSingleByObjectType(f, source, ActorItr->GetActorLocation(), TraceParams, QueryParams);

				// If the trace hit a switch and it's closer to where we're aiming
				// at than any other switch, set it as the "closest" one.
				if (f.GetActor() != nullptr &&
					f.GetActor()->IsA(ASwitch::StaticClass()) &&
					dot > biggestdot &&
					blocked &&
					displacement.Size() < ActorItr->MaxDistance) {
					closestswitch = *ActorItr;
					biggestdot = dot;
				}
			}
		}

		// Activate the "closest" switch as long as it's in range
		// and within a certain angle tolerance.
		if (closestswitch != nullptr &&
			FMath::RadiansToDegrees(FMath::Acos(biggestdot)) < TeleportAngleTolerance &&
			(closestswitch->GetActorLocation() - GetActorLocation()).Size() < closestswitch->MaxDistance) {
			if (ActivateNextFrame) {
				closestswitch->Activate();
			}
		}

		ANPC* closestNPC = NULL;
		biggestdot = -1.0f;

		// Iterate over each NPC and cast a ray.
		for (TActorIterator<ANPC> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
			if (ActorItr->IsA(ANPC::StaticClass())) {

				// Get displacement vector from the player/camera to the NPC.
				FVector displacement = ActorItr->GetActorLocation() - source;

				// Get the dot product between the displacement and the source.
				float dot = displacement.GetSafeNormal() | forward.GetSafeNormal();

				// Set trace parameters. I have no idea what these do but
				// the raycast doesn't work if I don't put these here.
				FHitResult f;
				FCollisionObjectQueryParams TraceParams(ECC_Visibility);
				//TraceParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
				TraceParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel2);
				FCollisionQueryParams QueryParams = FCollisionQueryParams();

				// Don't want the ray to collide with the player model now do we?
				QueryParams.AddIgnoredActor(this);

				// Figure out if the ray is blocked by an object.
				bool blocked = GetWorld()->LineTraceSingleByObjectType(f, source, ActorItr->GetActorLocation(), TraceParams, QueryParams);

				// If the trace hit a NPC and it's closer to where we're aiming
				// at than any other NPC, set it as the "closest" one.
				if (f.GetActor() != nullptr &&
					f.GetActor()->IsA(ANPC::StaticClass()) &&
					dot > biggestdot &&
					blocked &&
					displacement.Size() < ActorItr->MaxDistance) {
					closestNPC = *ActorItr;
					biggestdot = dot;
				}
			}
		}

		// Activate the "closest" NPC as long as it's in
		// range and within a certain angle tolerance.
		if (closestNPC != nullptr &&
			FMath::RadiansToDegrees(FMath::Acos(biggestdot)) < TeleportAngleTolerance &&
			(closestNPC->GetActorLocation() - GetActorLocation()).Size() < closestNPC->MaxDistance) {
			if (ActivateNextFrame && OnTheGround) {
				// Start dialogue with the target NPC.
				closestNPC->Activate();
				IsInDialogue = true;

				lastcamerabeforedialogue = SpringArm->GetComponentTransform();

				// Store information about that NPC and their root DialogueCut.
				CurrentCut = closestNPC->CurrentCut;
				CurrentNPC = closestNPC;
				TArray<TCHAR> escape = TArray<TCHAR>();
				escape.Add('\n');
				stillscrolling = true;
				CurrentLine = CurrentCut->DialogueText.ReplaceEscapedCharWithChar(&escape);

				// Look at me when I'm talking to you!
				FVector displacement = (GetActorLocation() - CurrentNPC->GetActorLocation());
				FRotator rot = displacement.Rotation();
				rot.Pitch = 0.0f;
				rot.Roll = 0.0f;
				CurrentNPC->SetActorRotation(rot);
				TargetDirection = (-displacement).Rotation();

				// Code to prevent violations of personal space.
				FVector target = displacement.GetSafeNormal()*150.0f;
				SetActorLocation(target + CurrentNPC->GetActorLocation());

				CapsuleComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);

				// Snap the camera to the spring arm component's root (so we don't have to deal with
				// its arm length) and place it where the camera used to be. This effectively "nullifies"
				// the spring arm for the purposes of lerping the camera to its new DialogueCut-specified
				// position.
				FVector temp = Camera->GetComponentLocation();
				SpringArm->CameraLagSpeed = 0.0f;
				SpringArm->CameraRotationLagSpeed = 0.0f;
				SpringArm->SetWorldLocation(temp);
				SpringArm->TargetArmLength = 0.0f;

				// Activates the attached blueprint (if the cut has one).
				if (CurrentCut->BlueprintToExecute != nullptr) {
					FOutputDeviceNull dummy;
					CurrentCut->CallFunctionByNameWithArguments(TEXT("Activate"), dummy, NULL, true);
				}
			}
		}
	}

	{

		// Handle camera movement when the camera is controllable.
		if ((!InCameraOverrideRegion || ztarget) && !IsInDialogue&& GetWorldTimerManager().GetTimerElapsed(PreWarpTimer)==-1.0f) {
			timesinceoverrideenter = 0.0f;
			SpringArm->bDoCollisionTest = true;

			// Reset camera input timer if the camera controls were touched.
			if (CameraInput.X != 0.0f || CameraInput.Y != 0.0f) {
				TimeSinceLastMouseInput = 0.0f;
			} else {
				TimeSinceLastMouseInput += DeltaTime;
			}

			// Temporary variable to hold the camera's new rotation.
			FRotator NewRotation = SpringArm->GetComponentRotation();

			// Move the camera's yaw in response to the x input of the mouse/stick.
			NewRotation.Yaw = NewRotation.Yaw + CameraInput.X;

			// The camera should only turn with the player if the mouse hasn't been touched recently.
			if (TimeSinceLastMouseInput > CameraAutoTurnSettings.CameraResetTime && !ztarget && !movementlocked) {
				NewRotation.Yaw += FMath::Pow(FMath::Abs(MovementInput.X), 1.0f) * (Camera->GetRightVector().GetSafeNormal() | FVector::VectorPlaneProject(CapsuleComponent->GetPhysicsLinearVelocity(), FVector::UpVector) / PhysicsSettings.MaxVelocity) * DeltaTime * CameraAutoTurnSettings.CameraAutoTurnFactor;
				if (TimeSinceLastMovementInputReleased > CameraAutoTurnSettings.CameraResetTime) {
					NewRotation.Pitch = FMath::Lerp(SpringArm->RelativeRotation.Pitch, CameraAutoTurnSettings.CameraDefaultPitch, CameraAutoTurnSettings.CameraDefaultPitchRate);
				}
			}

			// Set the rotation of the camera.
			SpringArm->SetWorldRotation(NewRotation);

			// Move the camera's pitch in response to the y input of the mouse/stick.
			NewRotation = SpringArm->GetComponentRotation();
			NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -CameraMaxAngle, -CameraMinAngle);
			SpringArm->CameraRotationLagSpeed = CameraLagSettings.CameraRotationLag;
			SpringArm->SetWorldRotation(NewRotation);

		}
		
		// Handle CameraOverrideRegions.
		if(InCameraOverrideRegion&&!ztarget) {
			
			if (!wasztarget) {
				SpringArm->CameraLagSpeed = ActualDefaultCameraLag;
				SpringArm->CameraRotationLagSpeed = ActualDefaultCameraLag;
			}
			SpringArm->TargetArmLength = DefaultArmLength;
			SpringArm->SetWorldRotation(currentoverrideregion->TargetCamera->GetForwardVector().Rotation().Quaternion());
			timesinceoverrideenter += DeltaTime;
			SpringArm->bDoCollisionTest = false;

			if (currentoverrideregion->LockType == CameraLockType::POINT) {
				SpringArm->SetWorldLocation(currentoverrideregion->TargetCamera->GetComponentLocation());
				if (currentoverrideregion->LookAtPlayer) {
					SpringArm->SetRelativeRotation((-SpringArm->RelativeLocation).Rotation());
				}
			}
			if (currentoverrideregion->LockType == CameraLockType::AXIS) {
				FVector camerarelativedisplacement = (GetActorLocation() - currentoverrideregion->Axis->GetComponentLocation());
				SpringArm->SetWorldLocation(currentoverrideregion->TargetCamera->GetComponentLocation() + currentoverrideregion->Axis->GetForwardVector().GetSafeNormal() * (currentoverrideregion->Axis->GetForwardVector().GetSafeNormal() | camerarelativedisplacement));

			}
			if (currentoverrideregion->LockType == CameraLockType::PLANE) {
				FVector camerarelativedisplacement = (GetActorLocation() - currentoverrideregion->Axis->GetComponentLocation());
				SpringArm->SetWorldLocation(currentoverrideregion->TargetCamera->GetComponentLocation() + FVector::VectorPlaneProject(camerarelativedisplacement,currentoverrideregion->Axis->GetForwardVector().GetSafeNormal()));
			}
		} else if (!InCameraOverrideRegion) {
			Camera->RelativeRotation.Roll = 0.0f;
			SpringArm->RelativeRotation.Roll = 0.0f;
		}

		// Handle aiming.
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
				BackupDefaultArmLength = TargetDefaultArmLength;

				// ...orient the new rotation to be level with the xy plane...
				//NewRotation.Pitch = 0.0f;

				// ...and face the camera in direction that the player is facing...
				//if (MovementInput.IsNearlyZero()) {
					//NewRotation.Yaw = PlayerModel->GetComponentRotation().Yaw;//
				//} else {
					// ...unless the player is holding a direction,
					// in which case face that direction.
					//NewRotation.Yaw = SpringArm->GetComponentRotation().Yaw +
						//(MovementInput.X >= 0 ? 1 : -1) * FMath::RadiansToDegrees(FMath::Acos(MovementInput.GetSafeNormal() | FVector(0, 1, 0)));
				//}
				if (!MovementInput.IsNearlyZero()) {
					NewRotation.Pitch = 0.0f;
					NewRotation.Yaw = SpringArm->GetComponentRotation().Yaw +
						(MovementInput.X >= 0 ? 1 : -1) * FMath::RadiansToDegrees(FMath::Acos(MovementInput.GetSafeNormal() | FVector(0, 1, 0)));
				}
				if (TimeSinceLastMouseInput > CameraAutoTurnSettings.CameraResetTime && !movementlocked) {
					NewRotation.Pitch = 0.0f;
				}
			}

			// Face the player and camera to the new rotation.
			PlayerModel->SetWorldRotation(FRotator(0.0f, NewRotation.Yaw,0.0f));
			SpringArm->TargetArmLength = 300.0f;
			DefaultArmLength = 300.0f;
			SpringArm->CameraLagSpeed = CameraLagSettings.CameraLag * (wasztarget ? CameraLagSettings.AimingLagMultiplier : 1.0f);
			SpringArm->CameraRotationLagSpeed = CameraLagSettings.CameraRotationLag * (wasztarget ? CameraLagSettings.AimingLagMultiplier : 1.0f);
			SpringArm->SetRelativeRotation(NewRotation);

			// Offset the spring arm (and therefore the camera) a bit so the player model
			// isn't blocking the screen when we're trying to aim.
			FVector base = FVector(0.0f, 100.0f, 100.0f);
			base = (base.X*Forward + base.Y*Right + base.Z*FVector::UpVector);
			SpringArm->SetRelativeLocation(base);

			// Lock the player's movement inputs.
			movementlocked = true;

		} else if(!IsInDialogue) {

			// Set and range and angle to the "short range" values.
			TeleportRange = TeleportSettings.TeleportRangeWhenNotAiming;
			TeleportAngleTolerance = TeleportSettings.TeleportAngleToleranceWhenNotAiming;

			// Return the spring arm (and camera) to its original location.
			if (!InCameraOverrideRegion&&!IsInDialogue) {
				if (!justswished) {
					SpringArm->CameraLagSpeed = CameraLagSettings.CameraLag;
					SpringArm->CameraRotationLagSpeed = CameraLagSettings.CameraRotationLag;
				}

				if (GetWorldTimerManager().GetTimerElapsed(PreWarpTimer) >= 0.0f) {
					SpringArm->CameraLagSpeed = 0.0f; //CameraLagSettings.CameraLag*2.5f;
					SpringArm->CameraRotationLagSpeed = 0.0f; //CameraLagSettings.CameraRotationLag*2.5f;
					DefaultArmLength = TargetDefaultArmLength;
					SpringArm->TargetArmLength = DefaultArmLength;
				} else {
					SpringArm->TargetArmLength = DefaultArmLength;
				}

				SpringArm->SetRelativeLocation(FVector::ZeroVector);
				//Camera->bConstrainAspectRatio = false;

				// If we just stopped aiming, reset the camera's rotation as well.
				if (wasztarget&&GetWorldTimerManager().GetTimerElapsed(PreWarpTimer)==-1.0f) {
					TargetDefaultArmLength = BackupDefaultArmLength;
					SpringArm->SetRelativeRotation(FRotator(-30.0f, SpringArm->GetComponentRotation().Yaw, 0.0f));
				}
			}

			if (!IsInDialogue) {
				// Re-enable the player's movement inputs.
				movementlocked = false;
			}
		}

	}

	// Physiiiiicccss.
	{
		if (!isclimbing) {
			// Get horizontal velocity projection.
			FVector Velocity2D = FVector::VectorPlaneProject(CapsuleComponent->GetPhysicsLinearVelocity(), FVector::UpVector);

			// Adjust movement input to reflect camera direction.
			FVector AdjustedMovementInput = (Right*MovementInput.X + Forward*MovementInput.Y).GetClampedToMaxSize(1.0f);

			// Get acceleration magnitude depending if the player is in the air or not.
			float CurrentAccelRate = (OnTheGround ? PhysicsSettings.GroundAccelerationRate : PhysicsSettings.AirAccelerationRate) * 10000.0f;

			// Apply acceleration based on movement inputs.
			if (!dashing) {
				CapsuleComponent->AddForce(AdjustedMovementInput * CurrentAccelRate, NAME_None, false);
			}

			// If the platform we're standing on is accelerating, add that acceleration to the player's acceleration,
			// but only if the player didn't just jump onto or off of the platform, and the platform didn't just
			// quickly and immediately change directions.
			if (!(previousgroundvelocity.IsNearlyZero() && !MovementComponent->groundvelocity.IsNearlyZero()) &&
				!(!previousgroundvelocity.IsNearlyZero() && MovementComponent->groundvelocity.IsNearlyZero()) &&
				(MovementComponent->groundvelocity - previousgroundvelocity).Size() / DeltaTime < 1000.0f) {
				CapsuleComponent->AddImpulse((MovementComponent->groundvelocity - previousgroundvelocity), NAME_None, true);
			}

			// Apply gravity if in the air, and stop vertical movement if on the ground.
			if (!OnTheGround && !IsInDialogue&&GetWorldTimerManager().GetTimerElapsed(PreWarpTimer) == -1.0f) {
				CapsuleComponent->AddForce(PhysicsSettings.Gravity*FVector::UpVector, NAME_None, true);
			} else {
				FlattenVelocity();
			}

			// Apply drag.
			if (!OnTheGround && FMath::Abs(CapsuleComponent->GetPhysicsLinearVelocity().Z) > JumpSettings.JumpPower) {
				float mult = ((CapsuleComponent->GetPhysicsLinearVelocity().Z > 0.0f && (AppliedForce.Z > 0.0f || IsGliding)) ? 16.0f : 1.0f);
				CapsuleComponent->AddForce(FVector::UpVector * PhysicsSettings.Gravity * FMath::Pow(CapsuleComponent->GetPhysicsLinearVelocity().Z / PhysicsSettings.TerminalVelocity, 2.0f) * FMath::Sign(CapsuleComponent->GetPhysicsLinearVelocity().Z)*200.0f*mult);
			}

			// Handle force regions.
			CapsuleComponent->AddForce(AppliedForce*200.0f, NAME_None, false);
			if (AppliedForce.Z > 0.0f) {
				AlreadyUnjumped = true;
				OnTheGround = false;
			}

			// Apply a deceleration that scales with the player's velocity
			// in such a way that it limits it to MaxVelocity.
			if (!dashing) {
				float mult = FMath::Lerp(FMath::Sqrt(Velocity2D.Size() / PhysicsSettings.MaxVelocity), Velocity2D.Size() / PhysicsSettings.MaxVelocity, Velocity2D.Size() / PhysicsSettings.MaxVelocity);
				CapsuleComponent->AddForce(-Velocity2D.GetSafeNormal()*CurrentAccelRate*mult, NAME_None, false);
			}

			// This is a 2D platformer now.
			if (MovementAxisLocked) {
				FVector newpos(GetActorLocation());
				switch (LockedMovementAxis) {
				case MovementRegionLockedAxis::XAXIS:
					newpos.X = LockedAxisValue;
					CapsuleComponent->SetPhysicsLinearVelocity(CapsuleComponent->GetPhysicsLinearVelocity()*FVector(0.0f, 1.0f, 1.0f));
					MovementInput.Y = 0.0f;
					break;
				case MovementRegionLockedAxis::YAXIS:
					newpos.Y = LockedAxisValue;
					CapsuleComponent->SetPhysicsLinearVelocity(CapsuleComponent->GetPhysicsLinearVelocity()*FVector(1.0f, 0.0f, 1.0f));
					MovementInput.Y = 0.0f;
					break;
				case MovementRegionLockedAxis::ZAXIS:
					newpos.Z = LockedAxisValue;
					CapsuleComponent->SetPhysicsLinearVelocity(CapsuleComponent->GetPhysicsLinearVelocity()*FVector(1.0f, 1.0f, 0.0f));
					break;
				}
				SetActorLocation(newpos);
			}
		}

		// Put Velocity back in the reference frame of the stationary world.
		CapsuleComponent->SetPhysicsLinearVelocity(CapsuleComponent->GetPhysicsLinearVelocity() + MovementComponent->groundvelocity); // + (MovementComponent->groundvelocity + pushvelocity)

	}

	{
		WasOnTheGround = OnTheGround;
		WasInCameraOverrideRegion = InCameraOverrideRegion;
		previousposition = GetActorLocation();
		wasztarget = ztarget;
		previousvelocity = CapsuleComponent->GetPhysicsLinearVelocity();
		previousgroundvelocity = MovementComponent->groundvelocity;

		pushvelocity = FVector::ZeroVector;

		JumpNextFrame = false;
		ActivateNextFrame = false;
		SlamNextFrame = false;
		DashNextFrame = false;
		GlideNextFrame = false;
		swish = false;

		justswished = false;
		justteleported = false;
		JustJumped = false;
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
		if (!ztarget && (OnTheGround || IsGliding)) {
			// I'ma tell ya not even Unity was stupid enough to use -180 -> 180 for rotations.
			int8 reflect = (MovementInput.X >= 0 ? 1 : -1);

			// (but I forgive you)
			if (!MovementInput.IsNearlyZero() && !ztarget) {
				TargetDirection.Yaw = SpringArm->GetComponentRotation().Yaw +
					reflect * FMath::RadiansToDegrees(FMath::Acos(MovementInput.GetSafeNormal() | FVector(0, 1, 0)));
				if (MovementAxisLocked) {
					FVector tempvector(TargetDirection.Vector());
					switch (LockedMovementAxis) {
						case MovementRegionLockedAxis::XAXIS:
							tempvector.X = 0.0f;
							TargetDirection = tempvector.Rotation();
							break;
						case MovementRegionLockedAxis::YAXIS:
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
			TargetDirection.Roll = 0.0f;
			TargetDirection.Pitch = 0.0f;
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
	//InputComponent->bBlockInput = true;
	InputComponent->SetTickableWhenPaused(true);
	InputComponent->Priority = 1000;
	InputComponent->BindAxis("MoveX", this, &AAuyron::MoveRight);
	InputComponent->BindAxis("MoveY", this, &AAuyron::MoveForward);
	InputComponent->BindAxis("CameraPitch", this, &AAuyron::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &AAuyron::YawCamera);
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
	InputComponent->BindAction("Attack", IE_Pressed, this, &AAuyron::Attack);
	InputComponent->BindAction("Pause", IE_Pressed, this, &AAuyron::Pause);
	InputComponent->BindAction("Unpause", IE_Pressed, this, &AAuyron::Unpause);
	InputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &AAuyron::CameraZoomIn);
	InputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &AAuyron::CameraZoomOut);
}

// Can you believe the tutorial wanted me to use Y for horizontal movement
void AAuyron::MoveRight(float AxisValue)
{
	float sqr = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f)*FMath::Abs(FMath::Clamp<float>(AxisValue, -1.0f, 1.0f));
	MovementInput.X = FMath::Lerp(FMath::Clamp<float>(AxisValue, -1.0f, 1.0f), sqr, FMath::Abs(FMath::Clamp<float>(AxisValue, -1.0f, 1.0f)));
}

// and X for vertical? I mean who does that?
void AAuyron::MoveForward(float AxisValue)
{
	float sqr = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f)*FMath::Abs(FMath::Clamp<float>(AxisValue, -1.0f, 1.0f));
	MovementInput.Y = FMath::Lerp(FMath::Clamp<float>(AxisValue, -1.0f, 1.0f), sqr, FMath::Abs(FMath::Clamp<float>(AxisValue, -1.0f, 1.0f)));
}

void AAuyron::PitchCamera(float AxisValue)
{
	if ((ztarget && YAxisAimingStyle == INVERTED) || (!ztarget && YAxisStyle == INVERTED)) {
		AxisValue *= -1;
	}
	CameraInput.Y = AxisValue;
}

void AAuyron::YawCamera(float AxisValue)
{
	if ((ztarget && XAxisAimingStyle == INVERTED) || (!ztarget && XAxisStyle == INVERTED)) {
		AxisValue *= -1;
	}
	CameraInput.X = AxisValue;
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
	//FVector wnproject = FVector::VectorPlaneProject(MovementComponent->wallnormal, FVector::UpVector);
	//if (OnTheGround || MovementComponent->offGroundTime < JumpSettings.CoyoteJumpTime || RidingWall || (!MovementComponent->wallnormal.IsNearlyZero() && wnproject.Size()>0.9f)) {
	//if (OnTheGround) {
		JumpNextFrame = true;
		if (!OnTheGround && MovementComponent->offGroundTime > JumpSettings.CoyoteJumpTime) {
			GlideNextFrame = true;
		}
	//} else {
		//if (GlideSettings.HasGlide&&!AlreadyGlided) {
			//GlideNextFrame = true;
		//}
	//}
	HoldingJump = true;
}

// I wish you could unjump in real life.
void AAuyron::Unjump()
{
	HoldingJump = false;
}

void AAuyron::Use()
{
	if (!IsInDialogue) {
		ActivateNextFrame = true;
	} else {
		JumpNextFrame = true;
	}
}

// HEY LINK TALK TO ME USING Z TARGETING
void AAuyron::CameraFaceForward()
{
	if (!dashing&&!MovementAxisLocked&&TeleportSettings.HasTeleport&&!IsInDialogue&&GetWorldTimerManager().GetTimerElapsed(PreWarpTimer)==-1.0f) {
		if (AimStyle == TOGGLE) {
			ztarget = !ztarget;
		} else {
			ztarget = true;
		}
	}
}

void AAuyron::CameraUnFaceForward()
{
	if (AimStyle == HOLD && !dashing) {
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
	if (TeleportSettings.HasTeleport&&GetWorldTimerManager().GetTimerElapsed(PreWarpTimer)==-1.0f) {
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
	if (!dashing&&!ztarget) {
		DashNextFrame = true;
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


void AAuyron::Attack() {
	AttackPressed = true;
}

void AAuyron::CameraZoomIn() {
	bool justno = !ztarget && !IsInDialogue && !cameralocked && !InCameraOverrideRegion && GetWorldTimerManager().GetTimerElapsed(PreWarpTimer) == -1.0f;
	if (TargetDefaultArmLength > MinimumArmLength && justno) {
		if (TargetDefaultArmLength < ActualDefaultArmLength) {
			CameraLagSettings.CameraLag *= CameraLagZoomScale;
		}
		TargetDefaultArmLength -= CameraZoomStep;
	}
}

void AAuyron::CameraZoomOut() {
	bool justno = !ztarget && !IsInDialogue && !cameralocked && !InCameraOverrideRegion && GetWorldTimerManager().GetTimerElapsed(PreWarpTimer) == -1.0f;
	if (TargetDefaultArmLength < MaximumArmLength && justno) {
		TargetDefaultArmLength += CameraZoomStep;
		if (TargetDefaultArmLength < ActualDefaultArmLength) {
			CameraLagSettings.CameraLag /= CameraLagZoomScale;
		}
	}
}

// Getter functions used by the animation blueprints.
float AAuyron::GetSpeed()
{
	return (FVector::VectorPlaneProject(CapsuleComponent->GetPhysicsLinearVelocity() - MovementComponent->groundvelocity, FVector::UpVector)).Size();
}

FVector AAuyron::GetPlayerVelocity()
{
	return (CapsuleComponent->GetPhysicsLinearVelocity() - MovementComponent->groundvelocity);
}

float AAuyron::GetActualSpeed()
{
	//return (CapsuleComponent->GetPhysicsLinearVelocity() - MovementComponent->groundvelocity).Size();
	return (FVector::VectorPlaneProject(CapsuleComponent->GetPhysicsLinearVelocity() - MovementComponent->groundvelocity, FVector::UpVector)).Size();
}

float AAuyron::GetModelOpacity()
{
	float dist = (Camera->GetComponentLocation() - GetActorLocation()).Size();
	return FMath::Clamp(dist / CameraModelFadeSettings.ModelFadeDistance, 0.0f, 1.0f);
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

void AAuyron::SetAimStyle(FString Style)
{
	TEnumAsByte<AimType> NewStyle = (Style == "Toggle" ? TOGGLE : HOLD);
	AimStyle = NewStyle;
}

FString AAuyron::GetAimStyle()
{
	return (AimStyle == TOGGLE ? "Toggle" : "Hold");
}

void AAuyron::SetXAxisStyle(FString Style)
{
	XAxisStyle = (Style == "Inverted" ? INVERTED : STANDARD);
}

FString AAuyron::GetXAxisStyle()
{
	return (XAxisStyle == INVERTED ? "Inverted" : "Standard");
}

void AAuyron::SetXAxisAimingStyle(FString Style)
{
	XAxisAimingStyle = (Style == "Inverted" ? INVERTED : STANDARD);
}

FString AAuyron::GetXAxisAimingStyle()
{
	return (XAxisAimingStyle == INVERTED ? "Inverted" : "Standard");
}

void AAuyron::SetYAxisStyle(FString Style)
{
	YAxisStyle = (Style == "Inverted" ? INVERTED : STANDARD);
}

FString AAuyron::GetYAxisStyle()
{
	return (YAxisStyle == INVERTED ? "Inverted" : "Standard");
}

void AAuyron::SetYAxisAimingStyle(FString Style)
{
	YAxisAimingStyle = (Style == "Inverted" ? INVERTED : STANDARD);
}

FString AAuyron::GetYAxisAimingStyle()
{
	return (YAxisAimingStyle == INVERTED ? "Inverted" : "Standard");
}

void AAuyron::SetEnableTeleportAnimation(bool ShouldEnable)
{
	TeleportSettings.TeleportAnimationDuration = ActualTeleportAnimationDuration * ShouldEnable;
}

bool AAuyron::GetEnableTeleportAnimation()
{
	return (TeleportSettings.TeleportAnimationDuration > 0.0f);
}

void AAuyron::HandlePause()
{
	dashing = false;
	IsGliding = false;
	ztarget = false;
	HoldingJump = false;
}

void AAuyron::BlockInput()
{
	blockedbyblueprint = true;
	movementlocked = true;
	cameralocked = true;
}

void AAuyron::ResumeInput()
{
	blockedbyblueprint = false;
	movementlocked = false;
	cameralocked = false;
}

void AAuyron::SetMaterial(int32 index, UMaterialInterface * newmat)
{
	PlayerModel->SetMaterial(index,newmat);
}

bool AAuyron::HasTeleport()
{
	return TeleportSettings.HasTeleport;
}

void AAuyron::SetHasTeleport(bool has)
{
	TeleportSettings.HasTeleport = has;
}

bool AAuyron::HasDash()
{
	return DashSettings.HasDash;
}

void AAuyron::SetHasDash(bool has)
{
	DashSettings.HasDash = has;
}

bool AAuyron::HasWallJump()
{
	return true;
}

void AAuyron::SetHasWallJump(bool has)
{
	JumpSettings.HasWallJump = has;
}

bool AAuyron::HasGlide()
{
	return GlideSettings.HasGlide;
}

void AAuyron::SetHasGlide(bool has)
{
	GlideSettings.HasGlide = has;
}

bool AAuyron::HasSlam()
{
	return SlamSettings.HasSlam;
}

void AAuyron::SetHasSlam(bool has)
{
	SlamSettings.HasSlam = has;
}

void AAuyron::SetStillScrolling(bool b)
{
	stillscrolling = b;
}

bool AAuyron::GetSkipText()
{
	return skiptext;
}

bool AAuyron::GetIsClimbing()
{
	return isclimbing;
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

bool AAuyron::GetIsInDialogue()
{
	return IsInDialogue && !CurrentCut->NoText;
}

FString AAuyron::GetDialogueText()
{
	return CurrentLine;
}

USkeletalMeshComponent* AAuyron::GetMesh() {
	return PlayerModel;
}