#include "Hotsauce.h"
#include "HSCharacter.h"
#include "UnrealNetwork.h"
#include "Engine.h"


AHSCharacter::AHSCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    MaxLevel = 20;

    Health = 100.0f;
    HealthMax = 100.0f;
    Mana = 100.f;
    ManaMax = 100.f;
    Stamina = 100.f;
    StaminaMax = 100.f;
    Experience = 0;
    Level = 1;

    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

    for (int32 level = 0; level <= MaxLevel; level++)
    {
        int32 PreviousLevel = 100 * level;
        int32 ThisLevel = 100 * (level + 1);
        ExperiencePerLevelArray.Add(PreviousLevel + ThisLevel);
    }
}

void AHSCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AHSCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AHSCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AHSCharacter, Health);
    DOREPLIFETIME(AHSCharacter, Mana);
    DOREPLIFETIME(AHSCharacter, Stamina);
    DOREPLIFETIME(AHSCharacter, Experience);
    DOREPLIFETIME(AHSCharacter, Level);
}

void AHSCharacter::GainExperience(int32 GainedExperience)
{
    Experience += GainedExperience;
    GainExperience();
    EventExperienceUpdate();
}

void AHSCharacter::GainExperience()
{
    if (Level >= MaxLevel)
    {
        return;
    }
    if (Experience > ExperiencePerLevelArray[Level - 1])
    {
        Level++;
        EventGainLevelUpdate();
        GetWorldTimerManager().SetTimer(this, &AHSCharacter::GainExperience, 1.0f, false);
    }
}

bool AHSCharacter::IsAlive()
{
    
    if (Health > 0)
    {
        return true;
    }
    return false;
}

void AHSCharacter::CharacterDeath(AActor* Killer)
{
    EventCharacterDeath(Killer);
}

float AHSCharacter::TakeDamage(float DamageAmount, const FDamageEvent &DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float Result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    Health = Health - DamageAmount;
    EventHealthUpdate();
    if (!IsAlive())
    {
        CharacterDeath(DamageCauser);
    }
    return Result;
}