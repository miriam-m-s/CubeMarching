#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExpandingSphere.generated.h"

class USphereComponent;

UCLASS()
class PROYECTO_API AExpandingSphere : public AActor
{
	GENERATED_BODY()
	
public:	
	AExpandingSphere();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;

	float CurrentRadius;
	UPROPERTY(EditAnywhere)
	float ExpansionRate = 100.0f; // Units per second

	UPROPERTY(EditAnywhere)
	float MaxRadius = 500.0f;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);
};
