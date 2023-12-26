#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <exception>


class EBookManager {
public:
    EBookManager(): 
        users(users_cnt+1, -1), all_pages(pages_cnt, 0)
    {}
    
    void ScanAndResponse(std::istream& is, std::ostream& os) const{
        int query_count;
        is >> query_count;
        if(query_count > q_cnt) throw std::logic_error("too many requests");
        if(query_count <= 0) throw std::logic_error("incorrect number of requests");
        assert(query_count <= q_cnt);
        for (size_t i = 0; i < query_count; ++i) {
            std::string query;
            int user_id;
            is >> query >> user_id;
            if(!std::all_of(query.begin(),query.end(),[](const auto& ch){
                return isalpha(ch) && isupper(ch);
            })) throw std::logic_error("wrong query");
            
            if(user_id > users_cnt || user_id < 0) throw std::logic_error("wrong user id");
            
            if (query == "READ") {
                int page_num;
                is >> page_num;
                if(page_num > pages_cnt || page_num < 0) throw std::logic_error("wrong page num");
                Read(user_id, page_num);
            }
            if (query == "CHEER") {
                os << std::setprecision(6) << Cheer(user_id) << "\n";
            }
        }
    }
    
    void Read(const int& index, const int& page_num) const noexcept{
       std::for_each(std::next(all_pages.begin(),users[index] + 1),std::next(all_pages.begin(), page_num + 1),[](auto& page){
            ++page;
        });
        users[index] = page_num;
    }
    
    double Cheer(const int& index) const noexcept{
        if (users[index] == -1) return 0;
        if (all_pages[0] == 1) return 1;
        assert(users[index] != -1 && all_pages[0] != 1);
        return (all_pages[0] - all_pages[users[index]]) * 1.0 / (all_pages[0] - 1) * 1.0;
    }
    
private:
    static constexpr int users_cnt = 100000;
    static constexpr int pages_cnt = 1000;
    static constexpr int q_cnt = 1000000;
    
    mutable std::vector<int> users;
    mutable std::vector<int> all_pages;
};

int main() {
    EBookManager manager;
    manager.ScanAndResponse(std::cin, std::cout);
    
    return 0;
}