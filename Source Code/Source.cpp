#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <math.h>
#include "ClassesAndStructs.h"
using namespace std;

void initialize_registers(short int registers[8]) {
	for (int i = 0; i < 8; i++)
		registers[i] = i;
	return;
}

void print_registers(short int registers[8]) {
	cout << "Registers:" << endl;
	for (int i = 0; i < 8; i++)
		cout << "R" << i << ": " << registers[i] << endl;
	cout << endl;
	return;
}

void parseLW(Instruction& tempInst, string restOfLine) {
	string tempType = "LW";
	int tempRd;
	int tempRs1;
	int tempRs2 = -1;
	int tempImm;

	string placeholder;

	placeholder = restOfLine[0];
	tempRd = stoi(placeholder);
	restOfLine.erase(0, 3); //string now starts at imm

	stringstream reader (restOfLine);
	getline(reader, placeholder, '(');
	tempImm = stoi(placeholder);

	placeholder = restOfLine[restOfLine.length() - 2];
	tempRs1 = stoi(placeholder);

	tempInst.set_instruction(tempType, tempRd, tempRs1, tempRs2, tempImm);

	return;
}

void parseSW(Instruction& tempInst, string restOfLine) {
	string tempType = "SW";
	int tempRd = -1;
	int tempRs1;
	int tempRs2;
	int tempImm;

	string placeholder;

	placeholder = restOfLine[0];
	tempRs2 = stoi(placeholder);
	restOfLine.erase(0, 3); //string now starts at imm

	stringstream reader(restOfLine);
	getline(reader, placeholder, '(');
	tempImm = stoi(placeholder);

	placeholder = restOfLine[restOfLine.length() - 2];
	tempRs1 = stoi(placeholder);


	tempInst.set_instruction(tempType, tempRd, tempRs1, tempRs2, tempImm);

	return;
}

void parseBEQ(Instruction& tempInst, string restOfLine) {
	string tempType = "BEQ";
	int tempRd = -1;
	int tempRs1;
	int tempRs2;
	int tempImm;

	string placeholder;

	placeholder = restOfLine[0];
	tempRs1 = stoi(placeholder);
	restOfLine.erase(0, 4);

	placeholder = restOfLine[0];
	tempRs2 = stoi(placeholder);
	restOfLine.erase(0, 3);

	tempImm = stoi(restOfLine);

	tempInst.set_instruction(tempType, tempRd, tempRs1, tempRs2, tempImm);

	return;
}

void parseNEG(Instruction& tempInst, string restOfLine) {
	string tempType = "NEG";
	int tempRd;
	int tempRs1;
	int tempRs2 = -1;
	int tempImm = -1;

	string placeholder;

	placeholder = restOfLine[0];
	tempRd = stoi(placeholder);

	placeholder = restOfLine[restOfLine.length() - 1];
	tempRs1 = stoi(placeholder);

	tempInst.set_instruction(tempType, tempRd, tempRs1, tempRs2, tempImm);

	return;
}

void parseADDI(Instruction& tempInst, string restOfLine) {
	string tempType = "ADDI";
	int tempRd;
	int tempRs1;
	int tempRs2 = -1;
	int tempImm;

	string placeholder;

	placeholder = restOfLine[0];
	tempRd = stoi(placeholder);
	restOfLine.erase(0, 4);

	placeholder = restOfLine[0];
	tempRs1 = stoi(placeholder);
	restOfLine.erase(0, 3);

	tempImm = stoi(restOfLine);

	tempInst.set_instruction(tempType, tempRd, tempRs1, tempRs2, tempImm);

	return;
}

void parseAddDiv(Instruction& tempInst, string restOfLine, string type) {
	string tempType = type;
	int tempRd;
	int tempRs1;
	int tempRs2;
	int tempImm = -1;

	string placeholder;

	placeholder = restOfLine[0];
	tempRd = stoi(placeholder);
	restOfLine.erase(0, 4);

	placeholder = restOfLine[0];
	tempRs1 = stoi(placeholder);

	placeholder = restOfLine[restOfLine.length() - 1];
	tempRs2 = stoi(placeholder);

	tempInst.set_instruction(tempType, tempRd, tempRs1, tempRs2, tempImm);

	return;
}

bool readInstructionsFromFile(vector<Instruction>& instructions, int& startingAddress, vector<string>& string_instructions) {
	
	Instruction tempInst;

	string current_line = "";
	string current_word = "";

	fstream fileProgram("Program3.txt");
	if (fileProgram.is_open()) {

		//read starting address
		getline(fileProgram, current_word);
		startingAddress = stoi(current_word);

		//read instructions
		while (getline(fileProgram, current_line)) {
			string_instructions.push_back(current_line);
			stringstream line_reader(current_line);
			getline(line_reader, current_word, ' ');
			
			if (current_word == "LW") {
 				current_line.erase(0, 4); //string now starts with the number of the register rd (without the R)
				parseLW(tempInst, current_line);
			}
			else if (current_word == "SW") {
				current_line.erase(0, 4);
				parseSW(tempInst, current_line);
			}
			else if (current_word == "BEQ") {
				current_line.erase(0, 5);
				parseBEQ(tempInst, current_line);
			}
			else if (current_word == "JALR") {
				current_line.erase(0, 6); //string now only contain numbers of the register (without the R)
				tempInst.set_instruction("JALR", -1, stoi(current_line) , -1, -1);
			}
			else if (current_word == "RET") {
				tempInst.set_instruction("RET", -1, 1, -1, -1);
			}
			else if (current_word == "NEG") {
				current_line.erase(0, 5);
				parseNEG(tempInst, current_line);
			}
			else if (current_word == "ADDI") {
				current_line.erase(0, 6);
				parseADDI(tempInst, current_line);
			}
			else if (current_word == "ADD" || current_word == "DIV") {
				current_line.erase(0, 5); //string now starts with the number of the register rd (without the R)
				parseAddDiv(tempInst, current_line, current_word);
			}
			else {
				cout << "Invalid or Unsupported Instruction." << endl;
				return false;
			}
			instructions.push_back(tempInst);
		}

		fileProgram.close();
		return true;
	}
	else
		return false;
}

bool readMemoryFromFile(vector<MemoryEntry>& memory) {
	
	MemoryEntry tempMemoryEntry;

	string current_line = "";
	string current_word = "";

	fstream fileMemory("Memory.txt");
	if (fileMemory.is_open()) {
		while (getline(fileMemory, current_line)) {
			stringstream line_reader(current_line);
			getline(line_reader, current_word, ' ');
			tempMemoryEntry.address = stoi(current_word);
			getline(line_reader, current_word, ' ');
			tempMemoryEntry.data = stoi(current_word);
			memory.push_back(tempMemoryEntry);
		}
		return true;
	}
	else
		return false;
}

void printMemory(vector<MemoryEntry>& memory) {
	cout << "Address  Data" << endl;
	for (int i = 0; i < memory.size(); i++)
		cout << setw(4) << left << memory[i].address << setw(9) << right << memory[i].data << endl;
	cout << endl;
}

void print_instructions(vector<Instruction>& instructions) {
	cout << "Instructions:" << endl;
	cout << "Inst  Rd  Rs1  Rs2  Imm" << endl;
	for (int i = 0; i < instructions.size(); i++)
		instructions[i].print_instruction();
	cout << endl;
	return;
}

void variable_hardware_organization(FunctionalUnitSpecifications hardwareOrganization[6]) {
	
	char choice;
	
	cout << "Please enter 'V' or 'v' if you want to input your own hardware organization, "
		<< "or any other character if you want to use the default values." << endl;
	cin >> choice;
	cout << endl;

	if (choice == 'V' || choice == 'v') {

		int temp;
		int sumRS = 0;

		hardwareOrganization[0].unitname = "Load";
		hardwareOrganization[0].startindex = 0;
		cout << "Please input the number of Reservation Stations for the Load Functional Units." << endl;
		cin >> hardwareOrganization[0].numberOfRS;
		cout << "Please input the number of cycles needed to compute the address." << endl;
		cin >> hardwareOrganization[0].FirstStageCycles;
		cout << "Please input the number of cycles need to access the memory." << endl;
		cin >> temp;
		hardwareOrganization[0].totalCycles = temp + hardwareOrganization[0].FirstStageCycles;
		cout << endl;
		sumRS += hardwareOrganization[0].numberOfRS;

		hardwareOrganization[1].unitname = "Store";
		hardwareOrganization[1].startindex = sumRS;
		hardwareOrganization[1].FirstStageCycles = 0;
		cout << "Please input the number of Reservation Stations for the Store Functional Units." << endl;
		cin >> hardwareOrganization[1].numberOfRS;
		cout << "Please input the number of cycles needed to compute the address. (This is also the total execution time)." << endl;
		cin >> hardwareOrganization[1].totalCycles;
		cout << endl;
		sumRS += hardwareOrganization[1].numberOfRS;

		hardwareOrganization[2].unitname = "Branch";
		hardwareOrganization[2].startindex = sumRS;
		hardwareOrganization[2].FirstStageCycles = 0;
		cout << "Please input the number of Reservation Stations for the Branch Functional Units." << endl;
		cin >> hardwareOrganization[2].numberOfRS;
		cout << "Please input the TOTAL number of execution cycles." << endl;
		cin >> hardwareOrganization[2].totalCycles;
		cout << endl;
		sumRS += hardwareOrganization[2].numberOfRS;

		hardwareOrganization[3].unitname = "Jump";
		hardwareOrganization[3].startindex = sumRS;
		hardwareOrganization[3].FirstStageCycles = 0;
		cout << "Please input the number of Reservation Stations for the Jump (JALR & RET) Functional Units." << endl;
		cin >> hardwareOrganization[3].numberOfRS;
		cout << "Please input the TOTAL number of execution cycles." << endl;
		cin >> hardwareOrganization[3].totalCycles;
		cout << endl;
		sumRS += hardwareOrganization[3].numberOfRS;

		hardwareOrganization[4].unitname = "Arith";
		hardwareOrganization[4].startindex = sumRS;
		hardwareOrganization[4].FirstStageCycles = 0;
		cout << "Please input the number of Reservation Stations for the Simple Arithmetic (ADD/NEG/ADDI) Functional Units." << endl;
		cin >> hardwareOrganization[4].numberOfRS;
		cout << "Please input the TOTAL number of execution cycles." << endl;
		cin >> hardwareOrganization[4].totalCycles;
		cout << endl;
		sumRS += hardwareOrganization[4].numberOfRS;

		hardwareOrganization[5].unitname = "Div";
		hardwareOrganization[5].startindex = sumRS;
		hardwareOrganization[5].FirstStageCycles = 0;
		cout << "Please input the number of Reservation Stations for the Division Functional Units." << endl;
		cin >> hardwareOrganization[5].numberOfRS;
		cout << "Please input the TOTAL number of execution cycles." << endl;
		cin >> hardwareOrganization[5].totalCycles;
		cout << endl;
		sumRS += hardwareOrganization[5].numberOfRS;

	}

	else {
		hardwareOrganization[0].unitname = "Load";
		hardwareOrganization[0].numberOfRS = 2;
		hardwareOrganization[0].totalCycles = 2;
		hardwareOrganization[0].FirstStageCycles = 1;
		hardwareOrganization[0].startindex = 0;

		hardwareOrganization[1].unitname = "Store";
		hardwareOrganization[1].numberOfRS = 2;
		hardwareOrganization[1].totalCycles = 1;
		hardwareOrganization[1].FirstStageCycles = 0;
		hardwareOrganization[1].startindex = 2;

		hardwareOrganization[2].unitname = "Branch";
		hardwareOrganization[2].numberOfRS = 1;
		hardwareOrganization[2].totalCycles = 1;
		hardwareOrganization[2].FirstStageCycles = 0;
		hardwareOrganization[2].startindex = 4;

		hardwareOrganization[3].unitname = "Jump";
		hardwareOrganization[3].numberOfRS = 1;
		hardwareOrganization[3].totalCycles = 1;
		hardwareOrganization[3].FirstStageCycles = 0;
		hardwareOrganization[3].startindex = 5;

		hardwareOrganization[4].unitname = "Arith";
		hardwareOrganization[4].numberOfRS = 2;
		hardwareOrganization[4].totalCycles = 2;
		hardwareOrganization[4].FirstStageCycles = 0;
		hardwareOrganization[4].startindex = 6;

		hardwareOrganization[5].unitname = "Div";
		hardwareOrganization[5].numberOfRS = 1;
		hardwareOrganization[5].totalCycles = 8;
		hardwareOrganization[5].FirstStageCycles = 0;
		hardwareOrganization[5].startindex = 8;
	}
}

void print_hardwareOrganization(FunctionalUnitSpecifications hardwareOrganization[6]) {
	cout << "You have specified the following Hardware Organization:" << endl;
	cout << "UnitName   #RS   #TC   #FSC   Start" << endl;
	for (int i = 0; i < 6; i++)
		hardwareOrganization[i].print_specifications();
	cout << endl;
	return;
}

void initialize_reservationstations(vector<ReservationStation>& reservationstations, FunctionalUnitSpecifications hardwareOrganization[6]) {

	ReservationStation tempRS;

	for (int i = 0; i < hardwareOrganization[0].numberOfRS; i++) {
		tempRS.set_RS("Load" + to_string(i+1), 0, "load", 0, 0, "empty", "empty", 0,
			hardwareOrganization[0].totalCycles, hardwareOrganization[0].FirstStageCycles, -1, INT_MAX, 0, - 1);
		reservationstations.push_back(tempRS);
	}

	for (int i = 0; i < hardwareOrganization[1].numberOfRS; i++) {
		tempRS.set_RS("Store" + to_string(i+1), 0, "store", 0, 0, "empty", "empty", 0,
			hardwareOrganization[1].totalCycles, hardwareOrganization[1].FirstStageCycles, -1, INT_MAX, 0, - 1);
		reservationstations.push_back(tempRS);
	}

	for (int i = 0; i < hardwareOrganization[2].numberOfRS; i++) {
		tempRS.set_RS("Branch" + to_string(i+1), 0, "beq", 0, 0, "empty", "empty", 0,
			hardwareOrganization[2].totalCycles, hardwareOrganization[2].FirstStageCycles, -1, INT_MAX, 0, - 1);
		reservationstations.push_back(tempRS);
	}

	for (int i = 0; i < hardwareOrganization[3].numberOfRS; i++) {
		tempRS.set_RS("Jump" + to_string(i+1), 0, "jalr", 0, 0, "empty", "empty", 0,
			hardwareOrganization[3].totalCycles, hardwareOrganization[3].FirstStageCycles, -1, INT_MAX, 0, - 1); //Op can be jalr or ret
		reservationstations.push_back(tempRS);
	}

	for (int i = 0; i < hardwareOrganization[4].numberOfRS; i++) {
		tempRS.set_RS("Arith" + to_string(i+1), 0, "add", 0, 0, "empty", "empty", 0,
			hardwareOrganization[4].totalCycles, hardwareOrganization[4].FirstStageCycles, -1, INT_MAX, 0, - 1); //Op can be add or neg or addi
		reservationstations.push_back(tempRS);
	}

	for (int i = 0; i < hardwareOrganization[5].numberOfRS; i++) {
		tempRS.set_RS("Div" + to_string(i+1), 0, "div", 0, 0, "empty", "empty", 0,
			hardwareOrganization[5].totalCycles, hardwareOrganization[5].FirstStageCycles, -1, INT_MAX, 0, - 1);
		reservationstations.push_back(tempRS);
	}

	return;

}

void print_reservationstations(vector<ReservationStation>& reservationstations) {
	cout << "Reservation Stations:" << endl;
	cout << "Name   Busy    Op    Vj   Vk   Qj      Qk     A           TC  FSC  CC        Fetch        Result  Inst" << endl;
	for (int i = 0; i < reservationstations.size(); i++)
		reservationstations[i].print_RS();
	cout << endl;
	return;
}

void intialize_RegStatus(string RegStatus[8]) {
	for (int i = 0; i < 8; i++)
		RegStatus[i] = "empty";
	return;
}

void print_RegStatus(string RegStatus[8]) {
	cout << "RegStatus:" << endl;
	for (int i = 0; i < 8; i++)
		cout << "Reg " << i << " Status: " << RegStatus[i] << endl;
	cout << endl;
}

//B, J, R need work
void update(short int registers[8], vector<Instruction>& instructions, vector<InstructionDetails>& instructiondetails,
	vector<ReservationStation>& reservationstations, string RegStatus[8], CommonDataBus& CDB, int cycle, vector<MemoryEntry>& memory,
	FunctionalUnitSpecifications hardwareOrganization[6], vector<string>& string_instructions, int& defactoPC, int& actualPC, int startingAddress,
	bool& pauseIssuing, bool& pauseExecuting, int& totalNumberOfInstructions, int& causedPause) {

	int MostPriorityValue = INT_MAX;
	int MostPriorityIndex = INT_MAX;
	bool existFinishedExecution = false;
	char operationIdentifer;

	int memoryIndex = 0;
	bool memoryAddressFound = false;
	MemoryEntry tempMemoryEntry;

	//write (from Lec 19 - with modifications ofc)
	if (CDB.active == false) {
		for (int i = 0; i < reservationstations.size(); i++) { //to keep track of highest priority to write to the CDB
			if (reservationstations[i].Busy == true) {
				if (reservationstations[i].ExecuteCurrentCycle == reservationstations[i].ExecuteTotalCycles)
					instructiondetails[reservationstations[i].inst_index].finishedExecution = cycle - 1; 
				else if (reservationstations[i].ExecuteCurrentCycle > reservationstations[i].ExecuteTotalCycles) {
					
					existFinishedExecution = true;
					if (MostPriorityValue > reservationstations[i].order) {
						MostPriorityValue = reservationstations[i].order;
						MostPriorityIndex = i;
					}
				}
			}
		}
		if (existFinishedExecution == true) { //there is actually smth to write
			CDB.active = true;
			if (reservationstations[MostPriorityIndex].Name[0] == 'L' //Load
				|| reservationstations[MostPriorityIndex].Name[0] == 'A' //Neg, Add, Addi
				|| reservationstations[MostPriorityIndex].Name[0] == 'D') { //Div
			
				for (int i = 0; i < 8; i++) {
					if (RegStatus[i] == reservationstations[MostPriorityIndex].Name) {
						registers[i] = reservationstations[MostPriorityIndex].result;
						RegStatus[i] = "empty";
					}
				}
				for (int i = 0; i < reservationstations.size(); i++) {
					if (reservationstations[i].Qj == reservationstations[MostPriorityIndex].Name) {
						reservationstations[i].Vj = reservationstations[MostPriorityIndex].result;
						reservationstations[i].Qj = "empty";
					}
				}
				for (int i = 0; i < reservationstations.size(); i++) {
					if (reservationstations[i].Qk == reservationstations[MostPriorityIndex].Name) {
						reservationstations[i].Vk = reservationstations[MostPriorityIndex].result;
						reservationstations[i].Qk = "empty";
					}
				}
				CDB.value = reservationstations[MostPriorityIndex].result;
				
			}
			else if (reservationstations[MostPriorityIndex].Name[0] == 'S') {
				
				if (reservationstations[MostPriorityIndex].Qk == "empty") {
					for (memoryIndex; memoryIndex < memory.size(); memoryIndex++) {
						if (memory[memoryIndex].address == reservationstations[MostPriorityIndex].A) {
							memoryAddressFound = true;
							memory[memoryIndex].data = reservationstations[MostPriorityIndex].Vk;
							break;
						}
					}
					if (memoryAddressFound == false) {
						tempMemoryEntry.address = reservationstations[MostPriorityIndex].A;
						tempMemoryEntry.data = reservationstations[MostPriorityIndex].Vk;
						memory.push_back(tempMemoryEntry);
					}

					CDB.value = reservationstations[MostPriorityIndex].Vk;
					
				}
			}
			else if (reservationstations[MostPriorityIndex].Name[0] == 'J') {
				if (reservationstations[MostPriorityIndex].Op[0] == 'j') {
					registers[1] = reservationstations[MostPriorityIndex].inst_index + 1 + startingAddress;
					RegStatus[1] = "empty";
					actualPC = reservationstations[MostPriorityIndex].result;
					defactoPC = actualPC - startingAddress;
				}
				else { //ret 
					actualPC = reservationstations[MostPriorityIndex].Vj;
					defactoPC = actualPC - startingAddress;
				}
				pauseIssuing = false;
			}
			else if (reservationstations[MostPriorityIndex].Name[0] == 'B') {

				if (reservationstations[MostPriorityIndex].Vj == reservationstations[MostPriorityIndex].Vk) {
					actualPC = reservationstations[MostPriorityIndex].result;
					defactoPC = actualPC - startingAddress;
					//flush newly issued instructions (they can be identified by having order (issue time) bigger than the branch itself)
					for (int f = 0; f < reservationstations.size(); f++) {
						if (reservationstations[f].order > reservationstations[MostPriorityIndex].order) {
							reservationstations[f].free_RS();
						}
					}
					//and resume executing
				}
				//else (branch not taken) resume executing and do nothing else
				pauseExecuting = false;
			}
			
			instructiondetails[reservationstations[MostPriorityIndex].inst_index].written = cycle - 1;
			totalNumberOfInstructions++;

			cout << setw(18) << left << string_instructions[reservationstations[MostPriorityIndex].inst_index]
				<< setw(10) << right << instructiondetails[reservationstations[MostPriorityIndex].inst_index].issued
				<< setw(10) << instructiondetails[reservationstations[MostPriorityIndex].inst_index].startedExecution
				<< setw(12) << instructiondetails[reservationstations[MostPriorityIndex].inst_index].finishedExecution
				<< setw(13) << instructiondetails[reservationstations[MostPriorityIndex].inst_index].written << endl;

			reservationstations[MostPriorityIndex].free_RS(); //set Busy to false and reset a few other member varaibles
			CDB.active = false;
		}
	}
	//else do not attempt to write anything

	


	int LoadStoreHeadValue = INT_MAX;
	int LoadStoreHeadIndex = INT_MAX;
	bool needToComputeAddress = false;

	memoryIndex = 0;
	memoryAddressFound = false;

	if (pauseExecuting == false) {
		//issue --> execute / advance Load to next stage / Increment cycle	
		for (int i = 0; i < reservationstations.size(); i++) {

			operationIdentifer = reservationstations[i].Op[0];
			if (reservationstations[i].Busy == true) {
				switch (operationIdentifer)
				{
				//'b' case is redundant (because this segment is paused, so we handle it outside) but we are keeping it just in case
				case 'b': //beq 
					if (reservationstations[i].ExecuteCurrentCycle == -1) {
						if (reservationstations[i].Qj == "empty" && reservationstations[i].Qk == "empty") {
							
							//1 branch taken -- 2 branch not taken
							if (reservationstations[i].Vj == reservationstations[i].Vk)
								reservationstations[i].result = 1;
							else
								reservationstations[i].result = 2;
							
							reservationstations[i].ExecuteCurrentCycle = 1;
							instructiondetails[reservationstations[i].inst_index].startedExecution = cycle;
						}
					}
					else {
						reservationstations[i].ExecuteCurrentCycle++;
					}
					break;

				case 'j': //jalr
				case 'r': //ret
					if (reservationstations[i].ExecuteCurrentCycle == -1) {
						if (reservationstations[i].Qj == "empty") {
							reservationstations[i].result = reservationstations[i].Vj + startingAddress;
							reservationstations[i].ExecuteCurrentCycle = 1;
							instructiondetails[reservationstations[i].inst_index].startedExecution = cycle;
						}
					}
					else {
						reservationstations[i].ExecuteCurrentCycle++;
					}

					break;

				case 'a': //add
				case 'i': //i-addi
					if (reservationstations[i].ExecuteCurrentCycle == -1) {
						if (reservationstations[i].Qj == "empty" && reservationstations[i].Qk == "empty") {
							reservationstations[i].result = reservationstations[i].Vj + reservationstations[i].Vk;
							reservationstations[i].ExecuteCurrentCycle = 1;
							instructiondetails[reservationstations[i].inst_index].startedExecution = cycle;
						}
					}
					else {
						reservationstations[i].ExecuteCurrentCycle++;
					}
					break;

				case 'n': //neg
					if (reservationstations[i].ExecuteCurrentCycle == -1) {
						if (reservationstations[i].Qj == "empty") {
							reservationstations[i].result = -reservationstations[i].Vj;
							reservationstations[i].ExecuteCurrentCycle = 1;
							instructiondetails[reservationstations[i].inst_index].startedExecution = cycle;
						}
					}
					else {
						reservationstations[i].ExecuteCurrentCycle++;
					}
					break;

				case 'd': //div
					if (reservationstations[i].ExecuteCurrentCycle == -1) {
						if (reservationstations[i].Qj == "empty" && reservationstations[i].Qk == "empty") {
							reservationstations[i].result = reservationstations[i].Vj / reservationstations[i].Vk;
							reservationstations[i].ExecuteCurrentCycle = 1;
							instructiondetails[reservationstations[i].inst_index].startedExecution = cycle;
						}
					}
					else {
						reservationstations[i].ExecuteCurrentCycle++;
					}
					break;

				default:

					//we will group them:
					//Group 1: Load stage 2
					//Group 2: Store or Load stage 1

					if (operationIdentifer == 'l' && reservationstations[i].ExecuteCurrentCycle >= reservationstations[i].ExecuteFirstStageCycles) {
						if (operationIdentifer == 'l' && reservationstations[i].ExecuteCurrentCycle == reservationstations[i].ExecuteFirstStageCycles) {
							for (memoryIndex; memoryIndex < memory.size(); memoryIndex++) {
								if (memory[memoryIndex].address == reservationstations[i].A) {
									memoryAddressFound = true;
									reservationstations[i].result = memory[memoryIndex].data;
									break;
								}
							}
							if (memoryAddressFound == false) {
								cout << "You are trying to load data from an uninitialized memory location. "
									<< "You will therefore load gibberish data." << endl;
								reservationstations[i].result = rand();
							}
						}
						reservationstations[i].ExecuteCurrentCycle++;
					}
					else { //Store or Load stage 1 (need to keep track of load-store queue)
						if (reservationstations[i].ExecuteCurrentCycle == -1) { //convert S or L from issue to execute - need load-store queue
							if (reservationstations[i].Qj == "empty") {
								needToComputeAddress = true;
								if (LoadStoreHeadValue > reservationstations[i].order) {
									LoadStoreHeadValue = reservationstations[i].order;
									LoadStoreHeadIndex = i;
								}
							}
						}
						else { //just increment
							reservationstations[i].ExecuteCurrentCycle++;
						}
					}

					break;
				}
			}

		}

		if (needToComputeAddress == true) {
			reservationstations[LoadStoreHeadIndex].A = reservationstations[LoadStoreHeadIndex].Vj + reservationstations[LoadStoreHeadIndex].A;
			reservationstations[LoadStoreHeadIndex].ExecuteCurrentCycle = 1;
			instructiondetails[reservationstations[LoadStoreHeadIndex].inst_index].startedExecution = cycle;
		}
	}
	else {
		if (reservationstations[causedPause].ExecuteCurrentCycle == -1) {
			reservationstations[causedPause].result = actualPC + reservationstations[causedPause].A - 1; //because of previous cycle
			
			reservationstations[causedPause].ExecuteCurrentCycle = 1;
			instructiondetails[reservationstations[causedPause].inst_index].startedExecution = cycle;
		}
		else
			reservationstations[causedPause].ExecuteCurrentCycle++;
		}
	return;
}

//B, J, R need work
bool attemptIssue(vector<ReservationStation>& reservationstations, Instruction& inst,
	FunctionalUnitSpecifications hardwareOrganization[6], string RegStatus[8], short int registers[8],
	int cycle, int InstructionIndex, bool& pauseIssuing, bool& pauseExecuting, int& causedPause) {
	
	//L for LW, S for SW, B for BEQ, J for JALR, R for RET, N for NEG, D for DIV
	//A for ADD or ADDI (overlap is acceptable because they both use the same reservation station)
	char typeIdentifier = inst.type[0];

	int end_index = 0;

	bool successfulIssue = false;

	switch (typeIdentifier)
	{

	case 'L':	
		end_index = hardwareOrganization[0].startindex + hardwareOrganization[0].numberOfRS;
		for (int i = hardwareOrganization[0].startindex; i < end_index; i++) {
			if (reservationstations[i].Busy == false) {
				reservationstations[i].inst_index = InstructionIndex;
				successfulIssue = true;

				//rs
				if (RegStatus[inst.rs1] != "empty")
					reservationstations[i].Qj = RegStatus[inst.rs1];
				else {
					reservationstations[i].Vj = registers[inst.rs1];
					reservationstations[i].Qj = "empty";
				}

				//rd
				RegStatus[inst.rd] = reservationstations[i].Name;

				reservationstations[i].Qk = "empty";
				reservationstations[i].A = inst.imm;
				reservationstations[i].Busy = true;
				reservationstations[i].order = cycle;
				reservationstations[i].Op = "load";
				reservationstations[i].ExecuteCurrentCycle = -1;

				break;
			}
		}
		break;

	case 'S':
		end_index = hardwareOrganization[1].startindex + hardwareOrganization[1].numberOfRS;
		for (int i = hardwareOrganization[1].startindex; i < end_index; i++) {
			if (reservationstations[i].Busy == false) {
				reservationstations[i].inst_index = InstructionIndex;
				successfulIssue = true;
				//rs
				if (RegStatus[inst.rs1] != "empty")
					reservationstations[i].Qj = RegStatus[inst.rs1];
				else {
					reservationstations[i].Vj = registers[inst.rs1];
					reservationstations[i].Qj = "empty";
				}
				
				//rt
				if (RegStatus[inst.rs2] != "empty")
					reservationstations[i].Qk = RegStatus[inst.rs2];
				else {
					reservationstations[i].Vk = registers[inst.rs2];
					reservationstations[i].Qk = "empty";
				}

				reservationstations[i].A = inst.imm;
				reservationstations[i].Busy = true;
				reservationstations[i].order = cycle;
				reservationstations[i].Op = "store";
				reservationstations[i].ExecuteCurrentCycle = -1;

				break;
			}
		}
		break;

	case 'B':
		end_index = hardwareOrganization[2].startindex + hardwareOrganization[2].numberOfRS;
		for (int i = hardwareOrganization[2].startindex; i < end_index; i++) {
			if (reservationstations[i].Busy == false) {
				reservationstations[i].inst_index = InstructionIndex;
				successfulIssue = true;
				//rs
				if (RegStatus[inst.rs1] != "empty")
					reservationstations[i].Qj = RegStatus[inst.rs1];
				else {
					reservationstations[i].Vj = registers[inst.rs1];
					reservationstations[i].Qj = "empty";
				}

				//rt
				if (RegStatus[inst.rs2] != "empty")
					reservationstations[i].Qk = RegStatus[inst.rs2];
				else {
					reservationstations[i].Vk = registers[inst.rs2];
					reservationstations[i].Qk = "empty";
				}

				pauseExecuting = true;
				causedPause = i;

				reservationstations[i].A = inst.imm;
				reservationstations[i].Busy = true;
				reservationstations[i].order = cycle;
				reservationstations[i].Op = "beq";
				reservationstations[i].ExecuteCurrentCycle = -1;

				break;
			}
		}
		break;

	case 'J':
		end_index = hardwareOrganization[3].startindex + hardwareOrganization[3].numberOfRS;
		for (int i = hardwareOrganization[3].startindex; i < end_index; i++) {
			if (reservationstations[i].Busy == false) {
				reservationstations[i].inst_index = InstructionIndex;
				successfulIssue = true;

				//rs
				if (RegStatus[inst.rs1] != "empty")
					reservationstations[i].Qj = RegStatus[inst.rs1];
				else {
					reservationstations[i].Vj = registers[inst.rs1];
					reservationstations[i].Qj = "empty";
				}

				//rd = R1
				RegStatus[1] = reservationstations[i].Name;

				reservationstations[i].Qk = "empty";
				reservationstations[i].Busy = true;
				reservationstations[i].order = cycle;
				reservationstations[i].Op = "jalr";
				reservationstations[i].ExecuteCurrentCycle = -1;

				pauseIssuing = true;

				break;
			}
		}

		break;

	case 'R':
		end_index = hardwareOrganization[3].startindex + hardwareOrganization[3].numberOfRS;
		for (int i = hardwareOrganization[3].startindex; i < end_index; i++) {
			if (reservationstations[i].Busy == false) {
				reservationstations[i].inst_index = InstructionIndex;
				successfulIssue = true;

				//rs
				if (RegStatus[1] != "empty")
					reservationstations[i].Qj = RegStatus[1];
				else {
					reservationstations[i].Vj = registers[1];
					reservationstations[i].Qj = "empty";
				}

				reservationstations[i].Qk = "empty";
				reservationstations[i].Busy = true;
				reservationstations[i].order = cycle;
				reservationstations[i].Op = "ret";
				reservationstations[i].ExecuteCurrentCycle = -1;

				pauseIssuing = true;

				break;
			}
		}

		break;

	case 'N':
		end_index = hardwareOrganization[4].startindex + hardwareOrganization[4].numberOfRS;
		for (int i = hardwareOrganization[4].startindex; i < end_index; i++) {
			if (reservationstations[i].Busy == false) {
				reservationstations[i].inst_index = InstructionIndex;
				successfulIssue = true;
				//rs
				if (RegStatus[inst.rs1] != "empty")
					reservationstations[i].Qj = RegStatus[inst.rs1];
				else {
					reservationstations[i].Vj = registers[inst.rs1];
					reservationstations[i].Qj = "empty";
				}

				//rd
				RegStatus[inst.rd] = reservationstations[i].Name;

				reservationstations[i].Qk = "empty";
				reservationstations[i].Busy = true;
				reservationstations[i].order = cycle;
				reservationstations[i].Op = "neg";
				reservationstations[i].ExecuteCurrentCycle = -1;

				break;
			}
		}
		break;

	case 'D':
		end_index = hardwareOrganization[5].startindex + hardwareOrganization[5].numberOfRS;
		for (int i = hardwareOrganization[5].startindex; i < end_index; i++) {
			if (reservationstations[i].Busy == false) {
				reservationstations[i].inst_index = InstructionIndex;
				successfulIssue = true;
				//rs
				if (RegStatus[inst.rs1] != "empty")
					reservationstations[i].Qj = RegStatus[inst.rs1];
				else {
					reservationstations[i].Vj = registers[inst.rs1];
					reservationstations[i].Qj = "empty";
				}

				//rt
				if (RegStatus[inst.rs2] != "empty")
					reservationstations[i].Qk = RegStatus[inst.rs2];
				else {
					reservationstations[i].Vk = registers[inst.rs2];
					reservationstations[i].Qk = "empty";
				}

				//rd
				RegStatus[inst.rd] = reservationstations[i].Name;

				reservationstations[i].Busy = true;
				reservationstations[i].order = cycle;
				reservationstations[i].Op = "div";
				reservationstations[i].ExecuteCurrentCycle = -1;

				break;
			}
		}
		break;

	case 'A':
		
		end_index = hardwareOrganization[4].startindex + hardwareOrganization[4].numberOfRS;
		if (inst.type.length() == 3) { //ADD
			for (int i = hardwareOrganization[4].startindex; i < end_index; i++) {
				if (reservationstations[i].Busy == false) {
					reservationstations[i].inst_index = InstructionIndex;
					successfulIssue = true;
					//rs
					if (RegStatus[inst.rs1] != "empty")
						reservationstations[i].Qj = RegStatus[inst.rs1];
					else {
						reservationstations[i].Vj = registers[inst.rs1];
						reservationstations[i].Qj = "empty";
					}

					//rt
					if (RegStatus[inst.rs2] != "empty")
						reservationstations[i].Qk = RegStatus[inst.rs2];
					else {
						reservationstations[i].Vk = registers[inst.rs2];
						reservationstations[i].Qk = "empty";
					}

					//rd
					RegStatus[inst.rd] = reservationstations[i].Name;

					reservationstations[i].Busy = true;
					reservationstations[i].order = cycle;
					reservationstations[i].Op = "add";
					reservationstations[i].ExecuteCurrentCycle = -1;

					break;
				}
			}
		}
		else { //ADDI
			for (int i = hardwareOrganization[4].startindex; i < end_index; i++) {
				if (reservationstations[i].Busy == false) {
					reservationstations[i].inst_index = InstructionIndex;
					successfulIssue = true;
					//rs
					if (RegStatus[inst.rs1] != "empty")
						reservationstations[i].Qj = RegStatus[inst.rs1];
					else {
						reservationstations[i].Vj = registers[inst.rs1];
						reservationstations[i].Qj = "empty";
					}

					//imm
					reservationstations[i].Vk = inst.imm;

					//rd
					RegStatus[inst.rd] = reservationstations[i].Name;

					reservationstations[i].Qk = "empty";
					reservationstations[i].Busy = true;
					reservationstations[i].order = cycle;
					reservationstations[i].Op = "i-addi";
					reservationstations[i].ExecuteCurrentCycle = -1;

					break;
				}
			}
		}
		break;

	default: //irregular instruction - should never reach here anyway
		break;
	}

	return successfulIssue;
}

void print_instructiondetails(vector<InstructionDetails>& instructiondetails, vector<string>& string_instructions) {
	cout << "Instruction              Issue   StartExec   FinishExec    Writing" << endl;
	for (int i = 0; i < instructiondetails.size(); i++) {
		cout << setw(18) << left << string_instructions[i]
			<< setw(10) << right << instructiondetails[i].issued << setw(10) << instructiondetails[i].startedExecution
			<< setw(12) << instructiondetails[i].finishedExecution << setw(13) << instructiondetails[i].written << endl;
	}
	cout << endl;
}

bool areAllReservationStationsFree(vector<ReservationStation>& reservationstations) {
	for (int i = 0; i < reservationstations.size(); i++)
		if (reservationstations[i].Busy == true)
			return false;
	return true;
}

void Tomasulo() {
	
	//Declaration


	short int registers[8]; //short int for 16-bits
	
	int startingAddress;

	vector<string> string_instructions;
	vector<Instruction> instructions;
	vector<InstructionDetails> instructiondetails;

	vector<MemoryEntry> memory;

	//0 LOAD, 1 STORE, 2 BRANCH, 3 JUMP (JALR/RET), 4 ARITH (ADD/NEG/ADDI), 5 DIV
	FunctionalUnitSpecifications hardwareOrganization[6];
	
	vector<ReservationStation> reservationstations;
	string RegStatus[8];

	int cycle = 1;
	int defactoPC = 0;
	int actualPC;

	CommonDataBus CDB;
	CDB.active = false;

	bool pauseIssuing = false;
	bool pauseExecuting = false;
	int causedPause = -1;

	bool DoneFlag = false;

	int totalNumberOfInstructions = 0;
	double IPC;



	//For Debugging Purposes (Makes Breakpoints at certain points easier)
	int desired_clk_cycle = 20;
	int desired_inst = 5;
	int debug_trash = 0;
	//For Debugging Purposes (Makes Breakpoints at certain points easier)




	//Functionality


	//registers
	initialize_registers(registers);
	print_registers(registers);


	//read instructions
	if (!readInstructionsFromFile(instructions, startingAddress, string_instructions)) {
		cout << "Cannot read instructions from file." << endl;
		return;
	}
	print_instructions(instructions);
	instructiondetails.resize(instructions.size());

	actualPC = startingAddress;

	//read memory
	if (!readMemoryFromFile(memory)) {
		cout << "Cannot read data memory initialization from file." << endl;
		return;
	}
	printMemory(memory);


	//BONUS: variable hardware organization
	variable_hardware_organization(hardwareOrganization);
	print_hardwareOrganization(hardwareOrganization);


	cout << "Clock Cycle: 0" << endl;
	//reservation stations
	initialize_reservationstations(reservationstations, hardwareOrganization);
	print_reservationstations(reservationstations);


	//register status table
	intialize_RegStatus(RegStatus);
	print_RegStatus(RegStatus);
	
	cout << endl;



	//CORE
	cout << "Instruction              Issue   StartExec   FinishExec    Writing" << endl;
	while (DoneFlag == false) {

		//debugging
		if (defactoPC == desired_inst)
			debug_trash++;

		update(registers, instructions, instructiondetails, reservationstations, RegStatus, CDB, cycle,
			memory, hardwareOrganization, string_instructions, defactoPC, actualPC, startingAddress,
			pauseIssuing, pauseExecuting, totalNumberOfInstructions, causedPause);
		
		if (defactoPC < instructions.size() && pauseIssuing==false) {
			if (attemptIssue(reservationstations, instructions[defactoPC], hardwareOrganization,
				RegStatus, registers, cycle, defactoPC, pauseIssuing, pauseExecuting, causedPause) == true) {

				instructiondetails[defactoPC].issued = cycle;
				defactoPC += 1;
				actualPC += 1;
			}
		}
		//else defactoPC not incremented

		/*cout << "Clock Cycle: " << cycle << endl;
		print_reservationstations(reservationstations);
		print_RegStatus(RegStatus);
		cout << endl;*/

		cycle++;

		//we check > because maybe beq or jalr send us to a segment that's not in the instruction memory
		//we check = because we have either issued / skipped over all instructions

		DoneFlag = areAllReservationStationsFree(reservationstations);
	}
	cout << endl;

	IPC = double(totalNumberOfInstructions) / double(cycle-double(2)); //-1 because there is an additional ++ & -1 for the 0 cycle
	cout << "Total Execution Time: " << cycle-2 << endl; //-1 because there is an additional ++ & -1 for the 0 cycle
	cout << "Total number of instructions executed: " << totalNumberOfInstructions << endl;
	cout << "IPC: " << IPC << endl;
	cout << "Branch Misprediction Percentage: " << endl;

	cout << endl;
	print_registers(registers);

	printMemory(memory);

	return;
}

int main() {
	Tomasulo();
	system("pause");
	return 0;
}