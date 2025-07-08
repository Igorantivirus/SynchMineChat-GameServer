# SynchMineChat-GameServer
## (Синхронный майнкрафт чат — игровой сервер) 

[![English version](https://img.shields.io/badge/English%20version-blue)](README.md)

## Описание

Парсер майнкрафт логов, который по WebSocket протоколу отправляет их на сервер. 
Обрабатывает сообщения из чата игры в реальном времени их создания, способен фиксировать:
1) Текстовые сообщения
2) Сообщения о входе в игру
3) Сообщения о выходе из игры
4) Получение достижения игроком

Основной сервер можно найти здесь: [Ссылка на основной сервер](https://github.com/Igorantivirus/SynchMineChat-MainServer)

## Зависимости

Язык программирования: **С++20**

Библиотеки:
* [RconPP](https://github.com/JBakamovic/rconpp) - RCON клиент для C++
* [Boost](https://www.boost.org/) (версия 1.75+)
  * [Asio](https://www.boost.org/doc/libs/release/doc/html/boost_asio.html) - для асинхронного I/O
  * [WebSocket](https://www.boost.org/doc/libs/release/libs/beast/doc/html/beast/using_websocket.html) - реализация WebSocket
* [nlohmann-json](https://github.com/nlohmann/json) - библиотека для работы с JSON

## Особенности реализации

* При отсутствии подключения к серверу или RCON в течение длительного времени (по умолчанию: 50 секунд) — программа завершает работу
* При обновлении файла логов майнкрафта начинает читать только новые сообщения
* Работает в 1 потоке
* Поддержка конфигурации через JSON-файлы

## Установка и запуск

### Предварительные требования
* Компилятор с поддержкой C++20 (GCC 10+, Clang 10+, MSVC 19.29+)
* CMake 3.15+
* Установленные зависимости (см. выше)

### Инструкция по сборке

1. **Клонируйте репозиторий**
   ```sh
   git clone https://github.com/Igorantivirus/SynchMineChat-GameServer
   cd SynchMineChat-GameServer 
   ```
2. **Соберите CMake**
   ```sh
   cmake -B build -DCMAKE_BUILD_TYPE=Release
   ```
3. **Соберите проект** 
   ```sh
   cmake --build build --config Release
   ```

### Запуск 

1. Создайте конфиг файл  *MainConfig.json* (структура конфига описана в файле *src/Services/MainConfog.hpp*)
   Файл должен быть в *build/resources/*
2. Перейдите в директорию build и запустите программу
   ```sh
   ./main
   ``` 

## Лицензия 
MIT Лицензия
