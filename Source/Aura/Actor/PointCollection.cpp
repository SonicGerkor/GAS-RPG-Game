// Copyright Gerkor Games

#include "PointCollection.h"
#include "Aura/AbilitySystem/MainAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetupInitialValues();
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride)
{
	checkf(ImmutablePoints.Num() >= NumPoints, TEXT("Attempted to Access ImmutablePoints out of bounds!"));
	
	TArray<USceneComponent*> ArrayCopy = ImmutablePoints;
	
	for (USceneComponent* PT : ImmutablePoints)
	{
		if (ArrayCopy.Num() >= NumPoints) return ArrayCopy;
		
		if (PT != Point0)
		{
			FVector ToPoint = PT->GetComponentLocation() - Point0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			PT->SetWorldLocation(Point0->GetComponentLocation() + ToPoint);
		}
		
		const FVector RaisedLocation = FVector(PT->GetComponentLocation().X, PT->GetComponentLocation().Y, PT->GetComponentLocation().Z + 500.f);
		const FVector LoweredLocation = FVector(PT->GetComponentLocation().X, PT->GetComponentLocation().Y, PT->GetComponentLocation().Z - 500.f);
		
		FHitResult HitResult;
		TArray<AActor*> IgnoreActors;
		
		UMainAbilitySystemLibrary::GetLivePlayersWithinRadius(this, IgnoreActors, TArray<AActor*>(), 1500.f, GetActorLocation());
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName("BlockAll"), QueryParams);
		
		const FVector AdjustedLocation = FVector(PT->GetComponentLocation().X, PT->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		PT->SetWorldLocation(AdjustedLocation);
		PT->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));
		
		ArrayCopy.Add(PT);
	}
	
	return ArrayCopy;
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}

void APointCollection::SetupInitialValues()
{
	Point0 = CreateDefaultSubobject<USceneComponent>("Point0");
	ImmutablePoints.Add(Point0);
	SetRootComponent(Point0);
	
	Point1 = CreateDefaultSubobject<USceneComponent>("Point1");
	ImmutablePoints.Add(Point1);
	Point1->SetupAttachment(GetRootComponent());
	
	Point2 = CreateDefaultSubobject<USceneComponent>("Point2");
	ImmutablePoints.Add(Point2);
	Point2->SetupAttachment(GetRootComponent());
	
	Point3 = CreateDefaultSubobject<USceneComponent>("Point3");
	ImmutablePoints.Add(Point3);
	Point3->SetupAttachment(GetRootComponent());
	
	Point4 = CreateDefaultSubobject<USceneComponent>("Point4");
	ImmutablePoints.Add(Point4);
	Point4->SetupAttachment(GetRootComponent());
	
	Point5 = CreateDefaultSubobject<USceneComponent>("Point5");
	ImmutablePoints.Add(Point5);
	Point5->SetupAttachment(GetRootComponent());
	
	Point6 = CreateDefaultSubobject<USceneComponent>("Point6");
	ImmutablePoints.Add(Point6);
	Point6->SetupAttachment(GetRootComponent());
	
	Point7 = CreateDefaultSubobject<USceneComponent>("Point7");
	ImmutablePoints.Add(Point1);
	Point7->SetupAttachment(GetRootComponent());
	
	Point8 = CreateDefaultSubobject<USceneComponent>("Point8");
	ImmutablePoints.Add(Point8);
	Point8->SetupAttachment(GetRootComponent());
		
	Point9 = CreateDefaultSubobject<USceneComponent>("Point9");
	ImmutablePoints.Add(Point9);
	Point9->SetupAttachment(GetRootComponent());
		
	Point10 = CreateDefaultSubobject<USceneComponent>("Point10");
	ImmutablePoints.Add(Point10);
	Point10->SetupAttachment(GetRootComponent());
}
