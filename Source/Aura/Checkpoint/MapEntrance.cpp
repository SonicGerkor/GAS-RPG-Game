// Copyright Gerkor Games

#include "MapEntrance.h"

#include "Aura/Game/MainGameModeBase.h"
#include "Aura/Interaction/PlayerInterface.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OverlapComponent->SetupAttachment(MoveToComponent);
}

void AMapEntrance::LoadActor_Implementation()
{
	// DO NOTHING WHEN LOADING A MAP ENTRANCE
}

void AMapEntrance::HighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(true);
}

void AMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;
		if (const AMainGameModeBase* MainGM = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			MainGM->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
		}
		
		IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);
		
		if (CheckpointSound)
		{
			UGameplayStatics::PlaySound2D(this, CheckpointSound);
		}
		
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
}
