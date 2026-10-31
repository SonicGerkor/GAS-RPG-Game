// Copyright Gerkor Games

#include "NPCCharacter.h"
#include "Aura/Aura.h"
#include "Aura/Game/LoadScreenSaveGame.h"
#include "Aura/Game/MainGameModeBase.h"
#include "Aura/Player/MainPlayerController.h"
#include "Kismet/GameplayStatics.h"

ANPCCharacter::ANPCCharacter()
{
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	GetMesh()->MarkRenderStateDirty();
}

void ANPCCharacter::HighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(true);
}

void ANPCCharacter::UnHighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
}

void ANPCCharacter::SetMoveToLocation_Implementation(FVector& OutDestination)
{
	OutDestination = GetActorLocation();
}

void ANPCCharacter::StartDialogue_Implementation()
{
	bIsTalking = true;
	CurrentLineIndex = 0;

	if (const FDialogueLine* Row = DialogueTable->FindRow<FDialogueLine>(FName(*FString::FromInt(CurrentLineIndex)), TEXT("StartDialogue")))
	{
		if (AMainPlayerController* PC = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			PC->OpenDialogueWidget(this, Row->SpeakerName, Row->LineText);
		}
	}
	OnDialogueStateChanged(bIsTalking);
}

void ANPCCharacter::AdvanceDialogue_Implementation()
{
	CurrentLineIndex++;

	if (const FDialogueLine* Row = DialogueTable->FindRow<FDialogueLine>(FName(*FString::FromInt(CurrentLineIndex)), TEXT("AdvanceDialogue")))
	{
		if (AMainPlayerController* PC = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			PC->UpdateDialogueLine(Row->SpeakerName, Row->LineText);
		}
	}
	else
	{
		EndDialogue_Implementation();
	}
}

void ANPCCharacter::EndDialogue_Implementation()
{
	bIsTalking = false;
	bHasCompletedDialogue = true;
	if (AMainPlayerController* PC = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PC->CloseDialogueWidget();
	}
	
	if (StageOnDialogueComplete > 0)
	{
		if (AMainGameModeBase* GM = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			if (ULoadScreenSaveGame* SaveData = GM->RetrieveInGameSaveData())
			{
				if (SaveData->StoryStage < StageOnDialogueComplete)
				{
					SaveData->StoryStage = StageOnDialogueComplete;
					GM->SaveGameProgress(SaveData);
				}
			}
		}
	}
	OnDialogueStateChanged(bIsTalking);
}
