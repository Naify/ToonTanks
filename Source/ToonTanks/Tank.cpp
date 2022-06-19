// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
// #include "DrawDebugHelpers.h"

ATank::ATank()
{

    ArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
    ArmComp->SetupAttachment(RootComponent);

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(ArmComp);    
};


// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (TankPlayerController)
    {
        FHitResult HitResult;

        TankPlayerController->GetHitResultUnderCursor
        (
            ECollisionChannel::ECC_Visibility,
            false,
            HitResult
        );

        // DrawDebugSphere
        // (
        //     GetWorld(),
        //     HitResult.ImpactPoint,
        //     25.f,
        //     12,
        //     FColor::Red,
        //     false,
        //     -1.f
        // );

        RotateTurret(HitResult.ImpactPoint);
        
    }
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
    AController* Controler = GetController();    
    TankPlayerController = Cast<APlayerController>(Controller);    
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::Move(float Value)
{
    // UE_LOG(LogTemp, Warning, TEXT("Value: %f"), Value);
    float Delta = UGameplayStatics::GetWorldDeltaSeconds(this);
    FVector DeltaLocation(0.f);
    DeltaLocation.X = Value * Delta * Speed;
    AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
    float Delta = UGameplayStatics::GetWorldDeltaSeconds(this);
    FRotator Rotator = FRotator::ZeroRotator;    
    Rotator.Yaw = Value * Delta * TurnRate;
    AddActorLocalRotation(Rotator, true);
}

void ATank::HandleDestrustion()
{
    UE_LOG(LogTemp, Warning, TEXT("Tank destroied!"));

    Super::HandleDestraction();

    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);

    bAlive = false;    
}