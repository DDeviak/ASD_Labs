#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "D:\ASD\doctest.h"
#include <iostream>
#include <string>
#include <bitset>

using namespace std;

struct Markov
{
    string* rules_in = nullptr;
    string* rules_out = nullptr;
    size_t rules_count = 0;

    Markov(string* r_in = nullptr, string* r_out = nullptr, size_t r_c = 0) : rules_count(r_c) {
        rules_in = new string[rules_count];
        rules_out = new string[rules_count];
        for (size_t i = 0; i < rules_count; i++)
        {
            rules_in[i] = r_in[i];
            rules_out[i] = r_out[i];
        }
    }
    ~Markov() {
        if (rules_in!=nullptr)
        {
            delete[] rules_in;
        }
        if (rules_out != nullptr)
        {
            delete[] rules_out;
        }
    }

    string Process(string s) {
        size_t pos;
        for (int i = 0; i < rules_count; i++)
        {
            pos = s.find(rules_in[i], 0);
            if (pos != string::npos)
            {
                s.replace(pos, rules_in[i].length(), rules_out[i]);
                i = -1;
            }
        }
        return s;
    }
};

TEST_CASE("Testing Markov algorithm")
{
    string rules_in[3] = { "1","|0","0" };
    string rules_out[3] = { "0|", "0||", "" };
    Markov m(rules_in, rules_out, 3);

    string res = "";
    bitset<8> bits;
    for (size_t i = 0; i < 256; i++)
    {
        bits = i;
        CHECK(res == m.Process(bits.to_string()));
        res += "|";
    }
}

TEST_CASE("Testing Markov algorithm")
{
    string rules_in[] = { "|b","ab","b","*|","*","|c","ac","c" };
    string rules_out[] = { "ba|","ba","","b*","c","c","c|","" };
    Markov m(rules_in, rules_out, 8);

    string res = "";
    string p1 = "";
    string p2 = "";
    bitset<8> bits;
    for (size_t i = 0; i < 10; i++)
    {
        p1 += "|";
        p2 = "";
        res = "";
        for (size_t j = 0; j < 10; j++)
        {
            p2 += "|";
            res += p1;
            CHECK(res == m.Process(p1 + "*" + p2));
        }
    }
}