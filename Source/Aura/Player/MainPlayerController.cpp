// Copyright Gerkor Games

#include "MainPlayerController.h"

#include "DrawDebugHelpers.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Aura/Aura.h"
#include "Aura/MainGameplayTags.h"
#include "Components/SplineComponent.h"
#include "Aura/Input/MainInputComponent.h"
#include "Aura/AbilitySystem/MainAbilitySystemComponent.h"
#include "Aura/Actor/MagicCircle.h"
#include "Aura/Interaction/EnemyInterface.h"
#include "Aura/UI/Widget/DamageTextComponent.h"
#include "Components/DecalComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"

AMainPlayerController::AMainPlayerController()
{
	bReplicates = true;
	
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	check(MainContext);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MainContext, 0);	
	}
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AMainPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	CursorTrace();
	AutoRun();
	UpdateMagicCircleLocation();
}

void AMainPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (MagicCircleClass == nullptr) return;
	
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if (DecalMaterial)
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
		}
	}
}

void AMainPlayerController::HideMagicCircle()
{
	if (!IsValid(MagicCircle)) return;
	MagicCircle->Destroy();
}

void AMainPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, const bool bBlockedHit, const bool bCriticalHit)
{
	if (!IsValid(TargetCharacter) || !DamageTextComponentClass || !IsLocalController()) return;
	
	UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
	DamageText->RegisterComponent();
	DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
}

void AMainPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);
		
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AMainPlayerController::UpdateMagicCircleLocation()
{
	if (!IsValid(MagicCircle)) return;
	
	MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
}

UMainAbilitySystemComponent* AMainPlayerController::GetASC()
{
	if (MainAbilitySystemComponent == nullptr)
	{
		MainAbilitySystemComponent = Cast<UMainAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	
	return MainAbilitySystemComponent;
}

void AMainPlayerController::CursorTrace()
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FMainGameplayTags::Get().Player_Block_CursorTrace))
	{
		UnHighlightActor(LastActor);
		UnHighlightActor(ThisActor);
		LastActor = nullptr;
		ThisActor = nullptr;
		return;	
	}
	
	const ECollisionChannel TraceChannel = IsValid(MagicCircle) ? ECC_ExcludePlayers : ECC_Visibility;
	GetHitResultUnderCursor(TraceChannel, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	
	LastActor = ThisActor;
	if (IsValid(CursorHit.GetActor()) && CursorHit.GetActor()->Implements<UHighlightInterface>())
	{
		ThisActor = CursorHit.GetActor();
	}
	else
	{
		ThisActor = nullptr;
	}

	if (LastActor != ThisActor)
	{
		UnHighlightActor(LastActor);
		HighlightActor(ThisActor);
	}
}

void AMainPlayerController::AbilityInputTagPressed(const FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FMainGameplayTags::Get().Player_Block_InputPressed)) return;
	
	if (InputTag.MatchesTagExact(FMainGameplayTags::Get().InputTag_RMB))
	{
		if (IsValid(ThisActor))
		{
			TargetingStatus = ThisActor->Implements<UEnemyInterface>() ? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNonEnemy;
		}
		else
		{
			TargetingStatus = ETargetingStatus::NotTargeting;
		}
		bAutoRunning = false;
	}
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void AMainPlayerController::AbilityInputTagReleased(const FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FMainGameplayTags::Get().Player_Block_InputReleased)) return;
	
	if (!InputTag.MatchesTagExact(FMainGameplayTags::Get().InputTag_RMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}
	
	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
	
	if (TargetingStatus != ETargetingStatus::TargetingEnemy && !bShiftKeyDown)
	{
		if (const APawn* ControlledPawn = GetPawn(); ControlledPawn && FollowTime <= ShortPressThreshold)
		{
			if (IsValid(ThisActor) && ThisActor->Implements<UHighlightInterface>())
			{
				IHighlightInterface::Execute_SetMoveToLocation(ThisActor, CachedDestination);
			}
			else if (GetASC() && !GetASC()->HasMatchingGameplayTag(FMainGameplayTags::Get().Player_Block_InputReleased))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
			}
			if (UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavigationPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}
				if (NavigationPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavigationPath->PathPoints[NavigationPath->PathPoints.Num() - 1];
					bAutoRunning = true;
				}
			}
		}
		FollowTime = 0.f;
		TargetingStatus = ETargetingStatus::NotTargeting;
	}
}

void AMainPlayerController::AbilityInputTagHeld(const FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FMainGameplayTags::Get().Player_Block_InputHeld)) return;
	
	if (!InputTag.MatchesTagExact(FMainGameplayTags::Get().InputTag_RMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}
	
	if (TargetingStatus == ETargetingStatus::TargetingEnemy || bShiftKeyDown)
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;
		
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

void AMainPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FMainGameplayTags::Get().Player_Block_InputPressed)) return;
	
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UMainInputComponent* MainInputComponent = CastChecked<UMainInputComponent>(InputComponent);
	
	MainInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	MainInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ThisClass::ShiftPressed);
	MainInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ThisClass::ShiftReleased);
	MainInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AMainPlayerController::HighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		if (InActor) IHighlightInterface::Execute_HighlightActor(InActor);	
	}
}

void AMainPlayerController::UnHighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		if (InActor) IHighlightInterface::Execute_UnHighlightActor(InActor);	
	}
}
