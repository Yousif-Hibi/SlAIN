// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UOptionUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    
protected:
    UPROPERTY(meta = (BindWidget))
    class UButton* HealthLess;

    UPROPERTY(meta = (BindWidget))
    class UButton* HealthPluse;

    UPROPERTY(meta = (BindWidget))
    class UButton* StaminaLess;

    UPROPERTY(meta = (BindWidget))
    class UButton* StaminaPluse;

    UPROPERTY(meta = (BindWidget))
    class UButton* DamageLess;

    UPROPERTY(meta = (BindWidget))
    class UButton* DamagePluse;

    UPROPERTY(meta = (BindWidget))
    class UButton* BackButton;
    UPROPERTY()
    class  ASoulsLikeCharacter* PlayerCharacter;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Points = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 HealthPoints = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StaminaPoints = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DamagePoints = 0;
    UFUNCTION()
    void OnButtonClick();
    UFUNCTION()
    void OnHealthLessClicked();

    UFUNCTION()
    void OnHealthPluseClicked();

    UFUNCTION()
    void OnStaminaLessClicked();

    UFUNCTION()
    void OnStaminaPluseClicked();

    UFUNCTION()
    void OnDamageLessClicked();

    UFUNCTION()
    void OnDamagePluseClicked();

    UFUNCTION()
    void OnBackButtonClicked();
};
