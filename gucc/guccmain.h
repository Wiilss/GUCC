#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <ostream>
#include <algorithm>
#include <vector> 
#include <istream>
#include <stdio.h>
#include <cmath>
#include <fstream>
#include <array>
#include <thread>
#include "stringM"
#include "Gucc.h"
#include "error.h"
#include "pl.h"

#include <curses.h>

#include <unistd.h>

#if defined(_WIN32)
    #define wait Sleep // Windows
#elif defined(_WIN64)
    #define wait Sleep // Windows
#elif defined(__CYGWIN__) && !defined(_WIN32)
    #define wait Sleep
#elif defined(__linux__)
    #define wait sleep
#endif


#define null var("nullptr", "nullptr", 0, "nullptr")
using namespace std;
using namespace gucc;
using namespace error;

const string ftype = ".gucc";
const string fname = "main";

// ofstream OUT_TXT("gucc/out.txt", ios::app);
ofstream reg("gucc/registry.txt", ios::app);
ostream out(cout.rdbuf());
ifstream file(fname + ftype);

bool experimentalversion = false;
char splitter = '>';
unsigned int arch = 8;
int executetimout = 0;
vector<var> vars;
vector<var*> ptrs;
vector<function> functions;

vector<ofstream> writes;
vector<ifstream> reads;
vector<fstream> files;

string insert(string s, int pos)
{
	string ret;
	return ret;
}

string pl::getvar(string val) {
	string r;
	for (int i = 0; i < vars.size(); i++) {
		if (vars[i].name == val) {
			return vars[i].value;
		}
	}
	return val;
}

var getvarp(string val)
{
	var r = null;
	for (int i = 0; i < vars.size(); i++) {
		if (vars[i].name == val) {
			return vars[i];
		}
	}
	return r;
}

vector<string> params(string s, bool vals)
{
	vector<string> args = psplit(s, ' ');

	vector<string> a;

	for (int i = 0; i < args.size(); i++) {
		if (args[i].front() == '~')
		{

			string name = args[i].substr(1);

			var v(name,"", 0);

			bool su = false;
			if (!vals)
			{
				for (int c = 0; c < vars.size(); c++) {
					if (vars[c].name == name) {
						su = true;
					}
				}
				if (!su) {
					vars.push_back(v);

				}
			}
			a.push_back(name);
		}

	}
	return a;
}

void pl::setvar(string val, string r) {
	for (int i = 0; i < vars.size(); i++) {
		if (vars[i].name == val) {
			vars[i].value = r;
		}
	}
	return;
}

void pl::callfunction(string s, vector<string> arguments = {}) {
	for (int i = 0; i < functions.size(); i++) {
		if (functions[i].name == s) {
			for (int c = 0; c < arguments.size(); c++)
			{
				string f = "redef>"+functions[i].arguments[c];
				cout << f << endl;
				intline(f, -1);
			}
			intline(functions[i].exec, -1);
		}
	}
}

string pl::parsevar(string s) {
	string ret;
	char prev;
	string con = "";
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == '\\' && s.length() >= i+1 && s.at(i+1) == '?')
		{
			i+=2;
		}
		else
		{
			con+=s[i];
		}
	}
	vector<string> p = psplit(s, ' ');

	for (int i = 0; i < p.size(); i++) {
		

	  if (p[i].front() == '%') {
			string va = getvar(p[i].substr(1));
			ret += va;
		}
		else if (p[i] == "\\n" || p[i] == "\\endl") {
			ret += '\n';
		}
		else if (p[i] == "\\p") {
			ret += ' ';
		}
		else if (p[i] == "\\<") {
			ret += '>';
		}

		else if (p[i].front() == '*')
		{
			var v = getvarp(p[i].substr(1));
			if (v != null)
			{
				std::ofstream convertwrite("convert.catos"); convertwrite << &v << endl; std::ifstream convertread("convert.catos"); std::string same; std::getline(convertread, same);
				ret += same + ' ';
			}
			else
			{
				ret += "*nullptr ";
			}
			
		}
		else if (p[i].front() == '\\')
		{
			ret += p[i].at(1) + ' ';
		}
		else {
			ret += p[i] + ' ';
		}
	}
	return ret;
}

int times = 1;

bool pl::intline(string s, int line) {
	bool func = false;
	bool comment = false;
	bool ifstate = false;
	if (s == "") {
		//error::cerror(nullline, line);
		return failed;
	}

	if (!Validate(s.find(">"), s)) { error::cerror(synerror, line); return failed; }

	vector<string> arg = psplit(s, '>');



	if (arg[0].substr(0, 4) == "init")
	{
		if (arg[0].find(")") != string::npos)
		{
			string params = arg[0].substr(arg[0].find("("));
			params = params.substr(0, params.find(")"));
			params = params.substr(1);
			params = remc(params, ' ');
			vector<string> parr = psplit(params, ',');
			for (int i = 0; i < parr.size(); i++)
			{
				if (i == 0)
				{
					int ar = stoi(parr[i]);
					if (ar > 16 || ar < 0)
					{ar = 8; intline("print>#(31)Bits out of range!>print>#(32)(set to 8-bit) \\n", 0);}
					arch = ar;
				}
				else if (i == 1)
				{
					if (parr[i] == "false") experimentalversion = false;
					else if (parr[i] == "true") experimentalversion = true;
					else experimentalversion = false;
				}
				else if (i == 2)
				{
					if (parr[i] != "nullptr")
					{
						int ar = stoi(parr[i]);
						executetimout = ar;
					}
				}
				else if (i == 3)
				{
					if (experimentalversion) splitter = parr[i].front();
				}
				reg << &params << "*" << params << "->" << &parr << endl;
			}
		}
	}

	arg = psplit(s, splitter);

	for (int i = 0; i < arg.size(); i++) {
		int a = pow(2, arch);if (vars.size() > a) {out << "\033[1;" << "31" << "m" << "Out of memory" << "\033[0m\n"; return failed;}
		int def = i + 2;
		int name = i + 1;
		int value = i + 2;
		int t = i +3;
		int functt = i + 4;
		int el = i + 5;
		int next = i + 1;
		//removefb(arg[i]);
		bool comm = true;

		int mem = 0;

		for (int c = 0; c < functions.size(); c++)
		{
			mem+=functions[c].name.length()+functions[c].exec.length();
		}
		for (int c = 0; c < vars.size(); c++)
		{
			mem+=vars[c].name.length()+vars[c].value.length();
		}

		setvar("memoryusage", to_string((int)mem));
		setvar("bits", to_string((int)arch));

		if (arg[i] == "print" && !func && !comment) {

			if (arg.size() > next) {

				if (arg[next].front() == '#' && arg[next].length() > 1 && arg[next].at(1) == '(')
				{
					string col = arg[next].substr(2);
					if (col.find(")") == string::npos) intline("print>#(31)no close brackets \n>", 0);
					else
					{
						int cd = col.find(")") + 3;
						col = col.substr(0, col.find(")"));


						string pr = parsevar(arg[next].substr(cd));
						string p = "\033[1;" + col + "m" + pr + "\033[0m";
						out << p;
					}

				}
				else
				{
					string pr = parsevar(arg[next]);
					out << pr;
				}

       reg << &arg[next] << "*" << "->" << "\"" << arg[next] << "\"" << endl;

			}
			else {
				error::cerror(synerror, line);
				return failed;
			}
		}

		else if (arg[i] == "*ptr" && !func && !comment) 
		{
			
		}

		else if (arg[i] == "C{}")
		{
			comment = true;
		} 

		else if (arg[i] == "{}C" && comment)
		{
			comment = false;
		}

		else if (arg[i].front() == '+'&& !func && !comment)
		{
			int num = stoi(arg[i].substr(1));
			for (int i = 0; i < vars.size(); i++) {
				if (vars[i].name == arg[name]) {
					vars[i].value = to_string(stoi(vars[i].value)+ num);
				}
			}
		}
		else if (arg[i].front() == '-'&& !func && !comment)
		{
			int num = stoi(arg[i].substr(1));
			for (int i = 0; i < vars.size(); i++) {
				if (vars[i].name == arg[name]) {
					vars[i].value = to_string(stoi(vars[i].value) - num);
				}
			}
		}

		else if (arg[i].front() == '^'&& !func && !comment)
		{
			int num = stoi(arg[i].substr(1));
			for (int i = 0; i < vars.size(); i++) {
				if (vars[i].name == arg[name]) {
					vars[i].value = to_string(stoi(vars[i].value) * num);
				}
			}
		}

		else if (arg[i].front() == '/'&& !func && !comment)
		{
			int num = stoi(arg[i].substr(1));
			for (int i = 0; i < vars.size(); i++) {
				if (vars[i].name == arg[name]) {
					vars[i].value = to_string(stoi(vars[i].value) / num);
				}
			}
		}
		else if (arg[i] == "value" && !func && !comment)
		{
			intline("define>"+arg[next]+">"+arg[value], 0);
		}
		else if (arg[i] == "setting" && !func && !comment)
		{
			intline("define>"+arg[next]+">"+arg[value], 0);
		}

		else if (arg[i] == "set" && !func && !comment)
		{
			intline("redef>"+arg[next]+">"+arg[value], 0);
		}

		else if (arg[i] == "define" && !func && !comment) {
			if (arg.size() > def) {

				string type = "string";
				bool cons = false;
				int sub = 0;
				if (arg[value].front() == '*')
				{

					char datatype = arg[value].at(1);
					if (datatype == 'C')
					{
						cons = true;
					}
					else if (datatype == 'c')
					{
						type = "char";
					}
					else if (datatype == 's' || datatype == 'S')
					{
						type = "string";
					}
					sub = 2;
				}

				var v(arg[name], parsevar(arg[value].substr(sub)), cons, type);
				reg << &v << "*" << v.name << "->" << &arg[value] << endl;
				bool su = false;
				for (int c = 0; c < vars.size(); c++) {
					if (vars[c].name == arg[name]) {
						su = true;
					}
				}
				if (!su) {
					int val = vars.size()-1;
					vars.push_back(v);
					ptrs.push_back(&vars[val]);
				}

			}
			else {
				error::cerror(synerror, line);
				return failed;
			}
		}
		else if (arg[i] == "compile")
		{
			if (arg.size() > 1 && arg[name] != "!this")
			{
				fstream usefile(arg[name]);
				if (!usefile.fail())
				{
					string compile;
					string lin;
					while (getline(usefile, lin)) {
						compile += lin;
					}
					intline(compile, 0);
				}
				else
				{
					out << "Could not open file '" << arg[name] << "'" << endl;
				}
			}
		}

		else if (arg[i] == "loop{" && !func && !comment)
		{
			int n = i + 1;
			int repeat = stoi(parsevar(arg[n]));

			string excep;
			for (int f = i + 2; f < arg.size(); f++) {
				if (arg[f] == "}loop") {
					break;
				}
				else {
					excep += arg[f] + '>';
				}
			}
			for (int c = 0; c < repeat; c++)
			{
				intline(excep, line);
				
			}

		}
		else if (arg[i] == "using" && !comment)
		{
			if (arg.size() > 1)
			{
				fstream usefile(arg[name]);
				if (!usefile.fail())
				{
					string compile;
					string lin;
					while (getline(usefile, lin)) {
						compile += lin;
					}
					intline(compile, 0);

				}
				else
				{
					out << "Could not open file '" << arg[name] << "'" << endl;
				}
			}

		}

		else if (arg[i] == "redef" && !func && !comment) {

			for (int c = 0; c < vars.size(); c++) {
				if (vars[c].name == arg[name] && !vars[c].constant) {
					vars[c].value = parsevar(arg[value]);
				}
				else if (vars[c].name == arg[name] && vars[c].constant)
				{
					intline("print>#(31)cannot redefine constant variable ("+vars[c].name+") \\n", 0);
				}
			}
		} 
		
		else if (arg[i] == "function" && !func && !comment) {
			func = true;

			int st = 2;
			bool par = false;

			if (arg[value].front() == '~')
			{
				st++;
				params(arg[value], true);
				par = true;
			}

			string excep;
			for (int f = i+st; f < arg.size(); f++) {
				if (arg[f] == "end") {
					break;
				}
				else {
					excep += arg[f] + '>';
				}
			}
			function v(arg[name], excep);
			if (par)
			{
				string parr = "";
				for (int i = 0; i < params(arg[value], true).size(); i++)
				{
					parr += params(arg[value], true)[i] + ", ";
				}
				parr = parr.substr(0, parr.length() - 2);
				reg << &v << "*" << v.name << "(" << parr << ")" << "->" << &arg[value] << endl;
			}
			else
			{
				reg << &v << "*" << v.name << "()" << "->" << &arg[value] << endl;
			}
			
			bool su = false;
			for (int c = 0; c < functions.size(); c++) {
				if (functions[c].name == arg[name]) {
					su = true;
				}
			}
			if (!su) {
				functions.push_back(v);
			}

		} 
		
		else if (arg[i] == "!terminate")
		{
			return success;
		}

		else if (arg[i] == "call" && !func) {

			bool par(false);
			if (arg[value].front() == '~')
			{
				callfunction(arg[value], params(arg[value], true));
				par = true;
			}
			else
			{
				callfunction(arg[name]);
			}

			if (par)
			{
				string parr = "";
				for (int i = 0; i < params(arg[value], true).size(); i++)
				{
					parr += params(arg[value], true)[i] + ", ";
				}
				parr = parr.substr(0, parr.length() - 2);
				reg << &par << "*>" << arg[name] << "(" << parr << ")" << "->" << &arg[value] << endl;
			}
			else
			{
				reg << &par << "*>" << arg[name] << "()" << "->" << &arg[value] << endl;
			}

		} 
		
		else if (arg[i] == "write" && !func && !comment)
		{

			string file = arg[next];
			ofstream f(file, ios::app);
			f << parsevar(arg[value]);
		}
		else if (arg[i] == "readl" && !func && !comment)
		{
			string fl;
			fstream fi((arg[next]));
			if (fi.fail()) intline("print>#(31)could not open file\""+arg[next]+"\"\n", 0);
			ifstream os(arg[next]);
			getline(os, fl);
			setvar(arg[value], fl);
		}
		else if (arg[i] == "reada" && !func && !comment)
		{
			string fl;
			fstream fi((arg[next]));
			if (fi.fail()) intline("print>#(31)could not open file\""+arg[next]+"\"\n", 0);
			ifstream os(arg[next]);

			string line;
			while(getline(os, line))
			{
				
				fl+=line+'\n';
			}
			setvar(arg[value], fl);
		}
		else if (arg[i] == "clearfile" && !func && !comment)
		{
			string file = arg[next];
			ofstream f(file);
		}
		else if (arg[i] == "read" && !func && !comment)
		{
			intline("reada>"+arg[next]+arg[value]+">END>", 0);
		}
		else if (arg[i] == "file" && !func && !comment)
		{

		}
		
		else if (arg[i] == "end" && func && !comment) {
			func = false;
		} 
		
		else if (arg[i].front() == '{' && !func&& !comment)
		{
			intline("call>"+arg[i].substr(1)+">END", 0);
		}
		
		else if (arg[i] == "getline" && !func&& !comment)
		{
			string getl;
			getline(cin, getl);
			setvar(arg[next], getl);
		}
		
		else if (arg[i] == "source" && !func&& !comment)
		{
			intline(parsevar(arg[next]), 0);
		}
		else if (arg[i].front() == '}' && !func&& !comment)
		{
			intline("getline>"+arg[i].substr(1)+">END", 0);
		}
		
		else if (arg[i] == "out" && !func&& !comment)
		{
			if (arg.size() > 1)
			{
				if (arg[name] == "console")
				{

				}
				else if (arg[name] == "default" || arg[name] == "")
				{
				}
			}
		}

		else if (arg[i] == "if" && !func&& !comment) {
			string com1 = arg[next];
			com1 = parsevar(com1);
			char op = arg[value].front();
			string com2 = arg[t];
			string funct = arg[functt];
			com2 = parsevar(com2);
			if (op == '=') {
				if (com1 == com2) {
					callfunction(funct);
				}
			}
			if (arg.size() > el) {
				if (arg[el] == "else") {

					if (com1 != com2) {
						int f = el + 1;
						callfunction(arg[f]);
					}
				}
			}

		}
		
		else {
			//error::cerror(synerror, line);
			comm = false;
		}
		if (comm)
		{
		reg << &arg[i] << "*%" << arg[i] << "->" << &arg << endl;
		}
		else
		{
			
		}


		wait(executetimout);
	}
	return success;
}

void compileprogram()
{
	string compiledp;
	string l;
	ifstream f("source/main.cpp");
	while (getline(f, l))
	{
		if (l == "string compile;")
		{
			string fi;
			string line;
			while (getline(file, line))
			{
				fi += line + "\\n";
			}
			compiledp += "string compile = \""+fi+"\";";
		}
		else
		{
			compiledp += l + '\n';
		}
	}
	ofstream w("source/test.cpp");
	w << compiledp;
	//system("g++ --version");
	//system("g++.exe -o main.exe source/test.cpp");
}
pl Main;
int guccmain(std::string f = "gucc/main.gucc", bool commandline = false) 
{
	ofstream del("gucc/registry.txt");
	del << "";
	del.close();

	file = ifstream(f);

	string line;

	string compile;


	Main.init();
	int lin = 1;
	
	while (getline(file, line)) {
		compile+=line;
		for (int i = 0; i < vars.size(); i++) {
			cout << vars[i].name << "::" << vars[i].value << endl;
		}
		lin++;
	}

	if (compile.back() != '>')
	{
		compile+=">END";
	}
	else
	{
		compile+="END";
	}
	Main.intline("define>memoryusage>*Cnullptr>", 0);
	Main.intline("define>bits>*Cnullptr>", 0);
	Main.intline(compile, 0);
	

	while (commandline)
	{
		string command;

		cout << endl << "->";
		getline(cin, command);
		if (command == "!terminate")
		{
			Main.intline("print>#(33)Program terminated! \n", 0);
			break;
		}
		
  	if (command.back() != splitter)
	  {
		command+=splitter;
		command +="END";
	  }
	  else
	  {
		command+="END";
  	}

		Main.intline(command, 0);
	}
	return 0;
}

//g++.exe -o helloworld.exe pl.cpp
//params(16, true, nullptr, ])