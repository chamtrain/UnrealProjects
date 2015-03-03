#pragma once
#include "GameFramework/Character.h"
#include "HSCameraComponent.h"
#include "HSCharacter.generated.h"

UCLASS(Blueprintable)
class AHSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
    void Tick(float DeltaTime);

    virtual void BeginPlay() override;
	
    AHSCharacter(const FObjectInitializer& ObjectInitializer);

    int32 MaxLevel;
    UPROPERTY(BlueprintReadOnly, Category = Stats)
    TArray<int32> ExperiencePerLevelArray;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Stats)
    float Health;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    float HealthMax;
    UFUNCTION(BlueprintImplementableEvent, Category = "Stats Update Event")
    void EventHealthUpdate();

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Stats)
    float Mana;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    float ManaMax;
    UFUNCTION(BlueprintImplementableEvent, Category = "Stats Update Event")
    void EventManaUpdate();

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Stats)
    float Stamina;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    float StaminaMax;
    UFUNCTION(BlueprintImplementableEvent, Category = "Stats Update Event")
    void EventStaminaUpdate();


    UFUNCTION(BlueprintCallable, Category = Stats)
    void GainExperience(int32 GainedExperience);
    void GainExperience();
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Stats)
    int32  Experience;
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Stats)
    int32  Level;
    UFUNCTION(BlueprintImplementableEvent, Category = "Stats Update Event")
    void EventExperienceUpdate();
    UFUNCTION(BlueprintImplementableEvent, Category = "Stats Update Event")
    void EventGainLevelUpdate();

    bool IsAlive();

    virtual void CharacterDeath(AActor* Killer);

    UFUNCTION(BlueprintImplementableEvent, Category = "Death Event")
    void EventCharacterDeath(AActor* Killer);

    virtual float TakeDamage(float DamageAmount, const FDamageEvent &DamageEvent, AController* EventInstigator, AActor* DamageCauser);
};

