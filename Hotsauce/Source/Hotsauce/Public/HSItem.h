#pragma once

#include "GameFramework/Actor.h"
#include "HSItem.generated.h"


UCLASS(Blueprintable)
class HOTSAUCE_API AHSItem : public AActor
{
	GENERATED_BODY()
	
public:
	AHSItem();

	virtual void BeginPlay() override;
	
	virtual void Tick( float DeltaSeconds ) override;
	
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Item)
    FString Name;
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Item)
    int32 Charges;
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Item)
    bool IsUsable;
};
