# Traversal — Unreal Engine 5 Gameplay Project

**Traversal** — учебно-практический проект на Unreal Engine 5, разработанный на C++ с использованием Blueprint. Основная цель проекта — изучение и реализация игровых механик, архитектуры игровых систем и Gameplay Ability System (GAS).

Проект включает систему перемещения персонажа (Traversal), боевую систему и работу с Gameplay Effects, Attributes и Gameplay Abilities.

## Технологии

- Unreal Engine 5.5
- C++
- Blueprint
- Gameplay Ability System (GAS)
- Gameplay Effects
- Gameplay Tags
- Attribute Sets
- Motion Warping
- Root Motion
- Gameplay Ability Execution Calculations
- Actor Components

---

# 1. Traversal System

Система расширенного перемещения персонажа с использованием GAS и Motion Warping.

### Реализовано

- Vaulting — преодоление невысоких препятствий.
- Mantling — взбирание на более высокие поверхности.
- Определение препятствий с помощью трассировок.
- Проверка возможности выполнения перемещения.
- Использование Gameplay Ability для управления действиями.
- Интеграция Animation Montage.
- Использование Root Motion.
- Интеграция Motion Warping для адаптации анимации к положению препятствия.
- Настройка параметров перемещения через данные.
- Разделение логики перемещения и боевой системы.

### Основные параметры

- Максимальная высота препятствия для Vaulting.
- Максимальная глубина препятствия.
- Максимальная высота для Mantling.
- Дистанция обнаружения препятствий.
- Дистанция активации способности.
- Ограничения по скорости персонажа.

### Gameplay Tags

- `Ability.Traversal.Vault`
- `Ability.Traversal.Mantle`

---

# 2. Combat System

Боевая система построена на Gameplay Ability System.

Основная задача — создать расширяемую архитектуру, позволяющую добавлять различные типы атак, способностей и боевых эффектов без жёсткой зависимости между системами.

### Реализовано

- Интеграция Ability System Component в персонажа.
- Реализация собственного Attribute Set.
- Система Health и Mana.
- Система регенерации маны через Gameplay Effect.
- Система бонусов к урону.
- Система сопротивления огню.
- Разделение атакующей логики и характеристик персонажа.
- Использование Gameplay Effects для изменения характеристик.
- Использование Gameplay Tags для идентификации способностей и состояний.
- Базовая система выдачи и активации Gameplay Abilities.

### Архитектура

- `ATraversalCharacter` — основной игровой персонаж.
- `UAtackComponent` — управление активацией атакующих способностей.
- `UCombatAttributeSet` — характеристики персонажа.
- `UAbilitySystemComponent` — управление Gameplay Abilities и Gameplay Effects.
- `UStatusEffectComponent` — отдельный компонент для обработки негативных и положительных состояний.

---

# 3. Fireball Ability

Первая реализованная атакующая способность на базе GAS. Используется как основа для дальнейшего развития боевой системы.

### Реализовано

- Создание Gameplay Ability `GA_Fireball`.
- Активация способности через собственный `UAtackComponent`.
- Использование Gameplay Effect для стоимости способности.
- Система расхода маны.
- Система перезарядки способности.
- Создание и запуск снаряда.
- Использование Projectile Movement Component.
- Настройка скорости и времени жизни снаряда.
- Передача параметров снаряда через Data Asset.
- Обработка столкновений с объектами.
- Получение Ability System Component атакующего и цели.
- Создание Gameplay Effect Spec.
- Передача урона через SetByCaller.
- Использование Execution Calculation для расчёта итогового урона.

### Формула урона

Урон рассчитывается с учётом бонуса атакующего и сопротивления цели:

`FinalDamage = BaseDamage * (1 + FireDamageBonus) * (1 - FireResistance / 100)`

Таким образом, урон зависит как от характеристик атакующего, так и от сопротивления цели.

### Gameplay Effects

- `GE_FireballCost` — расход маны.
- `GE_FireballCooldown` — перезарядка способности.
- `GE_FireballDamage` — нанесение урона через Execution Calculation.
- `GE_DamageBonus` — увеличение бонуса огненного урона.

### Data Asset

`DA_Fireball` содержит основные параметры способности:

- Base Damage.
- Projectile Speed.
- Projectile Lifetime.
- Damage Effect.
- Burn Buildup Effect.
- Burn Buildup Decay Effect.

---

# 4. Damage Execution System

Для расчёта урона используется собственный класс `UDamageExecution`.

### Реализовано

- Использование `UGameplayEffectExecutionCalculation`.
- Получение характеристик атакующего и цели через Attribute Capture.
- Получение FireDamageBonus от источника урона.
- Получение FireResistance от получателя урона.
- Расчёт итогового урона.
- Передача результата в IncomingDamage.
- Обработка полученного урона в `UCombatAttributeSet`.
- Уменьшение Health.
- Обработка смерти персонажа при достижении нулевого здоровья.

### Поддерживаемые характеристики

- Health.
- MaxHealth.
- Mana.
- MaxMana.
- FireResistance.
- FireDamageBonus.
- PhysicalDamageBonus.
- BurnBuildup.
- IncomingDamage.

---

# 5. Burning System

Система накопления огненного воздействия и последующего поджигания цели.

Архитектура предусматривает независимость получения негативных эффектов от наличия у персонажа атакующих способностей.

### Реализовано

- Атрибут `BurnBuildup`.
- Накопление огненного воздействия при попадании Fireball.
- Gameplay Effect `GE_FireBuildup`.
- Уменьшение накопленного воздействия со временем.
- Gameplay Effect `GE_BurnBuildupDecay`.
- Обновление длительности эффекта затухания при повторном попадании.
- Отслеживание изменений BurnBuildup через Attribute Change Delegate.
- Создание отдельного `UStatusEffectComponent`.
- Инициализация компонента через Ability System Component.
- Проверка FireResistance при попытке поджога.
- Расчёт вероятности поджога на основе сопротивления.
- Расчёт длительности горения на основе сопротивления.
- Создание `GE_Burning` с динамической длительностью.
- Передача базового периодического урона через SetByCaller.
- Передача ASC атакующего как источника эффекта.

### Логика сопротивления огню

Сопротивление влияет на вероятность поджога и длительность горения.

`IgniteChance = 1 - FireResistance / 100`

`BurnDuration = BaseDuration * (1 - FireResistance / 100)`

Базовая длительность горения — 6 секунд.

Базовый периодический урон — 5 единиц в секунду. Итоговый урон дополнительно рассчитывается через `UDamageExecution` с учётом сопротивления цели.

### В разработке

- Завершение и отладка применения `GE_Burning`.
- Проверка периодического выполнения Execution Calculation.
- Проверка корректного уменьшения Health во время горения.
- Обработка повторного поджога.
- Сброс или повторное накопление BurnBuildup после неудачной попытки поджога.
- Ограничение BurnBuildup диапазоном 0–100.
- Проверка взаимодействия нескольких источников огненного урона.

---

# 6. Status Effect Component

`UStatusEffectComponent` предназначен для обработки состояний персонажа независимо от его атакующих возможностей.

### Реализовано

- Создание отдельного Actor Component.
- Подключение к Ability System Component.
- Получение Combat Attribute Set.
- Отслеживание изменения BurnBuildup.
- Проверка возможности поджога.
- Расчёт параметров горения.
- Применение Gameplay Effect.

### Планируется

- Burning.
- Frozen.
- Stunned.
- Poisoned.
- Slowed.
- Система иммунитетов к негативным эффектам.
- Расширяемая обработка взаимодействий между состояниями.

---

# 7. Planned Features

Дальнейшее развитие проекта:

### Combat

- IceBall Ability.
- Sword Attack Ability.
- Дополнительные типы урона.
- Расширение системы сопротивлений.
- Комбинации способностей.
- Взаимодействие между различными статусными эффектами.

### GAS

- Gameplay Cues.
- Gameplay Tasks.
- Target Data.
- SetByCaller для дополнительных параметров способностей.
- Расширение системы Gameplay Tags.
- Репликация способностей и эффектов.
- Улучшение управления состояниями персонажа.

### Visuals & Animation

- Анимации атак.
- Анимации применения способностей.
- Визуальные эффекты Fireball.
- Niagara Effects для огненного воздействия.
- Звуковое сопровождение способностей.
- Визуальная обратная связь для Burning, Frozen и других состояний.
- Отображение характеристик и состояний в HUD.

### Architecture

- Дальнейшее разделение игровых систем на независимые компоненты.
- Улучшение масштабируемости боевой системы.
- Уменьшение зависимостей между компонентами.
- Использование Data Assets для настройки новых способностей и эффектов.

---

# Project Status

Проект находится в активной разработке.

На текущем этапе реализованы базовые механики Traversal, интеграция GAS, система характеристик, Fireball Ability, расчёт урона и накопление огненного воздействия.

Основной фокус дальнейшей разработки — завершение Burning System и создание расширяемой боевой системы с использованием Gameplay Ability System.

**Development focus:** C++ Gameplay Programming, Unreal Engine 5, GAS, Gameplay Architecture.
# 8. System Interfaces & Communication

В проекте используются интерфейсы Unreal Engine, Actor Components, Gameplay Ability System и Gameplay Tags для взаимодействия между независимыми игровыми системами.

Основная задача архитектуры — минимизировать прямые зависимости между компонентами и обеспечить возможность повторного использования игровых механик.

## Gameplay Framework Interfaces

### IAbilitySystemInterface

Используется для предоставления доступа к Ability System Component через стандартный интерфейс GAS.

Реализован в:
- `ATraversalCharacter`
- `ABaseEnemy`

Основные задачи:
- Предоставление ASC другим игровым системам.
- Получение Ability System Component без необходимости прямой зависимости от конкретного класса персонажа.
- Взаимодействие с Gameplay Abilities и Gameplay Effects.

Используется при:
- Активации способностей.
- Получении характеристик цели.
- Создании и применении Gameplay Effect Spec.
- Передаче урона между персонажами.

## Gameplay Ability System Communication

### UAbilitySystemComponent

Центральный компонент взаимодействия с GAS.

Используется для:
- Инициализации Actor Info.
- Выдачи Gameplay Abilities.
- Активации способностей.
- Создания Gameplay Effect Spec.
- Применения Gameplay Effects.
- Управления Gameplay Tags.
- Отслеживания изменений атрибутов.

### Gameplay Attribute Change Delegate

Используется для отслеживания изменений характеристик без необходимости постоянного Tick.

Пример использования:

`GetGameplayAttributeValueChangeDelegate()`

В проекте применяется для отслеживания `BurnBuildup` в `UStatusEffectComponent`.

При изменении значения компонент получает новые данные и проверяет возможность активации состояния Burning.

### Gameplay Tags

Используются для идентификации способностей, состояний, типов урона и перезарядок.

Основные категории:

| Категория | Назначение |
|---|---|
| `Ability.*` | Идентификация способностей |
| `State.*` | Текущие состояния персонажа |
| `Status.*` | Наложенные эффекты и состояния |
| `Damage.*` | Типы наносимого урона |
| `Cooldown.*` | Перезарядки способностей |
| `GameplayCue.*` | Визуальные и звуковые события |
| `Data.*` | Передача дополнительных параметров |

## Actor Component Communication

### UAtackComponent

Компонент управления атакующими способностями.

Взаимодействует с:
- `UAbilitySystemComponent`
- `UGameplayAbility`
- `ATraversalCharacter`

Основные задачи:
- Получение ASC владельца.
- Выдача способностей.
- Активация атакующих способностей.

### UStatusEffectComponent

Компонент управления состояниями персонажа.

Взаимодействует с:
- `UAbilitySystemComponent`
- `UCombatAttributeSet`
- `UGameplayEffect`
- Gameplay Attribute Change Delegate

Основные задачи:
- Отслеживание изменений характеристик, связанных с состояниями.
- Проверка условий активации эффектов.
- Расчёт параметров статусов.
- Применение Gameplay Effects.

Компонент не зависит от `UAtackComponent`, поэтому получение статусов не требует наличия атакующей системы.

## Projectile Communication

### AFireballProjectile

Снаряд взаимодействует с игровыми объектами через систему столкновений Unreal Engine.

При попадании:
1. Определяет объект столкновения.
2. Получает ASC источника и цели через `IAbilitySystemInterface`.
3. Создаёт Gameplay Effect Context.
4. Создаёт Gameplay Effect Spec.
5. Передаёт параметры урона через SetByCaller.
6. Применяет эффекты к ASC цели.
7. Передаёт источник огненного воздействия в `UStatusEffectComponent`.

### Gameplay Effect Context

Используется для передачи контекста применения эффекта.

В проекте применяется для:
- Определения источника урона.
- Передачи Source Object.
- Сохранения информации об атакующем при создании Gameplay Effect.

## Attribute System Communication

### UCombatAttributeSet

Центральное хранилище боевых характеристик.

Взаимодействует с:
- `UAbilitySystemComponent`
- `UDamageExecution`
- Gameplay Effects
- `UStatusEffectComponent`

Основные задачи:
- Хранение характеристик.
- Контроль изменения атрибутов.
- Обработка полученного урона.
- Обработка смерти персонажа.
- Ограничение допустимых значений характеристик.

### UDamageExecution

Реализует расчёт итогового урона через `UGameplayEffectExecutionCalculation`.

Получает:
- `FireDamageBonus` от источника.
- `FireResistance` от цели.
- Базовый урон через SetByCaller.

Возвращает рассчитанный результат в систему атрибутов цели.

## Data-Driven Configuration

### UProjectileAbilityDataAsset

Используется для хранения параметров снаряда и связанных Gameplay Effects.

Позволяет настраивать характеристики способности без изменения C++ логики.

Содержит:
- Base Damage.
- Projectile Speed.
- Projectile Lifetime.
- Damage Effect.
- Burn Buildup Effect.
- Burn Buildup Decay Effect.

### Blueprint Gameplay Ability

`BP_GA_Fireball` используется для настройки параметров `UGA_Fireball` в редакторе Unreal Engine.

В частности, назначаются Gameplay Effects стоимости и перезарядки способности.

---

## Communication Flow

### Fireball → Damage → Burning

```text
ATraversalCharacter
       |
       v
 UAtackComponent
       |
       v
   UAbilitySystemComponent
       |
       v
    GA_Fireball
       |
       v
 AFireballProjectile
       |
       +----------------------+
       |                      |
       v                      v
   Damage Effect         Burn Buildup
       |                      |
       v                      v
   UDamageExecution      UCombatAttributeSet
       |                      |
       v                      v
   Health Reduction     Attribute Change Delegate
                              |
                              v
                     UStatusEffectComponent
                              |
                              v
                         GE_Burning
                              |
                              v
                      Periodic Damage
                              |
                              v
                        Health Reduction
```

*Примечание: схема отражает целевую цепочку взаимодействия. Периодический урон и завершение применения `GE_Burning` ещё находятся в стадии отладки.*

---

## Architectural Principles

- Разделение атакующих способностей и получаемых состояний.
- Использование GAS для управления способностями, характеристиками и эффектами.
- Использование интерфейсов для доступа к игровым системам без жёсткой привязки к конкретным классам.
- Использование Actor Components для разделения ответственности.
- Использование Gameplay Tags для идентификации игровых состояний и событий.
- Использование Data Assets для настройки параметров способностей.
- Использование Attribute Delegates вместо постоянного Tick там, где это возможно.