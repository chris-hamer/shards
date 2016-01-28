// STOP TELLING ME TO FILL OUT MY COPYRIGHT NOTICE

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "WorkGodamnitMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHARDS_API UWorkGodamnitMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
public:
	bool otg;
};
