#include <iostream>

using namespace std;

class CharSet
{
public:
	CharSet(string s="") {
		for(char t : s)
		{
			Set(t);
		}
	}
	~CharSet() {}

	void Set(char a) {
		sets_[a / 64] = sets_[a / 64] | (1 << (a % 64));
	}

	void Output(ostream& os) {
		for (size_t i = 0; i < 256; i++)
		{
			if ((sets_[i / 64] >> (i % 64)) & 1) {
				os << (char)i << " ";
			}
		}
	}

private:
	long sets_[4] = { 0,0,0,0 };
};



int main()
{
	CharSet set("abc1!");
	set.Output(cout);
}