// Copyright Gerkor Games

#include "SummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> USummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / MinionAmount;
	
	const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread * 0.5f, FVector::UpVector);
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread * 0.5f, FVector::UpVector);
	
	TArray<FVector> SpawnLocations;
	for (int32 i = 0; i < MinionAmount; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		FVector SpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);
		
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, SpawnLocation + FVector(0.f, 0.f, 300.f), SpawnLocation - FVector(0.f, 0.f, 300.f), ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			SpawnLocation = Hit.ImpactPoint;
		}
		
		SpawnLocations.Add(SpawnLocation);
	}
	
	return SpawnLocations;
}

TSubclassOf<APawn> USummonAbility::GetRandomMinionClass()
{
	if (MinionClasses.IsEmpty()) return nullptr;
	
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection];
}
