//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "Rays.h"
//
//#include "RayAcumalator.h"
//#include "Algo/Sort.h" // Para ordenar arrays
//#include "RayAcumalator.h"
//#include "Components/StaticMeshComponent.h"
//#include "Kismet/KismetMathLibrary.h"
//Rays::ARayAcumalator()
//{
//    PrimaryActorTick.bCanEverTick = true;
//}
//
//void Rays::BeginPlay()
//{
//    Super::BeginPlay();
//}
//
//void Rays::Tick(float DeltaTime)
//{
//    Super::Tick(DeltaTime);
//}
//
//void Rays::AddToArray(float Distance, AActor* Actor, FVector ClosestPoint)
//{
//    FActorData NewData(Distance, Actor, ClosestPoint);
//    SortedArray.Add(NewData);
//}
//
//void Rays::SortArray()
//{
//    SortedArray.Sort([](const FActorData& A, const FActorData& B)
//        {
//            return A.Distance < B.Distance;
//        });
//}
//
//AActor* Rays::getActorByIndex(int index)
//{
//    if (SortedArray.IsValidIndex(index))
//    {
//        return SortedArray[index].Actor;
//    }
//    return nullptr;
//}
//float Rays::getDistanceByIndex(int index)
//{
//    if (SortedArray.IsValidIndex(index))
//    {
//        return SortedArray[index].Distance;
//    }
//    return -1;
//}
//
//FVector Rays::getPositionByIndex(int index)
//{
//    if (SortedArray.IsValidIndex(index))
//    {
//        return SortedArray[index].ClosestPoint;
//    }
//    return FVector::ZeroVector;
//}
//
//
//void Rays::ClearArray()
//{
//    SortedArray.Empty();
//}
//
//FVector Rays::GetClosestPointToPosition(AActor* TargetActor, const FVector& Position)
//{
//    if (!TargetActor)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("TargetActor is null."));
//        return FVector::ZeroVector;
//    }
//
//    // Obtén el componente StaticMesh del Actor
//    const UStaticMeshComponent* MeshComponent = TargetActor->FindComponentByClass<UStaticMeshComponent>();
//    if (!MeshComponent || !MeshComponent->GetStaticMesh())
//    {
//        UE_LOG(LogTemp, Warning, TEXT("No valid StaticMeshComponent or StaticMesh found."));
//        return FVector::ZeroVector;
//    }
//
//
//    // Obtén la posición local del mundo del TargetActor
//    FVector ClosestPoint = FVector::ZeroVector;
//    float ClosestDistance = FLT_MAX;
//
//    // Itera sobre los vértices de la malla
//    const FStaticMeshLODResources& LOD = MeshComponent->GetStaticMesh()->GetRenderData()->LODResources[0];
//    const FPositionVertexBuffer& VertexBuffer = LOD.VertexBuffers.PositionVertexBuffer;
//
//    for (uint32 VertexIndex = 0; VertexIndex < VertexBuffer.GetNumVertices(); ++VertexIndex)
//    {
//        // Convierte el vértice a coordenadas globales
//        FVector3f LocalVertexPosition = VertexBuffer.VertexPosition(VertexIndex);
//        FVector localPosition = FVector(double(LocalVertexPosition.X), double(LocalVertexPosition.Y), double(LocalVertexPosition.Z));
//        FVector WorldVertexPosition = MeshComponent->GetComponentTransform().TransformPosition(localPosition);
//
//        // distancia a ese punto
//        float Distance = FVector::Dist(WorldVertexPosition, Position);
//
//        // Actualiza el punto más cercano
//        if (Distance < ClosestDistance)
//        {
//            ClosestDistance = Distance;
//            ClosestPoint = WorldVertexPosition;
//        }
//    }
//    AddToArray(ClosestDistance, TargetActor, ClosestPoint);
//    return ClosestPoint;
//}
//
//UStaticMesh* Rays::getStaticMesh(AActor* TargetActor)
//{
//    const UStaticMeshComponent* MeshComponent = TargetActor->FindComponentByClass<UStaticMeshComponent>();
//    if (!MeshComponent || !MeshComponent->GetStaticMesh())
//    {
//        UE_LOG(LogTemp, Warning, TEXT("No valid StaticMeshComponent or StaticMesh found."));
//        return nullptr;
//    }
//
//    return MeshComponent->GetStaticMesh();
//}
