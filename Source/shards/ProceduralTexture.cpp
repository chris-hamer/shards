// STOP TELLING ME TO FILL OUT MY COPYRIGHT NOTICE YOU'RE NOT MY REAL DAD

#include "shards.h"
#include "ProceduralTexture.h"

//
void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
{
	//if (Texture && Texture->Resource)
	//{
	//	struct FUpdateTextureRegionsData
	//	{
	//		FTexture2DResource* Texture2DResource;
	//		int32 MipIndex;
	//		uint32 NumRegions;
	//		FUpdateTextureRegion2D* Regions;
	//		uint32 SrcPitch;
	//		uint32 SrcBpp;
	//		uint8* SrcData;
	//	};

	//	FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

	//	RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
	//	RegionData->MipIndex = MipIndex;
	//	RegionData->NumRegions = NumRegions;
	//	RegionData->Regions = Regions;
	//	RegionData->SrcPitch = SrcPitch;
	//	RegionData->SrcBpp = SrcBpp;
	//	RegionData->SrcData = SrcData;

	//	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
	//		UpdateTextureRegionsData,
	//		FUpdateTextureRegionsData*, RegionData, RegionData,
	//		bool, bFreeData, bFreeData,
	//		{
	//			for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
	//			{
	//				int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
	//				if (RegionData->MipIndex >= CurrentFirstMip)
	//				{
	//					RHIUpdateTexture2D(
	//						RegionData->Texture2DResource->GetTexture2DRHI(),
	//						RegionData->MipIndex - CurrentFirstMip,
	//						RegionData->Regions[RegionIndex],
	//						RegionData->SrcPitch,
	//						RegionData->SrcData
	//						+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
	//						+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
	//					);
	//				}
	//			}
	//	if (bFreeData)
	//	{
	//		FMemory::Free(RegionData->Regions);
	//		FMemory::Free(RegionData->SrcData);
	//	}
	//	delete RegionData;
	//		});
	//}
}


// Sets default values
AProceduralTexture::AProceduralTexture()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProceduralTexture::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProceduralTexture::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

