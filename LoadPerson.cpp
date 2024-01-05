/*****************************************************************************************
        (+) агрегировал большое количество аргументов, передаваемых изначально
                    в ф-ю LoadPersons, в 2 разные структруры
 *****************************************************************************************/                   
struct DBParams {
    string_view name;
    int connection_timeout;
    bool allow_exceptions;
    DBLogLevel db_log_level;
}

struct PersonParams {
    int min_age;
    int max_age;
    string_view name_filter;
}
/*****************************************************************************************
        (+) выделил процесс обработки DB параметров в отдельную функцию
(+) использовал forwarding references (???) - неизвестно какой размер у объекта DBLogLevel 
*****************************************************************************************/
template<typename DBParams>
DBHandler Process(DBParams&& db_params) {
    DBConnector connector(forward<DBParams>(db_params).db_allow_exceptions, forward<DBParams>(db_params).db_log_level);
    if (forward<DBParams>(db_params).db_name.starts_with("tmp."s)) {
        return connector.ConnectTmp(forward<DBParams>(db_params).db_name, forward<DBParams>(db_params).db_connection_timeout);
    } 
    else {
        return connector.Connect(forward<DBParams>(db_params).db_name, forward<DBParams>(db_params).db_connection_timeout);
    }
}
/*****************************************************************************************
    (+) использовал forwarding references (???) - неизвестно какой размер у объекта DBLogLevel 
        (+) поменял возвращаемый тип функции LoadPersons на optional<vector<Person>>
*****************************************************************************************/  
template<typename DBParams>
optional<vector<Person>> LoadPersons(DBParams&& db_params, PersonParams person_params) {
    DBHandler db = Process(forward<DBParams>(db_params));

    if (!(forward<DBParams>(db_params).db_allow_exceptions && !db.IsOK())) {
        return nullopt;
    }    

    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << person_params.min_age << " and "s << person_params.max_age << " "s
              << "and Name like '%"s << db.Quote(person_params.name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
}
