// ПЕРВЫЙ ВАРИАНТ РЕФАКТОРИНГА 
/**********************************************************************************************
    (+) добавил string_view, чтобы использовать в сигнатурах методов классов вместо string
    (+) убрал изначально данную в условии ParseCitySubjson и 
                                              написал два билдера - для Country и для City
            (получилось объемно, но более-менее структурировано)
**********************************************************************************************/  
#include <string_view>

class CountryBuilder{
public:  
    CountryBuilder& SetCountryPhone(string_view country_phone_code){
        country_phone_code_ = country_phone_code;
        return *this;
    }
    CountryBuilder& SetCountryName(string_view country_name){
        country_name_ = country_name;
        return *this;
    }
    CountryBuilder& SetCountryIsoCode(string_view country_iso_code){
        country_phone_code_ = country_iso_code;
        return *this;
    }
    CountryBuilder& SetCountryTimeZone(string_view country_time_zone){
        country_time_zone_ = country_time_zone;
        return *this;
    }
    operator Country() const{
        return Country({country_name_,country_iso_code_,country_phone_code_,country_time_zone_});
    }

private:
    string country_name_;
    string country_iso_code_; 
    string country_phone_code_; 
    string country_time_zone_;

};


class CityBuilder{
public:    
    CityBuilder& SetName(string_view city_name){
        city_name_ = city_name;
        return *this;
    }
    CityBuilder& SetIsoCode(string_view city_iso_code){
        city_iso_code_ = city_iso_code;
        return *this;
    }
    CityBuilder& SetCountry(const Country& country){
        country_ =  country;
        return *this;
    }
    //------ передаем в метод два итератора вместо целого контейнера ----
    template<typename It>
    CityBuilder& SetCountryLanguages(It begin, It end){
        languages_(It begin, It end);
        return *this;
    }

    operator City() const{
        return City({city_name_,city_iso_code_,country_,languages_});
    }
private:
    string city_name_;
    string city_iso_code_;   
    Country country_;
    vector<Language> languages_;    
};


void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();
        countries.push_back(
            CountryBuilder().
            SetCountryName(country_obj["name"s].AsString()).
            SetCountryIsoCode(country_obj["iso_code"s].AsString()).
            SetCountryPhone(country_obj["phone_code"s].AsString()).
            SetCountryTimeZone(country_obj["time_zone"s].AsString())
        );
        Country& country = countries.back();
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
        for (const auto& city_json : country_obj["cities"s].AsList()){
            const auto& city_obj = city_json.AsObject();
            cities.push_back(
                CityBuilder().
                SetName(city_obj["name"s].AsString()).
                SetIsoCode(city_obj["iso_code"s].AsString()).
                SetCountry(
                    CountryBuilder().
                    SetCountryName(country.name).
                    SetCountryIsoCode(country.iso_code).
                    SetCountryPhone(country.phone_code + city_obj["phone_code"s].AsString()).
                    SetCountryTimeZone(country.time_zone)
                ).
                SetCountryLanguages(country.languages.begin(), country.languages.end())
                );                
        }
    }
}

// ВТОРОЙ ВАРИАНТ РЕФАКТОРИНГА
/**********************************************************************************************
(+) оставил изначально данную в условии ParseCitySubjson, которая сейчас принимает в качестве 
            параметра объект Country по константной ссылке                                  
**********************************************************************************************/

void ParseCitySubjson(vector<City>& cities, const Json& json, const Country& country) {
    for (const auto& city_json : json.AsList()) {
        const auto& city_obj = city_json.AsObject();
        cities.push_back({city_obj["name"s].AsString(), city_obj["iso_code"s].AsString(),
                          country.phone_code + city_obj["phone_code"s].AsString(), country.name, country.iso_code,
                          country.time_zone, country.languages});
    }
}

void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();
        countries.push_back({
            country_obj["name"s].AsString(),
            country_obj["iso_code"s].AsString(),
            country_obj["phone_code"s].AsString(),
            country_obj["time_zone"s].AsString(),
        });
        Country& country = countries.back();
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
        ParseCitySubjson(cities, country_obj["cities"s], country);
    }
} 
