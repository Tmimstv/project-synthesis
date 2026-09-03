// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/HitResult.h"
#include "CombatTraceComponent.generated.h"

// Dynamic multicast delegates so Blueprints/VFX can subscribe to trace outcomes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatTraceHitSignature, const FHitResult&, HitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatTraceMissSignature, const FVector&, TraceEndLocation);


UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class PROJECTSYNTH_API UCombatTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatTraceComponent();
	
	/** Executes a single raycast query, applies damage if applicable, and broadcasts delegates */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool ExecuteWeaponTrace(const FVector& TraceStart, const FVector& AimDirection);

	/** Fired when a trace connects with blocking geometry or a valid damageable target */
	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FOnCombatTraceHitSignature OnTargetHit;

	/** Fired when a trace exceeds maximum range without striking a blocking channel */
	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FOnCombatTraceMissSignature OnTraceMiss;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	/** Maximum range of the hitscan trace in Unreal Units (centimeters) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Config")
	float TraceDistance = 10000.0f;

	/** Raw point damage applied to the struck actor */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Config")
	float BaseDamage = 25.0f;

	/** Collision channel checked for blocking hits */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Config")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	/** Optional damage type class passed to UGameplayStatics */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Config")
	TSubclassOf<UDamageType> DamageTypeClass;

	/** Visually draw line queries and hit locations in the editor */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Debug")
	bool bDrawDebugTraces = false;

	/** Duration in seconds that debug geometry remains visible */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Debug")
	float DebugLifeTime = 1.5f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
