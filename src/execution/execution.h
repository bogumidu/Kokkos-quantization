//
// Created by bogum on 11.10.2022.
//

#ifndef PROJECT2_EXECUTION_H
#define PROJECT2_EXECUTION_H
namespace execution {
    int main(int argc, char *argv[]);
    void runSingleCore(std::string& fileName,int scale,bool createLog,bool debug);
    void runMultiCore(std::string& fileName, int thread_count, int scale, bool createLog, bool debug);
}
#endif //PROJECT2_EXECUTION_H
