#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

using std::cout;
using std::endl;

#define typeof

#define line_by_line (unsigned)0x0FFA
#define none (unsigned)0x000
#define split_commas (unsigned)0x2FFB
#define line_by_line_I (unsigned)0x1FFE
#define split_commas_I (unsigned)0xFFFF

template<typename value>
void initvector(int n, std::vector<value> &vector, value v)
{
	for (int i = 0; i < n; i++)
	{
		vector.push_back(v);
	}
}
template<typename Value>
void printvector(std::vector<Value> &vector, unsigned int type = 0x02)
{
	for (int i = 0; i < vector.size(); i++) 
	{
		if (type != none)
		{
			if (type == line_by_line)
			{
				cout << vector[i] << endl;		
			}
			else if (type == line_by_line_I)
			{
				cout << i << ": " << vector[i] << endl;
			}
			else if (type == split_commas)
			{
				if (i == vector.size()-1)
				{
					cout << vector[i] << endl;
				}
				else
				{
					cout << vector[i] << ", ";
				}

			}
			else if (type == split_commas_I)
			{
				if (i == vector.size()-1)
				{
					cout << i << ": " << vector[i] << endl;
				}
				else
				{
					cout << i << ": " << vector[i] << ", ";
				}
			}
			else
			{
				cout << vector[i] << endl;
				type = line_by_line;
			}
		}
		else
		{
			cout << vector[i];
		}

	}
}


namespace tostr {
	using namespace std;
	std::string tostring(void *ptr) {
		std::ofstream convertwrite("convert");
		convertwrite << ptr << endl;
		std::ifstream convertread("convert");
		std::string same;
		std::getline(convertread, same);
		convertwrite.close();
		convertread.close();
		std::ofstream cw("convert");
		cw << "";
		cw.close();
		remove("convert");
		return same;
	}
	std::string tostring(long long int i) {
		return std::to_string(i);
	}
	std::string tostring(long double d) {
		return std::to_string(d);
	}
	std::string tostring(std::vector<std::string> f) {
		std::string m;
		for (int i = 0; i < f.size(); i++) {
			if (i < f.size()) {
				m + (f[i] + ", ");
			} else {
				m + (f[i]);
			}
		}
		return m;
	}

	const char* toconst(int i)
	{
		std::string test = std::to_string(i);
		const char* c = test.c_str();
		return c;
	}
} // namespace tostr




struct var_t {
	std::string stringr = "";
	int intr = 0;
	float floatr = 0.0f;
	double doubler = 0.0;
	long double ldoubler = 0.0;
	char charr = '\0';
};

var_t test(void *data) {
	var_t returner;
	returner.stringr = *(std::string *)data;
	returner.intr = *(int *)data;
	returner.floatr = *(float *)data;
	returner.doubler = *(double *)data;
	returner.charr = *(char *)data;
	return returner;
}

std::string isa(void *ptr) {
	var_t first = test(ptr);
	var_t second = test(ptr);

	if (first.stringr != "") {
		return "string";
	}
	if (first.intr == second.intr && (int)first.charr == first.intr &&	(int)second.charr == second.intr && first.intr != 0) {
		return "int";
	} 
	else if (first.floatr == second.floatr && first.intr > first.floatr && first.doubler < first.floatr) {
		return "float";
	} 
	else if (first.doubler == second.doubler && first.intr < first.doubler && first.floatr < first.doubler) {
		return "double";
	} 
	else {
		return "int";
	}
}

int ID(std::string str) {
	std::string *ptr = &str;
	int ret = test(ptr).intr;
	return ret;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &dt) {
	os << dt[0];
	return os;
}

int isnum(char c) {
	if (c == '0') return 0;
	else if (c == '1') return 1;
	else if (c == '2') return 2;
	else if (c == '3') return 3;
	else if (c == '4') return 4;
	else if (c == '5') return 5;
	else if (c == '6') return 6;
	else if (c == '7') return 7;
	else if (c == '8') return 8;
	else if (c == '9') return 9;

	return -1;
}

void removend(std::string &s) {
	std::string ret;
	for (int i = 0; i < s.length(); i++) {
		if (isnum(s[i]) != -1)
			ret += s[i];
	}
	s = ret;
}

bool digit(std::string str)
{
	if (str.front() == '-') str = str.substr(1);
	for (int i = 0; i < str.length(); i++)
	{
		if (isnum(str[i]) == -1){return false; }
	}

	return true;
}

long long int strtoint(std::string s) {
	bool negative = false;
	if (s[0] == '-') {
		std::string sub = s.substr(1);
		removend(sub);
		negative = true;
		s = sub;
	} else {
		removend(s);
	}
	long long int ret = 0;
	for (int i = 0; i < s.length(); i++) {
		int digit = s.length() - i;
		if (isnum(s[i]) == -1) return -1;
		ret += isnum(s[i]) * pow(10, digit - 1);
	}
	if (negative)
		ret = 0 - ret;
	return ret;
}

void format(void* ptr)
{
	std::string value = isa(ptr);
	var_t values = test(ptr);


	if (value == "string")
	{
		#undef typeof
		#define typeof std::string;
		cout << value << endl;
	}
	else if (value == "int") 
	{
		#undef typeof
		#define typeof int
	}
	else if (value == "float")
	{
		#undef typeof
		#define typeof float
	}
	else if (value == "double")
	{
		#undef typeof
		#define typeof double
	}

}


#define printauto( val ) outformat( val, # val )

class Auto
{
public:
		void* data;

	double asdouble;
	float asfloat;
	int asint;
	std::string asstring;

	bool errors = false;

	Auto(void* ptr = new std::string("nullptr"), bool showerrors = false)
	{
		if (ptr == NULL) ptr = new std::string("nullptr");
		data = ptr;
		var_t values = test(ptr);
		asdouble = values.doubler;
		asfloat = values.floatr;
		asint = values.intr;
		asstring = values.stringr;
		errors = showerrors;
	}

	std::string type(void* ptr = nullptr)
	{  
		if (ptr == nullptr) return isa(data);
		else return isa(ptr);
	}

	void* getptr()
	{
		return data;
	}
	void setptr(void* ptr)
	{
		data = nullptr;
		data = ptr;
	}

	void format()
	{
		cout << "string: \"" << asstring << "\"" << endl;
		cout << "int: " << asint << endl;
		cout << "float: " << asfloat << endl;
		cout << "double: " << asdouble << endl;
		cout << "char: " << (char)asint << "(" << asint << ")"	<< endl;
	}


	bool operator==(Auto &comp)
	{
		if (comp.type() != this->type() && errors)
		{
			std::string error = "\033[1;31mcannot compare types \"" +comp.type()+ "\" and \""+ this->type()+"\"!" + "\033[0m\n";
			cout << error;
		}
		if (comp.type() == "string") {if(comp.asstring == asstring) return true;}
		else if (comp.type() == "int") {if(comp.asint == asint) return true;}
		else if (comp.type() == "float") {if(comp.asfloat == asfloat) return true;}
		else if (comp.type() == "double") {if(comp.asdouble == asdouble) return true;}
		else {if(comp.asint == asint) return true;}
		return false;
	}
	bool operator!=(Auto &comp)
	{
		if(this->operator==(comp)) return false;
		return true;
	}

	std::string string(){
		if (this->type() != "string")
		{
			if (this->type() == "int")
			{
				return std::to_string(this->asint);
			}
			else if (this->type() == "float")
			{
				return std::to_string(this->asfloat);
			}
			else if (this->type() == "double")
			{
				return std::to_string(this->asdouble);
			}
			else return std::to_string(this->asint);
		}
		return this->asstring;
		
		}
	long int Int(){
			if (this->type() != "int")
			{
				if (this->type() == "string" && !digit(asstring))
				{
					return std::stol(this->asstring);
				}
				else if (this->type() == "float")
				{
					return (this->asfloat);
				}
				else if (this->type() == "double")
				{
					return (this->asdouble);
				}
				else return (this->asint);
			}

		return this->asint;
		
		}
	float Float(){
		
			if (this->type() != "float")
			{
				if (this->type() == "string" && !digit(asstring))
				{
					return std::stof(this->asstring);
				}
				else if (this->type() == "int")
				{
					return (this->asint);
				}
				else if (this->type() == "double")
				{
					return (this->asdouble);
				}
				else return (this->asint);
			}
		return this->asfloat;
		}
	double Double(){return this->asdouble;}


	int operator()(unsigned long long int index = SIZE_MAX)
	{

		return index;
	}

	double operator+(Auto a)
	{
		if (a.type() == "string") return 0x271006;
		
		if (a.type() == "int")
		{
			return asint+a.asint;
		}
		else if (a.type() == "float")
		{
			return asfloat+a.asfloat;
		}
		else if (a.type() == "double")
		{
			return asdouble+a.asdouble;
		}
		return 0;

	}

	double operator-(int index)
	{
		if (this->type() == "string") return(false); 
		
		if (this->type() == "int")
		{
			return asint-index;
		}
		else if (this->type() == "float")
		{
			return asfloat-index;
		}
		else if (this->type() == "double")
		{
			return asdouble-index;
		}

		return 0;
	}

	char operator[](int index)
	{
		if (this->type() == "string") return asstring[index]; 
		return false;
	}

	operator int() {return this->Int();}
	operator float() {return this->Float();}
	operator double() {return this->Double();}
	operator std::string() {return this->string();}

	void operator=(void* &equal)
	{
		data = equal;
	}
};

std::ostream &operator<<(std::ostream &os, Auto &dt) {
	if (dt.type() == "string") os<<dt.asstring;
	else if (dt.type() == "int") os<<dt.asint;
	else if (dt.type() == "float") os<<dt.asfloat;
	else if (dt.type() == "double") os<<dt.asdouble;
	else os<<dt.asint;
	return os;
}

void outformat(Auto num, const char* constname)
{
	std::string name = constname;
	std::string string = isa(num.getptr());
	if(string == "string")
	{
		cout << name << " is a " << isa(num.getptr()) << " value: " << *(std::string*)num.getptr() << endl;
	}
	else if (string == "int")
	{
		cout << name << " is an " << isa(num.getptr()) << " value: " << *(int*)num.getptr() << endl;
	}
	else if (string == "float")
	{
		cout << name << " is a " << isa(num.getptr()) << " value: " << *(float*)num.getptr() << endl;
	}
	else if (string == "double")
	{
		cout << name << " is a " << isa(num.getptr()) << " value: " << *(double*)num.getptr() << endl;
	}
}
