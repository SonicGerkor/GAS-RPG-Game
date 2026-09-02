// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "MainProjectile.h"
#include "MainFireBall.generated.h"

UCLASS()
class AURA_API AMainFireBall : public AMainProjectile
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeline();
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> ReturnToActor;
	
	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams ExplosionDamageParams;
	
protected:
	
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnHit() override;
};
