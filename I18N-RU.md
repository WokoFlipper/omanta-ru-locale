# Omanta i18n + Russian locale (ru_RU)

## English

This tree is Omanta (file manager for Omarchy) plus Qt Linguist localisation
support and a complete Russian translation.

### What was added (on top of upstream master)

- `CMakeLists.txt` — `find_package(Qt6 LinguistTools)`,
  `qt_add_translations(TARGETS omanta TS_FILES i18n/omanta_en_US.ts
  i18n/omanta_ru_RU.ts)`, `.qm` install to `share/omanta/translations/`.
- `src/LocaleManager.{h,cpp}` (new) — loads `omanta_<locale>.qm` by
  `QLocale::system()` from `/usr/share/omanta/translations`,
  `/usr/local/share/omanta/translations`, then `:/i18n`; stores the override
  in `QSettings "ui/locale"` (`""` = follow the system).
- `src/main.cpp` — creates `LocaleManager` before the QML engine loads and
  exposes it as the `LocaleManager` context property.
- `qml/PreferencesDialog.qml` — “Interface Language” combo (System default /
  English / Russian), applies instantly via `engine->retranslate()`, no
  restart needed.
- `qml/Sidebar.qml` — `displayPlaceName()` maps the stable English row IDs
  (Home, Documents, Recent, Starred, Network, Trash) through `qsTr()`;
  device/mount/bookmark names pass through untranslated (they are
  user/system names).
- `src/Location.cpp` — virtual-root labels via
  `QCoreApplication::translate("Location", …)`.
- `src/Platform.cpp` — relative dates (`Today`, `Yesterday`, `%n days ago`
  with proper plural forms) and the `Home` breadcrumb via `tr()`.
- `qml/Main.qml` — remaining hardcoded UI strings (header, tooltips,
  context-menu items) wrapped in `qsTr()`.
- `i18n/omanta_ru_RU.ts` — full Russian translation, 343 source strings.
- `i18n/omanta_en_US.ts` — American English stub next to it
  (source == translation).

### How language switching works

1. At startup the saved `ui/locale` value is read (`""` by default).
   Empty means “use `QLocale::system()`”, exactly the old behaviour.
2. The matching `omanta_ru_RU.qm` is loaded with `QTranslator` and installed
   **before** any QML is loaded, so the first frame is already translated.
3. Changing the combo in Preferences calls `setLocale()`: the old translator
   is removed, the new `.qm` loaded, `engine->retranslate()` refreshes every
   `qsTr()` binding live.

### Build

```bash
cmake -B build -DOMANTA_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
sudo cmake --install build   # binary + omanta_ru_RU.qm / omanta_en_US.qm
```

Regenerate translation templates after UI changes:

```bash
lupdate qml/ src/ -ts i18n/omanta_ru_RU.ts i18n/omanta_en_US.ts
```

### Note for the upstream author

This repo is offered to the Omanta project: happy to transfer ownership on
first request, or to reshape any part of it into a pull request — just say
the word (see upstream issue 28allday/omanta#25).

---

## Русский

Это дерево — Omanta (файловый менеджер для Omarchy) плюс поддержка
локализации Qt Linguist и полный русский перевод.

### Что добавлено (поверх upstream master)

- `CMakeLists.txt` — `find_package(Qt6 LinguistTools)`,
  `qt_add_translations(...)`, установка `.qm` в
  `share/omanta/translations/`.
- `src/LocaleManager.{h,cpp}` (новый) — грузит `omanta_<локаль>.qm` по
  `QLocale::system()` из `/usr/share/omanta/translations`,
  `/usr/local/share/omanta/translations`, затем `:/i18n`; выбор хранится в
  `QSettings "ui/locale"` (`""` — системная локаль).
- `src/main.cpp` — создаёт `LocaleManager` до загрузки QML и отдаёт его как
  контекст-свойство `LocaleManager`.
- `qml/PreferencesDialog.qml` — «Язык интерфейса» (Система / English /
  Русский), применяется мгновенно через `engine->retranslate()`, без
  перезапуска.
- `qml/Sidebar.qml` — `displayPlaceName()` маппит стабильные английские ID
  строк через `qsTr()`; имена устройств и закладок не переводятся (это имена,
  а не интерфейс).
- `src/Location.cpp` — подписи виртуальных корней через
  `QCoreApplication::translate("Location", …)`.
- `src/Platform.cpp` — относительные даты и крошка «Домашняя» через `tr()`,
  у `%n days ago` правильные формы множественного числа.
- `qml/Main.qml` — остатки хардкода обёрнуты в `qsTr()`.
- `i18n/omanta_ru_RU.ts` — полный русский перевод, 343 строки.
- `i18n/omanta_en_US.ts` — американская заглушка рядом
  (source == translation).

### Как работает переключение языка

1. На старте читается `ui/locale` (по умолчанию `""`).
   Пусто — значит системная локаль, как было раньше.
2. Подходящий `omanta_ru_RU.qm` грузится через `QTranslator` и ставится
   **до** загрузки QML — первый кадр уже переведён.
3. Смена в настройках: старый переводчик снимается, новый грузится,
   `engine->retranslate()` обновляет все `qsTr()` живьём.

### Сборка

```bash
cmake -B build -DOMANTA_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
sudo cmake --install build   # бинарь + omanta_ru_RU.qm / omanta_en_US.qm
```

Обновить шаблоны после изменения UI:

```bash
lupdate qml/ src/ -ts i18n/omanta_ru_RU.ts i18n/omanta_en_US.ts
```

### Автору апстрима

Репозиторий предлагается проекту Omanta: готов передать владение по первой
просьбе или переоформить любую часть в pull request — см. issue
28allday/omanta#25.
