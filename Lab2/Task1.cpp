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
	TERMINAL, // begin, end
	MULT, //*
	DIV, // /
	POW, // ^
	WORD, // ab, ba
	LETTER, //a, b
	NUMBER, //10, 100
	LBR, // {
	RBR, // }
	SMCLN, // ;   
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
		string result = " < " + to_string(num) + ", " + value + ", " ;
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

	int i=0, j=0;

public:
	
	string line="";
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
		 
		if( position < line.size() ) c = line.at(position); 
		
		else return { lineNumber, "end line" , END };

		if (position < line.size() - 1 && c == ' ') {
			lexema = { lineNumber, "space" , SPACE }; 
			++position;
			return lexema;
		}

		else if (position > line.size() -1 ||c=='\n') {
			lexema = { lineNumber, "end line" , END}; 
			if (file.is_open()) file << lexema.getInfo() << endl;
			++position;
			return lexema;
		} 

		if (c == 'b') {
			if (line.at(position + 4)) {
				string word; 

				word = line.substr(position, 5);
				if (word == "begin") {

					lexema = Lex{ lineNumber, word, TERMINAL }; 
					if (file.is_open()) file << lexema.getInfo() << endl;

					position+=5;
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
			for (i = 1; line.at(position + i) && !flag; ++i) {
				if (!isalpha(line.at(position + i))) {
					flag = true;
				}
			} 
			
			return Lex{ lineNumber,  line.substr(position++, i-1), UNKNOWN };
		}

		if (c == 'e') {
			if (line.at(position + 2)) {
				string word; 

				word = line.substr(position, 3);
				if (word == "end") {

					lexema = Lex{ lineNumber, word, TERMINAL }; 
					if (file.is_open()) file << lexema.getInfo() << endl;

					position+=3;
					return lexema;
				}
			}
			++position;
			return Lex{ lineNumber, string{c}, UNKNOWN };
		}

		if (c == ';') {

			if (line.at(position - 1)) {
				if (line.at(position - 1) == '{' || line.at(position - 1) == 'd') {

						lexema = Lex{ lineNumber,  string{c}, SMCLN }; 
						if (file.is_open()) file << lexema.getInfo() << endl;

						++position;
						return lexema;
					}
					 
			}
			++position;
			return Lex{ lineNumber,  string{c}, UNKNOWN };
		}

		if (c == '{') { 
			skipSpaces();
			if (line.at(position - j + 1 ) && line.at(position + i - 1)) {
				if ((line.at(position - j + 1) == 'a' || line.at(position - j + 1) == 'b' || line.at(position - j + 1) == '0')
					&& line.at(position + i - 1 )==';') {

					lexema = Lex{ lineNumber,  string{c}, LBR }; 
					if (file.is_open()) file << lexema.getInfo() << endl;

					++position;
					return lexema;
				}
			}
			++position;
			return Lex{ position,  string{c}, UNKNOWN };
		}

		if (c == '}') {

			int i, j;

			int flag=false;
			for (i = 1; line.at(position + i)&&!flag; ++i) {
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
					&& line.at(position + i - 1 ) == 'e') {  
					lexema = Lex{ lineNumber,  string{c}, RBR }; 
					if (file.is_open()) file << lexema.getInfo() << endl;

					position++;
					return lexema;
				}
			}
			++position;
			return Lex{ lineNumber,  string{c}, UNKNOWN };
		}

		if (c == '^' || c=='*' ||c=='/' || c=='=') {
			skipSpaces();
			if (line.at(position - j + 1) && line.at(position + 1)) {

				if ((line.at(position +i-1) == 'a' || line.at(position +i-1) == 'b' || line.at(position +i-1) == '1') &&
					(line.at(position - j + 1) == 'a' || line.at(position - j + 1) == 'b' || line.at(position - j + 1) == '0')) {
					

					lexema = Lex{ lineNumber,  string{c}, (c == '^')?POW: (c == '*')? MULT: (c == '/')?DIV:EQ }; 
					if (file.is_open()) file << lexema.getInfo() << endl;

					++position;
					return lexema;
				}
			}
			++position;
			return Lex{ lineNumber,  string{c}, UNKNOWN };
		}
		if (c == 'a') {
			if (line.at(position + 1)) {

				if (line.at(position + 1) == 'b') {

					lexema = Lex{ lineNumber, "ab", Token::WORD }; 
					if (file.is_open()) file << lexema.getInfo() << endl;

					position+=2;
					return lexema;
				}
			}
			++position;
			return Lex{ lineNumber,  string{c}, UNKNOWN };
		}
	
		if (c == '1') {
			if (line.at(position + 1)) {
				if (line.at(position + 2)) {
					if (line.at(position + 1) == '0' && line.at(position + 2) == '0') {
						lexema = Lex{ lineNumber, "100", Token::NUMBER }; 
						if (file.is_open()) file << lexema.getInfo() << endl;

						position += 3;
						return lexema;
					}
				}
					if (line.at(position + 1) == '0') {

						lexema = Lex{ lineNumber, "10", Token::NUMBER }; 
						if (file.is_open()) file << lexema.getInfo() << endl;

						position += 2;
						return lexema;
					}
				}
				++position;
				return Lex{ lineNumber,  string{c}, UNKNOWN };
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

			do {
				currentToken = analizator.get_token(); 
			} while (currentToken.token != END && currentToken.token != UNKNOWN && currentToken.num<line.size()-1);
				  
			if (currentToken.token == END || currentToken.num == line.size() - 1)
				cout << line << " is valid in given grammar." << endl; 

			 if (currentToken.token == UNKNOWN) {
				analizator.file.close();
				cout << "Unknown lexema '" + currentToken.value + "' at line " + to_string(analizator.lineNumber ) + " at position " + to_string(analizator.position);
				cout << "  " << line << " is not valid in given grammar." << endl;
			}
			 analizator.lineNumber++;
	}
		 
	return 0;
}
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
