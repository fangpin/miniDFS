#include <thread>
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "dataserver.h"


int chunkSize = 2 * 1024 * 1024;

DataServer::DataServer(const std::string &name):name_(name), buf(nullptr), finish(true){
    std::string cmd = "mkdir -p " + name_;
    system(cmd.c_str());
}

void DataServer::operator()(){
    while(true){
        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk, [&](){return !this->finish;});
        if (cmd == "put"){
            size_ += bufSize / 1024.0 / 1024.0;
            put();
        }
        else if(cmd == "read")
            read();
        else if(cmd == "locate")
            locate();
        else if(cmd == "fetch")
            fetch();
        this->finish = true;
        lk.unlock();
        cv.notify_all();
    }
}


void DataServer::put(){
    int start = 0;
    std::ofstream os;
    while(start < bufSize ){
        int offset = start / chunkSize;
        std::string filePath = name_ + "/" + std::to_string(fid) + " " + std::to_string(offset);
        os.open(filePath);
        if(!os)
            std::cerr << "create file error in dataserver: (file name) " << filePath << std::endl;
        os.write(&buf[start], std::min(chunkSize, bufSize - start));
        start += chunkSize;
        os.close();
    }
}


void DataServer::read(){
    int start = 0;
    buf = new char[bufSize];
    while(start < bufSize){
        int offset = start / chunkSize;
        std::string filePath = name_ + "/" + std::to_string(fid) + " " + std::to_string(offset);
        std::ifstream is(filePath);
        // file found not in this server.
        if(!is){
            delete []buf;
            bufSize = 0;
            break;
        }
        is.read(&buf[start], std::min(chunkSize, bufSize - start));
        start += chunkSize;
    }
}

void DataServer::fetch(){
    buf = new char[chunkSize];
    std::string filePath = name_ + "/" + std::to_string(fid) + " " + std::to_string(offset);
    std::ifstream is(filePath);
    // file found not in this server.
    if(!is){
        delete []buf;
        bufSize = 0;
    }
    else{
        is.read(buf, std::min(chunkSize, bufSize - chunkSize * offset));
        bufSize = is.tellg();
    }
}


void DataServer::locate(){
    std::string filePath = name_ + "/" + std::to_string(fid) + " " + std::to_string(offset);
    std::ifstream is(filePath);
    if(is)
        bufSize = 1;
    else
        bufSize = 0;
}

std::string DataServer::get_name()const{
    return name_;
}
