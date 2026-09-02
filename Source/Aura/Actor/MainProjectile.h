// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "Aura/MainAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "MainProjectile.generated.h"

class UAudioComponent;
class UNiagaraSystem;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class AURA_API AMainProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	AMainProjectile();
	
	virtual void Destroyed() override;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;
	
	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;

protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereComponent;
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	virtual void OnHit();
	
	bool IsValidOverlap(AActor* OtherActor);

	bool bHit = false;
	
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopSound;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopSoundComponent;
};
