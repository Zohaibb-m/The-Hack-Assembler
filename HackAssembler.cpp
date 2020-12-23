#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <conio.h>
using namespace std;
//Initializing streams for reading and writing files
ifstream in;
ofstream file;

//Function Declarations
void Assembler(string);
void Rename(string);
string removespaces(string);
void AInstruction();
void CInstruction();
bool numberextractor(string);
void Comp(string);
void Dest(string);
void Jump(string);
void ReadLabels(string);

//Variables Initialization
string Instruction[42]={"0","1","-1","D","A","!D","!A","-D","-A","D+1","A+1","D-1","A-1","D+A","D-A","A-D","D&A","D|A","M","!M","-M","M+1","M-1","D+M","D-M","M-D","D&M","D|M","M","D","MD","A","AM","AD","AMD","JGT","JEQ","JGE","JLT","JNE","JLE","JMP"};                    
string Value[42]={"101010","111111","111010","001100","110000","001101","110001","001111","110011","011111","110111","001110","110010","000010","010011","000111","000000","010101","110000","110001","110011","110111","110010","000010","010011","000111","000000","010101","001","010","011","100","101","100","111","001","010","011","100","101","110","111"};
string Symbol[1000]={"R0","R1","R2","R3","R4","R5","R6","R7","R8","R9","R10","R11","R12","R13","R14","R15","SP","LCL","ARG","THIS","THAT","SCREEN","KBD"};
int Address[1000]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,16384,24576};
int LineNumber;
bool Error=false;
string na;
int symbolindex=23,n=16;


void ReadLabels(string filename){//To read all the labels of the program
	ifstream file1;
	string line;
	int lineno=-1;
	file1.open(filename.c_str(),ios::in);		//Opening the file
	if(file1.is_open()){
		while(getline(file1,line)){				//Reading file line by line
		if(line.find("//") != string::npos)		
			line.erase(line.find("//"));		//If any comment is found then it is removed
		line=removespaces(line);				//Function to remove spacess (Defined below)
		if(!line.empty()){						//If line is empty then it is skipped
		lineno++;								//Line counter to count the no of instructions
		if(line[0]!='(')continue;				//The code below this line is only executed if the line is a label i.e starts from "("
		lineno--;
		string label(line.begin()+1,line.begin()+line.find(')'));		//This line seperates out the label and stores it in a variable named label
		Symbol[symbolindex]=label;				//Storing the label in the symbol table along with its address
		Address[symbolindex++]=lineno+1;
		}
	}
}
file1.close();
}

bool numberextractor(string num){
	if(atoi(num.c_str())==0)return false;			//Checks if the given string is a number or not returns true if the given 
	return true;									//string is an integer else return false
}

void AInstruction(string code){
	int k=15;
	if (code[0]!='@')return;						//Checks if the instruction is an A instruction
	int binary[16]={0},number;						//Declaring a 16bit array and stroing 0 in it
	string num(code.begin()+1,code.end());			//Skipping the @ in code and saving the rest of code in variable named num
	if(num!="0"){
	if(numberextractor(num)==false){				//Calling the numberextracor function to check if the extracted code is a number or a variable/label
		for(int i=0;i<symbolindex;i++){				//Incase it is not a number
		if(Symbol[i]==num){							//Check if the extraced code is already present in the symbol table
			number=Address[i];						//Incase it is present in symbol table its value is sored in integer variable number for further processing
			goto no;								//Goto statement "no" for binary conversion
		}
		}
		{
			Symbol[symbolindex]=num;				//Incase it is a new variable not present in the symbol table then
			Address[symbolindex++]=number=n++;		//I is saved in the symbol table and its value is set starting from 16 ,17 and so on and saved in var number
			goto no;								//Goto statement "no" for binary conversion
		}
	}
}
	number=atoi(num.c_str());						//In case it is not a variable nor a label but a number its value is saved in var number using a to i function 
	no:												//The label "no"
	for(int i=0;number>0;i++){
		int temp;									//Taking a temp named variable
		temp=number%2;								
		number=number/2;							//Converting the number to its equivilant 15 bit binary code and saving it in binary array
		binary[k--]=temp;
	}
	for (int i=0;i<16;i++){
		file << binary[i];							//Writing the binary array in file
	}
	file << "\n";									//Writing a newline character
}

void CInstruction(string code){
	bool flag=false;								//Taking a variable flag for error handling
	if(code[0]=='@')return;							//Checking if the code is a C instruction or not
	for(int i=0;code[i]!=0;i++){					//This loop runs and checks which type of C Instruction is present in the code(Total 2 types)
		if(code[i]=='='){							//If an '=' character is found means it is a dest=comp type 
		flag=true;									//Setting flag o true means no error
		string DEST(code.begin(),code.begin()+code.find('=')),COMP(code.begin()+code.find('=')+1,code.end()); //Extracing DEST and COMP from the code for furher processing
		Comp(COMP);									//Calling Comp Function
		Dest(DEST);									//Calling Dest Function
		}
		else if(code[i]==';'){						//If an ';' character is found means it is a comp;jump type 
		flag=true;									//Setting flag o true means no error
		string COMP(code.begin(),code.begin()+code.find(';')),JUMP(code.begin()+code.find(';')+1,code.end());	//Extracing DEST and COMP from the code for furher processing
		Comp(COMP);									//Calling Comp Function
		Jump(JUMP);									//Calling Jump Function
		}
		if(code[0]=='(')flag=true;					//If the code's firs characer is a '(' setting flag to true means no error since the code has labels which arent a syntax error
	}
	if(flag==false){								
		Error=true;									//If flag is false means heres some error so setting Error to true
	}
}

void Comp(string COMP){
	int a=1;
	int fl=0;
	for(int i=0;i<28;i++){
		if(Instruction[i]==COMP){					//This loop matches the COMP instruction with the Instruction array and if it matches
		if(i<18)a=0;								//It takes its value from the Value array and writes to file
		file << "111";								//If the instruction is from 1 - 17 a is set to 0 otherwise 1
		file << a;									//Then it writes in the file
		file << Value[i]; 
		fl=1;
		}
	}
	if(fl=0) Error=true;							//If the code not matches setting error to true
}

void Dest(string DEST){
	int fl=0;
	for(int i=28;i<35;i++){
		if(Instruction[i]==DEST){
		file << Value[i];							//Similar Operation from the Comp Function
		file << "000"; 								//Starts the loop from 28 till 34 and checks if these indexes matches the DEST variables since the 
		file << '\n';								//Since the Insrucion array contains DEST from 28 to 34
		fl=1;										//Incase it matches it writes to the file
		}
	}
	if(fl=0) Error=true;							//If the code not matches setting error to true
}

void Jump(string JUMP){
	int fl=0;
	for(int i=35;i<42;i++){
		if(Instruction[i]==JUMP){
		file <<"000";							//Similar Operation from the Comp Function
		file << Value[i]; 						//Starts the loop from 35 till 42 and checks if these indexes matches the JUMP variables since the 
		file <<'\n';							//Since the Insrucion array contains JUMP from 35 to 42
		fl=1;									//Incase it matches it writes to the file
		}
	}
	if(fl=0) Error=true;							//If the code not matches setting error to true
}

void Openfile(string filename){
	string ext=".hack";													
	string name(filename.begin(),filename.begin()+filename.find('.'));	//Extracing the name of the file without extension from the filename
	na=name+ext;									//Saving the filename with the new extension .hack 
	file.open(na.c_str(),ios::out);					//Opening the file with .hack extension for writing
}

string removespaces(string s){
	string st="";
	for(int i=0;i<s.length();i++){
		if(s[i]!=' ' && s[i]!='\t')st.push_back(s[i]);		//If the code contains any spaces or tab characters they will be removed
	}
	return st;
}

void Assembler(string filename){
		in.open(filename.c_str(),ios::in);					//Opening file for reading
		string line;										//A variable o sore insructions in
		Openfile(filename);									//Calling the openfile function for opening output file
		if(!in.is_open()){
			cout << "\nError Reading File\n";				//If error opening file gives out error and exits the program
			exit(1);
		}
		if(in.is_open()){
		while(getline(in,line)){							//Reading the file line by line
		if(line.find("//") != string::npos)
			line.erase(line.find("//"));					//If any comments are present they are removed
		line=removespaces(line);							//Calling the removespaces  function to remove spaces
		if(!line.empty()){									//If line is empty skip
		LineNumber++;										//Counting line number for error handling
		AInstruction(line);									//Calling the A Instruction
		CInstruction(line);									//Calling the C Instruction
		if(Error==true){
		cout << "Syntax Error at line no: " << LineNumber << "\n";	//Incase any error is found print error message and exit the program
		remove(na.c_str());
		exit(1);
	}
		}
	}
	
}
in.close();														//Closing the
file.close();													//Files
}

void Showsymboltable(){											//Function for shwoing the symbol table
		for(int i=0;i<symbolindex;i++)
		cout << Symbol[i] << "\t\t" << Address[i] << "\n";
}

int main(){
	string filename;
	cout << "\t\t\t THE HACK ASSEMBLER \n\n\n";
	cout << "Enter the filename: ";
	cin >> filename;					//Taking filename from the user
	ReadLabels(filename);
	Assembler(filename);				//Giving it to Assembler Function for further processing
	cout << "~ Press A to Assemble the Code\n~ Press S to view the Symbol Table\n";
	char c=getch();
	if(c=='A' || c=='a')
		cout << "\n\nThe Program has been Assembled..\n";
	else if(c=='S' || c=='s')
		Showsymboltable();
	else cout << "\n\nThe Program has been Assembled..\n";
}
