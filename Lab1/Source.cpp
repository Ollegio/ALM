#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main() {
	string str, program;
	ifstream file("cycle.pas");

	while (getline(file, str)) {
		program += str + "\n";
	}

	int cyclePos = program.find("while");
	int cycleLength = program.find("end;") - cyclePos + 4;
	string cycle = program.substr(cyclePos, cycleLength);
	int conditionStart = cyclePos + 6;
	string condition = program.substr(conditionStart, program.find("do") - conditionStart);
	int cycleBodyStart = cycle.find("begin") + 6;
	string cycleBody = cycle.substr(cycleBodyStart, cycle.find("end;") - cycleBodyStart - 1);
	
	string labelName = "a";
	string output = program;
	
	output.insert(cyclePos + cycleLength - 4, "goto " + labelName + ";\n");
	output.replace(conditionStart + condition.length(), 2, "then");
	output.replace(cyclePos, 5, "if");
	output.insert(cyclePos, labelName + ":\n");
	output.insert(output.find("var"), "label " + labelName + ";\n");
	cout << output;
}