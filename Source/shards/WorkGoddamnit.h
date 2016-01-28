// STOP TELLING ME TO FILL OUT MY COPYRIGHT NOTICE

#pragma once

#include "GameFramework/Pawn.h"
#include "WorkGodamnitMovementComponent.h"
#include "WorkGoddamnit.generated.h"

UCLASS()
class SHARDS_API AWorkGoddamnit : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWorkGoddamnit();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere) UCapsuleComponent* Root;
	UPROPERTY(EditAnywhere) UStaticMeshComponent* Model;
	UPROPERTY(EditAnywhere) USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere) UCameraComponent* Camera;
	UPROPERTY(EditAnywhere) class UWorkGodamnitMovementComponent* MC;

	UPROPERTY(EditAnywhere) float vroom;
	UPROPERTY(EditAnywhere) float grav;

	void MoveRight(float AxisValue);
	void MoveForward(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);
	void ControllerYawCamera(float AxisValue);
	
	FVector MovementInput;
	FVector CameraInput;

};
