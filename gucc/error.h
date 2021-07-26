#pragma once
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
#include <math.h>
#include <fstream>
#include <algorithm>
#include <thread>
#include "Gucc.h"

#define success true
#define failed false
#define synerror "_0x226009"
#define nullline "_0x436009"
#define undefined "_0x726537"
#define nullerror "_0x102931"
#define memoutofrange "_0x123891"

using namespace std;



namespace error {
	fstream o("out");
	void cerror(string er, int line) {
		if (er == synerror) {
			o << "syntax error::" << synerror << "::" << line << endl;
		} else if (er == nullline) {
			o << "NULL line::" << nullline << "::" << line << endl;
		} else if (er == undefined) {
			o << "undefined::" << undefined << "::" << line << endl;
		} else if (er == nullerror) {
			o << "undefined error::" << nullerror << "::" << line << endl;
		} else {
			cerror(nullerror, line);
		}
	}
}