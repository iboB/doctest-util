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
    CHECK(lc_root.total == 1);

    auto b = a;
    CHECK(lc_root.c_ctr == 1);
    CHECK(lc_root.copies == 1);
    CHECK(lc_root.living == 2);
    CHECK(lc_root.total == 2);

    auto c = std::move(b);
    CHECK(lc_root.m_ctr == 1);
    CHECK(lc_root.copies == 1);
    CHECK(lc_root.living == 3);
    CHECK(lc_root.total == 3);

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
        CHECK(local.total == 2);
    }

    CHECK(lc_root.d_ctr == 2);
    CHECK(lc_root.c_ctr == 2);
    CHECK(lc_root.c_asgn == 1);
    CHECK(lc_root.copies == 3);
    CHECK(lc_root.m_ctr == 1);
    CHECK(lc_root.m_asgn == 1);
    CHECK(lc_root.living == 3);
    CHECK(lc_root.total == 5);

    doctest::util::lifetime_stats empty_stats;
    CHECK(lci_root.checkpoint() == empty_stats);
}

struct foo : public doctest::util::lifetime_counter<foo> {
    int i = 5;
};

TEST_CASE("struct lc") {
    foo::lifetime_stats stats;
    CHECK(stats.total == 0);

    foo f1;
    CHECK(stats.total == 1);

    {
        foo::lifetime_stats lstats;
        CHECK(lstats.total == 0);
        CHECK(lstats.d_ctr == 0);

        foo f2;
        auto f3 = f2;

        CHECK(lstats.total == 2);
        CHECK(lstats.living == 2);
        CHECK(lstats.d_ctr == 1);
        CHECK(lstats.c_ctr == 1);
    }

    CHECK(stats.total == 3);
    CHECK(stats.living == 1);
}

TEST_CASE("struct no stats") {
    // check nullptr accesses in lifetime counter
    foo f1;
    auto f2 = f1;
    auto f3 = std::move(f2);
    f3 = f1;
    f1 = std::move(f2);
}