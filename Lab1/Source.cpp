#include <iostream>
#include <string>
#include <fstream>

using namespace std;



int main() {
	string str, program, labelName, output, condition;
	labelName = "cycle";
	ifstream file("cycle.pas");
	ofstream out("output.pas", ofstream::out);
	int curState = 0;
	int cyclePos, cycleLength, conditionStart;
	bool start = true;

	while (true) {
		if (curState == 0 && !start)
			break;
		switch (curState) {
		case 0:
			if (start) {
				start = false;
				curState = 1;
			}
			else {
				out.close();
				return 0;
			}
			break;
		case 1:

			curState = 2;
			break;
		case 2:
			if (!file.eof()) {
				getline(file, str);
				program += str + "\n";
				curState = 2;
			}
			else
				curState = 3;
			break;
		case 3:
			cyclePos = program.find("while");
			cycleLength = program.find("end;") - cyclePos + 4;
			conditionStart = cyclePos + 6;
			condition = program.substr(conditionStart, program.find("do") - conditionStart);
			curState = 4;
			break;
		case 4:
			output = program;
			curState = 5;
			break;
		case 5:
			output.insert(cyclePos + cycleLength - 4, "\tgoto " + labelName + ";\n");
			output.replace(conditionStart + condition.length(), 2, "then");
			output.replace(cyclePos, 5, "if");
			output.insert(cyclePos, labelName + ":\n");
			output.insert(output.find("var"), "label " + labelName + ";\n");
			curState = 6;
			break;
		case 6:
			out << output;
			curState = 0;
			break;
		}
	}
	//out << output;
}