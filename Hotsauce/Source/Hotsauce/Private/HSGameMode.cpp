#include "Hotsauce.h"
#include "HSGameMode.h"
#include "HSPlayerController.h"

AHSGameMode::AHSGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerControllerClass = AHSPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/MyCharacter"));
    if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

    static ConstructorHelpers::FClassFinder<AHUD> PlayerHUDBPClass(TEXT("/Game/Blueprints/MyHUD"));
    if (PlayerHUDBPClass.Class != NULL)
    {
        HUDClass = PlayerHUDBPClass.Class;
    }
}