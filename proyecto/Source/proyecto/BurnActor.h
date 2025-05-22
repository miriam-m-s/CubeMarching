#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BurnActor.generated.h" 

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class ABurnActor : public AActor
{
	GENERATED_BODY()

public:
	ABurnActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBurnFinished();

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Fire Expansion")
	void StartBurn();

	UFUNCTION(BlueprintCallable, Category = "Fire Expansion")
	void StopBurn();
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Fire Expansion")
	UStaticMeshComponent* MeshComponent;

	// Total time for the fire to finish expanding
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Expansion")
	float BurnTime = 5.0f;

	// Final scale factor applied to the curve (Y-axis scaling)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Expansion")
	float ScaleFactor = 3.0f;
	
	// Normalized curve (0–1 on X and Y)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Expansion")
	UCurveFloat* ExpansionCurve;

	// Expose to Blueprints and editable in editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* NiagaraTemplate;
	
private:
	float ElapsedTime = 0.0f;
	FVector InitialScale;
	
	bool bIsBurning = false;

	UNiagaraComponent* NiagaraComponent;
};
