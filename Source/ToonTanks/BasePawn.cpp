// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectilePoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Point"));
	ProjectilePoint->SetupAttachment(TurretMesh);
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotator = ToTarget.Rotation();
	LookAtRotator.Pitch = 0.f;
	LookAtRotator.Roll = 0.f;
	TurretMesh->SetWorldRotation(LookAtRotator);
}

void ABasePawn::Fire()
{
	// DrawDebugSphere
    // (
    //     GetWorld(),
    //     ProjectilePoint->GetComponentLocation(),
    //     25.f,
    //     12,
    //     FColor::Red,
    //     false,
    //     3.f
    // );

	auto Projectile = GetWorld()->SpawnActor<AProjectile>
					(ProjectileClass,
					ProjectilePoint->GetComponentLocation(),
					ProjectilePoint->GetComponentRotation()
					);
	
	Projectile->SetOwner(this);
}

void ABasePawn::HandleDestraction()
{
	// UE_LOG(LogTemp, Warning, TEXT("BasePawn destroied!"));

	if (IsPlayerControlled())
	{
		SetActorHiddenInGame(true);
    	SetActorTickEnabled(false);
	}
	else
	{
		if (DeathSound)
			UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());

		if (DeathParticle)
			UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation(), GetActorRotation());

		if (DeathShakeClass)
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathShakeClass);

		Destroy();
	}
}