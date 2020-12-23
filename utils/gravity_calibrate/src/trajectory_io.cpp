#include "trajectory_io.h"
#include <iostream>
#include <cstring>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>

TrajectoryIo::TrajectoryIo(const char *filename)
{
    input_file_.open(filename, std::ios::in);
}

TrajectoryIo::~TrajectoryIo()
{
    input_file_.close();
}

std::vector<std::vector<double>> TrajectoryIo::parse()
{
    std::vector<std::vector<double>> res;
    std::string tmp;
    int linenum = 1;
    while (std::getline(input_file_, tmp, '\n')) {
        try {
            auto q = split(tmp, ",");
            res.push_back(q);
        } catch (const char *p) {
            std::cerr << "Line: " << linenum << " \"" << p << "\""
                      << " is not a number of double" << std::endl;
            break;
        }
        linenum++;
    }
    return res;
}

std::vector<double> TrajectoryIo::split(const std::string &str,
                                        const char *delim)
{
    std::vector<double> res;
    if ("" == str) {
        return res;
    }
    //先将要切割的字符串从string类型转换为char*类型
    char *strs = new char[str.length() + 1]; //不要忘了
    std::strcpy(strs, str.c_str());

    char *p = std::strtok(strs, delim);
    char *endp = nullptr;
    while (p) {
        double v = std::strtod(p, &endp);
        if (endp[0] != 0 && endp[0] != '\r') {
            delete[] strs;
            strs = nullptr;
            throw p;
        }
        res.push_back(v); //存入结果数组
        p = std::strtok(nullptr, delim);
    }

    if (strs) {
        delete[] strs;
        strs = nullptr;
    }

    return res;
}
