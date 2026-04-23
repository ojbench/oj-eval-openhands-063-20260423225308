#include <iostream>
#include <string>
#include <cstring>
#include "MemoryRiver.hpp"

using namespace std;

struct Data {
    int key;
    char value[256];
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int n;
    cin >> n;
    
    MemoryRiver<Data, 10> river("data.dat");
    
    string cmd;
    for (int i = 0; i < n; i++) {
        cin >> cmd;
        
        if (cmd == "initialise") {
            river.initialise();
        } else if (cmd == "write_info") {
            int val, pos;
            cin >> val >> pos;
            river.write_info(val, pos);
        } else if (cmd == "get_info") {
            int pos;
            cin >> pos;
            int val;
            river.get_info(val, pos);
            cout << val << endl;
        } else if (cmd == "write") {
            Data d;
            cin >> d.key;
            cin.ignore();
            cin.getline(d.value, 256);
            int idx = river.write(d);
            cout << idx << endl;
        } else if (cmd == "read") {
            int idx;
            cin >> idx;
            Data d;
            river.read(d, idx);
            cout << d.key << " " << d.value << endl;
        } else if (cmd == "update") {
            int idx;
            cin >> idx;
            Data d;
            cin >> d.key;
            cin.ignore();
            cin.getline(d.value, 256);
            river.update(d, idx);
        } else if (cmd == "delete") {
            int idx;
            cin >> idx;
            river.Delete(idx);
        }
    }
    
    return 0;
}
