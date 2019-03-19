#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <sstream>

#include <windows.h>


using namespace std;

int current_level_x=0, current_level_y =0;
bool go_right;
void gotoxy(int x, int y)
{
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


//SCANNER 
void Trim(string& s)
{
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == ' ' && s[i + 1] == ' ')
		{
			s.erase(i, 1);
			i--;
		}
	}
}

void EditNewLineCharacter(string& s)
{
	for (int i = 0 ; i<s.length() ;i++)
	{
		if (s[i] == '\n')
		{
			s[i]= ' ';
		}

		if (s[i] == ';' || s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' 
			|| s[i] == '>' || s[i] == '<' || s[i] == '(' || s[i] == ')' )
		{
			s.insert(i+1," ");
			s.insert(i," ");
			i+=2;
		}
		if(s[i] == '=')
		{
			if(s[i-1] == ':')
			{
				s.insert(i+1," ");
				s.insert(i-1," ");
				i+=2;
			}
			else
			{
				s.insert(i+1," ");
				s.insert(i," ");
				i+=2;
			}
		}
	}
}

void RemoveComments(string& s)
{
	int end ;
	for (int i = 0 ; i<s.length() ;i++)
	{
		if(s[i] == '{')
		{
			end = i;
			while(s[end] != '}')
			{
				end++;
			}
			s.erase(i ,end-i+1);
			i--;
		}
		
	}
}

void split(string &s , vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, ' ')) {
        elems.push_back(item);
    }
}


bool checkReservedWord(string word)
{
	string ReservedWords[8] ={"if" ,"then","else","end","repeat","until" ,"read","write"};
	for(int i = 0 ; i<8; i++)
	{
		if(word == ReservedWords[i])
			return 1 ;
	}
	return 0 ;
}

string checkSpecialSymbols(string word)
{
	if(word == "+")
		return "Plus";

	else if (word == "-")
		return "Minus";

	else if (word == "*")
		return "Asterisk";

	else if (word == "/")
		return "Slash";

	else if (word == "=")
		return "Equal sign";

	else if (word == ">")
		return "Greater than";

	else if (word == "<")
		return "Less than";

	else if (word == "(")
		return "Minus";

	else if (word == ")")
		return "Minus";

	else if (word == ";")
		return "Semicolon";

	else if (word == ":=")
		return "assign";

	else
		return "none";
}

bool IsNumber(string word)
{
	int i =0 ;
	while(isdigit(word[i])) 
		i++;
	if (i>0)
		return 1;
	return 0;
}

vector<string> idss;
int x =0;
vector<string> numss;
int z =0;
void p_main()
{
	vector<string> words ;
	ifstream ip ("input.txt");
	ofstream op ("output.txt");
	string txtIp((istreambuf_iterator<char>(ip)),istreambuf_iterator<char>());
	RemoveComments(txtIp);
	EditNewLineCharacter(txtIp);
	Trim(txtIp);
	split(txtIp,words);
	for(int i =0 ; i<words.size() ;i++)
	{		
		if(checkReservedWord(words[i]))
			op << words[i] <<" " ;

		else if(checkSpecialSymbols(words[i]) != "none")
			op << words[i] <<" ";

		else if (IsNumber(words[i]))
		{
			op <<"number " ;
			numss.push_back(words[i]);
			z++;
		}
			
		else
		{
			op <<"identifier ";
			idss.push_back(words[i]);
			x++;

		}
			
	}
	x=0;
	z=0;
}



// PARSER 

ifstream file;
string token;

enum stmtTypes {IF, REPEAT, ASSIGN, READ, WRITE};

stmtTypes getStmtType(string s)
{
	if (s == "if") return IF;
	if (s == "identifier") return ASSIGN;
	if (s == "read") return READ;
	if (s == "write") return WRITE;
	if (s == "repeat") return REPEAT;
}

void match(string s) 
{
	if (s == token) 
	{ 
		file >> token;
	}
	else 
	{
		cout << endl << "-- Program not found" << endl;
		exit(1);
	}
}

void program();
void stmt_seq();
void stmt();
void if_stmt();
void repeat_stmt();
void assign_stmt();
void read_stmt();
void write_stmt();
void exp();
void comparison_op();
void simple_exp();
void addop();
void term();
void mulop();
void factor();


// program -> stmt-seq
void program()
{
	stmt_seq();
}

// stmt-seq -> stmt {; stmt}
void stmt_seq()
{
	stmt();
	while (token == ";") 
	{
		match(";");
		stmt();
	}
}

// stmt -> if-stmt | repeat-stmt | assign-stmt | read-stmt | write-stmt
void stmt() 
{
	switch (getStmtType(token))
	{
	case IF:
		if_stmt(); break;
	case REPEAT:
		repeat_stmt(); break;
	case READ: 
		read_stmt(); break;
	case WRITE:
		write_stmt(); break;
	default:
		assign_stmt(); break;
	}
}

// if-stmt -> if exp then stmt-seq [else stmt-seq] end
void if_stmt() 
{
	gotoxy(current_level_x,current_level_y+=2);
	match("if");
	cout<<"if";
	gotoxy(current_level_x,current_level_y+=2);
	exp();
	match("then");
	gotoxy(current_level_x+=17,current_level_y);
	stmt_seq();
	gotoxy(current_level_x+=17,current_level_y-=2);
	if (token == "else") 
	{
		match("else");
		gotoxy(current_level_x,current_level_y+=2);
		stmt_seq();
		gotoxy(current_level_x+=17,current_level_y-=2);
	}
	match("end");
	gotoxy(current_level_x,current_level_y-=2);
	gotoxy(current_level_x+=17,current_level_y-=2);
}

// repeat-stmt -> repeat stmt-seq until exp
void repeat_stmt() 
{
	match("repeat");
	cout<<"repeat";
	gotoxy(current_level_x,current_level_y+=2);
	stmt_seq();
	match("until");
	gotoxy(current_level_x+=17,current_level_y);
	exp();
	gotoxy(current_level_x+=17,current_level_y-=2);
}

// assign-stmt -> identifier := exp
void assign_stmt() 
{
	match("identifier");
	match(":=");
	cout<<"assign("<< idss[x]<<")";
	x++;
	gotoxy(current_level_x,current_level_y+=2);
	exp();
	gotoxy(current_level_x+=17,current_level_y-=2);
}

// read-stmt -> read identifier
void read_stmt() 
{
	match("read");
	match("identifier");
	cout << "read";
	gotoxy(current_level_x,current_level_y+=2);
	cout << idss[x];
	x++;
	gotoxy(current_level_x+=17,current_level_y-=2);


}

// write-stmt -> write exp
void write_stmt() 
{
	match("write");
	cout<<"write";
	gotoxy(current_level_x,current_level_y+=2);
	exp();
	gotoxy(current_level_x+=17,current_level_y-=2);
}

// exp -> simple-exp [comparison-op simple-exp]
void exp() 
{
	simple_exp();
	if (token == "<" || token == "=") 
	{
		comparison_op();
		simple_exp();
	}
}

// comparison-op -> < | =
void comparison_op() 
{
	if (token == "<") 
	{
		match("<");
		cout<<"<";
		go_right =true;
	}
	if (token == "=") 
	{
		match("=");
		cout<<"=";
		go_right =true;
	}

}

// simple-exp -> term { addop term }
void simple_exp() 
{
	term();
	int r=0;
	while (token == "+" || token == "-") {
		if (r>0)
			gotoxy(current_level_x+=15,current_level_y+=2);
		addop();
		term();
		if (r>0)
			gotoxy(current_level_x-=15,current_level_y-=2);
		r++;
	}
}

// addop -> + | -
void addop() 
{
	if (token == "+") 
	{
		match("+");
		cout<<"+";
		go_right =true;
	}
	if (token == "-") 
	{
		match("-");
		cout<<"-";
		go_right =true;
	}
}

// term -> factor { mulop factor }
void term() 
{
	factor();
	while (token == "*" || token == "/") 
	{
		mulop();
		factor();
	}
}

// mulop -> * | /
void mulop() 
{
	if (token == "*") 
	{
		match("*");
		cout<<"*";
		go_right =true;
	}
	if (token == "/") 
	{
		match("/");
		cout<<"/";
		go_right =true;
	}
}

// factor -> (exp) | number | identifier
void factor() 
{
	if (token == "(") 
	{ 
		match("(");
		exp();
		match(")");
	}
	else if (token == "number") 
	{
		match("number");
		if(go_right)
		{
			gotoxy(current_level_x+=6,current_level_y+=2);
			cout<<numss[z];
			z++;
			gotoxy(current_level_x-=6,current_level_y-=2);
			go_right = false;
		}
		else
		{
			gotoxy(current_level_x,current_level_y+=2);
			cout<<numss[z];
			z++;
			gotoxy(current_level_x,current_level_y-=2);
		}
		
	}
	else if (token == "identifier") 
	{
		match("identifier");
		if(go_right)
		{
			gotoxy(current_level_x+=6,current_level_y+=2);
			cout<<idss[x];
			x++;
			gotoxy(current_level_x-=6,current_level_y-=2);
			go_right = false;
		}
		else
		{
			gotoxy(current_level_x,current_level_y+=2);
			cout<<idss[x];
			x++;
			gotoxy(current_level_x,current_level_y-=2);
		}

	}
}

int main()
{
  /*typedef tree::node<int> node;
  node* head = new node();
  head->l    = new node(1);
  head->r    = new node(2);
  head->l->l = new node(3);
  head->l->r = new node(4);
  head->r->l = new node(5);
  head->r->r = new node(6);
  head->l->l->l = new node(7);
  head->l->l->r = new node(8);
  head->l->r->l = new node(9);
  head->l->r->r = new node (10);
  head->r->l->l = new node(11);
  head->r->l->r = new node(12);
  head->r->r->l = new node(13);
  head->r->r->r = new node (14);


  tree::prt(head);
*/

	p_main();
	file.open("output.txt");
	file >> token;
	program();
	file.close();
	system("pause");

}