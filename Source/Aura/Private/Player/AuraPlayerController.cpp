// Copyright Druid Mechanics


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"

#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"

#include "Aura/Aura.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"

#include "GameFramework/Character.h"


AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

// Инициализация контроллера при старте игры
void AAuraPlayerController::BeginPlay()
{
    Super::BeginPlay(); 
    // Вызываем родительскую реализацию BeginPlay() из APlayerController
    // Это важно, чтобы базовая логика контроллера (репликация, Input, Pawn и т.д.) корректно инициализировалась

    check(AuraContext); 
    // Проверка корректности указателя на контекст ввода (AuraContext)
    // Если указатель nullptr, игра аварийно завершится с ошибкой
    // Это защита от некорректной настройки проекта

    // Получаем подсистему улучшенного ввода для локального игрока
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (Subsystem)
    {
        // Добавляем контекст ввода AuraContext с приоритетом 0
        // Контекст ввода содержит привязки действий и осей (например, движение, клики, способности)
        // Приоритет 0 означает стандартный приоритет, выше можно давать спец-контекстам (например, меню)
        Subsystem->AddMappingContext(AuraContext, 0);
    }

    bShowMouseCursor = true; 
    // Включаем отображение мышиного курсора на экране
    // Без этого курсор был бы невидим для игрока

    DefaultMouseCursor = EMouseCursor::Default; 
    // Задаем стандартный вид курсора (стрелка)
    // Можно менять на кастомный вид для спецэффектов, но здесь используем стандартный

    // Настройка режима ввода для игры и UI
    FInputModeGameAndUI InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); 
    // Не блокируем курсор внутри игрового окна
    // Это важно, чтобы мышь могла свободно перемещаться между UI и игровым миром

    InputModeData.SetHideCursorDuringCapture(false); 
    // Курсор остаётся видимым даже при захвате ввода
    // Если бы true — курсор пропадал бы при взаимодействии с миром

    SetInputMode(InputModeData); 
    // Применяем настройки ввода к контроллеру
    // Это активирует режим GameAndUI с вышеописанными параметрами
}

