#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <sstream>
using namespace std;

enum Movement
{
	kStay,
	kLeft,
	kRight
};


struct Rule
{
	char a;
	size_t state;
	Movement move;
	Rule(char symbol, size_t state, Movement direction) : a(symbol), state(state), move(direction){}
};

class TuringMachine
{
public:
	static const char kEmptyChar = 0;
	list<char>* string_;
	
	vector<map<char, Rule>> rules_;

	TuringMachine(vector<map<char, Rule>> rules) : rules_(rules) {}

	bool Step() {
		map<char, Rule>::iterator t = rules_[state_].find(*head_);
		if (t == rules_[state_].end())return false;
		(*head_) = t->second.a;
		state_ = t->second.state;
		switch (t->second.move)
		{
		case kLeft:
			left();
			break;
		case kRight:
			right();
			break;
		}
		return true;
	}

	void Process() {
		while (Step()){}
	}

	void Set(list<char> *string, list<char>::iterator head, size_t state) {
		string_ = string;
		head_ = head;
		state_ = state;
	}

private:
	list<char>::iterator head_;
	size_t state_;

	void left() {
		if (head_ == string_->begin()) {
			string_->push_front(kEmptyChar);
		}
		head_--;
	}
	void right() {
		head_++;
		if (head_ == string_->end()) {
			head_--;
			string_->push_back(kEmptyChar);
			head_++;
		}
	}

};


int main()
{
	string abc = 0 + "ab";
	vector<map<char, Rule>> ruleset(4);
	ruleset[1].insert(pair<char, Rule>('a', Rule(TuringMachine::kEmptyChar, 2, kRight)));
	ruleset[1].insert(pair<char, Rule>('b', Rule(TuringMachine::kEmptyChar, 3, kRight)));
	ruleset[2].insert(pair<char, Rule>(TuringMachine::kEmptyChar, Rule('a', 0, kRight)));
	ruleset[2].insert(pair<char, Rule>('a', Rule('a', 2, kRight)));
	ruleset[2].insert(pair<char, Rule>('b', Rule('b', 2, kRight)));
	ruleset[3].insert(pair<char, Rule>(TuringMachine::kEmptyChar, Rule('a', 0, kRight)));
	ruleset[3].insert(pair<char, Rule>('a', Rule('a', 3, kRight)));
	ruleset[3].insert(pair<char, Rule>('b', Rule('b', 3, kRight)));

	stringstream ss;
	ss << "abb";
	list<char> string_;
	copy(istream_iterator<char>(ss), istream_iterator<char>(), back_insert_iterator<list<char>>(string_));

	TuringMachine TM(ruleset);
	TM.Set(&string_, string_.begin(), 1);
	TM.Process();
	copy(TM.string_->begin(), TM.string_->end(), ostream_iterator<char>(cout));
}