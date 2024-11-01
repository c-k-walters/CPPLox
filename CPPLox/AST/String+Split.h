//
//  String+Split.h
//  CPPLox
//
//  Created by Charles Kelley on 10/25/24.
//

#ifndef STRING_SPLIT_H
#define STRING_SPLIT_H

#include <string>
#include <string_view>
#include <vector>

using namespace std;

vector<string_view> split(string_view str, string delim) {
	vector<string_view> out;

	string_view::size_type start = 0;
	string_view::size_type end   = str.find(delim);

	while (end != string_view::npos)
	{
	  out.push_back(str.substr(start, end - start));

	  start = end + delim.length();
	  end   = str.find(delim, start);
	}

	out.push_back(str.substr(start, end - start));

	return out;
}

#endif
