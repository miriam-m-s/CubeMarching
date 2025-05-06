#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RayAcumalator.generated.h"

// Definición del struct
USTRUCT(BlueprintType)
struct FActorData
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor Data")
		float Distance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor Data")
		AActor* Actor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor Data")
		FVector ClosestPoint;

	// Constructor por defecto
	FActorData() : Distance(0.0f), Actor(nullptr), ClosestPoint(FVector::ZeroVector) {}

	// Constructor personalizado
	FActorData(float InDistance, AActor* InActor, const FVector& InClosestPoint)
		: Distance(InDistance), Actor(InActor), ClosestPoint(InClosestPoint) {}
};

UCLASS()
class CPLUSPLUS3_API ARayAcumalator : public AActor
{
	GENERATED_BODY()

public:
	ARayAcumalator();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sorted Array")
		TArray<FActorData> SortedArray;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RayProperties")
		float DistanceRay;

	UFUNCTION(BlueprintCallable, Category = "Utilities")
		void AddToArray(float Distance, AActor* Actor, FVector ClosestPoint);

	UFUNCTION(BlueprintCallable, Category = "Sorted Array")
		void SortArray();

	UFUNCTION(BlueprintCallable, Category = "Utilities")
		AActor* getActorByIndex(int index);
	UFUNCTION(BlueprintCallable, Category = "Utilities")
		float getDistanceByIndex(int index);
	UFUNCTION(BlueprintCallable, Category = "Utilities")
		FVector getPositionByIndex(int index);
	
	UFUNCTION(BlueprintCallable, Category = "Utilities")
		void ClearArray();

	UFUNCTION(BlueprintCallable, Category = "Utilities")
		FVector GetClosestPointToPosition( AActor* TargetActor, const FVector& Position);
	UFUNCTION(BlueprintCallable, Category = "Utilities")
		UStaticMesh* getStaticMesh(AActor* TargetActor);
	UFUNCTION(BlueprintCallable, Category = "Utilities")
		bool HasSamePosAsPrevious();
	UFUNCTION(BlueprintCallable, Category = "Utilities")
		bool UpdatePos();
	UFUNCTION(BlueprintCallable, Category = "Utilities")
		int SortedArraySize();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
		TArray<UStaticMesh*> MeshArray;
	
protected:
	virtual void BeginPlay() override;
	TArray< FVector> pos;

public:
	virtual void Tick(float DeltaTime) override;
};
