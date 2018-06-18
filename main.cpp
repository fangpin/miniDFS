#include <iostream>
#include <thread>
#include <vector>
#include "nameserver.h"
#include "dataserver.h"


const int numReplicate = 3;

int main(){
    NameServer ns(numReplicate);
    DataServer ds1("node1");
    DataServer ds2("node2");
    DataServer ds3("node3");
    DataServer ds4("node4");
    ns.add(&ds1);
    ns.add(&ds2);
    ns.add(&ds3);
    ns.add(&ds4);
    std::thread th1(std::ref(ds1));
    std::thread th2(std::ref(ds2));
    std::thread th3(std::ref(ds3));
    std::thread th4(std::ref(ds4));
    th1.detach();
    th2.detach();
    th3.detach();
    th4.detach();
    ns();
    return 0;
}
