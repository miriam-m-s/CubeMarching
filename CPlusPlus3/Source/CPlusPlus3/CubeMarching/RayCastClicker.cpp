// Fill out your copyright notice in the Description page of Project Settings.


#include "RayCastClicker.h"

// Sets default values
ARayCastClicker::ARayCastClicker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARayCastClicker::BeginPlay()
{
	Super::BeginPlay();
	EnableInput(GetWorld()->GetFirstPlayerController());

	if (InputComponent)
	{
		InputComponent->BindAction("LeftClick", IE_Pressed, this, &ARayCastClicker::HandleMouseClick);
	}
	
}

// Called every frame
void ARayCastClicker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ARayCastClicker::HandleMouseClick()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	FVector WorldLocation, WorldDirection;
	if (PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		FVector Start = WorldLocation;
		FVector End = Start + WorldDirection * 10000.0f;

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.bTraceComplex = true;
		Params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
		{
			// if (!TargetTag.IsNone() && !HitResult.GetActor()->ActorHasTag(TargetTag))
			// 	return;

			FVector HitLocation = HitResult.ImpactPoint;
			UE_LOG(LogTemp, Warning, TEXT("¡Colisión en: %s"), *HitLocation.ToString());
			DrawDebugSphere(GetWorld(), HitLocation, 20.0f, 12, FColor::Green, false, 2.0f);
		}
	}
}

