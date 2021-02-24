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

std::vector<std::vector<double>> TrajectoryIo::parse()
{
    std::vector<std::vector<double>> res;
    std::string tmp;
    while (std::getline(input_file_, tmp, '\n')) {
        auto q = split(tmp, ",");
        res.push_back(q);
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
    while (p) {
        res.push_back(std::atof(p)); //存入结果数组
        p = std::strtok(nullptr, delim);
    }

    if (strs) {
        delete[] strs;
        strs = nullptr;
    }

    return res;
}
