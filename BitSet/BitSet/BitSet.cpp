#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "D:\ASD\doctest.h"
#include <iostream>

using namespace std;

class CharSet
{
public:
	CharSet(string s="") {
		for(char t : s)
		{
			Add(t);
		}
	}
	~CharSet() {}

	void Add(char a) {
		sets_[a / 32] = sets_[a / 32] | (1 << (a % 32));
	}
	void Delete(char a) {
		sets_[a / 32] = sets_[a / 32] & (~(1 << (a % 32)));
	}

	bool Test(char a) {
		return ((sets_[a / 32] >> (a % 32)) & 1) != 0;
	}

	CharSet& Association(CharSet& set) {
		CharSet result;
		for (size_t i = 0; i < 8; i++)
		{
			result.sets_[i] = this->sets_[i] | set.sets_[i];
		}
		return result;
	}
	CharSet& Intersection(CharSet& set) {
		CharSet result;
		for (size_t i = 0; i < 8; i++)
		{
			result.sets_[i] = this->sets_[i] & set.sets_[i];
		}
		return result;
	}
	CharSet& Difference(CharSet& set) {
		CharSet result;
		for (size_t i = 0; i < 8; i++)
		{
			result.sets_[i] = this->sets_[i] & (~set.sets_[i]);
		}
		return result;
	}

	friend bool operator==(CharSet& t1, CharSet& t2) {
		bool result = true;
		for (size_t i = 0; i < 8; i++)
		{
			if (t1.sets_[i] != t2.sets_[i])result = false;
		}
		return result;
	}

	void Output(ostream& os) {
		for (size_t i = 0; i < 256; i++)
		{
			if (((sets_[i / 32] >> (i % 32)) & 1) != 0) {
				os << (char)i << " ";
			}
		}
	}

private:
	unsigned int sets_[8] = { 0,0,0,0,0,0,0,0 };
};

TEST_CASE("Testing Add method") {
	CharSet s;
	s.Add('a');
	s.Add('b');
	s.Add('c');
	CHECK(s.Test('a') == true);
	CHECK(s.Test('b') == true);
	CHECK(s.Test('c') == true);
	CHECK(s.Test('d') == false);
}

TEST_CASE("Testing Delete method") {
	CharSet s("abc");
	s.Delete('a');
	CHECK(s.Test('a') == false);
	CHECK(s.Test('b') == true);
	CHECK(s.Test('c') == true);
}

TEST_CASE("Testing Test method") {
	CharSet s("abc");
	CHECK(s.Test('a') == true);
	CHECK(s.Test('b') == true);
	CHECK(s.Test('c') == true);
	CHECK(s.Test('d') == false);
}

TEST_CASE("Testing Association method") {
	CharSet s1("abc");
	CharSet s2("bcd");
	s1 = s1.Association(s2);
	CHECK(s1.Test('a') == true);
	CHECK(s1.Test('b') == true);
	CHECK(s1.Test('c') == true);
	CHECK(s1.Test('d') == true);
}

TEST_CASE("Testing Intersection method") {
	CharSet s1("abc");
	CharSet s2("bcd");
	s1 = s1.Intersection(s2);
	CHECK(s1.Test('a') == false);
	CHECK(s1.Test('b') == true);
	CHECK(s1.Test('c') == true);
	CHECK(s1.Test('d') == false);
}

TEST_CASE("Testing Difference method") {
	CharSet s1("abc");
	CharSet s2("bcd");
	s1 = s1.Difference(s2);
	CHECK(s1.Test('a') == true);
	CHECK(s1.Test('b') == false);
	CHECK(s1.Test('c') == false);
	CHECK(s1.Test('d') == false);
}

TEST_CASE("Testing operator ==") {
	CharSet s1("abc");
	CharSet s2("abc");
	CharSet s3("bcd");
	CHECK(s1 == s2);
	CHECK(!(s1 == s3));
}