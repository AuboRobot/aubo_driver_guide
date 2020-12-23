#ifndef TRAJECTORYIO_H
#define TRAJECTORYIO_H
#include <iostream>
#include <fstream>
#include <vector>

class TrajectoryIo
{
public:
    TrajectoryIo(const char *filename);

    std::vector<std::vector<double>> parse();
    std::vector<double> split(const std::string &str, const char *delim);

private:
    std::ifstream input_file_;
};

#endif // TRAJECTORYIO_H
