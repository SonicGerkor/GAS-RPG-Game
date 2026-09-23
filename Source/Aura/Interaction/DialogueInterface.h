// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DialogueInterface.generated.h"

USTRUCT(BlueprintType)
struct FDialogueLine : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FText SpeakerName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FText LineText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	USoundBase* VoiceLine = nullptr;
};

UINTERFACE(MinimalAPI)
class UDialogueInterface : public UInterface
{
	GENERATED_BODY()
};

class AURA_API IDialogueInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartDialogue();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AdvanceDialogue();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndDialogue();
};
