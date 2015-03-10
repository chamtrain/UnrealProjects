#include "Hotsauce.h"
#include "HSItem.h"
#include "UnrealNetwork.h"
#include "Engine.h"

AHSItem::AHSItem()
{
	
    Name = "Unknown";
    Charges = -1;
    IsUsable = false;

    PrimaryActorTick.bCanEverTick = true;
    SetReplicates(true);
}

void AHSItem::BeginPlay()
{
	Super::BeginPlay();
}

void AHSItem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AHSItem::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AHSItem, Name);
    DOREPLIFETIME(AHSItem, Charges);
    DOREPLIFETIME(AHSItem, IsUsable);
}
