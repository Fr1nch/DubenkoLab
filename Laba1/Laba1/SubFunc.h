#pragma once
#include <cctype>    // std::tolower
#include <algorithm> // std::equal
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

const int INF = 99999;

void inputString(std::istream& in, std::string& str);

bool inString(std::string str_where, std::string str_what);

float tryInputNum(float min, float max);

int tryChoose(int min, int max);

void enterInterval(const std::vector <int>& index, std::vector <int>& index_ch);

void enterPacket(const std::vector <int>& index, std::vector <int>& index_ch);

void enterElement(const std::vector <int>& index, std::vector <int>& index_ch);

void choosingElements(const std::vector <int>& index, std::vector <int>& index_ch);

void dfs(std::vector<std::vector<int>>& graph, int v, std::vector<int>& visited, std::vector<int>& order);

bool loopcheck(std::vector<std::vector<int>>& graph);

std::string chooseFiles(const std::string& path);

struct int_pair
{
	int x, y;
};

class redirect_output_wrapper
{
	std::ostream& stream;
	std::streambuf* const old_buf;
public:
	redirect_output_wrapper(std::ostream& src)
		:old_buf(src.rdbuf()), stream(src)
	{
	}

	~redirect_output_wrapper() {
		stream.rdbuf(old_buf);
	}
	void redirect(std::ostream& dest)
	{
		stream.rdbuf(dest.rdbuf());
	}
};