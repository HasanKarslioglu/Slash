
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UNiagaraComponent;

enum class EItemState : uint8
{
	EIS_Hovering,
	ETS_Equipped,
};


UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;

protected:
	EItemState ItemState = EItemState::EIS_Hovering;

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintPure)
	float CalculatedSin();
	
	UFUNCTION(BlueprintPure)
	float CalculatedCos();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* ItemMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* Sphere;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> EmbersEffect;
	
private:
	float RunningTime = 0.f;
	UPROPERTY(EditAnywhere)
	float WaveLenght = 10.f;
	UPROPERTY(EditAnywhere)
	float Speed = 5.f;
};
