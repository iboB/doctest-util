// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <random>
#include <cstdio>

// a macro to get a random seed from std::random_device
// it prints the seed to stdout for debugging purposes
// use in fuzz tests or other places where you need an auditable random seed
#define GET_RANDOM_DEVICE_SEED(name) \
    [] { \
        std::random_device rd; \
        auto seed = rd(); \
        std::printf("%s:%d %s random seed: %u\n", __FILE__, __LINE__, name, seed); \
        return seed; \
    }()
