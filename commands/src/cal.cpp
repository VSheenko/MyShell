#include <iostream>
#include <iomanip>
#include <ctime>
#include <string>
#include <windows.h>

#define MONTH_CAL_WIDTH 23
#define MONTH_CAL_HEIGHT 9

struct Coord {
    short x;
    short y;

    Coord() : x(0), y(0) {}
    Coord(short x, short y) : x(x), y(y) {}
};

struct Date {
    int day;
    int month;
    int year;

    Date() : day(0), month(0), year(0) {}
    Date(int day, int month, int year) : day(day), month(month), year(year) {}
};

std::string GetMonthName(int month) {
    switch (month) {
        case 1: return "January";
        case 2: return "February";
        case 3: return "March";
        case 4: return "April";
        case 5: return "May";
        case 6: return "June";
        case 7: return "July";
        case 8: return "August";
        case 9: return "September";
        case 10: return "October";
        case 11: return "November";
        case 12: return "December";
        default: return "Unknown";
    }
}

int GetNumDayInMonth(Date date) {
    int month = date.month;
    int year = date.year;

    switch (month) {
        case 1: return 31;
        case 2: return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) ? 29 : 28;
        case 3: return 31;
        case 4: return 30;
        case 5: return 31;
        case 6: return 30;
        case 7: return 31;
        case 8: return 31;
        case 9: return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31;
        default: return -1;
    }
}

int GetStartDayInMonth(Date date) {
    std::tm time_in = {};
    time_in.tm_year = date.year - 1900; // Год с 1900
    time_in.tm_mon = date.month - 1;    // Месяцы считаются с 0 (0 - январь)
    time_in.tm_mday = date.day;
    std::mktime(&time_in);

    return time_in.tm_wday;
}

void MoveCursorTo(short x, short y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {x, y}; // Установка позиции (координаты)
    SetConsoleCursorPosition(hConsole, pos);
}

void PrintMonth(Date& date, Coord& coord_to_pos) {
    MoveCursorTo(coord_to_pos.x * MONTH_CAL_WIDTH, coord_to_pos.y * MONTH_CAL_HEIGHT);

    std::string s_month_line = std::string((MONTH_CAL_WIDTH - GetMonthName(date.month).size() - std::to_string(date.year).size() - 2) / 2, ' ')
            + GetMonthName(date.month)
            + " " + std::to_string(date.year);

    std::cout << std::left << std::setw(MONTH_CAL_WIDTH - 1) << s_month_line;
    MoveCursorTo(coord_to_pos.x * MONTH_CAL_WIDTH, coord_to_pos.y * MONTH_CAL_HEIGHT + 1);
    std::cout << std::left << "Su Mo Tu We Th Fr Sa";

    int num_days = GetNumDayInMonth(date);
    int start_day = GetStartDayInMonth(date);

    for (int i = 0; i < start_day + num_days; i++) {
        if (i % 7 == 0) {
            MoveCursorTo(coord_to_pos.x * MONTH_CAL_WIDTH, coord_to_pos.y * MONTH_CAL_HEIGHT + 2 + i / 7);
        }
        if (i < start_day) {
            std::cout << "   ";
        } else {
            std::cout << std::setw(3) << i - start_day + 1;
        }
    }
}

void PrintYear(int year) {
    for (int i = 1; i <= 12; i++) {
        Date date(1, i, year);
        Coord coord((i - 1) % 3, (i - 1) / 3);
        PrintMonth(date, coord);
    }
}



int main (int argc, char* argv[]) {
    PrintYear(2024);
    std::cin.get();
}