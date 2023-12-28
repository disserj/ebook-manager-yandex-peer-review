#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <utility> 
#include <string_view>
#include <memory>
#include <cassert>
#include <list>
#include <set>

using namespace std;


//--------------------ASSERT------------------------------------
template<typename T>
void AssertIm(const T& value,  const string& str, const string& file,const string& func, unsigned line, const string& hint) {
    if (!value) {
        cout << boolalpha;
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT("s << str <<") failed."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}
#define ASSERT(expr) AssertIm((expr), #expr,  __FILE__, __FUNCTION__, __LINE__,"")

#define ASSERT_HINT(expr, hint) AssertIm((expr), #expr,  __FILE__, __FUNCTION__, __LINE__, (hint))


//--------------------ASSERT_EQUAL------------------------------
template <typename T, typename U>
void AssertEqualIm(const T& t, const U& u, const string& t_str, const string& u_str, const string& file, const string& func, unsigned line, const string& hint) {
    if (t != u) {
        cout << boolalpha;
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cout << t << " != "s << u ;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

#define ASSERT_EQUAL(a, b) AssertEqualIm((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, "")

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualIm((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

//--------------------RUN_TEST---------------------------------
template <typename T>
void RunTest(T f,const string& s) {
    f();
    cerr<<s<<" OK\n";
}

#define RUN_TEST(func) RunTest(func,#func)
//-------------------------------------------------------------


class Domain {
private:
    mutable std::string reverse_domain;
    mutable std::string_view domain;
public:  
    explicit Domain(std::string_view dmn): domain(dmn) {
        reverse_domain = string(dmn.rbegin(), dmn.rend());
        reverse_domain += '.';
    }

    bool operator==(const Domain& other) const {
        return GetReverseDomain() == other.GetReverseDomain();
    }
    
    bool operator!=(const Domain& other) const {
        return GetReverseDomain() != other.GetReverseDomain();
    }
    

    bool IsSubdomain(const Domain& other) const {
        if (reverse_domain.size() < other.reverse_domain.size()) return false;
        if(reverse_domain == other.reverse_domain) return true;
        return reverse_domain.substr(0, other.reverse_domain.size()) == other.reverse_domain;

    }
    
    std::string_view GetDomain() const {
        return domain;
    }
    
    const std::string GetReverseDomain() const {
        return reverse_domain;
    }
    
};

 ostream& operator<<(ostream& os, const Domain& d){
        os << d.GetDomain() << "\n";
        return os;
    }

class DomainChecker {
private:
    mutable vector<Domain> ban_list;    
public:   
    template<typename It>
    DomainChecker(It start, It finish)
    :ban_list(start, finish)
    {
        sort(ban_list.begin(), ban_list.end(), [](const auto& lhs, const auto& rhs){
             return lhs.GetReverseDomain() < rhs.GetReverseDomain();
        });
        
        ban_list.erase(std::unique(ban_list.begin(), ban_list.end(), [](const auto& lhs,const auto& rhs){
            return rhs.IsSubdomain(lhs);
        }), ban_list.end());
    }
    
    bool IsForbidden(const Domain& domain) const {
        auto it = upper_bound(ban_list.begin(), ban_list.end(), domain, [](const auto& lhs, const auto& rhs){
                    return lhs.GetReverseDomain() < rhs.GetReverseDomain();
                });
        if (it == ban_list.begin()) return false;
        else return domain.IsSubdomain(*prev(it));
    }
};

template <typename Number>
vector<Domain> ReadDomains(istream& in, Number num) {
    if(num <= 0) return {};
    vector<Domain> domains;
    domains.reserve(num);
    string domain;
    for (Number i = 0; i < num; ++i) {
        getline(in, domain);
        domains.push_back(Domain(domain));
    }
    return domains;
}


template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}


void DomainTest(){
    {
        Domain d("gdz.ru");
        Domain d1("gdz.ru");
        Domain d2("gdz");
        Domain d3("gdz.ua");
        Domain d4("alg.m.gdz.ru");
        
        // == / != test
        ASSERT_EQUAL(d,d1);
        ASSERT(d==d1);
        ASSERT(d!=d3);
        ASSERT(d!=d2);
       
        // IsSubdomain test 
        ASSERT(d4.IsSubdomain(d));
        ASSERT(!d.IsSubdomain(d4));
        ASSERT(!d2.IsSubdomain(d));
        ASSERT(d.IsSubdomain(d1));
        ASSERT(!d4.IsSubdomain(d2));
        ASSERT(d4.IsSubdomain(d1));
        
        //get... test
        ASSERT_EQUAL(d.GetDomain(),"gdz.ru"sv);
        ASSERT_EQUAL(d.GetReverseDomain(),"ur.zdg."s);
        ASSERT_EQUAL(d4.GetDomain(),"alg.m.gdz.ru"sv);
        ASSERT_EQUAL(d4.GetReverseDomain(),"ur.zdg.m.gla."s);
    }
    
    {  
        Domain d("maps.com");
        Domain d1("maps.ru");
        Domain d2("m.maps.me");
        Domain d3("m.maps.me");
        Domain d4("gdz.m.maps.meh.com");
        Domain d5("gdz.m.maps.me.com");
        Domain d6("gdz.m.maps.com");
        Domain d7("gdz");
        
        // == / != test
        ASSERT_EQUAL(d2,d3);
        ASSERT(d!=d1);
        ASSERT(d3!=d4);
       
        // IsSubdomain test 
        ASSERT(d6.IsSubdomain(d));
        ASSERT(d2.IsSubdomain(d3));
        ASSERT(!d7.IsSubdomain(d6));
        ASSERT(!d4.IsSubdomain(d2));
        ASSERT(!d5.IsSubdomain(d2));
        ASSERT(!d4.IsSubdomain(d5));
        
        //get... test
        ASSERT_EQUAL(d7.GetDomain(),"gdz"sv);
        ASSERT_EQUAL(d7.GetReverseDomain(),"zdg."s);
        ASSERT_EQUAL(d6.GetDomain(),"gdz.m.maps.com"sv);
        ASSERT_EQUAL(d6.GetReverseDomain(),"moc.spam.m.zdg."s);
    }
}

void DomainCheckerTest(){

    {
        //constructor test
        std::vector<std::string>vs{"gdz.ru"s,"gdz.com"s,"m.maps.me"s,"alg.m.gdz.ru"s,
        "maps.com"s,"maps.ru"s,"gdz.ua"s};

        DomainChecker dc(vs.begin(),vs.end());
        DomainChecker dc1(next(vs.begin()),prev(vs.end()));
        DomainChecker dc2(vs.begin(),vs.begin()+2);
        auto it = find_if(vs.begin(),vs.end(),[](const auto& s){return s.find("me")!=s.npos;});
        DomainChecker dc3(vs.begin(),it);
        DomainChecker dc4(it,vs.end());


        //IsForbidden test
        ASSERT_EQUAL(dc.IsForbidden(Domain("gdz.com")),true);
        ASSERT_EQUAL(dc.IsForbidden(Domain("gdz.me")),false);
        ASSERT_EQUAL(dc1.IsForbidden(Domain("gdz.ru")),false);
        ASSERT_EQUAL(dc2.IsForbidden(Domain("gdz.ru")),true);
        ASSERT_EQUAL(dc3.IsForbidden(Domain("gdz.com")),true);
        ASSERT_EQUAL(dc3.IsForbidden(Domain("gdz.ua")),false);
        ASSERT_EQUAL(dc4.IsForbidden(Domain("gdz.ua")),true);
        ASSERT_EQUAL(dc4.IsForbidden(Domain("gdz.com")),false);
    
    }
    
    {
         //constructor test
         std::vector<std::string_view>vsv{"gdz.ru\t"sv,"gdz.com\n"sv,"m.maps.me"sv,"alg.m.gdz.ru"sv,
        "maps-.com"sv,"maps .ru"sv,"gdzua"sv};
        DomainChecker dc5(vsv.begin(),vsv.end());

        //IsForbidden test
        ASSERT_EQUAL(dc5.IsForbidden(Domain("m.maps.me")),true);
        ASSERT_EQUAL(dc5.IsForbidden(Domain("gdz.ru\t")),true);
        ASSERT_EQUAL(dc5.IsForbidden(Domain("gdz.com")),false);
        ASSERT_EQUAL(dc5.IsForbidden(Domain("gdz.ua")),false);

    }
    
    {
        //constructor test
        std::list<std::string_view>lsv{"gdz.ru"sv,"maps.me"sv,"m.gdz.ru"sv,"com"sv};
        DomainChecker dc6(lsv.begin(),lsv.end());
        std::vector<std::string> tst{"gdz.ru"s,"gdz.com"s,"m.maps.me"s,"alg.m.gdz.ru"s,
        "maps.com"s};
        std::vector<std::string> tst1{"maps.ru"s,"gdz.ua"s };
        
        //IsForbidden test
        std::for_each(tst.begin(),tst.end(),[&dc6](const auto& s){
            ASSERT_EQUAL(dc6.IsForbidden(Domain(s)),true);
        });
        std::for_each(tst1.begin(),tst1.end(),[&dc6](const auto& s){
            ASSERT_EQUAL(dc6.IsForbidden(Domain(s)),false);
        });
    }
    
    {
        //constructor test
        std::set<std::string_view>ssv{"gdz.ru"sv,"maps.me"sv,"m.gdz.ru"sv,"com"sv};
        DomainChecker dc7(ssv.begin(),ssv.end());
         std::vector<std::string> tst{"maps.ru"s,"gdz.ua"s };
          std::vector<std::string> tst1{"gdz.ru"s,"gdz.com"s,"m.maps.me"s,"alg.m.gdz.ru"s,
        "maps.com"s};
         
        //IsForbidden test
        std::for_each(tst.begin(),tst.end(),[&dc7](const auto& s){
            ASSERT_EQUAL(dc7.IsForbidden(Domain(s)),false);
        }); 
        std::for_each(tst1.begin(),tst1.end(),[&dc7](const auto& s){
            ASSERT_EQUAL(dc7.IsForbidden(Domain(s)),true);
        });
    }
    
    {
        //constructor test
         std::vector<std::string>big_v(1000000);
         DomainChecker dc8(big_v.begin(),big_v.end());
         std::vector<std::string> tst{"gdz.ru"s,"gdz.com"s,"m.maps.me"s,"alg.m.gdz.ru"s,
        "maps.com"s,"maps.ru"s,"gdz.ua"s};
         
        //IsForbidden test
        std::for_each(tst.begin(),tst.end(),[&dc8](const auto& s){
            ASSERT_EQUAL(dc8.IsForbidden(Domain(s)),false);
        });  
    }
}

void ReadDomainsTest(){
    {
        //size_t test
        constexpr size_t n = 1000000;
        std::vector<std::string>big_v(n);
        std::istringstream in;
        std::vector<Domain>ex = ReadDomains(in,n);
        ASSERT_EQUAL(ex.size(),n);
        ASSERT(ex.size()!=(size_t)100000);
    }
    
    {
        //int test
        constexpr int n = 0;
        std::istringstream in;
        std::vector<Domain>ex = ReadDomains(in,n);
        ASSERT(ex.empty());
    }
    
    {
        //double test
        constexpr double n = -1;
        std::istringstream in;
        std::vector<Domain>ex = ReadDomains(in,n);
        ASSERT(ex.empty());
    }
    
    {
        //test with strings
         std::vector<std::string>vs{"gdz.ru","gdz.com","m.maps.me","alg.m.gdz.ru",
        "maps.com","maps.ru","gdz.ua"};
        std::istringstream in;
       in.str("gdz.ru\ngdz.com\nm.maps.me\nalg.m.gdz.ru\nmaps.com\nmaps.ru\ngdz.ua");
        std::vector<Domain>ex = ReadDomains(in,vs.size());
        
        //size test
        ASSERT_EQUAL(ex.size(),vs.size());
        
        //domain equality test
        std::transform(vs.begin(),vs.end(),vs.begin(),[](auto& s){ 
            std::string t(s.rbegin(),s.rend());
            t+='.';
            return t;
        });
        std::for_each(vs.begin(),vs.end(),[&ex](const std::string& s){
            ASSERT((std::find_if(ex.begin(),ex.end(),[&s](const auto& domain){
                return domain.GetReverseDomain()==s;
            })!=ex.end()));
        });
    }
    
}


void RunningUnitTests(){
    RUN_TEST(DomainTest);
    RUN_TEST(DomainCheckerTest);
    RUN_TEST(ReadDomainsTest);
}

int main() {
    #if 1
    RunningUnitTests();
    #endif
    
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
