#include <doctest/util/lifetime_counter.hpp>

TEST_CASE("bare lifetime counter")
{
    using lc = doctest::util::lifetime_counter<void>;
    using lci = doctest::util::lifetime_counter<int>;

    auto& lc_root = lc::root_lifetime_stats();
    auto& lci_root = lci::root_lifetime_stats();

    doctest::util::lifetime_counter_sentry sentry_lc(lc_root), sentry_lci(lci_root);

    CHECK(&lc_root == &lc::top_lifetime_stats());
    CHECK((const void*)&lc_root != (const void*)&lci_root);

    lc a;
    CHECK(lc_root.living == 1);
    CHECK(lc_root.d_ctr == 1);

    auto b = a;
    CHECK(lc_root.c_ctr == 1);
    CHECK(lc_root.copies == 1);
    CHECK(lc_root.living == 2);

    auto c = std::move(b);
    CHECK(lc_root.m_ctr == 1);
    CHECK(lc_root.copies == 1);
    CHECK(lc_root.living == 3);

    a = c;
    CHECK(lc_root.c_asgn == 1);
    CHECK(lc_root.copies == 2);

    b = std::move(c);
    CHECK(lc_root.m_asgn == 1);
    CHECK(lc_root.living == 3);

    {
        lc::lifetime_stats local;
        doctest::util::lifetime_counter_sentry sentry_local(local);
        lc la;
        CHECK(local.d_ctr == 1);
        CHECK(local.living == 1);
        {
            lc::lifetime_stats sub_local;
            doctest::util::lifetime_counter_sentry sentry_sub_local(sub_local);
            auto slb = la;
            CHECK(sub_local.c_ctr == 1);
            CHECK(sub_local.copies == 1);
            CHECK(sub_local.living == 1);
            CHECK(local.living == 2);
        }
        CHECK(local.living == 1);
    }

    CHECK(lc_root.d_ctr == 2);
    CHECK(lc_root.c_ctr == 2);
    CHECK(lc_root.c_asgn == 1);
    CHECK(lc_root.copies == 3);
    CHECK(lc_root.m_ctr == 1);
    CHECK(lc_root.m_asgn == 1);
    CHECK(lc_root.living == 3);

    lci::lifetime_stats empty_stats;
    CHECK(lci_root == empty_stats);
}
