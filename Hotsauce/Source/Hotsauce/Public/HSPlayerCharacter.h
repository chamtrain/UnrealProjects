#pragma once

#include "HSCharacter.h"
#include "HSPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class HOTSAUCE_API AHSPlayerCharacter : public AHSCharacter
{
	GENERATED_BODY()
	
public:
    void Tick(float DeltaTime);
    
    AHSPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	   
};
