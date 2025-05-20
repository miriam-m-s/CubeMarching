// Fill out your copyright notice in the Description page of Project Settings.


#include "Clicker.h"

// Sets default values
AClicker::AClicker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AClicker::BeginPlay()
{
	Super::BeginPlay();
	Super::BeginPlay();
	EnableInput(GetWorld()->GetFirstPlayerController());
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;
	}
	if (InputComponent)
	{
		InputComponent->BindAction("LeftClick", IE_Pressed, this, &AClicker::HandleMouseClick);
	}
	
}

// Called every frame
void AClicker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AClicker::HandleMouseClick()
{
}

