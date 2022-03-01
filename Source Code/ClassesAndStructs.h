#ifndef ClassesAndStructs
#define ClassesAndStructs

#include <string>
#include <vector>
#include <iostream>
using namespace std;
#pragma once


struct Instruction {
	string type;
	int rd;
	int rs1;
	int rs2;
	int imm;

	void set_instruction(string ttype, int trd, int trs1, int trs2, int timm) {
		type = ttype;
		rd = trd;
		rs1 = trs1;
		rs2 = trs2;
		imm = timm;
		return;
	}

	void print_instruction() {
		cout << setw(4) << left << type << setw(4) << right << rd << setw(5) << rs1 << setw(5) << rs2 << setw(5) << imm << endl;
		return;
	}
};

struct MemoryEntry {
	int address;
	int data;
};

struct InstructionDetails {
	int issued;
	int startedExecution;
	int finishedExecution;
	int written;
};

struct ReservationStation {
	string Name;
	bool Busy;
	string Op;
	short int Vj;
	short int Vk;
	string Qj;
	string Qk;
	int A;
	int ExecuteTotalCycles;
	int ExecuteFirstStageCycles;
	int ExecuteCurrentCycle;
	int order;
	int result;
	int inst_index;

	void set_RS(string tName, bool tBusy, string tOp, short int tVj, short int tVk, string tQj, string tQk, int tA,
		int tTC, int tFSC, int tCC, int torder, int tresult, int tinst_index) {
		Name = tName;
		Busy = tBusy;
		Op = tOp;
		Vj = tVj;
		Vk = tVk;
		Qj = tQj;
		Qk = tQk;
		A = tA;
		ExecuteTotalCycles = tTC;
		ExecuteFirstStageCycles = tFSC;
		ExecuteCurrentCycle = tCC;
		order = torder;
		result = tresult;
		inst_index = tinst_index;
		return;
	}

	void free_RS() {
		Busy = false;
		Vj = 0;
		Vk = 0;
		Qj = "empty";
		Qk = "empty";
		A = 0;
		ExecuteCurrentCycle = -1;
		order = INT_MAX;
		result = 0;
		inst_index = -1;
	}

	void print_RS() {
		cout << setw(7) << left << Name
			 << setw(3) << right << Busy
			 << setw(8) << Op << setw(4) << Vj << setw(5) << Vk
			 << setw(8) << Qj << setw(8) << Qk << setw(4) << A
			 << setw(13) << ExecuteTotalCycles << setw(4) << ExecuteFirstStageCycles << setw(5) << ExecuteCurrentCycle
			 << setw(16) << order
			 << setw(8) << result
			 << setw(8) << inst_index << endl;

		return;
	}
};

struct CommonDataBus {
	bool active;
	int value;
};

struct FunctionalUnitSpecifications {
	int numberOfRS;
	int totalCycles;
	int FirstStageCycles;
	string unitname;
	int startindex;

	void print_specifications() {
		cout << setw(7) << left << unitname
			<< setw(6) << right << numberOfRS
			<< setw(6) << totalCycles
			<< setw(6) << FirstStageCycles
			<< setw(8) << startindex << endl;
	}
};

#endif ClassesAndStructs