#include <iostream>
#include "../include/SystemResource.h"

using namespace br::ufscar::lince::ginga::recommender;

int
main() {
    SystemResource S;

    cout << "   Ginga Recommender System Resource Test" << endl << endl;
    cout << "Number of CPUs: " << S.getCPUCount() << endl;
    cout << "CPU Clock: " << S.getCPUClock() << "Hz" << endl;
    cout << "Physical memory size: " << S.getMemorySize() << "B" << endl;
    cout << "Free physical memory: " << S.getMemoryFree() << "B" << endl;
    cout << "Disk Size: " << (float) 1024*1024* S.getDiskSize() << "GB" << endl;
    cout << "Free Disk Space: " << S.getDiskFree() << "B" << endl;

    return 0;
}

