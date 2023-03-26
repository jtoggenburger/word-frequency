//-----------------------------------------------
// Jack Toggenburger
// jtoggenb
// WordFrequency.cpp
//-----------------------------------------------


#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <algorithm>

#include "Dictionary.h"

using namespace std;

int main (int argc, char * argv[]) {

	ifstream in;
	ofstream out;

	if (argc != 3) {
		cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
		return (EXIT_FAILURE);
	}

	in.open(argv[1]);
	if (!in.is_open()) {
		cerr << "Unable to open file " << argv[1] << " for reading" << endl;
		return (EXIT_FAILURE);
	}

	out.open(argv[2]);
        if (!out.is_open()) {
                cerr << "Unable to open file " << argv[2] << " for writing" << endl;
                return (EXIT_FAILURE);
        }

	Dictionary A = Dictionary();

	// string
	string delim = " \t\\\"\',<.>/?;:[{]}|`~!@#$%^&*()-_=+0123456789";
	size_t begin, end, len;	

	string line;
	string tokenBuffer;
	string token;
	int line_count = 0;

	while (getline(in, line)) {
		line_count++;

		len = line.length();

		// first token
		begin = min(line.find_first_not_of(delim, 0), len);
		end = min(line.find_first_of(delim, begin), len);
		token = line.substr(begin, end-begin);

		//convert to lower
		std::for_each(token.begin(), token.end(), [](char & c) {
			c = ::tolower(c);
		});


		if (token != "" && token != "") {
			// add the token to the dictionary
			if (A.contains(token)) {
				A.getValue(token) = A.getValue(token) + 1;
			}

			else {
				A.setValue(token, 1);
			}
		}


		while (token != "") {
			// get next token
			
			begin = min(line.find_first_not_of(delim, end + 1), len);
                	end = min(line.find_first_of(delim, begin), len);
                	token = line.substr(begin, end-begin);	

			std::for_each(token.begin(), token.end(), [](char & c) {
                        	c = ::tolower(c);
                	});
	
			if (token != " " && token != "") {
				// add the token to the dictionary
				if (A.contains(token)) {
                        		A.getValue(token) = A.getValue(token) + 1;       
                		}

                		else {
                        		A.setValue(token, 1);
                		}
			}
		}		


	}
		
	//prints

	out << A;

	return (EXIT_SUCCESS);
}
