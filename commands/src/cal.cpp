#include <iostream>
#include <iomanip>
#include <ctime>
#include <string>
#include <windows.h>
#include <vector>
#include <cstring>

#define MONTH_CAL_WIDTH 23
#define MONTH_CAL_HEIGHT 9

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

COORD GetCursorPosition() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    return csbi.dwCursorPosition;
}

void MoveCursorTo(short x, short y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {x, y}; // Установка позиции (координаты)
    SetConsoleCursorPosition(hConsole, pos);
}

void PrintMonth(Date& date, COORD& coord_to_pos, COORD& initial_position, bool m) {
    short x = initial_position.X + coord_to_pos.X * MONTH_CAL_WIDTH;
    short y = initial_position.Y + coord_to_pos.Y * MONTH_CAL_HEIGHT;
    MoveCursorTo(x, y);

    std::string s_month_line = std::string((MONTH_CAL_WIDTH - GetMonthName(date.month).size() - std::to_string(date.year).size() - 2) / 2, ' ')
            + GetMonthName(date.month)
            + " " + std::to_string(date.year);

    std::cout << std::left << std::setw(MONTH_CAL_WIDTH - 1) << s_month_line;
    MoveCursorTo(x, y + 1);


    int num_days = GetNumDayInMonth(date);
    int start_day = GetStartDayInMonth(date);

    if (m) {
        std::cout << std::left << "Mo Tu We Th Fr Sa Su";
        if (start_day == 0) {
            start_day = 6;
        } else
            start_day--;
    } else {
        std::cout << std::left << "Su Mo Tu We Th Fr Sa";
    }

    for (int i = 0; i < start_day + num_days; i++) {
        if (i % 7 == 0) {
            MoveCursorTo(x, y + 2 + i / 7);
        }
        if (i < start_day) {
            std::cout << "   ";
        } else {
            std::cout << std::right << std::setw(2) << i - start_day + 1 << ' ';
        }
    }

    std::cout << '\n' << std::endl;
}

void PrintYear(COORD& initial_coord, int year, bool m) {
    COORD cal_position;
    for (int i = 1; i <= 12; i++) {
        Date date(1, i, year);
        cal_position.X = (i - 1) % 3;
        cal_position.Y = (i - 1) / 3;
        PrintMonth(date, cal_position, initial_coord, m);
    }
}


// -keys --------------------

bool m = false; // Monday as first day of week
bool y = false; // show the whole year
bool Y = false; // show the next twelve months
bool three = false; // show three months spanning the date


bool SetKeys(const std::string& key) {
    if (key == "-m" || key == "--monday") {
        m = true;
    } else if (key == "-y" || key == "--year") {
        y = true;
    } else if (key == "-Y" || key == "--next-year") {
        Y = true;
    } else if (key == "-3" || key == "--three") {
        three = true;
    } else {
        std::cerr << "cal: invalid option -- '" << key << "'" << std::endl;
        return false;
    }

    return true;
}


void PrintCal(COORD& initial_coord, Date& date) {
    MoveCursorTo(initial_coord.X, ++initial_coord.Y);

    if (y) {
        PrintYear(initial_coord, date.year, m);
        return;
    }

    if (Y) {
        date.year++;
        PrintYear(initial_coord, date.year, m);
        return;
    }

    if (three) {
        COORD cal_position;
        for (int i = -1; i < 2; i++) {
            Date new_date(date.day, date.month + i, date.year);
            cal_position.X = i + 1;
            cal_position.Y = 0;
            PrintMonth(new_date, cal_position, initial_coord, m);
        }
        return;
    }

    COORD c;
    c.X = 0;
    c.Y = 0;

    PrintMonth(date, c, initial_coord, m);
}

bool CheckDate(Date& date) {
    if (date.month < 1 || date.month > 12) {
        std::cerr << "cal: " << date.month << ": month must be in range 1..12" << std::endl;
        return false;
    }

    if (date.day < 1 || date.day > GetNumDayInMonth(date)) {
        std::cerr << "cal: " << date.day << ": day must be in range 1.." << GetNumDayInMonth(date) << std::endl;
        return false;
    }

    if (date.year < 1970) {
        std::cerr << "cal: " << date.year << ": year must be in range 1970.." << std::endl;
        return false;
    }

    return true;
}

int main (int argc, char* argv[]) {

    COORD cur_pos = GetCursorPosition();

    SYSTEMTIME st;
    GetLocalTime(&st);
    Date date(1, st.wMonth, st.wYear);

    if (argc == 1) {
        PrintCal(cur_pos, date);
        return 0;
    }

    std::vector<std::string> params;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strcmp(argv[i], "--help") == 0) {
                std::cout << "Usage:\n"
                             " cal [options] [[[day] month] year]\n"
                             " \n"
                             "Display a calendar, or some part of it.\n"
                             "Without any arguments, display the current month.\n"
                             " \n"
                             "Options:\n"
                             " -3, --three           show three months spanning the date\n"
                             " -m, --monday          Monday as first day of week\n"
                             " -y, --year            show the whole year\n"
                             " -Y, --twelve          show the next twelve months\n"
                             " \n"
                             " -h, --help            display this help\n";
                return 0;
            }

            if (!SetKeys(argv[i])) {
                std::cerr << "Try 'cal --help' for more information." << std::endl;
                return 1;
            }
        } else {
            params.push_back(argv[i]);
        }
    }

    if (params.size() > 3) {
        std::cerr << "cal: bad usage" << std::endl;
        std::cerr << "Try 'cal --help' for more information." << std::endl;
        return 1;
    }

    if (params.size() == 1) {
        date.year = std::stoi(params[0]);

        if (!CheckDate(date))
            return 1;

        std::cout << '\n';
        PrintYear(cur_pos, date.year, m);
        return 0;
    }

    if (params.size() == 2) {
        date.month = std::stoi(params[0]);
        date.year = std::stoi(params[1]);

        if (!CheckDate(date))
            return 1;
    }

    if (params.size() == 3) {
        date.day = std::stoi(params[0]);
        date.month = std::stoi(params[1]);
        date.year = std::stoi(params[2]);

        if (!CheckDate(date))
            return 1;
    }

    PrintCal(cur_pos, date);
}