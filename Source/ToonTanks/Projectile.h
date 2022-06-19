// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

private:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditAnyWhere, Category = "Combat")
	float Damage = 25.f;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);

	UPROPERTY(EditAnyWhere, Category = "Combat")
	class UParticleSystem* HitParticle;

	UPROPERTY(VisibleAnyWhere, Category = "Combat")
	class UParticleSystemComponent* TrailParticleComponent;

	UPROPERTY(EditAnyWhere, Category = "Combat")
	class USoundBase* LaunchSound;

	UPROPERTY(EditAnyWhere, Category = "Combat")
	USoundBase* HitSound;

	UPROPERTY(EditAnyWhere, Category = "Combat")
	TSubclassOf<UCameraShakeBase> HitShakeClass;
};
