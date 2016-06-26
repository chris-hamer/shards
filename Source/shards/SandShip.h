// STOP TELLING ME TO FILL OUT MY COPYRIGHT NOTICE YOU'RE NOT MY REAL DAD

#pragma once

#include "GameFramework/Pawn.h"
#include "Auyron.h"
#include "SandShip.generated.h"

USTRUCT()
struct FSandShipPhysicsSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float AccelRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CoastingDrag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float BrakingDrag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float MaxVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TurnRate;
};

UCLASS()
class SHARDS_API ASandShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASandShip();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual void PostInitializeComponents() override;

	void MoveRight(float AxisValue);
	void MoveForward(float AxisValue);
	void YawCamera(float AxisValue);
	void PitchCamera(float AxisValue);
	void Jump();
	void Unjump();
	void GetOut();

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UCapsuleComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* Model;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UCameraComponent* Camera;
	UMaterialInterface* CelShaderMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FSandShipPhysicsSettings PhysicsSettings;
	
	FVector MovementInput;
	FVector CameraInput;

	AAuyron* Player;

	float CurrentDrag;
	UMaterialInstanceDynamic* celshadermat;

};
