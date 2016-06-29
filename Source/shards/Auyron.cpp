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
#include "SandShip.h"
#include "DialogueCut.h"
#include "NPC.h"
#include "TwoDimensionalMovementRegion.h"
#include "ForceRegion.h"
#include "WarpCrystal.h"
#include "Switch.h"

// Sets default values4//
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
	DashSettings.HasAirDash = true;

	GlideSettings.HasGlide = false;
	GlideSettings.GlideTurnRateMultiplier = 1.0f;
	GlideSettings.GlideDuration = 0.5f;
	GlideSettings.InitialGlideVelocity = 200.0f;
	GlideSettings.GlideSoundsPerSecond = 8.0f;
	GlideSettings.GlideSoundPitchMin = 0.996f;
	GlideSettings.GlideSoundPitchMax = 1.004f;

	SlamSettings.HasSlam = false;
	SlamSettings.SlamVelocity = 1500.0f;

	CelShaderSettings.LightMin = 0.4f;
	CelShaderSettings.LightMax = 0.8f;
	CelShaderSettings.AdditiveLightBias = 0.0f;
	CelShaderSettings.MultiplicativeLightBias = 2.0f;
	CelShaderSettings.Tint = FLinearColor(1.0f,1.0f,1.0f,0.0f);

	CameraMaxAngle = 85.0f;
	CameraMinAngle = -85.0f;
	DefaultArmLength = 1000.0f;
	MinimumArmLength = 200.0f;
	MaximumArmLength = 1600.0f;
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
	CameraAutoTurnSettings.CameraDefaultPitch = -15.0f;
	CameraAutoTurnSettings.CameraDefaultPitchRate = 2.0f;

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
	//onbeginoverlapdelegate.BindUFunction(this, "Hit");
	//CapsuleComponent->OnComponentBeginOverlap.Add(onbeginoverlapdelegate);
	//onendoverlapdelegate.BindUFunction(this, "UnHit");
	//CapsuleComponent->OnComponentEndOverlap.Add(onendoverlapdelegate);
	//onhitdelegate.BindUFunction(this, "Stay");
	//CapsuleComponent->OnComponentHit.Add(onhitdelegate);
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuyron::Hit);
	//CapsuleComponent->OnComponentHit.AddDynamic(this, &AAuyron::Stay);
	CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &AAuyron::UnHit);
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
	CapsuleComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);//4.12
	SetActorEnableCollision(true);

	// It you.
	PlayerModel = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerModel"));
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMeshObj(TEXT("/Game/Models/Characters/Auyron/Auyron2"));
	PlayerModel->SetSkeletalMesh(PlayerMeshObj.Object);
	PlayerModel->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	PlayerModel->bRenderCustomDepth = true;
	PlayerModel->SetSimulatePhysics(false);
	PlayerModel->bReceivesDecals = false;
	PlayerModel->bCastInsetShadow = false;
	PlayerModel->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PlayerModel->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	PlayerModel->SetCollisionResponseToChannel(ECC_PhysicsBody, ECollisionResponse::ECR_Block);
	PlayerModel->SetCollisionObjectType(ECC_PhysicsBody);
	PlayerModel->SetSimulatePhysics(false);
	//PlayerModel->AttachTo(CapsuleComponent);
	PlayerModel->AttachToComponent(CapsuleComponent, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	//static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimationBlueprint(TEXT("/Game/Animations/Characters/Auyron/Anim_Auyron"));
	//PlayerModel->SetAnimInstanceClass(AnimationBlueprint.Object->GetAnimBlueprintGeneratedClass());
	
	// Use a spring arm so the camera can be all like swoosh.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(-15.0f, 0.0f, 0.0f));
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = CameraLagSettings.CameraLag;
	SpringArm->CameraRotationLagSpeed = CameraLagSettings.CameraRotationLag;
	SpringArm->CameraLagMaxDistance = 1000.0f;
	SpringArm->ProbeSize = 20.0f;
	//SpringArm->AttachTo(CapsuleComponent);
	SpringArm->AttachToComponent(CapsuleComponent, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	// Camera so the casuals can "see what they're doing" or whatever.
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	//Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);//4.12

	// May god have mercy on your GPU.
	DashParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Dash Particles"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> dp(TEXT("/Game/Particles/DashParticles"));
	DashParticles->SetTemplate(dp.Object);
	DashParticles->bAutoActivate = false;
	DashParticles->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	//DashParticles->AttachTo(PlayerModel);
	DashParticles->AttachToComponent(PlayerModel, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	FloatParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Float Particles"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> fp(TEXT("/Game/Particles/FloatParticles"));
	FloatParticles->SetTemplate(fp.Object);
	FloatParticles->bAutoActivate = false;
	FloatParticles->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	//FloatParticles->AttachTo(PlayerModel);
	FloatParticles->AttachToComponent(PlayerModel, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	SlamParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Slam Particles"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> sp(TEXT("/Game/Particles/SlamParticles"));
	SlamParticles->SetTemplate(sp.Object);
	SlamParticles->bAutoActivate = false;
	SlamParticles->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//SlamParticles->AttachTo(PlayerModel);
	SlamParticles->AttachToComponent(PlayerModel, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	SlamTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Slam Trail"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> st(TEXT("/Game/Particles/SlamTrail"));
	SlamTrail->SetTemplate(st.Object);
	SlamTrail->bAutoActivate = false;
	SlamTrail->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	//SlamTrail->AttachTo(PlayerModel);
	SlamTrail->AttachToComponent(PlayerModel, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	TrailParticlesL = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particles L"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> tpl(TEXT("/Game/Particles/TrailParticles"));
	TrailParticlesL->SetTemplate(tpl.Object);
	TrailParticlesL->bAutoActivate = false;
	TrailParticlesL->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	//TrailParticlesL->AttachTo(PlayerModel);
	TrailParticlesL->AttachToComponent(PlayerModel, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	TrailParticlesR = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particles R"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> tpr(TEXT("/Game/Particles/TrailParticles"));
	TrailParticlesR->SetTemplate(tpr.Object);
	TrailParticlesR->bAutoActivate = false;
	TrailParticlesR->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	//TrailParticlesR->AttachTo(PlayerModel);
	TrailParticlesR->AttachToComponent(PlayerModel, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	grassparticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Grass Particles"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> gpr(TEXT("/Game/grassparticles"));
	grassparticles->SetTemplate(gpr.Object);
	grassparticles->bAutoActivate = false;
	grassparticles->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	//grassparticles->AttachTo(PlayerModel);
	grassparticles->AttachToComponent(PlayerModel, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	DropShadow = CreateDefaultSubobject<UDecalComponent>(TEXT("Drop Shadow"));
	const ConstructorHelpers::FObjectFinder<UMaterialInterface> dsmat(TEXT("/Game/Textures/Effects/dropshadow"));
	DropShadow->SetMaterial(0,dsmat.Object);
	DropShadow->DecalSize = FVector(1000.0f, 45.0f, 45.0f);
	DropShadow->DecalSize = FVector(1000.0f, 90.0f, 90.0f);
	DropShadow->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	DropShadow->SetRelativeLocation(FVector(0.0f, 0.0f, -950.0f));
	DropShadow->SetRelativeLocation(FVector(0.0f, 0.0f, -850.0f));
	DropShadow->DecalSize = FVector(90.0f, 90.0f, 90.0f);
	DropShadow->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	DropShadow->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//DropShadow->AttachTo(PlayerModel);
	DropShadow->AttachToComponent(PlayerModel, FAttachmentTransformRules::KeepRelativeTransform);//4.12

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
	//TheAbyss->AttachTo(PlayerModel);
	TheAbyss->AttachToComponent(PlayerModel, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	TeleClaw->SetVisibility(TeleportSettings.HasTeleport);
	BootsR->SetVisibility(DashSettings.HasDash);
	BootsL->SetVisibility(DashSettings.HasDash);
	Bracelet->SetVisibility(SlamSettings.HasSlam);
	Belt->SetVisibility(false);
	Wings->SetVisibility(GlideSettings.HasGlide);
	TheAbyss->SetVisibility(false);

	// ASSUMING DIRECT CONTROL.
	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Apparently we need some newfangled "MovementComponent".
	MovementComponent = CreateDefaultSubobject<UAuyronMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CapsuleComponent;
	
	// BLAST PROCESSING.
	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	//PostProcess->AttachTo(RootComponent);
	PostProcess->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);//4.12

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

	const ConstructorHelpers::FObjectFinder<UMaterialInterface> riftmat2(TEXT("/Game/Textures/Effects/warpmat"));
	TestTeleEffectBase = riftmat2.Object;

	const ConstructorHelpers::FObjectFinder<UMaterialInterface> behindmat(TEXT("/Game/Textures/Effects/Outline"));
	DrawBehindMaterial = behindmat.Object;

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
	IsGliding = false;
	dunk = false;
	SetActorLocation(RespawnPoint, false,NULL,ETeleportType::TeleportPhysics);
}

void AAuyron::StopClimbing() {
	CurrentState = &Normal;
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
	CurrentState = &Normal;
}

void AAuyron::MoveIt()
{
	Capture2D->AddActorLocalOffset(FVector::ZeroVector);
	Capture2D->AddActorLocalOffset(FVector::ZeroVector);
	Capture2D->AddActorLocalOffset(FVector::ZeroVector);
	Capture2D->AddActorLocalOffset(FVector::ZeroVector);
	Capture2D->GetCaptureComponent2D()->UpdateContent();
	Capture2D->GetCaptureComponent2D()->Activate();
	SetActorLocation(warphere, false, nullptr, ETeleportType::TeleportPhysics);
	GetWorld()->GetTimerManager().ClearTimer(PreWarpTimer2);
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
	outlinemat = UMaterialInstanceDynamic::Create(DrawBehindMaterial, this);

	Camera->AddOrUpdateBlendable(teletestmat);
	Camera->AddOrUpdateBlendable(celshadermat);
	Camera->AddOrUpdateBlendable(outlinemat);

	PlayerModel->SetMaterial(0, bodymat);
	PlayerModel->SetMaterial(1, hairmat);
	PlayerModel->SetMaterial(2, bandanamat);

	CapsuleComponent->SetPhysMaterialOverride(physmat);

	PostProcess->bUnbound = true;
}

UFUNCTION()
void AAuyron::UnHit(class UPrimitiveComponent* thisguy, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
		if (OtherActor->IsA(ATwoDimensionalMovementRegion::StaticClass())) {
			InCameraOverrideRegion = false;
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

UFUNCTION()
void AAuyron::Hit(class UPrimitiveComponent* thisguy, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// Stop hitting yourself.
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
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
			LockedAxisValue = ((ATwoDimensionalMovementRegion*)OtherActor)->LockedCoordinate;//
			LockedMovementAxis = ((ATwoDimensionalMovementRegion*)OtherActor)->LockedAxis;
			newspline = ((ATwoDimensionalMovementRegion*)OtherActor)->path;
			InCameraOverrideRegion = true;
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
	
	TeleClaw->AttachToComponent(PlayerModel, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "RightHand");
	BootsR->AttachToComponent(PlayerModel, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WHATARETHOSE_R");
	BootsL->AttachToComponent(PlayerModel, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WHATARETHOSE_L");
	Bracelet->AttachToComponent(PlayerModel, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Bracelet");
	Belt->AttachToComponent(PlayerModel, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Belt");
	Wings->AttachToComponent(PlayerModel, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Wings");

	//TeleClaw->AttachTo(PlayerModel, "RightHand");
	//BootsR->AttachTo(PlayerModel, "WHATARETHOSE_R");
	//BootsL->AttachTo(PlayerModel, "WHATARETHOSE_L");
	//Bracelet->AttachTo(PlayerModel, "Bracelet");
	//Belt->AttachTo(PlayerModel, "Belt");
	//Wings->AttachTo(PlayerModel,  "Wings");

	//((APlayerController*)GetController())->SetAudioListenerOverride(PlayerModel, FVector::ZeroVector, FRotator::ZeroRotator);

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
	CurrentState = &Normal;
}

void AAuyron::whywhy() {
	Capture2D->GetCaptureComponent2D()->bCaptureEveryFrame = false;
}

// Called every frame UNLIKE UNITY MIRITE?
void AAuyron::Tick(float DeltaTime)
{

	// 3x^2 - 2x^3

	Super::Tick(DeltaTime);

	CurrentState->Tick(this, DeltaTime);

	return;




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
	InputComponent->BindAxis("ControllerCameraPitch", this, &AAuyron::ControllerPitchCamera);
	InputComponent->BindAxis("ControllerCameraYaw", this, &AAuyron::ControllerYawCamera);
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
	if (FMath::Abs(AxisValue) > 0.67) {
		MovementInput.X = FMath::Sign(AxisValue);
	}
	MovementInput.X = AxisValue;
}

// and X for vertical? I mean who does that?
void AAuyron::MoveForward(float AxisValue)
{
	float sqr = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f)*FMath::Abs(FMath::Clamp<float>(AxisValue, -1.0f, 1.0f));
	MovementInput.Y = FMath::Lerp(FMath::Clamp<float>(AxisValue, -1.0f, 1.0f), sqr, FMath::Abs(FMath::Clamp<float>(AxisValue, -1.0f, 1.0f)));
	if (FMath::Abs(AxisValue) > 0.67) {
		MovementInput.Y = FMath::Sign(AxisValue);
	}
	MovementInput.Y = AxisValue;
}

void AAuyron::PitchCamera(float AxisValue)
{
	CameraMouseInput.Y = AxisValue;
	if(CameraInput.Y != 0.0f) {
		return;
	}
	if (((CurrentState == &Aiming) && YAxisAimingStyle == INVERTED) || (!(CurrentState == &Aiming) && YAxisStyle == INVERTED)) {
		AxisValue *= -1;
	}
	CameraInput.Y = AxisValue;
}

void AAuyron::YawCamera(float AxisValue)
{
	CameraMouseInput.X = AxisValue;
	if (CameraInput.X != 0.0f) {
		return;
	}
	if (((CurrentState == &Aiming) && XAxisAimingStyle == INVERTED) || (!(CurrentState == &Aiming) && XAxisStyle == INVERTED)) {
		AxisValue *= -1;
	}
	CameraInput.X = AxisValue;
}

void AAuyron::ControllerPitchCamera(float AxisValue)
{
	if (((CurrentState == &Aiming) && YAxisAimingStyle == INVERTED) || (!(CurrentState == &Aiming) && YAxisStyle == INVERTED)) {
		AxisValue *= -1;
	}
	float sqr = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f)*FMath::Abs(FMath::Clamp<float>(AxisValue, -1.0f, 1.0f));
	CameraControllerInput.Y = FMath::Lerp(FMath::Clamp<float>(AxisValue, -1.0f, 1.0f), sqr, FMath::Abs(FMath::Clamp<float>(AxisValue, -1.0f, 1.0f)));
	if (CameraInput.Y != 0.0f) {
		return;
	}
	CameraInput.Y = FMath::Lerp(FMath::Clamp<float>(AxisValue, -1.0f, 1.0f), sqr, FMath::Abs(FMath::Clamp<float>(AxisValue, -1.0f, 1.0f)));
}

void AAuyron::ControllerYawCamera(float AxisValue)
{
	if (((CurrentState == &Aiming) && XAxisAimingStyle == STANDARD) || (!(CurrentState == &Aiming) && XAxisStyle == INVERTED)) {
		AxisValue *= -1;
	}
	float sqr = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f)*FMath::Abs(FMath::Clamp<float>(AxisValue, -1.0f, 1.0f));
	CameraControllerInput.X = FMath::Lerp(FMath::Clamp<float>(AxisValue, -1.0f, 1.0f), sqr, FMath::Abs(FMath::Clamp<float>(AxisValue, -1.0f, 1.0f)));
	if (CameraInput.X != 0.0f) {
		return;
	}
	CameraInput.X = FMath::Lerp(FMath::Clamp<float>(AxisValue, -1.0f, 1.0f), sqr, FMath::Abs(FMath::Clamp<float>(AxisValue, -1.0f, 1.0f)));
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
		JumpPressed = true;
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
	ActivateNextFrame = true;
}

// HEY LINK TALK TO ME USING Z TARGETING
void AAuyron::CameraFaceForward()
{
	//if (!dashing&&!MovementAxisLocked&&TeleportSettings.HasTeleport&&(CurrentState == &Normal)&&GetWorldTimerManager().GetTimerElapsed(PreWarpTimer)==-1.0f) {
		if (AimStyle == TOGGLE) {
			ztarget = !ztarget;
		} else {
			ztarget = true;
		}
	//}
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
	if (!dashing&&!(CurrentState == &Aiming)) {
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
	bool justno = !(CurrentState == &Aiming) && (CurrentState != &Dialogue) && !cameralocked && !InCameraOverrideRegion && GetWorldTimerManager().GetTimerElapsed(PreWarpTimer) == -1.0f;
	if (TargetDefaultArmLength > MinimumArmLength && justno) {
		if (TargetDefaultArmLength < ActualDefaultArmLength) {
			CameraLagSettings.CameraLag *= CameraLagZoomScale;
		}
		TargetDefaultArmLength -= CameraZoomStep;
	}
}

void AAuyron::CameraZoomOut() {
	bool justno = !(CurrentState == &Aiming) && (CurrentState != &Dialogue) && !cameralocked && !InCameraOverrideRegion && GetWorldTimerManager().GetTimerElapsed(PreWarpTimer) == -1.0f;
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
	return SpeedRelativeToGround;
}

FVector AAuyron::GetPlayerVelocity()
{
	return (CapsuleComponent->GetPhysicsLinearVelocity() - MovementComponent->groundvelocity);
}

float AAuyron::GetActualSpeed()
{
	//return (CapsuleComponent->GetPhysicsLinearVelocity() - MovementComponent->groundvelocity).Size();
	return SpeedRelativeToGround;
	return (FVector::VectorPlaneProject(CapsuleComponent->GetPhysicsLinearVelocity() + (MovementComponent->groundvelocity - previousgroundvelocity), FVector::UpVector)).Size();
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
	return (CurrentState == &Aiming);
}

bool AAuyron::GetIsOnTheGround()
{
	return OnTheGround;
}

bool AAuyron::GetIsDashing()
{
	return dashing;
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

void AAuyron::SetLightMin(float newvalue)
{
	CelShaderSettings.LightMin = newvalue;
}

void AAuyron::SetLightMax(float newvalue)
{
	CelShaderSettings.LightMax = newvalue;
}

void AAuyron::SetAdditiveLightBias(float newvalue)
{
	CelShaderSettings.AdditiveLightBias = newvalue;
}

void AAuyron::SetMultiplicativeLightBias(float newvalue)
{
	CelShaderSettings.MultiplicativeLightBias = newvalue;
}

void AAuyron::SetTint(FLinearColor newvalue)
{
	CelShaderSettings.Tint = newvalue;
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
	return (CurrentState == &Climbing);
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
	return (CurrentState == &Dialogue) && !CurrentCut->NoText;
}

float AAuyron::GetWarpTimerCompleted()
{
	if (GetWorldTimerManager().GetTimerElapsed(PreWarpTimer) == -1.0f) {
		return -1.0f;
	}
	return GetWorldTimerManager().GetTimerElapsed(PreWarpTimer)/TeleportSettings.TeleportAnimationDuration;
}

FString AAuyron::GetDialogueText()
{
	return CurrentLine;
}

float AAuyron::GetDialogueWidth()
{
	return CurrentTextWidth;
}

USkeletalMeshComponent* AAuyron::GetMesh() {
	return PlayerModel;
}

void AAuyron::PlayerState::Tick(AAuyron* Player, float DeltaTime)
{
	Player->SpeedRelativeToGround = (FVector::VectorPlaneProject(Player->CapsuleComponent->GetPhysicsLinearVelocity() - Player->MovementComponent->groundvelocity, FVector::UpVector)).Size();
	Player->WasOnTheGround = Player->OnTheGround;

	// The definitions of "Right" and "Forward" depend on the direction that the camera's facing.
	Player->Right = Player->Camera->GetRightVector();
	Player->Right.Z = 0.0f;
	Player->Right = Player->Right.GetSafeNormal();
	Player->Forward = Player->Camera->GetForwardVector();
	Player->Forward.Z = 0.0f;
	Player->Forward = Player->Forward.GetSafeNormal();

	Player->Capture2D->GetCaptureComponent2D()->Deactivate();

	Player->TimeSinceLastMovementInputReleased += DeltaTime;
	if (!Player->justlandedcameraflag && (Player->MovementInput.IsNearlyZero() || !Player->OnTheGround)) {
		Player->TimeSinceLastMovementInputReleased = 0.0f;
	}

	if (!Player->CameraInput.IsNearlyZero() || FMath::Abs(Player->SpringArm->RelativeRotation.Pitch - Player->CameraAutoTurnSettings.CameraDefaultPitch) < 2.0f) {
		Player->justlandedcameraflag = false;
	}


	FRootMotionMovementParams RootMotion = Player->PlayerModel->ConsumeRootMotion();

	if (RootMotion.bHasRootMotion)
	{
		FTransform WorldRootMotion = Player->PlayerModel->ConvertLocalRootMotionToWorld(RootMotion.RootMotionTransform);
		Player->CapsuleComponent->SetWorldLocation(Player->CapsuleComponent->GetComponentLocation() + WorldRootMotion.GetTranslation(), false, NULL, ETeleportType::TeleportPhysics);
	}

	// This isn't Doom.
	Player->MovementInput = Player->MovementInput.GetClampedToMaxSize(1.0f);

	// Xbox controller a shit.
	if (Player->MovementInput.Size() > 0.9f) {
		Player->MovementInput = Player->MovementInput.GetSafeNormal();
	}

	// Apply a curve to the movement input that allows the player able to walk/move slowly without having to edge the deadzone.
	float sqr = FMath::Clamp<float>(Player->MovementInput.Size(), -1.0f, 1.0f)*FMath::Abs(FMath::Clamp<float>(Player->MovementInput.Size(), -1.0f, 1.0f));
	Player->MovementInput = Player->MovementInput.GetSafeNormal()*FMath::Lerp(FMath::Clamp<float>(Player->MovementInput.Size(), -1.0f, 1.0f), sqr, FMath::Abs(FMath::Clamp<float>(Player->MovementInput.Size(), -1.0f, 1.0f)));

	// Now how did you get down there?
	if (Player->GetActorLocation().Z < -20000.0f) {
		Player->Respawn();
	}

	if (Player->isclimbing) {
		Player->movementlocked = true;
		Player->JumpPressed = false;
		Player->ActivateNextFrame = false;
		Player->SlamNextFrame = false;
		Player->GlideNextFrame = false;
		Player->ztarget = false;
	}

	if (Player->ingrass && Player->OnTheGround) {
		if (!Player->grassactive) {
			Player->grassparticles->ActivateSystem();
		}
		Player->grassactive = true;
	} else {
		Player->grassactive = false;
		Player->grassparticles->DeactivateSystem();
	}

	Player->JustWallJumped = false;

	FLinearColor tintmult = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	for (TActorIterator<APointLight> ActorItr(Player->GetWorld()); ActorItr; ++ActorItr) {
		APointLight* a = ((APointLight*)ActorItr.operator->());
		float ar = a->PointLightComponent->AttenuationRadius;
		FLinearColor col = a->GetLightComponent()->LightColor;
		if ((a->GetActorLocation() - Player->GetActorLocation()).Size() < ar) {
			tintmult = FMath::Lerp(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), col, (1 - (a->GetActorLocation() - Player->GetActorLocation()).Size() / ar));
		}
	}

	FHitResult d;
	Player->teletestmat->SetScalarParameterValue("t", Player->GetWorld()->GetTimerManager().GetTimerElapsed(Player->PreWarpTimer) / Player->TeleportSettings.TeleportAnimationDuration);
	Player->outlinemat->SetScalarParameterValue("Nope", Player->GetWorld()->LineTraceSingleByChannel(d, Player->Camera->GetComponentLocation(), Player->GetActorLocation(), ECC_Visibility));
	Player->DashParticles->SetVectorParameter("WingColor", (Player->Wings->GetMaterial(0) == Player->bluewings ? FVector(2.0f, 26.0f, 30.0f) : FVector(5.0f, 5.0f, 25.0f)));
	Player->FloatParticles->SetVectorParameter("WingColor", (Player->Wings->GetMaterial(0) == Player->bluewings ? FVector(2.0f, 26.0f, 30.0f) : FVector(5.0f, 5.0f, 25.0f)));
	Player->celshadermat->SetScalarParameterValue("Light Min", Player->CelShaderSettings.LightMin);
	Player->celshadermat->SetScalarParameterValue("Light Max", Player->CelShaderSettings.LightMax);
	Player->celshadermat->SetScalarParameterValue("Additive Light Bias", Player->CelShaderSettings.AdditiveLightBias);
	Player->celshadermat->SetScalarParameterValue("Multiplicative Light Bias", Player->CelShaderSettings.MultiplicativeLightBias);
	Player->celshadermat->SetVectorParameterValue("Tint", Player->CelShaderSettings.Tint*tintmult);

	Player->DropShadow->SetWorldLocation(Player->MovementComponent->groundtracehit);

	if (Player->MovementAxisLocked) {
		//currentoverrideregion->Axis->SetWorldRotation(newspline->FindRightVectorClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World).Rotation());
	}

	// A blueprint is overriding player input.
	if (Player->blockedbyblueprint) {
		Player->movementlocked = true;
		Player->cameralocked = true;
	}

	// Going somewhere?
	if (Player->movementlocked) {
		Player->MovementInput = FVector::ZeroVector;

		// Lock jumping and gliding as well unless we're dashing.
		if (!Player->dashing) {
			Player->JumpPressed = false;
			Player->GlideNextFrame = false;
		}
	}

	// Nullify camera input if camera is locked.
	if (Player->cameralocked) {
		Player->CameraInput = FVector::ZeroVector;
	}

	// Nullify movement input if dashing with no dash control.
	if (Player->dashing && !Player->DashSettings.HasDashControl) {
		Player->MovementInput = FVector::ZeroVector;
	}

	// Update OnTheGround state.
	Player->OnTheGround = Player->MovementComponent->onground;

	//DropShadow->bVisible = !OnTheGround;

	// Temporarily remove camera lag if player is teleported by some
	// mechanism other than the TeleClaw.
	if (Player->justteleported && !Player->justswished) {
		Player->SpringArm->CameraLagMaxDistance = 1.0f;
	} else {
		Player->SpringArm->CameraLagMaxDistance = 0.0f;
	}

	// Allow for smooth camera zooming.
	Player->DefaultArmLength = FMath::Lerp(Player->DefaultArmLength, Player->TargetDefaultArmLength, Player->CameraZoomRate);

	// If there is a sharp change in the velocity of the platform that the player is
	// standing on, immediately snap the player's velocity to match it. 
	if (!Player->MovementComponent->groundvelocity.IsNearlyZero() && !Player->JumpPressed && FMath::Abs((FVector::VectorPlaneProject(Player->MovementComponent->groundvelocity, FVector::UpVector) - FVector::VectorPlaneProject(Player->previousgroundvelocity, FVector::UpVector)).Size()) > 200.0f) {
		Player->CapsuleComponent->SetPhysicsLinearVelocity(Player->MovementComponent->groundvelocity);
	}

	// Inform the MovementComponent of our velocity.
	Player->MovementComponent->PlayerVelocity = Player->CapsuleComponent->GetPhysicsLinearVelocity();
	Player->MovementComponent->forceregiondirection = Player->AppliedForce;
	Player->MovementComponent->isclimbing = Player->isclimbing;

	// Set our frame of reference for future calculations to be that of the surface we're standing on.

	// Make the equipment we have visible on the player model.
	Player->TeleClaw->SetVisibility(Player->TeleportSettings.HasTeleport);
	Player->BootsR->SetVisibility(Player->DashSettings.HasDash);
	Player->BootsL->SetVisibility(Player->DashSettings.HasDash);
	Player->Bracelet->SetVisibility(Player->SlamSettings.HasSlam);
	Player->Belt->SetVisibility(Player->JumpSettings.HasWallJump);
	Player->Wings->SetVisibility(Player->GlideSettings.HasGlide);
}
void AAuyron::PlayerState::Tick2(AAuyron* Player, float DeltaTime)
{
	{
		Player->WasInCameraOverrideRegion = Player->InCameraOverrideRegion;
		Player->previousposition = Player->GetActorLocation();
		Player->wasztarget = (Player->CurrentState == &Player->Aiming);
		Player->previousvelocity = Player->CapsuleComponent->GetPhysicsLinearVelocity();
		Player->previousgroundvelocity = Player->MovementComponent->groundvelocity;

		Player->pushvelocity = FVector::ZeroVector;
		Player->CameraInput = FVector::ZeroVector;

		Player->JumpPressed = false;
		Player->ActivateNextFrame = false;
		Player->SlamNextFrame = false;
		Player->DashNextFrame = false;
		Player->GlideNextFrame = false;
		Player->swish = false;

		Player->lastdt = DeltaTime;

		Player->justswished = false;
		Player->justteleported = false;
		Player->JustJumped = false;
	}
}
void AAuyron::PlayerState::PhysicsStuff(AAuyron * Player, float DeltaTime)
{
	// Get horizontal velocity projection.
	FVector Velocity2D = FVector::VectorPlaneProject(Player->CapsuleComponent->GetPhysicsLinearVelocity(), FVector::UpVector);

	// Adjust movement input to reflect camera direction.
	FVector AdjustedMovementInput = (Player->Right*Player->MovementInput.X + Player->Forward*Player->MovementInput.Y);// .GetClampedToMaxSize(1.0f);

	if (Player->MovementAxisLocked) {
		FVector tempvector(Player->MovementInput);
		FVector f = Player->newspline->FindTangentClosestToWorldLocation(Player->GetActorLocation(), ESplineCoordinateSpace::World).GetSafeNormal();
		tempvector = FMath::Sign(Player->Right | f) * f * Player->MovementInput.X;
		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, tempvector.ToString());
		AdjustedMovementInput = tempvector;
	}

	// Get acceleration magnitude depending if the player is in the air or not.
	float CurrentAccelRate = (Player->OnTheGround ? Player->PhysicsSettings.GroundAccelerationRate : Player->PhysicsSettings.AirAccelerationRate) * 10000.0f;

	// Apply acceleration based on movement inputs.
	if (!Player->dashing) {
		Player->CapsuleComponent->AddForce(AdjustedMovementInput * CurrentAccelRate, NAME_None, false);
	}

	// If the platform we're standing on is accelerating, add that acceleration to the player's acceleration,
	// but only if the player didn't just jump onto or off of the platform, and the platform didn't just
	// quickly and immediately change directions.
	//if (!(previousgroundvelocity.IsNearlyZero() && !MovementComponent->groundvelocity.IsNearlyZero()) &&
	//	!(!previousgroundvelocity.IsNearlyZero() && MovementComponent->groundvelocity.IsNearlyZero()) &&
	//	(MovementComponent->groundvelocity - previousgroundvelocity).Size() / DeltaTime < 1000.0f) {
	if ((Player->MovementComponent->groundvelocity - Player->previousgroundvelocity).Size() / DeltaTime < 10000.0f) {
		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, (MovementComponent->groundvelocity - previousgroundvelocity).ToString()+"             "+(CapsuleComponent->GetPhysicsLinearVelocity()-previousvelocity).ToString());
		Player->CapsuleComponent->AddImpulse((Player->MovementComponent->groundvelocity - Player->previousgroundvelocity), NAME_None, true);
		//SetActorLocation(GetActorLocation() + (MovementComponent->groundvelocity - previousgroundvelocity)*DeltaTime,false,nullptr,ETeleportType::TeleportPhysics);
	}

	// Apply gravity if in the air, and stop vertical movement if on the ground.
	if (!Player->OnTheGround && Player->GetWorldTimerManager().GetTimerElapsed(Player->PreWarpTimer) == -1.0f && !Player->airdashing) {
		Player->CapsuleComponent->AddForce(Player->PhysicsSettings.Gravity*FVector::UpVector, NAME_None, true);
	} else {
		Player->FlattenVelocity();
	}

	// Apply drag.
	if (!Player->OnTheGround && FMath::Abs(Player->CapsuleComponent->GetPhysicsLinearVelocity().Z) > Player->JumpSettings.JumpPower) {
		float mult = ((Player->CapsuleComponent->GetPhysicsLinearVelocity().Z > 0.0f && (Player->AppliedForce.Z > 0.0f || Player->IsGliding)) ? 16.0f : 1.0f);
		Player->CapsuleComponent->AddForce(FVector::UpVector * Player->PhysicsSettings.Gravity * FMath::Pow(Player->CapsuleComponent->GetPhysicsLinearVelocity().Z / Player->PhysicsSettings.TerminalVelocity, 2.0f) * FMath::Sign(Player->CapsuleComponent->GetPhysicsLinearVelocity().Z)*200.0f*mult);
	}

	// Handle force regions.
	Player->CapsuleComponent->AddForce(Player->AppliedForce*200.0f, NAME_None, false);
	if (Player->AppliedForce.Z > 0.0f) {
		Player->AlreadyUnjumped = true;
		Player->OnTheGround = false;
	}

	// Apply a deceleration that scales with the player's velocity
	// in such a way that it limits it to MaxVelocity.
	if (!Player->dashing) {
		float mult = FMath::Lerp(FMath::Sqrt(Velocity2D.Size() / Player->PhysicsSettings.MaxVelocity), Velocity2D.Size() / Player->PhysicsSettings.MaxVelocity, Velocity2D.Size() / Player->PhysicsSettings.MaxVelocity);
		Player->CapsuleComponent->AddForce(-Velocity2D.GetSafeNormal()*CurrentAccelRate*mult, NAME_None, false);
	}

	// This is a 2D platformer now.
	if (Player->MovementAxisLocked) {
		FVector newpos(Player->GetActorLocation());
		newpos = Player->newspline->FindLocationClosestToWorldLocation(Player->GetActorLocation(), ESplineCoordinateSpace::World);
		newpos = FVector::VectorPlaneProject(newpos, Player->newspline->FindUpVectorClosestToWorldLocation(Player->GetActorLocation(), ESplineCoordinateSpace::World));
		newpos += (Player->GetActorLocation() | Player->newspline->FindUpVectorClosestToWorldLocation(Player->GetActorLocation(), ESplineCoordinateSpace::World))*Player->newspline->FindUpVectorClosestToWorldLocation(Player->GetActorLocation(), ESplineCoordinateSpace::World);
		Player->SetActorLocation(newpos);
	}
	Player->CapsuleComponent->SetPhysicsLinearVelocity(Player->CapsuleComponent->GetPhysicsLinearVelocity() + Player->MovementComponent->groundvelocity); // + (MovementComponent->groundvelocity + pushvelocity)
}
void AAuyron::PlayerState::CameraStuff(AAuyron * Player, float DeltaTime)
{

	// Handle camera movement when the camera is controllable.
	if ((!Player->InCameraOverrideRegion) && Player->GetWorldTimerManager().GetTimerElapsed(Player->PreWarpTimer) == -1.0f) {
		Player->timesinceoverrideenter = 0.0f;
		Player->SpringArm->bDoCollisionTest = true;

		// Reset camera input timer if the camera controls were touched.
		if (Player->CameraInput.X != 0.0f || Player->CameraInput.Y != 0.0f) {
			Player->TimeSinceLastMouseInput = 0.0f;
		} else {
			Player->TimeSinceLastMouseInput += DeltaTime;
		}

		if (Player->CapsuleComponent->GetPhysicsLinearVelocity().Z < -900.0f && Player->TimeSinceLastMouseInput > Player->CameraAutoTurnSettings.CameraResetTime) {
			Player->CameraInput.Y = -FMath::Clamp(-0.0005f*Player->CapsuleComponent->GetPhysicsLinearVelocity().Z, 0.0f, 1.0f);
		}

		// Temporary variable to hold the camera's new rotation.
		//FRotator NewRotation = SpringArm->GetComponentRotation();
		FRotator NewRotation = FRotator::ZeroRotator;

		// Move the camera's yaw in response to the x input of the mouse/stick.
		NewRotation.Yaw += (DeltaTime*120.0f)*Player->CameraInput.X;

		// The camera should only turn with the player if the mouse hasn't been touched recently.
		if (Player->TimeSinceLastMouseInput > Player->CameraAutoTurnSettings.CameraResetTime && !Player->movementlocked) {
			NewRotation.Yaw += FMath::Pow(FMath::Abs(Player->MovementInput.X), 1.0f) * (Player->Camera->GetRightVector().GetSafeNormal() | FVector::VectorPlaneProject(Player->CapsuleComponent->GetPhysicsLinearVelocity(), FVector::UpVector) / Player->PhysicsSettings.MaxVelocity) * DeltaTime * Player->CameraAutoTurnSettings.CameraAutoTurnFactor;
			float thing = FMath::Pow(FMath::Abs(Player->MovementInput.X), 1.0f) * (Player->Camera->GetRightVector().GetSafeNormal() | FVector::VectorPlaneProject(Player->CapsuleComponent->GetPhysicsLinearVelocity(), FVector::UpVector) / Player->PhysicsSettings.MaxVelocity) * DeltaTime * Player->CameraAutoTurnSettings.CameraAutoTurnFactor;

			if (Player->TimeSinceLastMouseInput > Player->CameraAutoTurnSettings.CameraResetTime) {
				FVector camf = Player->Camera->GetForwardVector();
				FVector camu = Player->Camera->GetUpVector();
				int imax = 100;
				float finalr = 0.0f;
				float numhits = 0.0f;
				float numl = 0.0f;
				float numr = 0.0f;
				float angle = 15.0f;
				for (int i = 0; i < imax; i++) {
					float asdf = -angle + i*(2.0f * angle / (imax - 1));
					FVector testdir = camf.RotateAngleAxis(asdf, camu).GetSafeNormal();
					FHitResult camhit;
					float modifieddot = 3 * FMath::Pow(2.0f*(camf | testdir) - 1.0f, 2) - 2 * FMath::Pow(2.0f*(camf | testdir) - 1.0f, 3);
					Player->GetWorld()->LineTraceSingleByChannel(camhit, Player->Camera->GetComponentLocation(), Player->Camera->GetComponentLocation() + modifieddot * (((Player->Camera->GetComponentLocation() - Player->GetActorLocation()).Size() - 90.0f)*testdir), ECC_Camera);
					if (camhit.IsValidBlockingHit() && camhit.ImpactNormal.Z < 0.3f) {
						float dot = FMath::Clamp((camhit.ImpactPoint - Player->Camera->GetComponentLocation()).GetSafeNormal() | Player->Camera->GetForwardVector(), 0.0f, 1.0f);
						float dist = (camhit.ImpactPoint - Player->Camera->GetComponentLocation()).Size();
						float sign = FMath::Sign(asdf);
						float scale = 10000.0f;
						//float thing = FMath::Lerp(FMath::Pow(dot, 2), FMath::Pow(dot, 0.5f), dot);
						float thing = 3 * FMath::Pow(dot, 2) - 2 * FMath::Pow(dot, 3);
						finalr += scale*sign*thing*(1.0f / dist)*(1.0f / imax);
						numhits++;
						if (sign < 0) {
							numl++;
						} else {
							numr++;
						}
					}
				}
				finalr *= FMath::Pow(FMath::Abs(numl - numr) / numhits, 2);
				if (FMath::Sign(finalr) == FMath::Sign(thing)) {
					Player->SpringArm->AddRelativeRotation(FRotator(0.0f, finalr*FMath::Pow(FMath::Abs(thing), 0.5f), 0.0f));
				}
			}
			if (Player->TimeSinceLastMovementInputReleased > Player->CameraAutoTurnSettings.CameraResetTime) {
				//float modifiedpitch = CameraAutoTurnSettings.CameraDefaultPitch - 50.0f*(MovementComponent->FloorNormal | TargetDirection.Vector());
				float modifiedpitch = Player->CameraAutoTurnSettings.CameraDefaultPitch - 50.0f*(Player->MovementComponent->FloorNormal | FVector::VectorPlaneProject(Player->Camera->GetForwardVector(), FVector::UpVector).GetSafeNormal());
				//NewRotation.Pitch += FMath::Lerp(SpringArm->RelativeRotation.Pitch, modifiedpitch, CameraAutoTurnSettings.CameraDefaultPitchRate);
			}
			if ((Player->OnTheGround && !Player->WasOnTheGround&&Player->TimeSinceLastMouseInput > Player->CameraAutoTurnSettings.CameraResetTime)) {
				Player->justlandedcameraflag = true;
				Player->TimeSinceLastMovementInputReleased = Player->CameraAutoTurnSettings.CameraResetTime + DeltaTime;
			}
		}

		// Set the rotation of the camera.
		Player->SpringArm->SetWorldRotation(Player->SpringArm->GetComponentRotation() + NewRotation);

		// Move the camera's pitch in response to the y input of the mouse/stick.
		//NewRotation = SpringArm->GetComponentRotation();
		NewRotation = FRotator::ZeroRotator;
		float slowfactor = 1.0f;
		if (Player->CameraInput.Y < 0.0f && Player->CameraControllerInput.Y < 0.0f && Player->SpringArm->RelativeRotation.Pitch < 0.0f) {
			slowfactor = FMath::Pow(1.0f - (Player->SpringArm->RelativeRotation.Pitch / (-Player->CameraMaxAngle)), 1.0f);
		}

		NewRotation.Pitch = FMath::Clamp(Player->SpringArm->GetComponentRotation().Pitch + (DeltaTime*120.0f)*slowfactor*Player->CameraInput.Y, -Player->CameraMaxAngle, -Player->CameraMinAngle);
		if (Player->TimeSinceLastMouseInput > Player->CameraAutoTurnSettings.CameraResetTime && !Player->movementlocked) {
			FVector f = FVector::VectorPlaneProject(Player->Camera->GetForwardVector(), FVector::UpVector).GetSafeNormal();
			FHitResult camhit;//
			Player->GetWorld()->LineTraceSingleByChannel(camhit, Player->Camera->GetComponentLocation() + 40.0f*f, Player->Camera->GetComponentLocation() - 80.0f*f, ECC_Camera);
			////////////////////////////////
			if (false && camhit.IsValidBlockingHit()) {
				float buttcast = (camhit.Normal | f)*60.0f;
				buttcast = FMath::Clamp(60.0f*((Player->SpringArm->TargetArmLength / (Player->GetActorLocation() - Player->Camera->GetComponentLocation()).Size()) - 1), -Player->CameraAutoTurnSettings.CameraDefaultPitch, 82.0f);
				NewRotation.Pitch = FMath::Clamp(FMath::Lerp(Player->SpringArm->RelativeRotation.Pitch, -buttcast, 1), -90.0f, 90.0f);
			} else {
				if (Player->TimeSinceLastMovementInputReleased > Player->CameraAutoTurnSettings.CameraResetTime) {
					//float modifiedpitch = CameraAutoTurnSettings.CameraDefaultPitch - 50.0f*(MovementComponent->FloorNormal | TargetDirection.Vector());


					float modifiedpitch = Player->CameraAutoTurnSettings.CameraDefaultPitch - 50.0f*(Player->MovementComponent->FloorNormal | FVector::VectorPlaneProject(Player->Camera->GetForwardVector(), FVector::UpVector).GetSafeNormal());
					NewRotation.Pitch = FMath::Lerp(Player->SpringArm->RelativeRotation.Pitch, modifiedpitch, DeltaTime*Player->CameraAutoTurnSettings.CameraDefaultPitchRate);
				}
			}
			////////////////////////////
			if (Player->TimeSinceLastMovementInputReleased > Player->CameraAutoTurnSettings.CameraResetTime) {
				//float modifiedpitch = CameraAutoTurnSettings.CameraDefaultPitch - 50.0f*(MovementComponent->FloorNormal | TargetDirection.Vector());
				float modifiedpitch = Player->CameraAutoTurnSettings.CameraDefaultPitch - 50.0f*(Player->MovementComponent->FloorNormal | FVector::VectorPlaneProject(Player->Camera->GetForwardVector(), FVector::UpVector).GetSafeNormal());
				NewRotation.Pitch = FMath::Lerp(Player->SpringArm->RelativeRotation.Pitch, modifiedpitch, DeltaTime*Player->CameraAutoTurnSettings.CameraDefaultPitchRate);
			}
		}
		Player->SpringArm->CameraRotationLagSpeed = Player->CameraLagSettings.CameraRotationLag;
		FRotator temp = Player->SpringArm->RelativeRotation;
		temp.Pitch = 0.0f;
		Player->SpringArm->SetWorldRotation(temp + NewRotation);

	}

	// Camera raycast shenanigans.

	// Handle CameraOverrideRegions.
	if (Player->InCameraOverrideRegion) {
		if (Player->MovementAxisLocked || Player->currentoverrideregion->UseSpline) {

			if (!Player->MovementAxisLocked&&Player->currentoverrideregion->UseSpline) {
				Player->newspline = Player->currentoverrideregion->path;
			}

			if (!Player->wasztarget) {
				Player->SpringArm->CameraLagSpeed = Player->ActualDefaultCameraLag;
				Player->SpringArm->CameraRotationLagSpeed = Player->ActualDefaultCameraLag;
			}
			Player->SpringArm->TargetArmLength = Player->DefaultArmLength;
			Player->SpringArm->SetWorldRotation(Player->newspline->FindRightVectorClosestToWorldLocation(Player->GetActorLocation(), ESplineCoordinateSpace::World).Rotation().Quaternion());
			Player->timesinceoverrideenter += DeltaTime;
			Player->SpringArm->bDoCollisionTest = false;

			FVector camerarelativedisplacement = (Player->GetActorLocation() - 600.0f*Player->newspline->FindRightVectorClosestToWorldLocation(Player->GetActorLocation(), ESplineCoordinateSpace::World).GetSafeNormal());
			//SpringArm->SetWorldLocation(FVector::VectorPlaneProject(camerarelativedisplacement, -newspline->FindRightVectorClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World).GetSafeNormal()));
			Player->SpringArm->SetWorldLocation(camerarelativedisplacement);
		} else {
			if (!Player->wasztarget) {
				Player->SpringArm->CameraLagSpeed = Player->ActualDefaultCameraLag;
				Player->SpringArm->CameraRotationLagSpeed = Player->ActualDefaultCameraLag;
			}
			Player->SpringArm->TargetArmLength = Player->DefaultArmLength;
			Player->SpringArm->SetWorldRotation(Player->currentoverrideregion->TargetCamera->GetForwardVector().Rotation().Quaternion());
			Player->timesinceoverrideenter += DeltaTime;
			Player->SpringArm->bDoCollisionTest = false;

			if (Player->currentoverrideregion->LockType == CameraLockType::POINT) {
				Player->SpringArm->SetWorldLocation(Player->currentoverrideregion->TargetCamera->GetComponentLocation());
				if (Player->currentoverrideregion->LookAtPlayer) {
					Player->SpringArm->SetRelativeRotation((-Player->SpringArm->RelativeLocation).Rotation());
				}
			}
			if (Player->currentoverrideregion->LockType == CameraLockType::AXIS) {
				FVector camerarelativedisplacement = (Player->GetActorLocation() - Player->currentoverrideregion->Axis->GetComponentLocation());
				Player->SpringArm->SetWorldLocation(Player->currentoverrideregion->TargetCamera->GetComponentLocation() + Player->currentoverrideregion->Axis->GetForwardVector().GetSafeNormal() * (Player->currentoverrideregion->Axis->GetForwardVector().GetSafeNormal() | camerarelativedisplacement));

			}
			if (Player->currentoverrideregion->LockType == CameraLockType::PLANE) {
				FVector camerarelativedisplacement = (Player->GetActorLocation() - Player->currentoverrideregion->Axis->GetComponentLocation());
				Player->SpringArm->SetWorldLocation(Player->currentoverrideregion->TargetCamera->GetComponentLocation() + FVector::VectorPlaneProject(camerarelativedisplacement, Player->currentoverrideregion->Axis->GetForwardVector().GetSafeNormal()));
			}
			if (Player->currentoverrideregion->HintRegion) {
				Player->InCameraOverrideRegion = false;
				Player->currentoverrideregion = nullptr;
			}
		}
	} else if (!Player->InCameraOverrideRegion) {
		Player->Camera->RelativeRotation.Roll = 0.0f;
		Player->SpringArm->RelativeRotation.Roll = 0.0f;
	}

	// Handle aiming.
	{

		// Set and range and angle to the "short range" values.
		Player->TeleportRange = Player->TeleportSettings.TeleportRangeWhenNotAiming;
		Player->TeleportAngleTolerance = Player->TeleportSettings.TeleportAngleToleranceWhenNotAiming;

		// Return the spring arm (and camera) to its original location.
		if (!Player->InCameraOverrideRegion) {
			if (!Player->justswished) {
				Player->SpringArm->CameraLagSpeed = Player->CameraLagSettings.CameraLag;
				Player->SpringArm->CameraRotationLagSpeed = Player->CameraLagSettings.CameraRotationLag;
			}
			Player->SpringArm->TargetArmLength = Player->DefaultArmLength;

			Player->SpringArm->SetRelativeLocation(FVector::ZeroVector);
			//Camera->bConstrainAspectRatio = false;

			// If we just stopped aiming, reset the camera's rotation as well.
			if (Player->wasztarget&&Player->GetWorldTimerManager().GetTimerElapsed(Player->PreWarpTimer) == -1.0f) {
				Player->TargetDefaultArmLength = Player->BackupDefaultArmLength;
				Player->SpringArm->SetRelativeRotation(FRotator(-30.0f, Player->SpringArm->GetComponentRotation().Yaw, 0.0f));
			}
		}

		// Re-enable the player's movement inputs.
		Player->movementlocked = false;
	}
}

void AAuyron::PlayerState::FaceTargetDirection(AAuyron* Player, float DeltaTime)
{

	// Handle rotating the player model in response to player input.

	// If the player is standing on a moving platform, they should rotate along with it.
	if (Player->MovementComponent->platformangularfrequency != 0.0f) {
		FQuat platformrotation = FQuat(FVector::UpVector, -Player->MovementComponent->platformspindir * Player->MovementComponent->platformangularfrequency * DeltaTime);
		Player->PlayerModel->AddLocalRotation(platformrotation);
		Player->TargetDirection.Yaw += platformrotation.Rotator().Yaw;
	}

	// If we're trying to move, take the camera's orientation into account to figure
	// out the direction we want to face.
	if (!Player->ztarget && (Player->OnTheGround || Player->IsGliding || Player->MovementAxisLocked)) {
		// I'ma tell ya not even Unity was stupid enough to use -180 -> 180 for rotations.
		int8 reflect = (Player->MovementInput.X >= 0 ? 1 : -1);

		// (but I forgive you)
		if (!Player->MovementInput.IsNearlyZero() && !Player->ztarget) {
			if (!Player->MovementAxisLocked || !Player->dashing) {
				Player->TargetDirection.Yaw = Player->SpringArm->GetComponentRotation().Yaw +
					reflect * FMath::RadiansToDegrees(FMath::Acos(Player->MovementInput.GetSafeNormal() | FVector(0, 1, 0)));
			}
			if (Player->MovementAxisLocked) {
				FVector tempvector(Player->TargetDirection.Vector());
				FVector f = Player->newspline->FindTangentClosestToWorldLocation(Player->GetActorLocation(), ESplineCoordinateSpace::World);
				tempvector = FMath::Sign(tempvector | f) * f;
				Player->TargetDirection = tempvector.Rotation();
			}
		}
	}

	// Turn to face the target direction, but do it all smooth like.
	{
		// Horrible quaternion voodoo. Viewer discretion is advised.
		// I'm honestly still not quite sure what I did.
		Player->TargetDirection.Roll = 0.0f;
		Player->TargetDirection.Pitch = 0.0f;
		FQuat test = FQuat::FindBetween(Player->PlayerModel->GetComponentRotation().Vector(), Player->TargetDirection.Vector());
		float angle = 0.0f;

		// We just need the angle between the vectors, not the axis (which should point upwards anyway).
		FVector dummy;
		test.ToAxisAndAngle(dummy, angle);

		// Turn faster if we're dashing.
		float multiply = (Player->dashing ? 3.0f : 1.0f);

		// Snap to the target angle if we're close enough, otherwise just keep turning.
		if (!Player->ztarget) {
			test = FQuat(dummy, FMath::DegreesToRadians(multiply*(Player->IsGliding ? 1.0f : Player->GlideSettings.GlideTurnRateMultiplier)*Player->TurnSettings.TurnRate)*DeltaTime);
			float angle2 = 0.0f;
			test.ToAxisAndAngle(dummy, angle2);
			if (FMath::Abs(angle2) > FMath::Abs(angle)) {
				Player->PlayerModel->SetWorldRotation(Player->TargetDirection);
			} else {
				Player->PlayerModel->AddLocalRotation(test);
			}
		}
	}
	// Like what even ARE quaternions anyway?

}

void AAuyron::NormalState::Tick(AAuyron * Player, float DeltaTime)
{
	PlayerState::Tick(Player, DeltaTime);

	if (Player->ztarget&&!Player->dashing&&!Player->dunk) {
		Player->CurrentState = &Player->Aiming;
		return;
	}

	Player->CapsuleComponent->SetPhysicsLinearVelocity(Player->CapsuleComponent->GetPhysicsLinearVelocity() - Player->MovementComponent->groundvelocity); // - (MovementComponent->groundvelocity + pushvelocity)

	 // Snap velocity to zero if it's really small.
	if (FVector::VectorPlaneProject(Player->CapsuleComponent->GetPhysicsLinearVelocity(), FVector::UpVector).Size() < Player->PhysicsSettings.MinVelocity && !Player->dashing && !Player->JumpPressed && Player->MovementInput.IsNearlyZero()) {
		float tempz = Player->CapsuleComponent->GetPhysicsLinearVelocity().Z;
	}

	{
		FCollisionShape LedgeFinderShape = FCollisionShape::MakeSphere(50.0f);// (55.0f, 90.0f);
		FCollisionObjectQueryParams QueryParams;
		QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);//
		QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		QueryParams.AddObjectTypesToQuery(ECC_Destructible);
		FCollisionQueryParams Params;
		FHitResult ShapeTraceResult;
		FVector head = Player->GetActorLocation() + 20.0f*FVector::UpVector;
		Player->GetWorld()->SweepSingleByChannel(ShapeTraceResult, head - 20.0f*Player->PlayerModel->GetForwardVector(), head + 50.0f*Player->PlayerModel->GetForwardVector(), FQuat::Identity, ECC_Visibility, LedgeFinderShape, Params);

		if (!Player->OnTheGround && Player->CapsuleComponent->GetPhysicsLinearVelocity().Z < 0.0f && ShapeTraceResult.IsValidBlockingHit()) {
			FVector WallNormal;
			FVector TraceHit;
			WallNormal = FVector::VectorPlaneProject(ShapeTraceResult.ImpactNormal, FVector::UpVector);
			TraceHit = ShapeTraceResult.ImpactPoint;
			FHitResult LedgeTraceResult;
			Player->GetWorld()->SweepSingleByChannel(LedgeTraceResult, TraceHit + 200.0f*FVector::UpVector, TraceHit, FQuat::Identity, ECC_Visibility, LedgeFinderShape, Params);
			FHitResult ceilhit;
			FCollisionShape CeilFinderShape = FCollisionShape::MakeSphere(20.0f);// (55.0f, 90.0f);
			Player->GetWorld()->SweepSingleByChannel(ceilhit, Player->GetActorLocation() + 65.0f*FVector::UpVector, Player->GetActorLocation() + 200.0f*FVector::UpVector, FQuat::Identity, ECC_Visibility, CeilFinderShape);
			bool underceiling = ceilhit.GetActor() != nullptr && ceilhit.GetComponent() != nullptr;// && ceilhit.GetComponent()->IsA(UStaticMeshComponent::StaticClass());
			FHitResult floorhit;
			Player->GetWorld()->SweepSingleByChannel(floorhit, Player->GetActorLocation() - 65.0f*FVector::UpVector, Player->GetActorLocation() - 200.0f*FVector::UpVector, FQuat::Identity, ECC_Visibility, CeilFinderShape);
			bool floorcheck = floorhit.GetActor() != nullptr && floorhit.GetComponent() != nullptr;// && floorhit.GetComponent()->IsA(UStaticMeshComponent::StaticClass());
			if (LedgeTraceResult.IsValidBlockingHit()) {
				if (LedgeTraceResult.GetActor() != nullptr && LedgeTraceResult.GetComponent() != nullptr && LedgeTraceResult.GetComponent()->IsA(UStaticMeshComponent::StaticClass())) {
					Player->grabbedledge = ((UStaticMeshComponent*)LedgeTraceResult.GetComponent());
					// The motion of a point on a rigid body is the combination of its motion about the center of mass...
					FVector angvel = FMath::DegreesToRadians(Player->grabbedledge->GetPhysicsAngularVelocity());
					FVector rr = LedgeTraceResult.ImpactPoint - (Player->grabbedledge->GetComponentLocation());
					FVector rvel = FVector::CrossProduct(angvel, rr);

					// ...and the motion of the center of mass itself.
					FVector cmvel = (Player->grabbedledge->GetPhysicsLinearVelocity());

					Player->ledgegroundvelocity = rvel + cmvel;
					Player->ledgeangularfrequency = -angvel.Z;
				} else {
					// BSP a shit
					Player->ledgegroundvelocity = FVector::ZeroVector;
					Player->ledgeangularfrequency = 0.0f;
				}

			}

			if (!Player->isclimbing && !underceiling && !floorcheck && !Player->dunk && !Player->airdashing && LedgeTraceResult.IsValidBlockingHit() && LedgeTraceResult.ImpactNormal.Z > 0.85f) {
				FVector LedgeTop = FVector(TraceHit.X, TraceHit.Y, LedgeTraceResult.ImpactPoint.Z);
				FVector NewPlayerLocation = LedgeTop + 45.0f*WallNormal;
				if (true) {
					Player->IsGliding = false;
					Player->dashing = false;
					Player->dunk = false;
					Player->JumpPressed = false;
					Player->TargetDirection = (-WallNormal).Rotation();
					Player->PlayerModel->SetWorldRotation(Player->TargetDirection);
					Player->SetActorLocation(NewPlayerLocation - 52.0f*FVector::UpVector);
					Player->ActivateNextFrame = false;
					Player->climblocation = LedgeTop;
					//Player->isclimbing = true;
					Player->CurrentState = &Player->Climbing;
					Player->PhysicsSettings.Gravity = 0.0f;
					Player->CapsuleComponent->SetPhysicsLinearVelocity(Player->ledgegroundvelocity);
					Player->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
					Player->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
					Player->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
					Player->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
					Player->GetWorld()->GetTimerManager().SetTimer(Player->climbtimer, Player, &AAuyron::StopClimbing, 1.667f / 2.0f);
				}
			}
		}
	}

	// Determine if the player is rubbing up against a wall and get the wall normal if they are.
	{
		FCollisionShape WallJumpCapsuleShape = FCollisionShape::MakeCapsule(55.0f, 90.0f);
		FCollisionObjectQueryParams QueryParams;
		QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);//
		QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		QueryParams.AddObjectTypesToQuery(ECC_Destructible);

		if (!Player->GetWorld()->OverlapAnyTestByObjectType(Player->GetActorLocation() - FVector::UpVector, FQuat::Identity, QueryParams, WallJumpCapsuleShape)) {
			Player->RidingWall = false;
			Player->StoredWallNormal = FVector::ZeroVector;
		}

		FCollisionQueryParams Params;

		// Telepads don't count.
		for (TActorIterator<AStick> ActorItr(Player->GetWorld()); ActorItr; ++ActorItr) {
			Params.AddIgnoredActor(ActorItr.operator->());
		}

		// Neither do destructibles.
		for (TActorIterator<ADestructibleBox> ActorItr(Player->GetWorld()); ActorItr; ++ActorItr) {
			// When they're broken, that is.
			if (ActorItr->fadetimer >= 0.0f) {
				Params.AddIgnoredActor(ActorItr.operator->());
			}
		}

		FHitResult ShapeTraceResult;
		Player->GetWorld()->SweepSingleByChannel(ShapeTraceResult, Player->GetActorLocation() + 10.0f*FVector::UpVector, Player->GetActorLocation() - 10.0f*FVector::UpVector, FQuat::Identity, ECC_Visibility, WallJumpCapsuleShape, Params); //100

		if (!Player->OnTheGround && FVector::VectorPlaneProject(ShapeTraceResult.Normal, FVector::UpVector).Size() > 0.95f) {
			if (ShapeTraceResult.GetActor() == nullptr ||
				(ShapeTraceResult.GetActor()->GetClass() != AMovingPlatform::StaticClass() &&
					ShapeTraceResult.GetActor()->GetClass() != ARotatingPlatform::StaticClass())) {
				Player->RidingWall = true;
				Player->StoredWallNormal = FVector::VectorPlaneProject(ShapeTraceResult.Normal, FVector::UpVector);
			}
		}
	}

	// Handle jumping.
	if (Player->JumpPressed) {
		Player->JumpPressed = false;
		bool wouldhavebeenotg = Player->OnTheGround;
		if (Player->OnTheGround || (Player->StoredWallNormal.Size() > 0.95f && Player->JumpSettings.HasWallJump) || Player->MovementComponent->offGroundTime < Player->JumpSettings.CoyoteJumpTime) {

			// Jump while taking the floor's angle and vertical movement into account.
			Player->CapsuleComponent->AddImpulse((Player->JumpSettings.JumpPower) * FVector::UpVector, NAME_None, true);

			Player->WasOnTheGround = false;
			Player->OnTheGround = false;
			Player->MovementComponent->onground = false;
			Player->MovementComponent->PlayerVelocity = Player->CapsuleComponent->GetPhysicsLinearVelocity();
			Player->AlreadyUnjumped = false;
			Player->JustJumped = true;
			Player->MovementComponent->justjumped = true;

			UGameplayStatics::PlaySound2D(Player, Player->JumpSound);

			if (Player->StoredWallNormal.Size() > 0.3f && !wouldhavebeenotg && Player->JumpSettings.HasWallJump) {
				// No cheating.
				if (Player->IsGliding || Player->AlreadyGlided) {
					Player->AlreadyGlided = true;
					Player->IsGliding = false;
				}

				// Set player velocity to be away from the wall.
				float multi = Player->JumpSettings.WallJumpMultiplier;
				if (Player->DashSettings.HasDashWallJump&&Player->holdingdash) {
					multi = Player->DashSettings.DashWallJumpMultiplier;
				}

				Player->FlattenVelocity();
				Player->CapsuleComponent->AddImpulse((Player->StoredWallNormal.GetSafeNormal()*Player->PhysicsSettings.MaxVelocity*multi), NAME_None, true);
				Player->GlideNextFrame = false;
				Player->JustWallJumped = true;

				// Make the player face away from the wall we just jumped off of.
				FRotator newmodelrotation = Player->PlayerModel->GetComponentRotation();
				FVector tangentalvelocity = FVector::VectorPlaneProject(Player->CapsuleComponent->GetPhysicsLinearVelocity(), Player->StoredWallNormal.GetSafeNormal());
				if (tangentalvelocity.Size() < Player->PhysicsSettings.MaxVelocity / 2.0f) {
					tangentalvelocity = FVector::ZeroVector;
				}

				newmodelrotation.Yaw = ((tangentalvelocity.IsNearlyZero() ? 1.0f : 0.75f)*Player->StoredWallNormal.GetSafeNormal() + 0.25f*tangentalvelocity.GetSafeNormal()).GetSafeNormal().Rotation().Yaw;// = (StoredWallNormal.GetSafeNormal() + FVector::VectorPlaneProject(CapsuleComponent->GetPhysicsLinearVelocity(), StoredWallNormal.GetSafeNormal()).GetSafeNormal() + FVector::VectorPlaneProject((Right*MovementInput.X + Forward*MovementInput.Y).GetSafeNormal(), StoredWallNormal.GetSafeNormal())).Rotation().Yaw;
				Player->PlayerModel->SetWorldRotation(newmodelrotation);
				Player->TargetDirection = newmodelrotation;//
			}
		}
	}

	// Apply a downward force if the player lets go of jump while still moving upwards.
	// This allows for variable jump heights.
	if (!Player->HoldingJump && Player->CapsuleComponent->GetPhysicsLinearVelocity().Z > 0 && !Player->AlreadyUnjumped && !Player->IsGliding) {
		Player->CapsuleComponent->AddImpulse(FVector::UpVector * Player->PhysicsSettings.Gravity * Player->JumpSettings.UnjumpRate * (Player->CapsuleComponent->GetPhysicsLinearVelocity().Z / Player->JumpSettings.JumpPower), NAME_None, true);
		Player->AlreadyUnjumped = true;
	}

	// >_>
	if (Player->MovementComponent->DistanceFromImpact < 25.0f + 45.0f && Player->MovementComponent->overground) {
		Player->GlideNextFrame = false;
	}

	if (Player->airdashing) {
		Player->GlideNextFrame = false;
	}

	// Start gliding.
	if (Player->GlideNextFrame && !Player->AlreadyGlided && !Player->dunk && Player->GlideSettings.HasGlide && !Player->WasOnTheGround) {
		if (Player->AppliedForce.Z <= 0.0f) {
			Player->FlattenVelocity();
			Player->CapsuleComponent->AddImpulse(FVector::UpVector * Player->GlideSettings.InitialGlideVelocity, NAME_None, true);
		}
		Player->IsGliding = true;
		Player->AlreadyGlided = true;
		Player->GlideNextFrame = false;
		Player->FloatParticles->ActivateSystem();
	}

	// Handle gliding.
	if (Player->IsGliding && !Player->WasOnTheGround) {
		Player->GlideTimer += DeltaTime;
		//FlattenVelocity();
		if (FMath::Abs(Player->GlideTimer*Player->GlideSettings.GlideSoundsPerSecond - FMath::FloorToInt(Player->GlideTimer*Player->GlideSettings.GlideSoundsPerSecond)) < DeltaTime*Player->GlideSettings.GlideSoundsPerSecond) {
			Player->WingSound->PitchMultiplier = FMath::FRandRange(Player->GlideSettings.GlideSoundPitchMin, Player->GlideSettings.GlideSoundPitchMax);
			UGameplayStatics::PlaySound2D(Player, Player->WingSound);
			//WingSound->PitchMultiplier = 1.0f;
		}
		Player->Wings->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		Player->PhysicsSettings.Gravity = Player->DefaultGravity * Player->GlideSettings.GlideGravityMultiplier;
		if (Player->AppliedForce.Z <= 0.0f) {
			//CapsuleComponent->AddForce(FVector::UpVector * PhysicsSettings.Gravity, NAME_None, true);
		} else {
			Player->PhysicsSettings.Gravity = Player->DefaultGravity;
		}
	} else {
		Player->FloatParticles->DeactivateSystem();
		Player->GlideTimer = 0.0f;
		Player->Wings->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
		if (Player->GetWorld()->GetTimerManager().GetTimerElapsed(Player->PreWarpTimer) < 0.0f) {
			Player->PhysicsSettings.Gravity = Player->DefaultGravity;
		}
	}

	// Stop gliding.
	if (Player->GlideTimer > Player->GlideSettings.GlideDuration || Player->OnTheGround || !Player->HoldingJump) {
		Player->IsGliding = false;
	}

	// Reset glide and unjump variables if the player is on the ground.
	if (Player->OnTheGround) {
		Player->AlreadyGlided = false;
		Player->AlreadyUnjumped = false;
		Player->AlreadyAirDash = false;
	}

	// Make the player start dashing in response to input.
	if (Player->DashNextFrame && Player->DashSettings.HasDash && !Player->movementlocked) {
		Player->DashNextFrame = false;
		if (Player->OnTheGround && !Player->MovementComponent->toosteep) {
			Player->DashParticles->ActivateSystem();
			Player->dashing = true;
			UGameplayStatics::PlaySound2D(Player, Player->DashSound);
		}
		if (!Player->OnTheGround&&Player->DashSettings.HasAirDash && !Player->AlreadyAirDash) {
			Player->DashParticles->ActivateSystem();
			Player->dashing = true;
			Player->airdashing = true;
			Player->IsGliding = false;
			Player->FlattenVelocity();
			if (!Player->MovementInput.IsNearlyZero()) {
				FRotator NewRotation;
				NewRotation.Pitch = 0.0f;
				NewRotation.Yaw = Player->SpringArm->GetComponentRotation().Yaw +
					(Player->MovementInput.X >= 0 ? 1 : -1) * FMath::RadiansToDegrees(FMath::Acos(Player->MovementInput.GetSafeNormal() | FVector(0, 1, 0)));
				Player->TargetDirection = NewRotation;
			}
			UGameplayStatics::PlaySound2D(Player, Player->DashSound);
		}
	}

	// Handle dashing.
	if (Player->dashing || Player->airdashing) {

		// Set the player's horizontal velocity while preserving their vertical velocity.
		Player->CapsuleComponent->SetPhysicsLinearVelocity(Player->TargetDirection.Vector().GetSafeNormal()*Player->DashSettings.DashSpeed + Player->CapsuleComponent->GetPhysicsLinearVelocity().Z*FVector::UpVector, NAME_None);

		// Tick up the dash timer.
		if (!Player->DashSettings.HasInfiniteDash) {
			Player->dashtimer += DeltaTime;
		}

		if (Player->MovementAxisLocked) {
			FVector f = Player->newspline->FindTangentClosestToWorldLocation(Player->GetActorLocation(), ESplineCoordinateSpace::World).GetSafeNormal();
			Player->TargetDirection = (FMath::Sign(Player->TargetDirection.Vector() | f) * f).Rotation();
			//TargetDirection = FMath::Sign(TargetDirection.Vector() | newspline->FindTangentClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World).GetSafeNormal()) * newspline->FindTangentClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World).GetSafeNormal().Rotation();
		}

		//if (airdashing) {
		Player->Wings->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		//}

		// Stop dashing if the player was dashing for too long.
		if (Player->dashtimer > Player->DashSettings.DashDuration) {
			Player->dashing = false;
			if (Player->airdashing) {
				Player->AlreadyAirDash = true;
			}
			Player->airdashing = false;
			Player->DashParticles->DeactivateSystem();
			Player->dashtimer = 0.0f;
		}
	}

	// This ain't Megaman X, kiddo.
	if (Player->dashing && !Player->airdashing && (!Player->OnTheGround || Player->JustJumped)) {
		Player->DashParticles->DeactivateSystem();
		Player->dashing = false;
		Player->dashtimer = 0.0f;
	}

	// COME ON AND SLAM
	if (Player->dunk) {
		if (Player->OnTheGround || Player->CapsuleComponent->GetPhysicsLinearVelocity().Z >= -Player->SlamSettings.SlamVelocity / 2.0f || Player->AppliedForce.Z < 0.0f) {
			Player->dunk = false;
			Player->SlamTrail->DeactivateSystem();
			if (Player->OnTheGround) {
				Player->SlamParticles->ActivateSystem();
				Player->SlamParticles->ActivateSystem();
				UGameplayStatics::PlaySound2D(Player, Player->DunkHitSound);
			}
		}
	}

	// AND WELCOME TO THE JAM
	if (Player->SlamNextFrame&&Player->SlamSettings.HasSlam) {
		Player->IsGliding = false;
		Player->SlamNextFrame = false;
		if (!Player->OnTheGround && Player->AppliedForce.Z <= 0.0f && !Player->dunk && !Player->ztarget) {
			Player->CapsuleComponent->AddImpulse(-Player->SlamSettings.SlamVelocity*FVector::UpVector, NAME_None, true);
			Player->dunk = true;
			Player->SlamTrail->ActivateSystem();
			UGameplayStatics::PlaySound2D(Player, Player->DunkSound);
		}
	}


	// Use the player as the source for the teleport raycast...
	FVector source = Player->GetActorLocation();
	FVector forward = Player->PlayerModel->GetForwardVector();

	ADestructibleBox* currentbox = NULL;
	float biggestdot = -1.0f;

	// Iterate over each DestructibleBox and cast a ray.
	for (TActorIterator<ADestructibleBox> ActorItr(Player->GetWorld()); ActorItr; ++ActorItr) {
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
			QueryParams.AddIgnoredActor(Player);
			FHitResult f;

			// Don't want the ray to collide with the player model now do we?
			QueryParams.AddIgnoredActor(Player);

			// Figure out if the ray is blocked by an object.
			bool blocked = Player->GetWorld()->LineTraceSingleByObjectType(f, source, ActorItr->GetActorLocation(), TraceParams, QueryParams);

			// If the trace hit a DestructibleBox and it's closer to where we're aiming
			// at than any other DestructibleBox, set it as the "closest" one.
			if (f.GetActor() != nullptr && f.GetActor()->GetClass() != nullptr && f.GetActor()->IsA(ADestructibleBox::StaticClass()) && blocked && displacement.Size() < Player->AttackRange && dot > 0.65f) {
				currentbox = (ADestructibleBox*)(f.GetActor());
				if (currentbox != nullptr&&Player->AttackPressed) {
					currentbox->BeginFadeout();
				}
			}
		}
	}

	Player->AttackPressed = false;

	// Reset the "player wants to teleport" variable.
	Player->swish = false;

	ASwitch* closestswitch = NULL;
	biggestdot = -1.0f;

	// Iterate over each Switch and cast a ray.
	for (TActorIterator<ASwitch> ActorItr(Player->GetWorld()); ActorItr; ++ActorItr) {
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
			QueryParams.AddIgnoredActor(Player);

			// Figure out if the ray is blocked by an object.
			bool blocked = Player->GetWorld()->LineTraceSingleByObjectType(f, source, ActorItr->GetActorLocation(), TraceParams, QueryParams);

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
		FMath::RadiansToDegrees(FMath::Acos(biggestdot)) < Player->TeleportAngleTolerance &&
		(closestswitch->GetActorLocation() - Player->GetActorLocation()).Size() < closestswitch->MaxDistance) {
		if (Player->ActivateNextFrame) {
			closestswitch->Activate();
		}
	}

	ANPC* closestNPC = NULL;
	biggestdot = -1.0f;

	// Iterate over each NPC and cast a ray.
	for (TActorIterator<ANPC> ActorItr(Player->GetWorld()); ActorItr; ++ActorItr) {
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
			QueryParams.AddIgnoredActor(Player);

			// Figure out if the ray is blocked by an object.
			bool blocked = Player->GetWorld()->LineTraceSingleByObjectType(f, source, ActorItr->GetActorLocation(), TraceParams, QueryParams);

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
		FMath::RadiansToDegrees(FMath::Acos(biggestdot)) < Player->TeleportAngleTolerance &&
		(closestNPC->GetActorLocation() - Player->GetActorLocation()).Size() < closestNPC->MaxDistance) {
		if (Player->ActivateNextFrame && Player->OnTheGround) {
			// Start dialogue with the target NPC.
			closestNPC->Activate();

			Player->lastcamerabeforedialogue = Player->SpringArm->GetComponentTransform();

			// Store information about that NPC and their root DialogueCut.
			Player->CurrentCut = closestNPC->CurrentCut;
			Player->CurrentNPC = closestNPC;
			TArray<TCHAR> escape = TArray<TCHAR>();
			escape.Add('\n');
			Player->stillscrolling = true;
			Player->CurrentLine = Player->CurrentCut->DialogueText.ReplaceEscapedCharWithChar(&escape);
			Player->CurrentTextWidth = Player->CurrentCut->DialogueWidth;

			// Look at me when I'm talking to you!
			FVector displacement = (Player->GetActorLocation() - Player->CurrentNPC->GetActorLocation());
			FRotator rot = displacement.Rotation();
			rot.Pitch = 0.0f;
			rot.Roll = 0.0f;
			Player->CurrentNPC->SetActorRotation(rot);
			Player->TargetDirection = (-displacement).Rotation();

			// Code to prevent violations of personal space.
			FVector target = displacement.GetSafeNormal()*150.0f;
			Player->SetActorLocation(target + Player->CurrentNPC->GetActorLocation());

			Player->CapsuleComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);

			// Snap the camera to the spring arm component's root (so we don't have to deal with
			// its arm length) and place it where the camera used to be. This effectively "nullifies"
			// the spring arm for the purposes of lerping the camera to its new DialogueCut-specified
			// position.
			FVector temp = Player->Camera->GetComponentLocation();
			Player->SpringArm->CameraLagSpeed = 0.0f;
			Player->SpringArm->CameraRotationLagSpeed = 0.0f;
			Player->SpringArm->SetWorldLocation(temp);
			Player->SpringArm->TargetArmLength = 0.0f;

			// Activates the attached blueprint (if the cut has one).
			if (Player->CurrentCut->BlueprintToExecute != nullptr) {
				FOutputDeviceNull dummy;
				Player->CurrentCut->CallFunctionByNameWithArguments(TEXT("Activate"), dummy, NULL, true);
			}
			Player->CurrentState = &Player->Dialogue;

			return;
		}
	}





	ASandShip* closestship = NULL;
	biggestdot = -1.0f;

	// Iterate over each SandShip and cast a ray.
	for (TActorIterator<ASandShip> ActorItr(Player->GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr->IsA(ASandShip::StaticClass())) {

			// Get displacement vector from the player/camera to the SandShip.
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
			QueryParams.AddIgnoredActor(Player);

			// Figure out if the ray is blocked by an object.
			bool blocked = Player->GetWorld()->LineTraceSingleByObjectType(f, source, ActorItr->GetActorLocation(), TraceParams, QueryParams);
			// If the trace hit a SandShip and it's closer to where we're aiming
			// at than any other SandShip, set it as the "closest" one.
			if (f.GetActor() != nullptr &&
				f.GetActor()->IsA(ASandShip::StaticClass()) &&
				dot > biggestdot &&
				blocked &&
				displacement.Size() < 500.0f) {
				closestship = *ActorItr;
				biggestdot = dot;
			}
		}
	}

	// Activate the "closest" SandShip as long as it's in
	// range and within a certain angle tolerance.
	if (closestship != nullptr &&
		FMath::RadiansToDegrees(FMath::Acos(biggestdot)) < Player->TeleportAngleTolerance &&
		(closestship->GetActorLocation() - Player->GetActorLocation()).Size() < 750.0f) {
		if (Player->ActivateNextFrame && Player->OnTheGround) {
			Player->BlockInput();
			Player->onsandship = true;
			closestship->Player = Player;
			Player->CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			Player->CapsuleComponent->SetSimulatePhysics(false);
			((APlayerController*)Player->GetController())->Possess(closestship);
			Player->CurrentState = &Player->Vehicle;
			Player->currentship = closestship;
			return;
		}

	}

	PlayerState::PhysicsStuff(Player, DeltaTime);
	PlayerState::CameraStuff(Player, DeltaTime);
	PlayerState::FaceTargetDirection(Player, DeltaTime);






	PlayerState::Tick2(Player, DeltaTime);
}

void AAuyron::DialogueState::Tick(AAuyron* Player, float DeltaTime)
{
	PlayerState::Tick(Player, DeltaTime);

	// Don't be rude.
	Player->MovementInput = FVector::ZeroVector;
	Player->CapsuleComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
	FVector displacement = (Player->GetActorLocation() - Player->CurrentNPC->GetActorLocation());
	Player->TargetDirection = (-displacement).Rotation();

	// Gotta go all smooth like with the camera.
	Player->SpringArm->CameraLagSpeed = Player->ActualDefaultCameraLag * Player->CameraLagSettings.DialogueLagMultiplier * !Player->CurrentCut->InstantTransition;
	Player->SpringArm->CameraRotationLagSpeed = Player->ActualDefaultCameraLag * Player->CameraLagSettings.DialogueLagMultiplier * !Player->CurrentCut->InstantTransition;
	Player->SpringArm->SetWorldTransform(Player->CurrentCut->Camera->GetComponentTransform());
	Player->SpringArm->SetWorldRotation(FRotator(Player->SpringArm->GetComponentRotation().Pitch, Player->SpringArm->GetComponentRotation().Yaw, 0.0f));
	Player->SpringArm->bDoCollisionTest = false;

	// Activates the attached blueprint (if the cut has one).
	if (Player->CurrentCut->BlueprintToExecute != nullptr && !Player->currentdialoguebpactivated) {
		Player->currentdialoguebpactivated = true;
		FOutputDeviceNull dummy;
		Player->CurrentCut->BlueprintToExecute->CallFunctionByNameWithArguments(TEXT("Activate"), dummy, NULL, true);
	}

	// Advances text.
	if (((Player->JumpPressed && Player->CurrentCut->CutDuration <= 0.0f) || Player->CurrentCut->cuttimer > Player->CurrentCut->CutDuration)) {
		Player->currentdialoguebpactivated = false;
		if ((Player->JumpPressed && Player->CurrentCut->CutDuration <= 0.0f) && Player->stillscrolling) {
			Player->skiptext = true;
		}
		if (!Player->stillscrolling) {
			if (Player->CurrentCut->Next != nullptr) {
				Player->CurrentCut->cuttimer = -1.0f;
				Player->CurrentCut = Player->CurrentCut->Next;
				Player->stillscrolling = true;
				Player->skiptext = false;
				FVector displacement = (Player->GetActorLocation() - Player->CurrentNPC->GetActorLocation());
				Player->TargetDirection = (-displacement).Rotation();
				FRotator rot = displacement.Rotation();
				rot.Pitch = 0.0f;
				rot.Roll = 0.0f;
				Player->CurrentNPC->SetActorRotation(rot);
				TArray<TCHAR> escape = TArray<TCHAR>();
				escape.Add('\n');
				Player->CurrentLine = Player->CurrentCut->DialogueText.ReplaceEscapedCharWithChar(&escape);
				Player->CurrentTextWidth = Player->CurrentCut->DialogueWidth;
				if (Player->CurrentCut->CutDuration > 0.0f) {
					Player->CurrentCut->cuttimer = 0.0f;
				}
			} else {
				Player->SpringArm->CameraLagSpeed = 0.0f;
				Player->SpringArm->CameraRotationLagSpeed = 0.0f;
				Player->SpringArm->SetWorldTransform(Player->lastcamerabeforedialogue);
				Player->SpringArm->SetRelativeRotation(FRotator(-30.0f, Player->SpringArm->GetComponentRotation().Yaw, 0.0f));
				Player->SpringArm->CameraLagSpeed = Player->CameraLagSettings.CameraLag;
				Player->SpringArm->CameraRotationLagSpeed = Player->CameraLagSettings.CameraRotationLag;
				Player->CurrentState = &Player->Normal;

			}
		}
		Player->JumpPressed = false;

		if (!Player->stillscrolling) {
			Player->skiptext = false;
		}
	}

	PlayerState::FaceTargetDirection(Player, DeltaTime);

	PlayerState::Tick2(Player, DeltaTime);
}

void AAuyron::ClimbingState::Tick(AAuyron* Player, float DeltaTime)
{

	PlayerState::Tick(Player, DeltaTime);

	FCollisionShape LedgeFinderShape = FCollisionShape::MakeSphere(50.0f);// (55.0f, 90.0f);
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);//
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	QueryParams.AddObjectTypesToQuery(ECC_Destructible);
	FCollisionQueryParams Params;
	FHitResult ShapeTraceResult;
	FVector head = Player->GetActorLocation() + 20.0f*FVector::UpVector;
	Player->GetWorld()->SweepSingleByChannel(ShapeTraceResult, head - 20.0f*Player->PlayerModel->GetForwardVector(), head + 50.0f*Player->PlayerModel->GetForwardVector(), FQuat::Identity, ECC_Visibility, LedgeFinderShape, Params);


	FVector WallNormal;
	FVector TraceHit;
	WallNormal = FVector::VectorPlaneProject(ShapeTraceResult.ImpactNormal, FVector::UpVector);
	TraceHit = ShapeTraceResult.ImpactPoint;
	FHitResult LedgeTraceResult;
	Player->GetWorld()->SweepSingleByChannel(LedgeTraceResult, head + 50.0f*Player->PlayerModel->GetForwardVector() + 200.0f*FVector::UpVector, head + 50.0f*Player->PlayerModel->GetForwardVector() - 200.0f*FVector::UpVector, FQuat::Identity, ECC_Visibility, LedgeFinderShape, Params);

	if (LedgeTraceResult.GetActor() != nullptr && LedgeTraceResult.GetComponent() != nullptr) {
		if (LedgeTraceResult.GetComponent()->IsA(UStaticMeshComponent::StaticClass())) {
			Player->grabbedledge = ((UStaticMeshComponent*)LedgeTraceResult.GetComponent());
			// The motion of a point on a rigid body is the combination of its motion about the center of mass...
			FVector angvel = FMath::DegreesToRadians(Player->grabbedledge->GetPhysicsAngularVelocity());
			FVector rr = LedgeTraceResult.ImpactPoint - (Player->grabbedledge->GetComponentLocation());
			FVector rvel = FVector::CrossProduct(angvel, rr);

			// ...and the motion of the center of mass itself.
			FVector cmvel = (Player->grabbedledge->GetPhysicsLinearVelocity());

			Player->ledgegroundvelocity = rvel + cmvel;
			Player->ledgeangularfrequency = -angvel.Z;
		} else {
			// BSP a shit
			Player->ledgegroundvelocity = FVector::ZeroVector;
			Player->ledgeangularfrequency = 0.0f;
		}

	}
	Player->CapsuleComponent->SetPhysicsLinearVelocity(Player->ledgegroundvelocity);
	Player->MovementComponent->platformangularfrequency = Player->ledgeangularfrequency;

	
	PlayerState::CameraStuff(Player, DeltaTime);
	PlayerState::FaceTargetDirection(Player, DeltaTime);
	PlayerState::Tick2(Player, DeltaTime);
}

void AAuyron::TeleportingState::Tick(AAuyron * Player, float DeltaTime)
{
	PlayerState::Tick(Player, DeltaTime);

	Player->SpringArm->CameraLagSpeed = 0.0f; //CameraLagSettings.CameraLag*2.5f;
	Player->SpringArm->CameraRotationLagSpeed = 0.0f; //CameraLagSettings.CameraRotationLag*2.5f;
	Player->DefaultArmLength = Player->TargetDefaultArmLength;
	Player->SpringArm->TargetArmLength = Player->DefaultArmLength;
	PlayerState::Tick2(Player, DeltaTime);
}

void AAuyron::AimingState::Tick(AAuyron * Player, float DeltaTime)
{
	PlayerState::Tick(Player, DeltaTime);

	FVector source = Player->Camera->GetComponentLocation();
	FVector forward = Player->Camera->GetForwardVector();

	AStick* closest = NULL;
	float biggestdot = -1.0f;

	// Iterate over each TelePad and cast a ray.
	for (TActorIterator<AStick> ActorItr(Player->GetWorld()); ActorItr; ++ActorItr) {
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
			QueryParams.AddIgnoredActor(Player);

			// Figure out if the ray is blocked by an object.
			bool blocked = Player->GetWorld()->LineTraceSingleByObjectType(f, source, ActorItr->GetActorLocation(), TraceParams, QueryParams);

			// If the trace hit a telepad and it's closer to where we're aiming
			// at than any other TelePad, set it as the "closest" one.
			if (f.GetActor() != nullptr && f.GetActor()->GetClass() != nullptr && f.GetActor()->GetClass()->GetName() == "Stick" && dot > biggestdot && blocked && displacement.Size() < Player->TeleportRange) {
				closest = *ActorItr;
				biggestdot = dot;
			}
		}
	}

	// Set the color of the "closest" TelePad as long as it's in range
	// and within a certain angle tolerance and teleport he player to it
	// if they are trying to teleport.
	if (closest != nullptr &&
		Player->TeleportSettings.HasTeleport &&
		FMath::RadiansToDegrees(FMath::Acos(biggestdot)) < Player->TeleportAngleTolerance &&
		(closest->GetActorLocation() - Player->GetActorLocation()).Size() < Player->TeleportRange) {

		// Set the target TelePad's color and brightness so that it stands out.
		closest->PointLight->LightColor = Player->TeleportSettings.TeleportLightColor;
		closest->PointLight->Intensity = 32.0f;
		closest->PointLight->UpdateColorAndBrightness();

		closest->asfd = true;
		//closest->mat->SetScalarParameterValue("On", 1.0f);

		// Teleport the player to the TelePad if they're trying to teleport
		// and impart the post teleport velocity on them.
		if (Player->swish) {
			if (closest != nullptr) {

				Player->IsGliding = false;
				Player->ztarget = false;

				Player->closeststick = closest;
				Player->warphere = closest->gohere;
				Player->warpvel = closest->PostTeleportVelocity;
				Player->BackupDefaultArmLength = Player->TargetDefaultArmLength;

				closest->PointLight->LightColor = FColor::White;
				closest->PointLight->Intensity = 0.0f; 1750.0f;
				closest->PointLight->UpdateColorAndBrightness();
				closest->asfd = false;

				UGameplayStatics::PlaySound2D(Player, Player->WarpSound);

				// Start the teleport animation timer.
				if (Player->InCameraOverrideRegion) {
					Player->HereWeGo();
				} else {
					Player->GetWorld()->GetTimerManager().SetTimer(Player->PreWarpTimer, Player, &AAuyron::HereWeGo, Player->TeleportSettings.TeleportAnimationDuration);
					Player->GetWorld()->GetTimerManager().SetTimer(Player->PreWarpTimer2, Player, &AAuyron::MoveIt, 0.75f*Player->TeleportSettings.TeleportAnimationDuration);
					Player->CurrentState = &Player->Teleporting;
					Player->TheAbyss->SetRelativeRotation(FRotator(0.0f, 5.0f, FMath::RandRange(-5.0f, 5.0f)));
					//TheAbyss->SetVisibility(true);

					//FRotator NewRotation = SpringArm->GetComponentRotation();
					FRotator NewRotation = FRotator::ZeroRotator;
					NewRotation.Yaw += Player->PlayerModel->GetComponentRotation().Yaw;
					NewRotation.Pitch = -30.0f;

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

					Player->FlattenVelocity();
					Player->PhysicsSettings.Gravity = 0.0f;
					//Capture2D->SetActorLocation(warphere - (warphere - GetActorLocation()).GetSafeNormal()*DefaultArmLength*4.0f);
					FRotator asdfasdf = FRotator::ZeroRotator;
					asdfasdf.Yaw = (Player->warphere - Player->GetActorLocation()).Rotation().Yaw;
					//Capture2D->SetActorLocation(warphere);
					//Capture2D->SetActorRotation(SpringArm->GetComponentRotation());
					//Capture2D->AddActorLocalOffset(-BackupDefaultArmLength*FVector::ForwardVector);
					//GetWorld()->GetTimerManager().SetTimer(WHY, this, &AAuyron::whywhy, 0.25f);
					Player->lel = true;
					Player->Capture2D->SetActorTransform(Player->Camera->GetComponentTransform());
					FVector temp = Player->GetActorLocation();
					Player->SetActorLocation(Player->warphere, false, nullptr, ETeleportType::TeleportPhysics);

					Player->TargetDefaultArmLength = Player->BackupDefaultArmLength;
					Player->DefaultArmLength = Player->TargetDefaultArmLength;
					Player->SpringArm->TargetArmLength = Player->TargetDefaultArmLength;

					Player->teletestmat->SetScalarParameterValue("t", 0.1f);

					Player->SpringArm->CameraLagSpeed = 0.0f; 1.0f / 128.0f;
					Player->SpringArm->CameraRotationLagSpeed = 0.0f; 1.0f / 128.0f;
					Player->SpringArm->SetRelativeLocation(FVector::ZeroVector);// = DefaultArmLength;
					Player->SetActorLocation(temp, false, nullptr, ETeleportType::TeleportPhysics);
					return;
				}
			}
		}
	}





	// Set teleport range and angle to the "long range" values.
	Player->TeleportRange = Player->TeleportSettings.TeleportRangeWhenAiming;
	Player->TeleportAngleTolerance = Player->TeleportSettings.TeleportAngleToleranceWhenAiming;

	// Find the direction the player and camera should be facing.
	FRotator NewRotation = Player->SpringArm->RelativeRotation;
	NewRotation.Yaw += (DeltaTime*120.0f)*Player->CameraInput.X;
	NewRotation.Pitch = FMath::Clamp(Player->SpringArm->GetComponentRotation().Pitch + (DeltaTime*120.0f)*Player->CameraInput.Y, -Player->CameraMaxAngle, -Player->CameraMinAngle);

	// Update the player model's target facing direction.
	Player->TargetDirection.Yaw = NewRotation.Yaw;

	// If the player just started aiming...
	if (!Player->wasztarget) {
		Player->BackupDefaultArmLength = Player->TargetDefaultArmLength;

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
		if (!Player->MovementInput.IsNearlyZero()) {
			NewRotation.Pitch = 0.0f;
			NewRotation.Yaw = Player->SpringArm->GetComponentRotation().Yaw +
				(Player->MovementInput.X >= 0 ? 1 : -1) * FMath::RadiansToDegrees(FMath::Acos(Player->MovementInput.GetSafeNormal() | FVector(0, 1, 0)));
		}
		if (Player->TimeSinceLastMouseInput > Player->CameraAutoTurnSettings.CameraResetTime && !Player->movementlocked) {
			NewRotation.Pitch = 0.0f;
		}
	}

	Player->SpringArm->SetWorldRotation(Player->SpringArm->GetComponentRotation() + NewRotation);

	// Face the player and camera to the new rotation.
	Player->PlayerModel->SetWorldRotation(FRotator(0.0f, NewRotation.Yaw, 0.0f));
	Player->SpringArm->TargetArmLength = 300.0f;
	Player->DefaultArmLength = 300.0f;
	Player->SpringArm->CameraLagSpeed = Player->CameraLagSettings.CameraLag * (Player->wasztarget ? Player->CameraLagSettings.AimingLagMultiplier : 1.0f);
	Player->SpringArm->CameraRotationLagSpeed = Player->CameraLagSettings.CameraRotationLag * (Player->wasztarget ? Player->CameraLagSettings.AimingLagMultiplier : 1.0f);
	Player->SpringArm->SetRelativeRotation(NewRotation);

	// Offset the spring arm (and therefore the camera) a bit so the player model
	// isn't blocking the screen when we're trying to aim.
	FVector base = FVector(0.0f, 100.0f, 100.0f);
	base = (base.X*Player->Forward + base.Y*Player->Right + base.Z*FVector::UpVector);
	Player->SpringArm->SetRelativeLocation(base);

	// Lock the player's movement inputs.
	Player->movementlocked = true;

	if (!Player->ztarget) {
		Player->CurrentState = &Player->Normal;
	}

	Player->wasztarget = true;

	PlayerState::PhysicsStuff(Player, DeltaTime);
	PlayerState::FaceTargetDirection(Player, DeltaTime);
	PlayerState::Tick2(Player, DeltaTime);
}


void AAuyron::VehicleState::Tick(AAuyron * Player, float DeltaTime)
{
	//PlayerState::Tick(Player, DeltaTime);
	if (Player->IsControlled()) {
		Player->CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		Player->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
		Player->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
		Player->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		Player->CapsuleComponent->SetSimulatePhysics(true);
		Player->CurrentState = &Player->Normal;
		return;
	}
	Player->CapsuleComponent->SetSimulatePhysics(false);


	Player->SetActorLocation(Player->currentship->GetActorLocation() + 260.0f*FVector::UpVector, false, nullptr, ETeleportType::TeleportPhysics);
	Player->PlayerModel->SetWorldRotation(FRotator(0.0f, Player->currentship->GetActorRotation().Yaw, 0.0f));

	((APlayerController*)Player->GetController());

	//PlayerState::Tick2(Player, DeltaTime);
}