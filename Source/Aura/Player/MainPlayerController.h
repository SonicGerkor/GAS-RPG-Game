// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "Aura/Interaction/HighlightInterface.h"
#include "Engine/HitResult.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UDialogueWindow;
class AMagicCircle;
class UNiagaraSystem;
class UDamageTextComponent;
class USplineComponent;
class UMainAbilitySystemComponent;
class UMainInputConfig;
class UInputMappingContext;
class UInputAction;
class AHubPortal;
struct FGameplayTag;
struct FInputActionValue;

enum class ETargetingStatus : uint8
{
	TargetingEnemy,
	TargetingNonEnemy,
	NotTargeting
};

UCLASS()
class AURA_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	AMainPlayerController();
	
	virtual void PlayerTick(float DeltaTime) override;
	
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, const bool bBlockedHit, const bool bCriticalHit);
	
	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);
	
	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void OpenDialogueWidget(AActor* NPC, const FText& Speaker, const FText& Line);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void UpdateDialogueLine(const FText& Speaker, const FText& Line);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void AdvanceActiveDialogue();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void CloseDialogueWidget();
	
	void SetNearbyPortal(AHubPortal* Portal) { NearbyPortal = Portal; }
	void ClearNearbyPortal(AHubPortal* Portal) { if (NearbyPortal == Portal) NearbyPortal = nullptr; }
	
protected:
	
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:
	
	UPROPERTY()
	TObjectPtr<UMainAbilitySystemComponent> MainAbilitySystemComponent;
	
	UMainAbilitySystemComponent* GetASC();
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> MainContext;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Dialogue")
	TSubclassOf<UDialogueWindow> DialogueWidgetClass;

	UPROPERTY()
	TObjectPtr<UDialogueWindow> ActiveDialogueWidget;

	UPROPERTY()
	TObjectPtr<AActor> ActiveDialogueNPC;
	
	UPROPERTY()
	TObjectPtr<AHubPortal> NearbyPortal;
	
	void ShiftPressed() { bShiftKeyDown = true; }
	void ShiftReleased() { bShiftKeyDown = false; }
	bool bShiftKeyDown = false;
	
	void Move(const FInputActionValue& InputActionValue);
	
	void Interact();
	
	FHitResult CursorHit;
	void CursorTrace();
	TObjectPtr<AActor> LastActor;
	TObjectPtr<AActor> ThisActor;
	
	static void HighlightActor(AActor* InActor);
	static void UnHighlightActor(AActor* InActor);
	
	void AbilityInputTagPressed(const FGameplayTag InputTag);
	void AbilityInputTagReleased(const FGameplayTag InputTag);
	void AbilityInputTagHeld(const FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UMainInputConfig> InputConfig;
	
	/*
	 * Click To Move
	 */
	
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	ETargetingStatus TargetingStatus = ETargetingStatus::NotTargeting;
	
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;
	
	void AutoRun();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;
	
	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;
	
	void UpdateMagicCircleLocation();
};
