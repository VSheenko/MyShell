## Задание
Разработать эмулятор для языка оболочки ОС. Необходимо сделать работу эмулятора как можно более похожей на сеанс shell в UNIX-подобной ОС. Эмулятор должен запускаться из реальной командной строки, а файл с виртуальной файловой системой не нужно распаковывать у пользователя. Эмулятор принимает образ виртуальной файловой системы в виде файла формата zip. Эмулятор должен работать в режиме CLI.

Конфигурационный файл имеет формат xml и содержит:
 - Имя пользователя для показа в приглашении к вводу;
 - Имя компьютера для показа в приглашении к вводу;
 - Путь к архиву виртуальной файловой системы;
 - Путь к лог-файлу;
 - Путь к стартовому скрипту.

***Лог-файл*** содержит все действия во время последнего сеанса работы с эмулятором. Для каждого действия указаны дата и время. Для каждого действия указан пользователь.

Необходимо поддержать в эмуляторе следующие команды:
 - ls
 - cd
 - exit
 - cal
 - tac
 - echo

## Зависимости
Установка vcpkg:
```bash
git clone https://github.com/microsoft/vcpkg.git
```
Перейдите в директорию vcpkg и выполните скрипт установки
```bash
cd vcpkg
.\bootstrap-vcpkg.bat # Для Windows
```
Установка зависимостей
- [tinyxml2](https://github.com/leethomason/tinyxml2) - TinyXML-2 — это простой, небольшой и эффективный парсер XML на языке C++, который можно легко интегрировать в другие программы.
- [miniz](https://github.com/richgel999/miniz) - Miniz — это высокопроизводительная библиотека сжатия данных без потерь в одном исходном файле, реализующая стандарты спецификаций форматов сжатых данных zlib (RFC 1950) и Deflate (RFC 1951).
- [Google Test](https://github.com/google/googletest) - это популярная библиотека для модульного тестирования C++ приложений.

Установите их через vcpkg:
```bash
vcpkg install tinyxml2 miniz gtest
```
## Сборка
### Требования
- OS: Windows x64
- Компилятор: GCC
- CMake 3.28+
- vcpkg для управления зависимостями

### Внутренняя структура
Внутри проекта можно собрать несколько исполняемых файлов:
 - Основная программа эмулятора консоли (/)
    - MyShell
 - Команды консоли (/commands/src)
    - cal - выводит календарь на текущий месяц. Можно указать месяц и год для отображения определенного календаря.
    - tac - выводит содержимое файла в обратном порядке строк (последняя строка становится первой).
    - ls - выводит список файлов и каталогов в текущем каталоге или в указанном пути.
    - echo - выводит переданный текст в стандартный вывод.
 - Тесты (/test)

Сборка осущесвтляется как для обычного проекта CMake

## Описание файла конфигураций
Файл конфигураций должен находится в одной директории с исполнемым файлом MyShell и иметь расширение xml

Пример appsettings.xml 
```xml
<?xml version="1.0" encoding="UTF-8"?>
<settings>
    <user_name>vladimir</user_name>
    <pc_name>pc</pc_name>
    <archive_path>archive.zip</archive_path>
    <log_path>logs/</log_path>
    <commands_path>commands</commands_path>
</settings>
```

| Ключ        | Описание                                           |
|-----------------|------------------------------------------------|
| `user_name`     | Имя пользователя для показа в приглашении к вводу|
| `pc_name`       | Имя компьютера для показа в приглашении к вводу|
| `archive_path`  | Путь к архиву виртуальной файловой системы формата zip|
| `log_path`      | Путь к директории с лог-файлами|
| `commands_path` | Путь к исполняемым файлам команд консоли|

## Пример работы и использования

### Структура приложения
```
MyShell/
    ├── MyShell.exe
    ├── libminiz.dll
    ├── libtinyxml2.dll
    ├── archive.zip
    ├── appsettings.xml
    ├── logs/
    └── commands/
        ├── cal.exe
        ├── tac.exe
        ├── echo.exe
        ├── ls.exe
        └── libminiz.dll
```
### Структура виртуальной файловой системы (архива archive.zip)

```
archive.zip/
    ├── file.bat
    └── folder_1/
        ├── file.txt
        └── folder_2/
            ├── file_2
            └── file.txt
```

Содержимое `file.bat`:
```bat
@echo off

echo Текущая дата и время:
date /t
time /t

cd %USERPROFILE%\Documents

echo.
echo Список файлов в папке Documents:
dir

echo.
echo Батник завершил выполнение. Нажмите любую клавишу для выхода...
pause > nul
```

Содержимое `folder_1/file.txt`
```txt
line 1
line 2
line 3
line 3
line 5
```
Содержимое `folder_1/folder_2/file.txt`
```txt
folder_1/folder_2/file => line 1
folder_1/folder_2/file => line 2
folder_1/folder_2/file => line 3
folder_1/folder_2/file => line 4
folder_1/folder_2/file => line 5
```
Содержимое `folder_1/folder_2/file_2`
```txt
folder_1/folder_2/file_2 => line 1
folder_1/folder_2/file_2 => line 2
folder_1/folder_2/file_2 => line 3
folder_1/folder_2/file_2 => line 4
folder_1/folder_2/file_2 => line 5
```

### Пример использования **`cd`**
Поддерживает относительные и абсолютные пути к директориям 

![doc/img/cd example.png](https://github.com/VSheenko/MyShell/blob/master/doc/img/cd%20example.png)

### **`ls`**
Поддерживает обработку нескольких параметров

![doc/img/ls example.png](https://github.com/VSheenko/MyShell/blob/master/doc/img/ls%20example.png)

### **`cal`**

Для полчения подробностей `cal --help`

![doc/img/cal example.png](https://github.com/VSheenko/MyShell/blob/master/doc/img/cal%20example%201.png)

![doc/img/cal example.png](https://github.com/VSheenko/MyShell/blob/master/doc/img/cal%20example%202.png)

### **`echo`**

Ключи:
- -e - обрабатывает escape-последовательности
- -E (по умолчанию) - отключает обработку escape-последовательностей
- -n - убирвет перенос на следующую строку

![doc/img/echo example.png](https://github.com/VSheenko/MyShell/blob/master/doc/img/echo%20example.png)

### **`tac`**

![doc/img/echo example.png](https://github.com/VSheenko/MyShell/blob/master/doc/img/tac%20example.png)

### **`exit`**
Используется для выхода из эмулятора
