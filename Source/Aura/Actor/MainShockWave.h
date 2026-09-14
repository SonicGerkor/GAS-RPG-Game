// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "MainProjectile.h"
#include "MainShockWave.generated.h"

UCLASS()
class AURA_API AMainShockWave : public AMainProjectile
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartShockwaveTimeline();
	
	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams ExplosionDamageParams;
	
protected:
	
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnHit() override;
};
