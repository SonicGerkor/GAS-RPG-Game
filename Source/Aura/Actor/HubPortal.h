// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "Aura/Interaction/DialogueInterface.h"
#include "GameFramework/Actor.h"
#include "HubPortal.generated.h"

class USphereComponent;

UCLASS()
class AURA_API AHubPortal : public AActor, public IDialogueInterface
{
	GENERATED_BODY()

public:
	
	AHubPortal();

	// Story stage in which this is the correct portal. 0 = never correct (decoy).
	UPROPERTY(EditAnywhere, Category = "Portal")
	int32 CorrectForStage = 0;

	UPROPERTY(EditAnywhere, Category = "Portal")
	TSoftObjectPtr<UWorld> DestinationMap;

	UPROPERTY(EditAnywhere, Category = "Portal")
	FName DestinationPlayerStartTag;

	UPROPERTY(EditAnywhere, Category = "Portal")
	FText SealedMessage;       // shown while StoryStage == 0

	UPROPERTY(EditAnywhere, Category = "Portal")
	FText WrongPortalMessage;  // "This is not the correct one..."

	void Interact();

	UFUNCTION(BlueprintCallable, Category = "Portal")
	void ConfirmTravel();

	// Blueprint: create the AreYouSure widget, bind Confirm -> ConfirmTravel
	UFUNCTION(BlueprintImplementableEvent, Category = "Portal")
	void ShowTravelPrompt();

	/* Dialogue Interface - lets the dialogue window drive the one-line messages */
	virtual void StartDialogue_Implementation() override { Interact(); }
	virtual void AdvanceDialogue_Implementation() override { EndDialogue_Implementation(); }
	virtual void EndDialogue_Implementation() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRangeBegin(UPrimitiveComponent* Comp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& Sweep);
	
	UFUNCTION()
	void OnRangeEnd(UPrimitiveComponent* Comp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> InteractionRange;
};
