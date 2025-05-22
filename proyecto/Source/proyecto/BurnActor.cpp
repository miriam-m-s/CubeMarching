#include "BurnActor.h"
#include "Components/StaticMeshComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

ABurnActor::ABurnActor()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

void ABurnActor::BeginPlay()
{
	Super::BeginPlay();

	if (!NiagaraTemplate) return;
	
	FVector SpawnLocation = FVector(MeshComponent->GetComponentLocation().X, MeshComponent->GetComponentLocation().Y, 0.0f);
	
	// Spawn and store the system
	NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		MeshComponent->GetWorld(),
		NiagaraTemplate,
		SpawnLocation,
		FRotator::ZeroRotator,
		FVector(1.0f),
		true, // bAutoDestroy
		true, // bAutoActivate
		ENCPoolMethod::None,
		true  // bPreCullCheck
	);

	float BoundsSize = 10000.0f;
	NiagaraComponent->SetSystemFixedBounds(FBox(FVector(-BoundsSize, -BoundsSize, -BoundsSize), FVector(BoundsSize, BoundsSize, BoundsSize)));
}

void ABurnActor::Tick(float DeltaTime)
{
	if (!bIsBurning || !ExpansionCurve || ElapsedTime > BurnTime)
		return;

	ElapsedTime += DeltaTime;

	if (ElapsedTime >= BurnTime)
	{
		StopBurn();
		return;
	}

	// Normalize time to [0,1]
	float NormalizedTime = FMath::Clamp(ElapsedTime / BurnTime, 0.0f, 1.0f);
    
	// Get normalized value from curve (0–1) and scale it
	float NormalizedGrowth = ExpansionCurve->GetFloatValue(NormalizedTime);
	float FinalScale = FMath::Lerp(1.0f, ScaleFactor, NormalizedGrowth);

	if (MeshComponent)
	{
		MeshComponent->SetWorldScale3D(InitialScale * FinalScale);

		if (NiagaraComponent) // Update Niagara Size
		{
			// Get local-space bounds
			FVector Origin;
			FVector BoxExtent;
			MeshComponent->GetLocalBounds(Origin, BoxExtent);

			FVector LocalSize = BoxExtent * 2.0f;
			FVector WorldScale = MeshComponent->GetComponentTransform().GetScale3D();

			// Compute real world size
			FVector WorldSize = LocalSize * WorldScale;
			
			NiagaraComponent->SetVariableVec2(FName("User.GridSize"), FVector2d(WorldSize.X, WorldSize.Y));
		}
		
	}
	
	Super::Tick(DeltaTime);
}

void ABurnActor::StartBurn()
{
	if (!MeshComponent) return;

	InitialScale = MeshComponent->GetRelativeScale3D();
	ElapsedTime = 0.0f;
	bIsBurning = true;
}

void ABurnActor::StopBurn()
{
	bIsBurning = false;
	OnBurnFinished();
}
