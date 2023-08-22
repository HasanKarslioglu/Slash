
#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"


UCLASS()
class SLASH_API ATreasure : public AItem
{
	GENERATED_BODY()

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> PickupSound;

	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	int32 Gold;
};
