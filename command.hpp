#ifndef COMMAND_HPP
#define COMMAND_HPP
#include "enumType.hpp"
#include "tokenscanner.hpp"
#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <queue>
#include <cmath>
using namespace std;

const int SIZE = 4 * 1024 * 1024;
static int reg[33], heapLen = 0, conti = 0;
static unsigned char Memory[SIZE];
static bool shutdown = false;
struct pre
{
	REGISTER reg = none;
	bool hReg = false;
};
static queue<pre> preReg;
static pre p;
struct cmd
{
	INSRTUCT type;
	REGISTER regi[3];
	int Lable, num, regiSize, regiValue[2];
	bool flag;
};
struct command
{
	INSRTUCT type;
	int Size;
	string v[4];
};
struct REG
{
	bool MUL;
	REGISTER Rtype;
	INSRTUCT Itype;
	int result, low, high;
};
struct MEM
{
	int pos;
	REGISTER Rtype;
	INSRTUCT Itype;
	int result, low, high;
	bool MUL;
};
static vector<string> code;
static vector<cmd> CMD;
static map<string, int> lable;
static map<string, REGISTER> regType;
static map<string, INSRTUCT> insType;
static cmd COMMAND;
static string instruct;
static REG Reg;
static MEM Mem;
static queue<REG> regi;
static queue<MEM> memo;
static queue<cmd> comm;
static queue<int> line;

void init()
{
	regType["$zero"] = regType["$0"] = zero;
	regType["$at"] = regType["$1"] = at;
	regType["$v0"] = regType["$2"] = v0;
	regType["$v1"] = regType["$3"] = v1;
	regType["$a0"] = regType["$4"] = a0;
	regType["$a1"] = regType["$5"] = a1;
	regType["$a2"] = regType["$6"] = a2;
	regType["$a3"] = regType["$7"] = a3;
	regType["$t0"] = regType["$8"] = t0;
	regType["$t1"] = regType["$9"] = t1;
	regType["$t2"] = regType["$10"] = t2;
	regType["$t3"] = regType["$11"] = t3;
	regType["$t4"] = regType["$12"] = t4;
	regType["$t5"] = regType["$13"] = t5;
	regType["$t6"] = regType["$14"] = t6;
	regType["$t7"] = regType["$15"] = t7;
	regType["$s0"] = regType["$16"] = s0;
	regType["$s1"] = regType["$17"] = s1;
	regType["$s2"] = regType["$18"] = s2;
	regType["$s3"] = regType["$19"] = s3;
	regType["$s4"] = regType["$20"] = s4;
	regType["$s5"] = regType["$21"] = s5;
	regType["$s6"] = regType["$22"] = s6;
	regType["$s7"] = regType["$23"] = s7;
	regType["$t8"] = regType["$24"] = t8;
	regType["$t9"] = regType["$25"] = t9;
	regType["$gp"] = regType["$28"] = gp;
	regType["$sp"] = regType["$29"] = sp;
	regType["$fp"] = regType["$30"] = fp;
	regType["$ra"] = regType["$31"] = ra;
	regType["$hi"] = hi;
	regType["$lo"] = lo;
	regType["$pc"] = pc;

	insType[".align"] = Align;
	insType[".ascii"] = ascii;
	insType[".asciiz"] = asciiz;
	insType[".byte"] = byte;
	insType[".half"] = half;
	insType[".word"] = word;
	insType[".space"] = space;
	insType[".data"] = Data;
	insType[".text"] = text;
	insType["add"] = add;
	insType["addu"] = addu;
	insType["addiu"] = addiu;
	insType["sub"] = sub;
	insType["subu"] = subu;
	insType["mul"] = mul;
	insType["mulu"] = mulu;
	insType["div"] = Div;
	insType["divu"] = divu;
	insType["xor"] = Xor;
	insType["xoru"] = xoru;
	insType["neg"] = neg;
	insType["negu"] = negu;
	insType["rem"] = rem;
	insType["remu"] = remu;
	insType["li"] = li;
	insType["seq"] = seq;
	insType["sge"] = sge;
	insType["sgt"] = sgt;
	insType["sle"] = sle;
	insType["slt"] = slt;
	insType["sne"] = sne;
	insType["b"] = b;
	insType["beq"] = beq;
	insType["bne"] = bne;
	insType["bge"] = bge;
	insType["ble"] = ble;
	insType["bgt"] = bgt;
	insType["blt"] = blt;
	insType["beqz"] = beqz;
	insType["bnez"] = bnez;
	insType["blez"] = blez;
	insType["bgez"] = bgez;
	insType["bgtz"] = bgtz;
	insType["bltz"] = bltz;
	insType["j"] = j;
	insType["jr"] = jr;
	insType["jal"] = jal;
	insType["jalr"] = jalr;
	insType["la"] = la;
	insType["lb"] = lb;
	insType["lh"] = lh;
	insType["lw"] = lw;
	insType["sb"] = sb;
	insType["sh"] = sh;
	insType["sw"] = sw;
	insType["move"] = Move;
	insType["mfhi"] = mfhi;
	insType["mflo"] = mflo;
	insType["nop"] = nop;
	insType["syscall"] = syscall;
}

inline command parse(string &s)
{
	tokenscanner ts;
	ts.set(s);
	command ret;
	ret.type = insType[ts.nextToken()];
	ret.Size = 0;
	while (ts.hasMoreTokens()) ret.v[ret.Size++] = ts.nextToken();
	return ret;
}

inline void dataExcute(command &CMD)
{
	switch (CMD.type) {
	case Align: {
		int n = to_int(CMD.v[0]);
		n = pow(2, n);
		heapLen = ((heapLen - 1) / n + 1) * n;
		break;
	}
	case ascii: {
		string str = CMD.v[0];
		int len = str.size();
		for (int i = 0; i < len; ++i) Memory[heapLen++] = str[i];
		break;
	}
	case asciiz: {
		string str = CMD.v[0];
		int len = str.size();
		for (int i = 0; i < len; ++i) Memory[heapLen++] = str[i];
		Memory[heapLen++] = '\0';
		break;
	}
	case byte: {
		int len = CMD.Size;
		for (int i = 0; i < len; ++i) {
			int num = to_int(CMD.v[i]);
			for (int j = 0; j < 1; ++j) {
				Memory[heapLen++] = num & 255;
				num >>= 8;
			}
		}
		break;
	}
	case half: {
		int len = CMD.Size;
		for (int i = 0; i < len; ++i) {
			int num = to_int(CMD.v[i]);
			for (int j = 0; j < 2; ++j) {
				Memory[heapLen++] = num & 255;
				num >>= 8;
			}
		}
		break;
	}
	case word: {
		int len = CMD.Size;
		for (int i = 0; i < len; ++i) {
			int num = to_int(CMD.v[i]);
			for (int j = 0; j < 4; ++j) {
				Memory[heapLen++] = num & 255;
				num >>= 8;
			}
		}
		break;
	}
	case space: {
		heapLen += to_int(CMD.v[0]);
		break;
	}
	}
}

inline void IF()
{
	++conti;
	if (line.empty() && conti >= 0) {
		line.push(reg[pc]);
		if (CMD[reg[pc]].type >= 30 && CMD[reg[pc]].type <= 46) conti = -5;
		++reg[pc];
	}
}

inline void WB();

inline void MA();

inline void IDDP()
{
	if (!line.empty()) {
		COMMAND = CMD[line.front()];
		switch (COMMAND.type) {
		case add:
		case addu:
		case sub:
		case subu: {
			if (COMMAND.regi[1] == preReg.back().reg) {
				WB();
				MA();
				WB();
			}
			else if (COMMAND.regi[1] == preReg.front().reg) WB();
			COMMAND.regiValue[0] = reg[COMMAND.regi[1]];
			if (COMMAND.regiSize == 3) {
				if (COMMAND.regi[2] == preReg.back().reg) {
					WB();
					MA();
					WB();
				}
				else if (COMMAND.regi[2] == preReg.front().reg) WB();
				COMMAND.regiValue[1] = reg[COMMAND.regi[2]];
			}
			preReg.pop();
			break;
		}
		case addiu: {
			if (COMMAND.regi[1] == preReg.back().reg) {
				WB();
				MA();
				WB();
			}
			else if (COMMAND.regi[1] == preReg.front().reg) WB();
			COMMAND.regiValue[0] = reg[COMMAND.regi[1]];
			preReg.pop();
			break;
		}
		case mul:
		case mulu:
		case Div:
		case divu: {
			if (COMMAND.regiSize + COMMAND.flag == 3) {
				if (COMMAND.regi[1] == preReg.back().reg) {
					WB();
					MA();
					WB();
				}
				else if (COMMAND.regi[1] == preReg.front().reg) WB();
				COMMAND.regiValue[0] = reg[COMMAND.regi[1]];
				if (!COMMAND.flag) {
					if (COMMAND.regi[2] == preReg.back().reg) {
						WB();
						MA();
						WB();
					}
					else if (COMMAND.regi[2] == preReg.front().reg) WB();
					COMMAND.regiValue[1] = reg[COMMAND.regi[2]];
				}
			}
			else {
				if (!COMMAND.flag) {
					if (COMMAND.regi[1] == preReg.back().reg || COMMAND.regi[0] == preReg.front().reg) {
						WB();
						MA();
						WB();
					}
					else if (COMMAND.regi[1] == preReg.front().reg || COMMAND.regi[0] == preReg.front().reg) WB();
					COMMAND.regiValue[0] = reg[COMMAND.regi[0]];
					COMMAND.regiValue[1] = reg[COMMAND.regi[1]];
				}
				else {
					if (COMMAND.regi[0] == preReg.back().reg) {
						WB();
						MA();
						WB();
					}
					else if (COMMAND.regi[0] == preReg.front().reg) WB();
					COMMAND.regiValue[0] = reg[COMMAND.regi[0]];
				}
			}
			preReg.pop();
			break;
		}
		case Xor:
		case xoru:
		case rem:
		case remu: {
			if (COMMAND.regi[1] == preReg.back().reg) {
				WB();
				MA();
				WB();
			}
			else if (COMMAND.regi[1] == preReg.front().reg) WB();
			COMMAND.regiValue[0] = reg[COMMAND.regi[1]];
			if (!COMMAND.flag) {
				if (COMMAND.regi[2] == preReg.back().reg) {
					WB();
					MA();
					WB();
				}
				else if (COMMAND.regi[2] == preReg.front().reg) WB();
				COMMAND.regiValue[1] = reg[COMMAND.regi[2]];
			}
			preReg.pop();
			break;
		}
		case neg:
		case negu: {
			if (!COMMAND.flag) {
				if (COMMAND.regi[1] == preReg.back().reg) {
					WB();
					MA();
					WB();
				}
				else if (COMMAND.regi[1] == preReg.front().reg) WB();
				COMMAND.regiValue[0] = reg[COMMAND.regi[1]];
			}
			preReg.pop();
			break;
		}
		case seq:
		case sge:
		case sgt:
		case sle:
		case slt:
		case sne: {
			if (COMMAND.regi[1] == preReg.back().reg) {
				WB();
				MA();
				WB();
			}
			else if (COMMAND.regi[1] == preReg.front().reg) WB();
			COMMAND.regiValue[0] = reg[COMMAND.regi[1]];
			if (!COMMAND.flag) {
				if (COMMAND.regi[2] == preReg.back().reg) {
					WB();
					MA();
					WB();
				}
				else if (COMMAND.regi[2] == preReg.front().reg) WB();
				COMMAND.regiValue[1] = reg[COMMAND.regi[2]];
			}
			preReg.pop();
			break;
		}
		case beq:
		case bne:
		case bge:
		case ble:
		case bgt:
		case blt: {
			if (COMMAND.regi[0] == preReg.back().reg) {
				WB();
				MA();
				WB();
			}
			else if (COMMAND.regi[0] == preReg.front().reg) WB();
			COMMAND.regiValue[0] = reg[COMMAND.regi[0]];
			if (!COMMAND.flag) {
				if (COMMAND.regi[1] == preReg.back().reg) {
					WB();
					MA();
					WB();
				}
				else if (COMMAND.regi[1] == preReg.front().reg) WB();
				COMMAND.regiValue[1] = reg[COMMAND.regi[1]];
			}
			preReg.pop();
			break;
		}
		case beqz:
		case bnez:
		case blez:
		case bgez:
		case bgtz:
		case bltz:
		case jr:
		case jalr: {
			if (COMMAND.regi[0] == preReg.back().reg) {
				WB();
				MA();
				WB();
			}
			else if (COMMAND.regi[0] == preReg.front().reg) WB();
			COMMAND.regiValue[0] = reg[COMMAND.regi[0]];
			preReg.pop();
			break;
		}
		case lb:
		case lh:
		case lw:
		case sb:
		case sh:
		case sw: {
			if (COMMAND.regiSize != 1) {
				if (COMMAND.regi[1] == preReg.back().reg) {
					WB();
					MA();
					WB();
				}
				else if (COMMAND.regi[1] == preReg.front().reg) WB();
				COMMAND.regiValue[0] = reg[COMMAND.regi[1]];
			}
			preReg.pop();
			break;
		}
		case Move: {
			if (COMMAND.regi[1] == preReg.back().reg) {
				WB();
				MA();
				WB();
			}
			else if (COMMAND.regi[1] == preReg.front().reg) WB();
			COMMAND.regiValue[0] = reg[COMMAND.regi[1]];
			preReg.pop();
			break;
		}
		case mfhi: {
			
			if (preReg.back().hReg) {
				WB();
				MA();
				WB();
			}
			else if (preReg.front().hReg) WB();
			COMMAND.regiValue[0] = reg[hi];
			preReg.pop();
			break;
		}
		case mflo: {
			if (lo == preReg.back().reg) {
				WB();
				MA();
				WB();
			}
			else if (lo == preReg.front().reg) WB();
			COMMAND.regiValue[0] = reg[lo];
			preReg.pop();
			break;
		}
		case syscall: {
			if (v0 == preReg.back().reg) {
				WB();
				MA();
				WB();
			}
			else if (v0 == preReg.front().reg) WB();
			switch (reg[v0]) {
			case 4: {
				if (a0 == preReg.back().reg) {
					WB();
					MA();
					WB();
				}
				else if (a0 == preReg.front().reg) WB();
				COMMAND.regiValue[0] = reg[a0];
				break;
			}
			case 8: {
				if (a0 == preReg.back().reg) {
					WB();
					MA();
					WB();
				}
				else if (a0 == preReg.front().reg) WB();
				COMMAND.regiValue[0] = reg[a0];
				break;
			}
			case 9: {
				if (a0 == preReg.back().reg) {
					WB();
					MA();
					WB();
				}
				else if (a0 == preReg.front().reg) WB();
				COMMAND.regiValue[0] = reg[a0];
				break;
			}
			}
			preReg.pop();
			break;
		}
		default: {
			if (preReg.size() >= 2) preReg.pop();
			break;
		}
		}
		comm.push(COMMAND);
		line.pop();
	}
}

inline void E()
{
	if (!comm.empty()) {
		COMMAND = comm.front();
		switch (COMMAND.type) {
		case add:
		case addu: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			int tmp = COMMAND.regiValue[0];
			if (COMMAND.regiSize == 3)
				Mem.result = tmp + COMMAND.regiValue[1];
			else Mem.result = tmp + COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case addiu: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			int tmp = COMMAND.regiValue[0];
			Mem.result = tmp + COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case sub:
		case subu: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			int tmp = COMMAND.regiValue[0];
			if (COMMAND.regiSize == 3) Mem.result = tmp - COMMAND.regiValue[1];
			else Mem.result = tmp - COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case mul:
		case mulu: {
			Mem.Itype = COMMAND.type;
			if (COMMAND.regiSize + COMMAND.flag == 3) {
				Mem.MUL = false;
				Mem.Rtype = COMMAND.regi[0];
				int tmp = COMMAND.regiValue[0];
				if (!COMMAND.flag) Mem.result = tmp * COMMAND.regiValue[1];
				else Mem.result = tmp * COMMAND.num;
				p.reg = Mem.Rtype;
				p.hReg = false;
				preReg.push(p);
			}
			else {
				Mem.MUL = true;
				if (!COMMAND.flag) {
					long long tmp = COMMAND.regiValue[0] * COMMAND.regiValue[1];
					Mem.low = tmp;
					Mem.high = tmp >> 32;
					p.reg = lo;
					p.hReg = true;
					preReg.push(p);
				}
				else {
					long long tmp = COMMAND.regiValue[0] * COMMAND.num;
					Mem.low = tmp;
					Mem.high = tmp >> 32;
					p.reg = lo;
					p.hReg = true;
					preReg.push(p);
				}
			}
			break;
		}
		case Div:
		case divu: {
			Mem.Itype = COMMAND.type;
			if (COMMAND.regiSize + COMMAND.flag == 3) {
				Mem.MUL = false;
				Mem.Rtype = COMMAND.regi[0];
				int tmp = COMMAND.regiValue[0];
				if (!COMMAND.flag) Mem.result = tmp / COMMAND.regiValue[1];
				else Mem.result = tmp / COMMAND.num;
				p.reg = Mem.Rtype;
				p.hReg = false;
				preReg.push(p);
			}
			else {
				Mem.MUL = true;
				if (!COMMAND.flag) {
					int tmp1 = COMMAND.regiValue[0], tmp2 = COMMAND.regiValue[1];
					Mem.low = tmp1 / tmp2;
					Mem.high = tmp1 % tmp2;
					p.reg = lo;
					p.hReg = true;
					preReg.push(p);
				}
				else {
					int tmp1 = COMMAND.regiValue[0], tmp2 = COMMAND.num;
					Mem.low = tmp1 / tmp2;
					Mem.high = tmp1 % tmp2;
					p.reg = lo;
					p.hReg = true;
					preReg.push(p);
				}
			}
			break;
		}
		case Xor:
		case xoru: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			int tmp = COMMAND.regiValue[0];
			if (!COMMAND.flag) Mem.result = tmp ^ COMMAND.regiValue[1];
			else Mem.result = tmp ^ COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case neg: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (!COMMAND.flag) Mem.result = -COMMAND.regiValue[0];
			else Mem.result = -COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case negu: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (!COMMAND.flag) Mem.result = ~COMMAND.regiValue[0];
			else Mem.result = ~COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case rem:
		case remu: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			int tmp = COMMAND.regiValue[0];
			if (!COMMAND.flag) Mem.result = tmp % COMMAND.regiValue[1];
			else Mem.result = tmp % COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case li: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			Mem.result = COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case la: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (!COMMAND.flag) Mem.result = COMMAND.Lable;
			else Mem.result = COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case seq: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			int tmp = COMMAND.regiValue[0];
			if (!COMMAND.flag) Mem.result = tmp == COMMAND.regiValue[1];
			else Mem.result = tmp == COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case sge: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			int tmp = COMMAND.regiValue[0];
			if (!COMMAND.flag) Mem.result = tmp >= COMMAND.regiValue[1];
			else Mem.result = tmp >= COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case sgt: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			int tmp = COMMAND.regiValue[0];
			if (!COMMAND.flag) Mem.result = tmp > COMMAND.regiValue[1];
			else Mem.result = tmp > COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case sle: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			int tmp = COMMAND.regiValue[0];
			if (!COMMAND.flag) Mem.result = tmp <= COMMAND.regiValue[1];
			else Mem.result = tmp <= COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case slt: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			int tmp = COMMAND.regiValue[0];
			if (!COMMAND.flag) Mem.result = tmp < COMMAND.regiValue[1];
			else Mem.result = tmp < COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case sne: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			int tmp = reg[COMMAND.regi[1]];
			if (!COMMAND.flag) Mem.result = tmp != COMMAND.regiValue[1];
			else Mem.result = tmp != COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case b:
		case j: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case beq: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			int tmp = COMMAND.regiValue[0];
			if (!COMMAND.flag) Mem.MUL = tmp == COMMAND.regiValue[1];
			else Mem.MUL = tmp == COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case bne: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			int tmp = reg[COMMAND.regi[0]];
			if (!COMMAND.flag) Mem.MUL = tmp != reg[COMMAND.regi[1]];
			else Mem.MUL = tmp != COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case bge: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			int tmp = COMMAND.regiValue[0];
			if (!COMMAND.flag) Mem.MUL = tmp >= COMMAND.regiValue[1];
			else Mem.MUL = tmp >= COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case ble: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			int tmp = COMMAND.regiValue[0];
			if (!COMMAND.flag) Mem.MUL = tmp <= COMMAND.regiValue[1];
			else Mem.MUL = tmp <= COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case bgt: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			int tmp = COMMAND.regiValue[0];
			if (!COMMAND.flag) Mem.MUL = tmp > COMMAND.regiValue[1];
			else Mem.MUL = tmp > COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case blt: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			int tmp = reg[COMMAND.regi[0]];
			if (!COMMAND.flag) Mem.MUL = tmp < COMMAND.regiValue[1];
			else Mem.MUL = tmp < COMMAND.num;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case beqz: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			Mem.MUL = COMMAND.regiValue[0] == 0;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case bnez: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			Mem.MUL = COMMAND.regiValue[0] != 0;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case blez: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			Mem.MUL = COMMAND.regiValue[0] <= 0;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case bgez: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			Mem.MUL = COMMAND.regiValue[0] >= 0;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case bgtz: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			Mem.MUL = COMMAND.regiValue[0] > 0;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case bltz: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			Mem.MUL = COMMAND.regiValue[0] < 0;
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case jr: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.regiValue[0];
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case jal: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			Mem.low = reg[pc];
			p.reg = ra;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case jalr: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.regiValue[0];
			Mem.low = reg[pc];
			p.reg = ra;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case lb:
		case lh:
		case lw:
		case sb:
		case sh:
		case sw: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (COMMAND.regiSize == 1) Mem.pos = COMMAND.Lable;
			else Mem.pos = COMMAND.num + COMMAND.regiValue[0];
			if (Mem.Itype == lb || Mem.Itype == lh || Mem.Itype == lw) p.reg = Mem.Rtype;
			else p.reg = none;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case Move: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			Mem.result = COMMAND.regiValue[0];
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case mfhi: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			Mem.result = COMMAND.regiValue[0];
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case mflo: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			Mem.result = COMMAND.regiValue[0];
			p.reg = Mem.Rtype;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case nop: {
			Mem.Itype = COMMAND.type;
			p.reg = none;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		case syscall: {
			Mem.Itype = COMMAND.type;
			Mem.MUL = false;
			p.hReg = false;
			switch (reg[v0]) {
			case 1: {
				printf("%d", reg[a0]);
				Mem.low = 1;
				p.reg = none;
				break;
			}
			case 4: {
				for (int i = reg[a0]; Memory[i] != '\0'; ++i) printf("%c", Memory[i]);
				Mem.low = 4;
				p.reg = none;
				break;
			}
			case 5: {
				int tmp;
				scanf("%d", &tmp);
				Mem.Rtype = v0;
				Mem.result = tmp;
				Mem.low = 5;
				p.reg = v0;
				break;
			}
			case 8: {
				char c = getchar();
				if (c == '\n') c = getchar();
				int i = reg[a0];
				while (c != '\0' && c != '\n' && c != -1) {
					Memory[i++] = c;
					c = getchar();
				}
				Memory[i++] = '\0';
				Mem.Rtype = a1;
				Mem.result = i - reg[a0];
				Mem.low = 8;
				p.reg = a1;
				break;
			}
			case 9: {
				Mem.Rtype = v0;
				Mem.result = heapLen;
				Mem.low = 9;
				heapLen += reg[a0];
				p.reg = v0;
				break;
			}
			case 10:
			case 17: {
				Mem.MUL = true;
				Mem.low = 17;
				p.reg = none;
				break;
			}
			}
			preReg.push(p);
			break;
		}
		default: {
			Mem.Itype = nop;
			p.reg = none;
			p.hReg = false;
			preReg.push(p);
			break;
		}
		}
		memo.push(Mem);
		comm.pop();
	}
}

inline void MA()
{
	if (!memo.empty()) {
		Mem = memo.front();
		switch (Mem.Itype) {
		case lb: {
			Reg.Itype = Mem.Itype;
			Reg.Rtype = Mem.Rtype;
			Reg.result = Memory[Mem.pos];
			break;
		}
		case lh: {
			Reg.Itype = Mem.Itype;
			Reg.Rtype = Mem.Rtype;
			Reg.result = Memory[Mem.pos] | Memory[Mem.pos + 1] << 8;
			break;
		}
		case lw: {
			Reg.Itype = Mem.Itype;
			Reg.Rtype = Mem.Rtype;
			Reg.result = Memory[Mem.pos] | Memory[Mem.pos + 1] << 8 | Memory[Mem.pos + 2] << 16 | Memory[Mem.pos + 3] << 24;
			break;
		}
		case sb: {
			int num = reg[Mem.Rtype];
			for (int i = 0; i < 1; ++i) {
				if (Mem.MUL) Memory[Mem.pos + i] = num & 255;
				else Memory[Mem.pos + i] = num & 255;
				num >>= 8;
			}
			Reg.Itype = Mem.Itype;
			break;
		}
		case sh: {
			int num = reg[Mem.Rtype];
			for (int i = 0; i < 2; ++i) {
				if (Mem.MUL) Memory[Mem.pos + i] = num & 255;
				else Memory[Mem.pos + i] = num & 255;
				num >>= 8;
			}
			Reg.Itype = Mem.Itype;
			break;
		}
		case sw: {
			int num = reg[Mem.Rtype];
			for (int i = 0; i < 4; ++i) {
				if (Mem.MUL) Memory[Mem.pos + i] = num & 255;
				else Memory[Mem.pos + i] = num & 255;
				num >>= 8;
			}
			Reg.Itype = Mem.Itype;
			break;
		}
		case nop: {
			Reg.Itype = Mem.Itype;
			break;
		}
		default: {
			Reg.Itype = Mem.Itype;
			Reg.Rtype = Mem.Rtype;
			Reg.MUL = Mem.MUL;
			Reg.result = Mem.result;
			Reg.low = Mem.low;
			Reg.high = Mem.high;
			break;
		}
		}
		regi.push(Reg);
		memo.pop();
	}
}

inline void WB()
{
	if (!regi.empty()) {
		Reg = regi.front();
		switch (Reg.Itype) {
		case add:
		case addu:
		case addiu:
		case sub:
		case subu: {
			reg[Reg.Rtype] = Reg.result;;
			break;
		}
		case mul:
		case mulu:
		case Div:
		case divu: {
			if (Reg.MUL) {
				reg[lo] = Reg.low;
				reg[hi] = Reg.high;
			}
			else reg[Reg.Rtype] = Reg.result;
			break;
		}
		case Xor:
		case xoru:
		case neg:
		case negu:
		case rem:
		case remu:
		case li:
		case seq:
		case sge:
		case sgt:
		case sle:
		case slt:
		case sne:
		case la:
		case lb:
		case lh:
		case lw:
		case Move:
		case mfhi:
		case mflo: {
			reg[Reg.Rtype] = Reg.result;
			break;
		}
		case b:
		case j:
		case jr: {
			reg[pc] = Reg.result;
			break;
		}
		case beq:
		case bne:
		case bge:
		case ble:
		case bgt:
		case blt:
		case beqz:
		case bnez:
		case blez:
		case bgez:
		case bgtz:
		case bltz: {
			if (Reg.MUL) reg[pc] = Reg.result;
			break;
		}
		case jal:
		case jalr: {
			reg[pc] = Reg.result;
			reg[ra] = Reg.low;
			break;
		}
		case syscall: {
			switch (Reg.low) {
			case 5:
			case 8:
			case 9: {
				reg[Reg.Rtype] = Reg.result;
				break;
			}
			case 10:
			case 17: {
				if (Mem.MUL) shutdown = true;
				Reg.Itype = Mem.Itype;
				break;
			}
			}
			break;
		}
		default:
			break;
		}
		regi.pop();
	}
}

#endif