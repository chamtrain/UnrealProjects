#include "Hotsauce.h"
#include "HSInventoryComponent.h"
#include "UnrealNetwork.h"
#include "Engine.h"

UHSInventoryComponent::UHSInventoryComponent()
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;

    SetIsReplicated(true);
}

void UHSInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
}

void UHSInventoryComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

}

void UHSInventoryComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UHSInventoryComponent, Inventory);
    DOREPLIFETIME(UHSInventoryComponent, Equipment);
}