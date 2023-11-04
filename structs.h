#pragma once

//Стек для магазина
class Stack {
private:
	struct Node {
		int data;
		Node* next;
	} *node;
public:
	Stack() {
		node = new Node;
		node->data = 0;
		node->next = nullptr;
	};
	~Stack() {
		int a;
		while (node->next) {
			(*this) >> a;
		}
		delete node;
	}

	Stack& operator<<(const int data) {
		Node* newNode = new Node();
		newNode->data = data;
		Node* p = node;
		while (p->next) {
			p = p->next;
		}
		p->next = newNode;
		return (*this);
	}
	int operator>>(int& data) {
		Node* p = node;
		if (p->next) {
			while (p->next->next) p = p->next;
			data = p->next->data;
			delete p->next;
			p->next = nullptr;
			return data;
		}
		else { 
			data = -1;
			return -1;
		}
	}

	int size() {
		Node* p = node->next;
		int size = 0;
		while (p) {
			size++;
			p = p->next;
		}
		return size;
	}
};

//Список для лексем
struct lexem {
	LexType type;
	char* word;
	lexem* next;
	lexem(LexType t = lWl, char* w = nullptr) {
		type = t;
		word = w;
		next = nullptr;
	}
};


//Список для ошибок
struct error {
	int pos;
	lexem* errorList;
	error* next;
	error(int p = 0, lexem* lex = nullptr) {
		pos = p;
		errorList = lex;
		next = nullptr;
	}
};

//Добавление нового элемента в конец списка лексем
lexem* pushback(lexem*& head, lexem* newLex) {
	lexem* p = head;
	while (p->next) p = p->next;
	p->next = newLex;
	return p;
}

//Добавление нового элемента в конец списка ошибок
error* pushback(error*& head, error* newErr) {
	error* p = head;
	while (p->next) p = p->next;
	p->next = newErr;
	return p;
}

//Удаление списка лексем
void pop(lexem* head) {
	if (head) {
		while (head->next) {
			lexem* l = head->next;
			head->next = l->next;
			delete l;
		}
		delete head;
	}
}

//Удаление списка ошибок
void pop(error* head) {
	if (head) {
		while (head->next) {
			error* l = head->next;
			pop(l->errorList);
			head->next = l->next;
			delete l;
		}
		delete head;
	}
}