// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "TimerManager.h"
#include "ToonTanksPlayerController.h"

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
    // UE_LOG(LogTemp, Warning, TEXT("ActorDied in GameMode called"));

    if (DeadActor == Tank)
    {
        // UE_LOG(LogTemp, Warning, TEXT("Calling HandleDestraction for Tank"));

        Tank->HandleDestraction();
        
        if (ToonTanksPlayerController)
        {            
            ToonTanksPlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);   
    } 
    else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
    {
        // UE_LOG(LogTemp, Warning, TEXT("Calling HandleDestraction for Tower"));

        DestroyedTower->HandleDestraction();
        --TargetTowers;
        if (TargetTowers == 0)
        {
            GameOver(true);
        }
    }
}

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();

    HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
    TargetTowers = GetTargetTowerCount();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    StartGame();

    if (ToonTanksPlayerController)
    {
        ToonTanksPlayerController->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableTimerHandle;
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(ToonTanksPlayerController, &AToonTanksPlayerController::SetPlayerEnabledState, true);
        GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, TimerDelegate, StartDelay, false);
    }
}

int32 AToonTanksGameMode::GetTargetTowerCount()
{
    TArray<AActor*> TowerArray;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), TowerArray);
    return TowerArray.Num();
}