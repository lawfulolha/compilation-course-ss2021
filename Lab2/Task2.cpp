// CompilLab2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
9) S -> begin I = T {; I = T} end;

		T -> T*F | T/F | F

		I -> ab | ba

		C-> 10 | 100

		F -> I | I^C

*/
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

enum Token {
	EQ, //=  
	TERMINAL,
	TBEGIN,
	TEND,// begin, end
	MULT, //*
	DIV, // /
	POW, // ^
	WORD, // ab, ba
	LETTER, //a, b
	A,
	B,
	NUMBER, //10, 100
	LBR, // {
	RBR, // }
	SMCLN, // ;   
	ONE,
	ZERO,
	END,
	UNKNOWN,
	SPACE
};

struct Lex {
	int num;  // номер речення
	Token token; // тип лексеми
	string value; // значення лексеми

	Lex() {
		value = "";
		token = UNKNOWN;
	}
	Lex(int sentenceNumber, string tokenValue, Token tokenType) {
		num = sentenceNumber;
		value = tokenValue;
		token = tokenType;
	}
	string getInfo() {
		string result = " < " + to_string(num) + ", " + value + ", ";
		switch (token) {
		case(EQ): result += "EQ"; //=  
		break; case(TERMINAL): result += "TERMINAL";	 // begin, end
		break; case(MULT): result += "MULT";	  //*
		break; case(DIV): result += "DIV";  // /
		break; case(POW): result += "POW";		  // ^
		break; case(LETTER): result += "LETTER";//a, b
		break; case(NUMBER): result += "NUMBER";  //10, 100
		break; case(LBR): result += "LBR";  // {
		break; case(WORD):result += "WORD";
		break; case(RBR): result += "RBR";	 // }
		break; case(SMCLN): result += "SMCLN"; // ;   
		break; case(END): result += "END";
		break; case(UNKNOWN): result += "UNKNOWN";
		break; case(A): result += "A";
		break; case(B): result += "B";
		break; case(TBEGIN): result += "TERM_BEGIN";
		break; case(TEND): result += "TERM_END";
		break;
		}
		result += " > ";
		return  result;
	}
};






class Analizator {
private:
	Lex lexema{};
	bool recognized = true;

	int i = 0, j = 0;

public:
	bool isValid = true;
	string line = "";
	int position = 0;
	int lineNumber = 0;

	ofstream file;
	Analizator() {
		file.open("result.txt", std::ofstream::out | std::ofstream::app);
	};
	inline void skipSpaces() {

		int flag = false;
		for (i = 1; line.at(position + i) && !flag; ++i) {
			if (line.at(position + i) != ' ') {
				flag = true;
			}
		}
		flag = false;
		for (j = 1; line.at(position - j) && !flag; ++j) {
			if (line.at(position - j) != ' ') {
				flag = true;
			}
		}
	}
	Lex get_token() {

		
		char c;

		if (position < line.size()) {
			c = line.at(position);
		}
		else return { lineNumber, "end line" , END };

		if (position < line.size() - 1 && c == ' ') {
			//lexema = { lineNumber, "space" , SPACE };
			++position;
			return get_token();
		}

		else if (position > line.size() - 1 || c == '\n') {
			lexema = { lineNumber, "end line" , END };
			if (file.is_open()) file << lexema.getInfo() << endl;
			++position;
			return lexema;
		}

		if (c == 'b') {
			if (position+4<line.size()) {
				string word;

				word = line.substr(position, 5);
				if (word == "begin") {

					lexema = Lex{ lineNumber, word, TBEGIN };
					if (file.is_open()) file << lexema.getInfo() << endl;

					position += 5;
					return lexema;
				}
			}
			if (line.at(position + 1)) {

				if (line.at(position + 1) == 'a') {

					lexema = Lex{ lineNumber, "ba", Token::WORD };
					if (file.is_open()) file << lexema.getInfo() << endl;

					position += 2;
					return lexema;
				}
			}

			int flag = false;
			for (i = 1; (position + i)<line.size() && !flag; ++i) {
				if (!isalpha(line.at(position + i))) {
					flag = true;
				}
			}

			return Lex{ lineNumber,  line.substr(position++, i - 1), UNKNOWN };
		}

		if (c == 'e') {
			if (line.at(position + 2)) {
				string word;

				word = line.substr(position, 3);
				if (word == "end") {

					lexema = Lex{ lineNumber, word, TEND };
					if (file.is_open()) file << lexema.getInfo() << endl;

					position += 3;
					return lexema;
				}
			}
			++position;
			return Lex{ lineNumber, string{c}, UNKNOWN };
		}

		if (c == ';') {
			lexema = Lex{ lineNumber,  string{c}, SMCLN };
			if (file.is_open()) file << lexema.getInfo() << endl;

			++position;
			return lexema; 
		}

		if (c == '{') {
	 
					lexema = Lex{ lineNumber,  string{c}, LBR };
					if (file.is_open()) file << lexema.getInfo() << endl;

					++position;
					return lexema;
		//		}
			//}
			++position;
			return Lex{ position,  string{c}, UNKNOWN };
		}

		if (c == '}') {
			lexema = Lex{ lineNumber,  string{c}, RBR };
			if (file.is_open()) file << lexema.getInfo() << endl;

			position++;
			return lexema;
			int i, j;

			int flag = false;
			for (i = 1; line.at(position + i) && !flag; ++i) {
				if (line.at(position + i) != ' ') {
					flag = true;
				}
			}
			flag = false;
			for (j = 1; line.at(position - j) && !flag; ++j) {
				if (line.at(position - j) != ' ') {
					flag = true;
				}
			}

			if (line.at(position - j + 1) && line.at(position + i - 1)) {
				if ((line.at(position - j + 1) == 'a' || line.at(position - j + 1) == 'b' || line.at(position - j + 1) == '0')
					&& line.at(position + i - 1) == 'e') {
					lexema = Lex{ lineNumber,  string{c}, RBR };
					if (file.is_open()) file << lexema.getInfo() << endl;

					position++;
					return lexema;
				}
			}
			++position;
			return Lex{ lineNumber,  string{c}, UNKNOWN };
		}

		if (c == '^' || c == '*' || c == '/' || c == '=') {
			lexema = Lex{ lineNumber,  string{c}, (c == '^') ? POW : (c == '*') ? MULT : (c == '/') ? DIV : EQ };
			if (file.is_open()) file << lexema.getInfo() << endl;

			++position;
			return lexema;
			skipSpaces();
			if (line.at(position - j + 1) && line.at(position + 1)) {

			lexema = Lex{ lineNumber,  string{c}, (c == '^') ? POW : (c == '*') ? MULT : (c == '/') ? DIV : EQ };
					if (file.is_open()) file << lexema.getInfo() << endl;

					++position;
					return lexema;
				//}
			 }
			++position;
			return Lex{ lineNumber,  string{c}, UNKNOWN };
		}
		if (c == 'a') {
			if (line.at(position + 1)) {

				if (line.at(position + 1) == 'b') {

					lexema = Lex{ lineNumber, "ab", Token::WORD };
					if (file.is_open()) file << lexema.getInfo() << endl;

					position += 2;
					return lexema;
				}
			}
			++position;
			return Lex{ lineNumber,  string{c}, LETTER };
		}

		if (c == '1') {
			if(position+2<line.size()){
				if (line.at(position + 2)) {
					if (line.at(position + 1) == '0' && line.at(position + 2) == '0') {
						lexema = Lex{ lineNumber, "100", Token::NUMBER };
						if (file.is_open()) file << lexema.getInfo() << endl;

						position += 3;
						return lexema;
					} 

				}}
			if (position + 1 < line.size()) {
				if (line.at(position + 1) == '0') {

					lexema = Lex{ lineNumber, "10", Token::NUMBER };
					if (file.is_open()) file << lexema.getInfo() << endl;

					position += 2;
					return lexema;
				}
			}
			
				++position;
			    return Lex{ lineNumber,  string{c}, ONE };
		}
		if (c == '0') {
			++position;
			return Lex{ lineNumber,  string{c}, ZERO };
		}
		else {

			int flag = false;
			for (i = 0; !flag && position + i < line.size(); ++i) {
				if (!isalpha(line.at(position + i))) {
					flag = true;
				}
			}

			return Lex{ lineNumber,  line.substr(position++, i), UNKNOWN };
		}
	};
	Lex token{};

	void C() {
		if (token.token == NUMBER)
			token = this->get_token();
		else if (token.token == ZERO || token.token == ONE) {
			token = this->get_token();
			isValid = false;
			cout << "Unknown NUMBER at line " + to_string(lineNumber) + " position " + to_string(position) << endl;
		}
		else {
			isValid = false;
			cout << "Missed NUMBER at line " + to_string(lineNumber) + " position " + to_string(position) << endl;
		}
	}
	void I() {
		if (token.token == WORD)
			token = this->get_token();
		else {
			isValid = false;
			cout << "Missed WORD at line " + to_string(lineNumber) + " position " + to_string(position) << endl;
		}
	}
	void F() {
		I();
		if (token.token == POW) {
			token = this->get_token();
			C();
		}

	}
	void T1() {
		F();
		if (token.token == MULT || token.token == DIV) {

			token = this->get_token();

			T1();
		}
	}
	void T() {
		T1();

		if (token.token == MULT || token.token == DIV) {
				
				token = this->get_token();

				F();
			}

	}

	void S() {

		if (token.token == TBEGIN) {

			token = this->get_token();

			I();

			if (token.token == EQ) {

				token = this->get_token();

				T();
					
				if (token.token == LBR) {

					token = this->get_token();

					if (token.token == SMCLN) {

						token = this->get_token();

						I();

						if (token.token == EQ) {

							token = this->get_token();

							T();

							if (token.token == RBR) {

								token = this->get_token();

								if (token.token == TEND) {

									token = this->get_token();

									if (token.token == SMCLN) {
										token = this->get_token();
										return;
									}
									else {
										isValid = false; cout << "Missed semilcolon ';' at line " + to_string(lineNumber) + " position " + to_string(position) << endl;
									}
								}
								else {
									isValid = false; cout << "Missed 'end' at line " + to_string(lineNumber) + " position " + to_string(position) << endl;
								}
							}
							else {
								isValid = false;  cout << "Missed right bracket '}' at line " + to_string(lineNumber) + " position " + to_string(position) << endl;
							}
						}
						else {
							isValid = false; cout << "Missed equation sign '=' at line " + to_string(lineNumber) + " position " + to_string(position) << endl;
						}
					}
					else {
						isValid = false; cout << "Missed semicolon ';' at line " + to_string(lineNumber) + " position " + to_string(position) << endl;
					}
				}
				else { isValid = false; cout << "Missed left bracket '{' at line " + to_string(lineNumber) + " position " + to_string(position) << endl; }
			}
			else { isValid = false; cout << "Missed equation sign '=' at line " + to_string(lineNumber) + " position " + to_string(position) << endl; }
		}
		else { isValid = false; cout << "Missed TBEGIN at line " + to_string(lineNumber) + " position " + to_string(position) << endl; }

	};

};

int main()
{

	string l, line;
	ifstream myfile;
	myfile.open("input.txt");
	Lex currentToken;
	Analizator analizator;

	if (!myfile.is_open()) {
		perror("Error open");
		exit(EXIT_FAILURE);
	}

	analizator.lineNumber = 1;
	while (getline(myfile, l)) {
		string line = l;

		analizator.position = 0;
		analizator.line = line;
		analizator.isValid = true; 
		analizator.token = analizator.get_token();
		analizator.S(); 
		if (analizator.isValid) { 
			cout << "\n"<< line << " is valid in given grammar.\n" << endl;
		}
		else { 
			cout <<"\n"<< line << " is not valid in given grammar.\n" << endl;
		}
		analizator.lineNumber++;
	}

	return 0;
};
/*
		Лабораторна робота №2(10 + 10 балів)
		Тема: Побудова спрощенного синтаксичного аналізатора
		Завдання

		Для даної граматики  написати аналізатор, що діє методом рекурсивного спуска.

		Створити файл з реченнями, належними і не належними мові граматики.Створений файл є вхідним файлом
		аналізатора.Кожне речення – окремий рядок, у частині речень зробити синтаксичні і лексичні помилки.
		2 речення w довжиною виведення k не менше п’яти(S = > k w) вивести у зошиті.

		Для одержання наступної лексеми речення створити окрему функцію get_token().

		Для речення з помилками передбачити видачу інформації про помилки,
		вказуючи номер рядка, позицію в ньому і суть помилки.

		Окремо протестувати функцію get_token().Для цього вивести у окремий файл лексеми
		у вигляді <номер  рядка, тип, лексема>.Типи лексем – невідома лексема, ідентифікатор,
		ключове слово, константа, символ, тег, знак, тощо.
		Можливі типи лексем для конкретного варіанту та їх приклади написати у зошиті.
		Зауваження: В умовах синім кольором виділені лексери
		(їм відповідають лексеми - регулярні вирази).IDEN, VALUE - також лексери.

		Приклад опису типів лексем і прототипу функції get_token() :

			enum Token // тип лексеми

		{

			ITER1,  // унарна операція *

			ITER2,  // унарна операція +

			RP,       //лексема )

			LP,        //лексема (

			ALT,      //бінарна операція |

			CONCAT,   //бінарна операція .

			OP,       //лексема операнду

			END      //лексема кінця речення

		};

		struct Lex {
			int num;  // номер речення
			Token token; // тип лексеми
			string value; // значення лексеми
		};

		Lex get_token(); // повертає лексему

		Алгоритм для функції main() :
			...
			for (int i = 0; i < 5; i++) {// для кожного речення з вхідного файлу
				do {
					... // друк лексем у файл, друк повідомлень про помилки на  консоль
				} while (get_token().token != END);
			}

		Лабораторну роботу здавати в 2 єтапи: 2a(обов'язково), 2б( не обов'язково):

			2a.Робота з функцією get_token() без синтаксичного аналізу.
			На вхід програми подається файл з реченнями,
			на виході отримується файл лексем, які утворилися з кожного речення
			до першої прочитаної невідомої лексеми.

			У електронному робочому зошиті розмістити умову задачі(вхідну граматику),
			типи лексем, 5 вхідних речень(вхідний файл) і містиме файлу з лексемами.

			2б.Синтаксичний аналіз методом рекурсивного спуску(для кожного нетерміналу
			пишеться окрема функція).На вхід подається файл з вхідними реченнями або
			файл з лексемами.На екран виводяться повідомлення про помилки лексичного і
			синтаксичного аналізу для кожного окремого речення, а також висновки про
			належність речення мові граматики.


				9) S -> begin I = T {; I = T} end;

				T -> T*F | T/F | F

				I -> ab | ba

				C-> 10 | 100

				F -> I | I^C

	*/
