//
// Created by bogum on 11.10.2022.
//

#ifndef PROJECT2_EXECUTION_H
#define PROJECT2_EXECUTION_H
namespace execution {
    int main(int argc, char *argv[]);
    void singleCoreRun(int scale, std::string* fileName);
    void openMPRun(int scale, std::string* fileName, int core_count);
}
#endif //PROJECT2_EXECUTION_H
