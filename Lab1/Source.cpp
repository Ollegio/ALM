#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main() {
	string str, program, labelName;
	labelName = "a";
	ifstream file("cycle.pas");
	ofstream out("output.pas", ofstream::out);

	while (getline(file, str)) {
		program += str + "\n";
	}

	int cyclePos = program.find("while");
	int cycleLength = program.find("end;") - cyclePos + 4;
	int conditionStart = cyclePos + 6;
	string condition = program.substr(conditionStart, program.find("do") - conditionStart);
	
	string output = program;
	
	output.insert(cyclePos + cycleLength - 4, "\tgoto " + labelName + ";\n");
	output.replace(conditionStart + condition.length(), 2, "then");
	output.replace(cyclePos, 5, "if");
	output.insert(cyclePos, labelName + ":\n");
	output.insert(output.find("var"), "label " + labelName + ";\n");
	
	out << output;
}