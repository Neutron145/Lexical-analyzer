#pragma once

//ѕеречисление состо€ний дл€ лексического анализатора
enum lexStates { lS, lA, lB, lC, lD, lG, lH, lJ, lI, lK, lE };

//ѕеречисление состо€ний дл€ синтаксического анализатора
enum syntStates { sS, sA, sB, sC, sD, sF, sG, sH, sI, sJ, sK, sL, sE };

//ѕеречисление алвафита 
enum signals { Alpha, Digits, Arithmetic, CompLess, CompMore, Equal, Separator, 
				Special, BracketL, BracketR, CurvyBrackerR, CurvyBracketL, Other };

//ѕеречисление типов лексем и представление типов в виде строк
/*
	lId - идентификатор
	lNo - логическое не (!)
	lVl - значение
	lCo - оператор сравнени€
	lAo - арифметический оператор
	lEq - оператор присваивани€/сравнени€
	lLo - логический оператор (and/or)
	lIf - if		lElse - else
	lCBl - {		lCBr - }
	lSc - ;
	lBl - (			lBr - )
	lWl - неверна€ лексема
*/
enum LexType { lId, lNo, lVl, lCo, lAo, lEq, lLo,
				lIf, lCBl, lElse, lCBr, lSc, lBl, lBr, lWl };

const char* lexType[15] = { "id", "no", "vl", "co", "ao", "eq", 
						"lo", "if", "cb", "el", "cb", "sc", "br", "br", "wl" };

//ћатрица состо€ний лексического анализатора
lexStates lexTable[11][11] = {
	//   a    d  +-   <   >   =       ;  (   )   wl
		{lA, lB, lC, lG, lG, lD, lS, lJ, lI, lK, lE}, //S 
		{lA, lA, lS, lS, lS, lS, lS, lS, lS, lS, lE}, //A 
		{lE, lB, lS, lS, lS, lS, lS, lS, lS, lS, lE}, //B 
		{lS, lS, lS, lS, lS, lS, lS, lS, lS, lS, lS}, //C 
		{lS, lS, lS, lS, lS, lS, lS, lS, lS, lS, lS}, //D 
		{lS, lS, lS, lS, lH, lS, lS, lS, lS, lS, lS}, //G 
		{lS, lS, lS, lS, lS, lS, lS, lS, lS, lS, lS}, //H 
		{lS, lS, lS, lS, lS, lS, lS, lS, lS, lS, lS}, //J 
		{lS, lS, lS, lS, lS, lS, lS, lS, lS, lS, lS}, //I
		{lS, lS, lS, lS, lS, lS, lS, lS, lS, lS, lS}, //K
		{lE, lE, lS, lS, lS, lS, lS, lS, lE, lS, lS}, //E 
};

//ћатрица состо€ний синтаксического анализатора
syntStates syntTable[13][15] = {
	//lId lNo lVl lCo lAo lEq lLo lIf lCBl lEl lCBr lSc lBl lBr lWl
	  {sE, sE, sE, sE, sE, sE, sE, sA, sE, sE, sE, sE, sE, sE, sE}, //S
	  {sE, sE, sE, sE, sE, sE, sE, sE, sE, sE, sE, sE, sB, sE, sE}, //A
	  {sC, sD, sC, sE, sE, sE, sE, sE, sE, sE, sE, sE, sE, sE, sE}, //B
	  {sE, sE, sE, sD, sB, sD, sB, sE, sE, sE, sE, sE, sE, sE, sE}, //C
	  {sF, sE, sF, sE, sE, sE, sE, sE, sE, sE, sE, sE, sE, sE, sE}, //D
	  {sE, sE, sE, sE, sD, sE, sB, sE, sE, sE, sE, sE, sE, sG, sE}, //F
	  {sE, sE, sE, sE, sE, sH, sE, sE, sH, sE, sE, sE, sE, sE, sE}, //G
	  {sI, sE, sE, sE, sE, sE, sE, sA, sE, sE, sL, sE, sE, sE, sE}, //H
	  {sE, sE, sE, sE, sH, sJ, sE, sE, sE, sE, sE, sE, sE, sE, sE}, //I
	  {sK, sE, sK, sE, sE, sE, sE, sE, sE, sE, sE, sH, sE, sE, sE}, //J
	  {sE, sE, sE, sE, sJ, sE, sE, sE, sE, sE, sE, sH, sE, sE, sE}, //K 
	  {sE, sE, sE, sE, sE, sE, sE, sA, sE, sG, sL, sE, sE, sE, sE}, //L
	  {sE, sE, sE, sE, sE, sE, sE, sE, sE, sE, sE, sE, sE, sE, sE}, //E
};