/*****************************************************************************************
         агрегировал большое количество аргументов, передаваемых изначально
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

vector<Person> LoadPersons(DBParams&& db_params, PersonParams person_params) { {
    DBConnector connector(db_allow_exceptions, db_log_level);
    DBHandler db;

    /* возможно, этот блок стоит вынести в отдельную ф-ю для удобства тестирования */
    //-------------------------------------------------------------------------------------     
    if (db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_name, db_connection_timeout);
    } else {
        db = connector.Connect(db_name, db_connection_timeout);
    }
    if (!db_allow_exceptions && !db.IsOK()) {
        return {};
    }
    //-------------------------------------------------------------------------------------
         
    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << min_age << " and "s << max_age << " "s
              << "and Name like '%"s << db.Quote(name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
}
