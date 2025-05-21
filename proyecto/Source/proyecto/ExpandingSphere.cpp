#include "C:\Users\miria\OneDrive\Escritorio\Documentos\Unreal Projects\CubeMarching\proyecto\Intermediate\Build\Win64\x64\proyectoEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.Cpp20.h"
#include "ExpandingSphere.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"

AExpandingSphere::AExpandingSphere()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;

	SphereComponent->InitSphereRadius(10.0f);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AExpandingSphere::OnOverlapBegin);

	CurrentRadius = 10.0f;
}

void AExpandingSphere::BeginPlay()
{
	Super::BeginPlay();
}

void AExpandingSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentRadius < MaxRadius)
	{
		CurrentRadius += ExpansionRate * DeltaTime;
		SphereComponent->SetSphereRadius(CurrentRadius);
		DrawDebugSphere(GetWorld(), GetActorLocation(), CurrentRadius, 16, FColor::Red, false, -1, 0, 1.0f);
	}
}

void AExpandingSphere::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
									  bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Colisión con %s"), *OtherActor->GetName());
	}
}
