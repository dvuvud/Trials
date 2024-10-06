
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GetHitInterface.generated.h"

UINTERFACE(MinimalAPI)
class UGetHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TRIALS_API IGetHitInterface
{
	GENERATED_BODY()

public:
	virtual void GetHit(const FVector& ImpactPoint) = 0;
	virtual void GetHitMagic(const FVector& ImpactPoint) = 0;
};
