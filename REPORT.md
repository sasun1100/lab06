# Отчёт по лабораторной работе №6

**Студент:** Maksim Alehanov
**Группа:** ИУ8-25
**Тема:** Изучение средств пакетирования на примере CPack

---

## 1. Цель работы

Научиться собирать установочные пакеты проекта с помощью CPack и настроить
автоматическую сборку пакетов по тегу с публикацией в GitHub Release.
Используется библиотека `print`.

> Примечание: вместо Travis CI используется GitHub Actions.

## 2. Выполнение

### 2.1. Версионирование

В `CMakeLists.txt` добавлены переменные версии:

```cmake
set(PRINT_VERSION_MAJOR 0)
set(PRINT_VERSION_MINOR 1)
set(PRINT_VERSION_PATCH 0)
set(PRINT_VERSION_TWEAK 0)
set(PRINT_VERSION "${PRINT_VERSION_MAJOR}.${PRINT_VERSION_MINOR}.${PRINT_VERSION_PATCH}.${PRINT_VERSION_TWEAK}")
```

### 2.2. DESCRIPTION и ChangeLog.md

Созданы файл `DESCRIPTION` и `ChangeLog.md` в формате для RPM:

```
* Tue Jun 09 2026 sasun1100 <maks4182@gmail.com> 0.1.0.0
- Initial RPM release
```

### 2.3. CPackConfig.cmake

```cmake
set(CPACK_PACKAGE_CONTACT "maks4182@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Static C++ library for printing")

set(CPACK_RPM_PACKAGE_NAME "print-devel")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_CHANGELOG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/ChangeLog.md)

set(CPACK_DEBIAN_PACKAGE_NAME "libprint-dev")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "cmake (>= 3.0)")

include(CPack)
```

В конце `CMakeLists.txt` добавлено `include(CPackConfig.cmake)`.

### 2.4. Сборка пакетов

```sh
$ cmake -H. -B_build
$ cmake --build _build
$ cd _build
$ cpack -G DEB && cpack -G RPM && cpack -G TGZ && cpack -G ZIP
```

Получены пакеты:

```
print-0.1.0.0-Linux.deb
print-0.1.0.0-Linux.rpm
print-0.1.0.0-Linux.tar.gz
print-0.1.0.0-Linux.zip
```

Проверка deb-пакета:

```
$ dpkg -c print-0.1.0.0-Linux.deb
./usr/include/print.hpp
./usr/lib/libprint.a
./usr/cmake/print-config.cmake

$ dpkg -I print-0.1.0.0-Linux.deb
 Package: libprint-dev
 Version: 0.1.0.0-1
```

## 3. Домашнее задание

Задание: автоматически собирать пакеты для коммитов с тегом и заливать их в
GitHub Release.

Реализовано через GitHub Actions (`.github/workflows/ci.yml`):
на push — сборка и тесты (gcc/clang); при теге `v*` — отдельный job собирает
`cpack -G DEB/RPM/TGZ/ZIP` и заливает пакеты в Release через
`softprops/action-gh-release`.

```yaml
  release:
    needs: build
    if: startsWith(github.ref, 'refs/tags/')
    runs-on: ubuntu-22.04
    permissions:
      contents: write
    steps:
      - uses: actions/checkout@v4
        with:
          submodules: recursive
      - name: Install dependencies
        run: sudo apt-get update && sudo apt-get install -y cmake build-essential rpm
      - name: Configure
        run: cmake -H. -B_build
      - name: Build
        run: cmake --build _build
      - name: Create packages
        run: |
          cd _build
          cpack -G DEB
          cpack -G RPM
          cpack -G TGZ
          cpack -G ZIP
      - name: Upload packages to Release
        uses: softprops/action-gh-release@v1
        with:
          files: |
            _build/*.deb
            _build/*.rpm
            _build/*.tar.gz
            _build/*.zip
```

После пуша тега `v0.1.0.0` workflow собрал пакеты и создал релиз с
прикреплёнными `.deb`, `.rpm`, `.tar.gz`, `.zip`.

## 4. Результаты

- Настроена сборка пакетов DEB/RPM/TGZ/ZIP через CPack.
- Тесты проходят на gcc и clang.
- По тегу пакеты автоматически собираются и публикуются в GitHub Release.
