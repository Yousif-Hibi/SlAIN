#include "C_QuestKnight.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include <Components/TextBlock.h>
#include "TimerManager.h"

AC_QuestKnight::AC_QuestKnight(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> QuestText(TEXT("/Game/Static/quests/WB_QuestText.WB_QuestText_C"));
	if (QuestText.Succeeded())
	{
		widget = QuestText.Class;
	}
}

void AC_QuestKnight::TalkToNPC()
{
	IInteractive_CI::TalkToNPC();

	DialogIndex++;
	if (Dialog.Num() > DialogIndex)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Dialog[DialogIndex]);
		ShowWidget(Dialog[DialogIndex]);
	}
	else
	{
		DialogIndex = 0;
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Dialog[DialogIndex]);
		ShowWidget(Dialog[DialogIndex]);
	}
}

void AC_QuestKnight::ShowWidget(FString str)
{
	if (UWorld* World = GetWorld())
	{
		// Remove the current widget if it exists
		if (CurrentWidget)
		{
			CurrentWidget->RemoveFromViewport();
			CurrentWidget = nullptr;
		}

		if (widget)
		{
			CurrentWidget = CreateWidget<UUserWidget>(World, widget);
			if (CurrentWidget)
			{
				CurrentWidget->AddToViewport();

				// Find the TextBlock in the widget
				UTextBlock* QuestTextBlock = Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(TEXT("questText")));
				if (QuestTextBlock)
				{
					QuestTextBlock->SetText(FText::FromString(str));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to find QuestTextBlock"));
				}

				// Set a timer to remove the widget after 5 seconds
				GetWorldTimerManager().SetTimer(WidgetTimerHandle, this, &AC_QuestKnight::RemoveCurrentWidget, 5.0f, false);
			}
		}
	}
}

void AC_QuestKnight::RemoveCurrentWidget()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
}

void AC_QuestKnight::BeginPlay()
{
	Super::BeginPlay();

	Dialog.Add(TEXT("You... you're not one of them, are you?"));
	Dialog.Add(TEXT("I see it in your eyes, the same emptiness I bear. You are Forsaken, just like me."));
	Dialog.Add(TEXT("Long ago, I had a family, a life, before the Iron Clan tore it all away."));
	Dialog.Add(TEXT("They wield their metallic powers with cruelty, crushing anyone who stands in their way."));
	Dialog.Add(TEXT("I vowed revenge, but without power, what can one like me do?"));
	Dialog.Add(TEXT("Yet, here you stand, perhaps together we can topple the Iron Conduit and avenge the fallen."));

}
