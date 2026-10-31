// Copyright Gerkor Games

#include "HubPortal.h"

#include "Aura/Game/LoadScreenSaveGame.h"
#include "Aura/Game/MainGameModeBase.h"
#include "Aura/Interaction/PlayerInterface.h"
#include "Aura/Player/MainPlayerController.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AHubPortal::AHubPortal()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	InteractionRange = CreateDefaultSubobject<USphereComponent>("InteractionRange");
	InteractionRange->SetupAttachment(GetRootComponent());
	InteractionRange->SetSphereRadius(200.f);
	InteractionRange->SetCollisionProfileName("OverlapOnlyPawn");
}

void AHubPortal::BeginPlay()
{
	Super::BeginPlay();
	InteractionRange->OnComponentBeginOverlap.AddDynamic(this, &AHubPortal::OnRangeBegin);
	InteractionRange->OnComponentEndOverlap.AddDynamic(this, &AHubPortal::OnRangeEnd);
}

void AHubPortal::OnRangeBegin(UPrimitiveComponent*, AActor* Other, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (const APawn* Pawn = Cast<APawn>(Other); Pawn && Other->Implements<UPlayerInterface>())
	{
		if (AMainPlayerController* PC = Cast<AMainPlayerController>(Pawn->GetController()))
		{
			PC->SetNearbyPortal(this);
		}
	}
}

void AHubPortal::OnRangeEnd(UPrimitiveComponent*, AActor* Other, UPrimitiveComponent*, int32)
{
	if (const APawn* Pawn = Cast<APawn>(Other))
	{
		if (AMainPlayerController* PC = Cast<AMainPlayerController>(Pawn->GetController()))
		{
			PC->ClearNearbyPortal(this);
		}
	}
}

void AHubPortal::Interact()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());
	AMainGameModeBase* GM = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(PC) || !IsValid(GM)) return;

	const ULoadScreenSaveGame* SaveData = GM->RetrieveInGameSaveData();
	const int32 Stage = IsValid(SaveData) ? SaveData->StoryStage : 0;

	if (Stage == 0)
	{
		PC->OpenDialogueWidget(this, FText::GetEmpty(), SealedMessage);
	}
	else if (Stage == CorrectForStage)
	{
		ShowTravelPrompt();
	}
	else
	{
		PC->OpenDialogueWidget(this, FText::GetEmpty(), WrongPortalMessage);
	}
}

void AHubPortal::ConfirmTravel()
{
	if (const AMainGameModeBase* GM = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GM->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
	}
	if (APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		IPlayerInterface::Execute_SaveProgress(Pawn, DestinationPlayerStartTag);
	}
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
}

void AHubPortal::EndDialogue_Implementation()
{
	if (AMainPlayerController* PC = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PC->CloseDialogueWidget();
	}
}
