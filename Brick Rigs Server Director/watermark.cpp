/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     watermark.cpp					                              */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "watermark.h"
#include "functions.h"
#include "uibase.h"
#include "global.h"

bool watermark::InitalizeWaterMark()
{
	//Basic Init
	if (!global::World) return false;
	panel = GetCanvasPanel();
	if (!panel) return false;

	//Text Block/Border Init
	SDK::UBrickTextBlock* TextBlock = static_cast<SDK::UBrickTextBlock*>(SDK::UGameplayStatics::SpawnObject(SDK::UBrickTextBlock::StaticClass(), panel));
	if (!TextBlock) return false;
	#undef TEXT
	#define TEXT(text) SDK::UKismetTextLibrary::Conv_StringToText(SDK::FString(text))

	#ifdef _DEBUG
		TextBlock->SetText(TEXT(L"(DEV)Brick Rigs Server Director V1.0 (BRSD)"));
	#else
		TextBlock->SetText(TEXT(L"Brick Rigs Server Director V1.0 (BRSD)"));
	#endif // _DEBUG

	TextBlock->SetColorStyle(SDK::EBrickUIColorStyle::Default);
	TextBlock->SetTextStyle(SDK::EBrickUITextStyle::Bold);
	TextBlock->SetStyleState(SDK::EBrickUIStyleState::Foreground);


	TextBorder = static_cast<SDK::UBrickBorder*>(SDK::UGameplayStatics::SpawnObject(SDK::UBrickBorder::StaticClass(), panel));
	if (!TextBorder) return false;
	TextBorder->SetContent(TextBlock);
	SDK::UCanvasPanelSlot* slot2 = panel->AddChildToCanvas(TextBorder);
	slot2->SetAutoSize(true);
	SDK::FAnchors anchor = SDK::FAnchors();
	anchor.Maximum = SDK::FVector2D(1.0f, 0.0f);
	anchor.Minimum = SDK::FVector2D(1.0f, 0.0f);
	slot2->SetAnchors(anchor);  // Top-Right Align
	// 2. Align to the top-right of the widget
	slot2->SetAlignment(SDK::FVector2D(1.f, 0.f));

	// 3. Offset inward from top-right screen corner
	slot2->SetPosition(SDK::FVector2D(-20.f, 20.f));  // x: 20px left, y: 20px down
	SynchronizeProperties(TextBorder);

	return true;
}

void watermark::ShowWaterMark()
{
	TextBorder->SetVisibility(SDK::ESlateVisibility::Visible);
}

void watermark::HideWaterWark()
{
	TextBorder->SetVisibility(SDK::ESlateVisibility::Hidden);
}

void watermark::UnInitalizeWaterMark()
{
	if (TextBorder) TextBorder->RemoveFromParent();
	TextBorder = nullptr;
	panel = nullptr;
}
