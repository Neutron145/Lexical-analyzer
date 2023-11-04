#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>

#include "declarations.h"
#include "structs.h"

using namespace std;

//Вывод списка лексем и ошибки в файл
void print(lexem* head, error* err, ofstream& fout) {
	//Вывод лексем
	lexem* p = head->next;
	while (p) {
		if (p->word) fout << p->word << "[" << lexType[p->type] << "] ";	
		p = p->next;
	}
	fout << '\n';
	//Вывод ошибки
	error* e = err->next;
	while (e) {
		fout << e->pos << ' ';
		lexem* p = e->errorList->next;
		while (p) {
			fout << lexType[p->type] << ' ';
			p = p->next;
		}
		fout << '\n';
		e = e->next;
	}
	if (err->next == nullptr) fout << "OK\n";
}

//Проверка типа символа 
signals check(char c) {
	if (c >= '0' && c <= '9') return Digits;
	else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '&' || c == '|') return Alpha;
	else if (c == '+' || c == '-' || c == '/' || c == '*') return Arithmetic;
	else if (c == '<') return CompLess;
	else if (c == '>') return CompMore;
	else if (c == '=') return Equal;
	else if (c == ' ' || c == '\r' || c == '\n' || c == '\t' || c == 0) return Separator;
	else if (c == ';') return Special;
	else if (c == '(') return BracketL;
	else if (c == ')') return BracketR;
	else return Other;
}

//Определние типа лексемы
LexType checkLexType(enum lexStates state, char* word) {
	switch (state)
	{
	case lA:
		if (strcmp(word, "if") == 0) return lIf;
		else if (strcmp(word, "{") == 0) return lCBl;
		else if (strcmp(word, "else") == 0) return lElse;
		else if (strcmp(word, "}") == 0) return lCBr;
		else if (strcmp(word, "&&") == 0 || strcmp(word, "||") == 0) return lLo;
		else if (strcmp(word, "!") == 0) return lNo;
		if (strlen(word) < 6) return lId;
		else return lWl;
		break;
	case lB:
		if (atoi(word) <= 32767 && strlen(word) < 6) return lVl;
		else return lWl;
		break;
	case lC:
		return lAo;
		break;
	case lD:
		return lEq;
		break;
	case lG:
		return lCo;
		break;
	case lJ:
		return lSc;
		break;
	case lH:
		return lCo;
		break;
	case lI:
		return lBl;
		break;
	case lK:
		return lBr;
		break;
	default:
		return lWl;
		break;
	}
}

//Определение типа лексемы
LexType checkLexType(int i) {
	switch (i)
	{
	case 0:
		return lId;
		break;
	case 1:
		return lNo;
		break;
	case 2:
		return lVl;
		break;
	case 3:
		return lCo;
		break;
	case 4:
		return lAo;
		break;
	case 5:
		return lEq;
		break;
	case 6:
		return lLo;
		break;
	case 7:
		return lIf;
		break;
	case 8:
		return lCBl;
		break;
	case 9:
		return lElse;
		break;
	case 10:
		return lCBr;
		break;
	case 11:
		return lSc;
		break;
	case 12:
		return lWl;
		break;
	default:
		return lWl;
		break;
	}
}

//Лексический анализатор
lexem* lexicalAnalyzer(char* text) {
	//Список, в котором хранятся все лексемы
	lexem* head = new lexem();
	//wordBeg указывает на начало лексемы
	char* wordBeg = text;
	//Начальное и предыдущее состояние автомата
	enum lexStates curState = lS, lastState;
	do {
		lastState = curState;
		curState = lexTable[curState][check(*text)];

		//Запись лексемы в список
		if (curState == lS && lastState != lS) {
			char* word = new char[text - wordBeg + 1];
			strncpy(word, wordBeg, text - wordBeg);
			word[text - wordBeg] = '\0';
			lexem* lex = new lexem(checkLexType(lastState, word), word);
			pushback(head, lex);
		}
		else text++;
		if (curState == lS) wordBeg = text;
	} while (*(text - 1));

	return head;
}

//Добавление новой ошибки в список ошибок
error* pushError(int pos, syntStates state) {
	//Добавляем позицию, на которой произошла ошибка
	error* err = new error(pos, new lexem());
	//Добавляем типы лексем, которые ожидались после лексемы на позиции pos
	for (int i = 0; i < 15; i++) {
		if (syntTable[state][i] != sE) {
			lexem* lex = new lexem();
			lex->type = checkLexType(i);
			pushback(err->errorList, lex);
		}
	}
	return err;
}

//Синтаксический анализатор
error* syntaxAnalyzer(lexem* lexems) {
	
	//Костыль, необходимый для корректной обработки последней лексемы
	pushback(lexems, new lexem());

	//Спискок ошибок
	error* Error = new error();
	Error->errorList = new lexem();

	//Текущее и прошлое состояние автомата
	enum syntStates curState = sS, lastState = sS;

	//Лексема и позиция лексемы в тексте
	lexem* p = lexems->next;
	int pos = 0;

	//Необходим для остановки анализа лексем при нахождении ошибки
	bool isError = false;

	//Стек для магазинного автомата
	Stack magazine;
	int a = 1;

	while (p && !isError) {
		//Переход по матрице состояний
		lastState = curState;
		curState = syntTable[curState][p->type];
		switch (p->type)
		{
		case lBl: //Записываем в стек 1, если встретилась (
			magazine << 1;
			break;
		case lBr: //Если ) отсутвует - ошибка
			if (magazine >> a != 1) { curState = sE; }
			break;
		case lCBl: //Записываем в стек 2, если встретилась {
			magazine << 2;
			break;
		case lCBr: //Если отсутсвует } - ошибка
			if (magazine >> a != 2) { curState = sE; }
			break;
		default:
			break;
		}

		//Определение ошибки и запись этой ошибки в список ошибок
		if ((curState == sE || p->next == nullptr) && (lastState != sS && lastState != sL)) {
			error* err = pushError(pos, lastState);
			if (Error->errorList->next) {
				curState = syntTable[lastState][Error->errorList->next->type];
			}
			pushback(Error, err);
			isError = true;
		}
		p = p->next;
		pos++;
	}
	return Error;
}

void spath(char** & filename) {
	filename[0] = new char[2];
	filename[1] = new char[2];
}


int main() {
	double* d = new double[4];
	for (double* p = d; p < &d[4]; p++) *p = 2;
	for (double* p = d; p < &d[4];) cout << *p++ << " ";
	//---------------Чтение данных из файла---------------
	ifstream fin("input.txt", ios::binary);
	size_t size = 0;
	fin.seekg(0, ios::end);
	size = fin.tellg();
	fin.seekg(0, ios::beg);
	char* text = new char[size + 1]; //Строка в которую будут считываться данные из файла
	fin.getline(text, size + 1, '\0');
	fin.close();


	//---------------Анализ текста---------------
	lexem* lexems = lexicalAnalyzer(text);
	error* Error = syntaxAnalyzer(lexems);


	//---------------Запись обработанных данных---------------
	ofstream fout("output.txt", ios::binary);
	print(lexems, Error, fout);
	fout.close();
	
	/*
	int mem = 0;
	lexem* p = lexems;
	cout << sizeof(*p->next) << p->next->word <<  '\n';
	while (p->next) {
		mem += sizeof(*p);
		p = p->next;
	}
	cout << mem;*/

	//---------------Удаление списков---------------
	pop(lexems);
	pop(Error);
	delete[] text;
	return 0;
}