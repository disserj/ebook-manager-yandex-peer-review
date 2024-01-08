#include <array>

namespace check
{
void CheckYear(const DateTime date_time){
    if (date_time.year < 1 || date_time.year > 9999) {
        date_time.year < 1?
                    throw domain_error("year is too small"s):
                                              throw domain_error("year is too big"s);
    }
}
void CheckMonth(const DateTime date_time){
    if (date_time.month < 1 || date_time.month > 12) {
        date_time.month < 1?
                    throw domain_error("month is too small"s):
                                              throw domain_error("month is too big"s);
    }
}
void CheckDay(const DateTime date_time){
    const bool is_leap_year = (date_time.year % 4 == 0) && !(date_time.year % 100 == 0 && date_time.year % 400 != 0);
    std::array<int,12> month_lengths= {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if(is_leap_year) ++ month_lengths[1];

    if (date_time.day < 1 || date_time.day > month_lengths[dt.month - 1]) {
        date_time.day < 1?
                    throw domain_error("day is too small"s):
                                              throw domain_error("day is too big"s);
    }
}
void CheckHour(const DateTime date_time){
    if (date_time.hour < 1 || date_time.hour > 23) {
        date_time.hour < 1?
                    throw domain_error("hour is too small"s):
                                              throw domain_error("hour is too big"s);
    }
}
void CheckMinute(const DateTime date_time){
    if (date_time.minute < 1 || date_time.minute > 59) {
        date_time.minute < 1?
                    throw domain_error("minute is too small"s):
                                              throw domain_error("minute is too big"s);
    }
}
void CheckSecond(const DateTime date_time){
    if (date_time.second < 1 || date_time.second > 59) {
        date_time.second < 1?
                    throw domain_error("second is too small"s):
                                              throw domain_error("second is too big"s);
    }
}

}//exit namespace check

/********************************************************************************************
    (+) изменил сигнатуру CheckDateTimeValidity, не вижу смысла передавать в ф-ю
        структуру из 6 полей int по ссылке
    (+) изменил имя параметра, dt - неинформативно
    (+) распределил проверки полей в отдельные ф-и, заключил эти ф-и в namespace 
    (+) использовал контейнер стандартной библиотеки std::array вместо встроенного массива
********************************************************************************************/

void CheckDateTimeValidity(const DateTime date_time){
    check::CheckYear(date_time);
    check::CheckMonth(date_time);
    check::CheckDay(date_time);
    check::CheckHour(date_time);
    check::CheckMinute(date_time);
    check::CheckSecond(date_time);
}
