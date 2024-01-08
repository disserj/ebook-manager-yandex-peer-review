#include <array>
#include <string_view>

/********************************************************************************************
    (+) изменил сигнатуру CheckDateTimeValidity, не вижу смысла передавать в ф-ю
        структуру из 6 полей int по ссылке
    (+) изменил имя параметра, dt - неинформативно
    (+) обобщил проверки полей, кроме проверки дня, в одну ф-ю, заключил эти ф-и в namespace 
    (+) использовал контейнер стандартной библиотеки std::array вместо встроенного массива
********************************************************************************************/

namespace check
{
void CheckDateOrTime(int date_or_time, int limit, std::string_view pref){
    if (date_or_time < 1 || date_or_time > limit) {
        date_or_time < 1?
                    throw domain_error((std::string)pref + " is too small"s):
                                              throw domain_error((std::string)pref + " is too big"s);
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

}//exit namespace check
enum Values{
    year = 9999;
    month = 12;
    hour = 23;
    min_or_sec = 59;
};

void CheckDateTimeValidity(const DateTime date_time){
    check::CheckDateOrTime(date_time.year, Values::year, "year");
    check::CheckDateOrTime(date_time.month, Values::month,"month");
    check::CheckDay(date_time);
    check::CheckDateOrTime(date_time.hour, Values::hour, "hour");
    check::CheckDateOrTime(date_time.minute, Values::min_or_sec, "minute");
    check::CheckDateOrTime(date_time.second, Values::min_or_sec, "second");
}
