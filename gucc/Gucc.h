#pragma once
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <ostream>
#include "pl.h"

using namespace std;

namespace gucc {

}

class var {
public:
	string datatype;
	string name;
	bool constant = false;
	string value = "";
	var(string s, string v, bool con, string d = "string") {
		name = s;
		datatype = d;
		value = v;
		constant = con;
	}
	bool operator==(const var& s)
	{
		if (s.constant == constant && s.datatype == datatype && s.name == name && s.value == value) return true;
		return false;
	}
	bool operator!=(const var& s)
	{
		if (s.constant == constant && s.datatype == datatype && s.name == name && s.value == value) return false;
		return true;
	}

};

class function {
public:
	string name;
	string exec;
	vector<string> arguments;
	function(string s, string v) {
		name = s;
		exec = v;
	}
	string returner = "";
};