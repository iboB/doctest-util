#pragma once
#include <functional>

namespace itlib
{

template <typename Tag>
class lifetime_counter
{
public:
    struct stats
    {
        int d_ctr = 0; // default construct
        int copies = 0; // total copies
        int c_ctr = 0; // copy constructed
        int c_asgn = 0; // copy assigned
        int m_ctr = 0; // move constructed
        int m_asgn = 0; // move assigned
        int living = 0; // total living

        std::function<void(const stats&)> at_end;

        stats* parent = nullptr;

        ~stats()
        {
            if (at_end) at_end(*this);
        }
    };

    static stats* top;
    static stats bottom;

    lifetime_counter() {
    }
    lifetime_counter(const lifetime_counter& other) {
    }
    lifetime_counter& operator=(const lifetime_counter& other) {
    }
    lifetime_counter(lifetime_counter&& other) {
    }
    lifetime_counter& operator=(lifetime_counter&& other) {
    }
    ~lifetime_counter() {
    }
};

}
