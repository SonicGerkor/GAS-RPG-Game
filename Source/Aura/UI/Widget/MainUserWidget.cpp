// Copyright Gerkor Games

#include "MainUserWidget.h"

void UMainUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
