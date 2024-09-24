/**
 * MIT License
 * Copyright (c) 2020 Mitosis-Project
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define CONFIG_RAND_SEED 42

/*
#ifdef HIGH_
    #define CONFIG_CACHELINE_SIZE 64
    #define CONFIG_LARGE_PAGE_SIZE (1 << 21)
    #define CONFIG_DEFAULT_OUTER_SIZE 1000000
    #define CONFIG_DEFAULT_INNER_SIZE 100000
    #define CONFIG_DEFAULT_HASH_SIZE CONFIG_DEFAULT_INNER_SIZE
    #define CONFIG_DEFAULT_NUM_LOOKUPS 250
    #define CONFIG_INNER_KEY_STRIDE 4
    #define CONFIG_ELEMENT_TUPLE_SIZE 15

#elif LOW_
    #define CONFIG_CACHELINE_SIZE 64
    #define CONFIG_LARGE_PAGE_SIZE (1 << 21)
    #define CONFIG_DEFAULT_OUTER_SIZE 500000
    #define CONFIG_DEFAULT_INNER_SIZE 100000
    #define CONFIG_DEFAULT_HASH_SIZE CONFIG_DEFAULT_INNER_SIZE
    #define CONFIG_DEFAULT_NUM_LOOKUPS 250
    #define CONFIG_INNER_KEY_STRIDE 4
    #define CONFIG_ELEMENT_TUPLE_SIZE 15

#elif LOW1_
    #define CONFIG_CACHELINE_SIZE 64
    #define CONFIG_LARGE_PAGE_SIZE (1 << 21)
    #define CONFIG_DEFAULT_OUTER_SIZE 650000
    #define CONFIG_DEFAULT_INNER_SIZE 100000
    #define CONFIG_DEFAULT_HASH_SIZE CONFIG_DEFAULT_INNER_SIZE
    #define CONFIG_DEFAULT_NUM_LOOKUPS 250
    #define CONFIG_INNER_KEY_STRIDE 4
    #define CONFIG_ELEMENT_TUPLE_SIZE 15

#else*/
    #define CONFIG_CACHELINE_SIZE 64
    #define CONFIG_LARGE_PAGE_SIZE (1 << 21)
    #define CONFIG_DEFAULT_OUTER_SIZE 500000
    #define CONFIG_DEFAULT_INNER_SIZE 100000
    #define CONFIG_DEFAULT_HASH_SIZE CONFIG_DEFAULT_INNER_SIZE
    #define CONFIG_DEFAULT_NUM_LOOKUPS 100
    #define CONFIG_INNER_KEY_STRIDE 4
    #define CONFIG_ELEMENT_TUPLE_SIZE 15
//#endif
