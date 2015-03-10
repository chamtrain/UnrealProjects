#pragma once
#include "Components/ActorComponent.h"
#include "HSItem.h"
#include "HSInventoryComponent.generated.h"


UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HOTSAUCE_API UHSInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHSInventoryComponent();

	virtual void InitializeComponent() override;
	
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Items)
    TArray<AHSItem*> Inventory;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Items)
    TArray<AHSItem*> Equipment;
};
