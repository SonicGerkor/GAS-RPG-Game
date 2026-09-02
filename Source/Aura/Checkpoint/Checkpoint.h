// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "Aura/Aura.h"
#include "Aura/Interaction/HighlightInterface.h"
#include "Aura/Interaction/SaveGameInterface.h"
#include "GameFramework/PlayerStart.h"
#include "Checkpoint.generated.h"

class UNiagaraSystem;
class USphereComponent;

UCLASS()
class AURA_API ACheckpoint : public APlayerStart, public ISaveGameInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:
	
	ACheckpoint(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bReached = false;
	
	UPROPERTY(EditDefaultsOnly)
	bool bBindOverlapCallback = true;
	
	/* Save Interface */
	virtual bool ShouldLoadTransform_Implementation() override { return false; }
	virtual void LoadActor_Implementation() override;
	
	/* Highlight Interface */
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MoveToComponent; 
	
	UPROPERTY(EditDefaultsOnly)
	int32 CustomDepthStencilOverride = CUSTOM_DEPTH_TAN;
	
protected:
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UFUNCTION(BlueprintCallable)
	void HandleGlowEffects();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> CheckpointEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* CheckpointSound = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> OverlapComponent;
};
