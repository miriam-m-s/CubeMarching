#include "RayAcumalator.h"
#include "Algo/Sort.h" // Para ordenar arrays
#include "RayAcumalator.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include <Kismet/KismetSystemLibrary.h>
ARayAcumalator::ARayAcumalator()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARayAcumalator::BeginPlay()
{
	Super::BeginPlay();
}

void ARayAcumalator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARayAcumalator::AddToArray(float Distance, AActor* Actor,FVector ClosestPoint)
{
    FActorData NewData(Distance, Actor, ClosestPoint);
    SortedArray.Add(NewData);
}

void ARayAcumalator::SortArray()
{
	SortedArray.Sort([](const FActorData& A, const FActorData& B)
		{
			return A.Distance < B.Distance;
		});
}

AActor* ARayAcumalator::getActorByIndex(int index)
{
	if (SortedArray.IsValidIndex(index))
	{
		return SortedArray[index].Actor;
	}
	return nullptr;
}
float ARayAcumalator::getDistanceByIndex(int index)
{
    if (SortedArray.IsValidIndex(index))
    {
        return SortedArray[index].Distance;
    }
    return -1;
}

FVector ARayAcumalator::getPositionByIndex(int index)
{
    if (SortedArray.IsValidIndex(index))
    {
        return SortedArray[index].ClosestPoint;
    }
    return FVector::ZeroVector;
}


void ARayAcumalator::ClearArray()
{
	SortedArray.Empty();
}

FVector ARayAcumalator::GetClosestPointToPosition( AActor* TargetActor, const FVector& Position)
{
    if (!TargetActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("TargetActor is null."));
        return FVector::ZeroVector;
    }

    // Obtén el componente StaticMesh del Actor
    const UStaticMeshComponent* MeshComponent = TargetActor->FindComponentByClass<UStaticMeshComponent>();
    if (!MeshComponent || !MeshComponent->GetStaticMesh())
    {
        UE_LOG(LogTemp, Warning, TEXT("No valid StaticMeshComponent or StaticMesh found."));
        return FVector::ZeroVector;
    }
 

    // Obtén la posición local del mundo del TargetActor
    FVector ClosestPoint = FVector::ZeroVector;
    float ClosestDistance = FLT_MAX;

    // Itera sobre los vértices de la malla
    const FStaticMeshLODResources& LOD = MeshComponent->GetStaticMesh()->GetRenderData()->LODResources[0];
    const FPositionVertexBuffer& VertexBuffer = LOD.VertexBuffers.PositionVertexBuffer;

    for (uint32 VertexIndex = 0; VertexIndex < VertexBuffer.GetNumVertices(); ++VertexIndex)
    {
        // Convierte el vértice a coordenadas globales
        FVector3f LocalVertexPosition = VertexBuffer.VertexPosition(VertexIndex);
        FVector localPosition = FVector(double(LocalVertexPosition.X), double(LocalVertexPosition.Y), double(LocalVertexPosition.Z));
        FVector WorldVertexPosition = MeshComponent->GetComponentTransform().TransformPosition(localPosition);

        // distancia a ese punto
        float Distance = FVector::Dist(WorldVertexPosition, Position);

        // Actualiza el punto más cercano
        if (Distance < ClosestDistance)
        {
            ClosestDistance = Distance;
            ClosestPoint = WorldVertexPosition;
        }
    }
    AddToArray(ClosestDistance, TargetActor, ClosestPoint);
    return ClosestPoint;
}

UStaticMesh* ARayAcumalator::getStaticMesh(AActor* TargetActor)
{
    const UStaticMeshComponent* MeshComponent = TargetActor->FindComponentByClass<UStaticMeshComponent>();
    if (!MeshComponent || !MeshComponent->GetStaticMesh())
    {
        UE_LOG(LogTemp, Warning, TEXT("No valid StaticMeshComponent or StaticMesh found."));
        return nullptr;
    }

    return MeshComponent->GetStaticMesh();
}

bool ARayAcumalator::HasSamePosAsPrevious()
{
    FString mensaje = FString::Printf(TEXT("¡Hola! pos.Num: %d, SortedArray.Num: %d"), pos.Num(), SortedArray.Num());
    if (SortedArray.Num() != pos.Num()) {
        UKismetSystemLibrary::PrintString(
            this,                                  // Contexto: usualmente 'this' en clases que heredan de UObject
            mensaje,                        // Mensaje a imprimir
            true,                                  // bPrintToScreen: mostrar en pantalla
            true,                                  // bPrintToLog: mostrar en el log
            FLinearColor::Green,                   // Color del texto
            2.0f                                   // Duración en segundos
        );
        return false;
    } 
    int i = 0;
    while (i < SortedArray.Num() && SortedArray[i].ClosestPoint == pos[i])i++;
    return i == SortedArray.Num();
}

bool ARayAcumalator::UpdatePos()
{
    pos.Empty(); // Vacía el array
    
    for (int i = 0; i < SortedArray.Num(); i++) {
        pos.Add(SortedArray[i].ClosestPoint);
    }
    return false;
}

int ARayAcumalator::SortedArraySize()
{
    return SortedArray.Num();
}
