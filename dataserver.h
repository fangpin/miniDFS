#ifndef _DATA_SERVER_H
#define _DATA_SERVER_H

#include <string>
#include <mutex>
#include <condition_variable>

extern int chunkSize;

class DataServer{
private:
    std::string name_;
    double size_;

    void put();
    void read();
    void locate();
    void fetch();
public:
    std::mutex mtx;
    std::condition_variable cv;
    std::string cmd;
    int fid, bufSize, offset;
    char* buf;
    bool finish;

    DataServer(const std::string &name);
    void operator()();
    double size()const{return size_;}
    std::string get_name()const;
};

#endif
