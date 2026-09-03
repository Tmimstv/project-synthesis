// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatTraceComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UCombatTraceComponent::UCombatTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false
	;

	// ...
}


// Called when the game starts
void UCombatTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCombatTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UCombatTraceComponent::ExecuteWeaponTrace(const FVector& TraceStart, const FVector& AimDirection)
{
	UWorld* World = GetWorld();
	AActor* Owner = GetOwner();

	if (!World || !Owner)
	{
		return false;
	}

	const FVector SafeDirection = AimDirection.GetSafeNormal();
	const FVector TraceEnd = TraceStart + (SafeDirection * TraceDistance);

	// Ensure the shooter cannot hit their own collision capsules
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);
	if (Owner->GetInstigator())
	{
		QueryParams.AddIgnoredActor(Owner->GetInstigator());
	}

	FHitResult HitResult;
	const bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		TraceChannel,
		QueryParams
	);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			AController* InstigatorController = Owner->GetInstigatorController();
			
			// Native Unreal Damage Pipeline
			UGameplayStatics::ApplyPointDamage(
				HitActor,
				BaseDamage,
				SafeDirection,
				HitResult,
				InstigatorController,
				Owner,
				DamageTypeClass
			);
		}

		if (bDrawDebugTraces)
		{
			DrawDebugLine(World, TraceStart, HitResult.ImpactPoint, FColor::Green, false, DebugLifeTime, 0, 1.5f);
			DrawDebugPoint(World, HitResult.ImpactPoint, 10.0f, FColor::Red, false, DebugLifeTime);
		}

		OnTargetHit.Broadcast(HitResult);
		return true;
	}

	// Trace missed (exceeded max distance)
	if (bDrawDebugTraces)
	{
		DrawDebugLine(World, TraceStart, TraceEnd, FColor::Orange, false, DebugLifeTime, 0, 1.0f);
	}

	OnTraceMiss.Broadcast(TraceEnd);
	return false;
}

