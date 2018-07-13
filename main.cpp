#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "tokenscanner.hpp"
#include "command.hpp"
using namespace std;

int main(int argc, char *argv[])
{
	init();
	ifstream fin;
	fin.open(argv[1]);
    string s;
	tokenscanner ts;
	while (getline(fin, s)) code.push_back(s);
	fin.close();
    reg[sp] = SIZE;
	int cnt = 0, Size = code.size();
    while(cnt < Size){
        while(cnt < Size){
            ts.set(code[cnt++]);
            s = ts.nextToken();
            if(s == ".text") break;
			if (s == "") continue;
            int len = s.size();
            if(s[len - 1] == ':'){
                s.erase(len - 1, 1);
                lable[s] = heapLen;
				if (s == "main") break;
            }
            else{
                command CMD = parse(code[cnt - 1]);
                dataExcute(CMD);
            }
        }
        while(cnt < Size){
            ts.set(code[cnt++]);
            s = ts.nextToken();
            if(s == ".data") break;
			if (s == "") continue;
            int len = s.size();
            if(s[len - 1] == ':'){
                s.erase(len - 1, 1);
                lable[s] = cnt;
            }
        }
    }
	for (int i = 0; i < Size; ++i) {
		ts.set(code[i]);
		COMMAND.regiSize = 0;
		COMMAND.flag = false;
		COMMAND.type = insType[ts.nextToken()];
		while (ts.hasMoreTokens()) {
			s = ts.nextToken();
			if (s[0] == '$') COMMAND.regi[COMMAND.regiSize++] = regType[s];
			else if (s[0] == '_') COMMAND.Lable = lable[s];
			else if (s[0] == '-' || (s[0] <= '9' && s[0] >= '0')) {
				COMMAND.num = to_int(s);
				COMMAND.flag = true;
			}
		}
		CMD.push_back(COMMAND);
	}
	reg[pc] = lable["main"];
    while(true){
		WB();
		MA();
		E();
		IDDP();
		IF();
        if(shutdown) break;
    }
    return 0;
}
