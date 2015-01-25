#pragma once
#include "GameFramework/Character.h"
#include "HSCameraComponent.h"
#include "HSCharacter.generated.h"

UCLASS(Blueprintable)
class AHSCharacter : public ACharacter
{
	GENERATED_BODY()

    virtual void BeginPlay() override;

public:
    void Tick(float DeltaTime);

	AHSCharacter(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Stats)
    float Health;
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Stats)
    float HealthMax;


};

