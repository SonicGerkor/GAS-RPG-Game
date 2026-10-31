// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Aura/Interaction/DialogueInterface.h"
#include "Aura/Interaction/HighlightInterface.h"
#include "NPCCharacter.generated.h"

UCLASS()
class AURA_API ANPCCharacter : public ACharacterBase, public IHighlightInterface, public IDialogueInterface
{
	GENERATED_BODY()
	
public:
	
	ANPCCharacter();

	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;

	virtual void StartDialogue_Implementation() override;
	virtual void AdvanceDialogue_Implementation() override;
	virtual void EndDialogue_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TObjectPtr<UDataTable> DialogueTable;

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	bool bIsTalking = false;
	
	// Stage to set when this NPC's dialogue finishes. 0 = doesn't change the story stage.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	int32 StageOnDialogueComplete = 0;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Dialogue")
	bool bHasCompletedDialogue = false;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
	void OnDialogueStateChanged(bool bTalking);

protected:
	
	int32 CurrentLineIndex = 0;
};
