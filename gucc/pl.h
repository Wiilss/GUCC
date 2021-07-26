#pragma once

class pl
{
public:
	string getvar(string val);
	void setvar(string val, string r);
	void callfunction(string s, vector<string> arguments);
	string parsevar(string s);
	bool intline(string s, int line);
	void init(void) {
	}
};

