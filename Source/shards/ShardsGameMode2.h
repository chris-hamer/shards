// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerController.h"
#include "ShardsPlayerController.h"
#include "ShardsGameMode2.generated.h"

/**
 * 
 */
UCLASS()
class SHARDS_API AShardsGameMode2 : public AGameMode
{
	GENERATED_BODY()

		void StartPlay() override;
public:
	
};
