#include <iostream>  //добавлено стирание backspace-ом , обновлены правила ввода
#include <conio.h>
#include <Windows.h>
using namespace std;
const int enter = 13;
const int escape = 27;
const int space = 32;
const int backspace = 8;
const int sleepTime = 1500;
const char alphabet[] = { '^', '*', '/', '+', '-' };

bool isoperator(char symbol) {
	for (int i = 0; i < 5; i++) {
		if (symbol == alphabet[i]) return true;
	}
		return false;
}

void enterStringHandler(char *enterS, short &top) {
	short T = 1;
	while (T) {
		char ch = _getch();
		if (ch == escape) exit(0);
		if (ch == enter) break;
		if (isdigit(ch) or isoperator(ch) or ch == space or ch == '(' or ch == ')') {
			cout << ch;
			top++;
			enterS[top] = ch;
		}
		if (ch == backspace) {
			if (top > -1) {
				cout << "\b \b";
				enterS[top] = 0;
				top--;
			}
		}
	}
}

bool inputIsCorrect(char *enterS, short top) {
	unsigned short openbr = 0, closebr = 0;
	if (isoperator(enterS[0]) or isoperator(enterS[top])) return false;
	for (int i = 0; i < top; i++) {
		if (isoperator(enterS[i]) and (isoperator(enterS[i + 1]) or isoperator(enterS[i + 2]) or enterS[i + 1] == ')' or isdigit(enterS[i + 1]))) return false;
		if (enterS[i] == space and enterS[i + 1] == space) return false;
		if (enterS[i] == '(' and (enterS[i + 1] == space or isoperator(enterS[i + 1]))) return false;
	}
	for (int i = 0; i < top - 1; i++) {
		if (isdigit(enterS[i]) and (isoperator(enterS[i + 1]) or (enterS[i + 1] == space and isdigit(enterS[i + 2])))) return false;
	}
	for (int i = 0; i <= top; i++) {
		if (enterS[i] == '(') openbr++;
		if (enterS[i] == ')') closebr++;
	}
	if (openbr != closebr) return false;
	return true;
}

class dArray {
	char *arr;
	unsigned short capacity;
public:
	dArray() {
		arr = NULL;
		capacity = 0;
	}

	~dArray() {
		free(arr);
	}

	void add(char symbol) {
		capacity++;
		arr = (char *)realloc(arr, capacity * sizeof(char));
		arr[capacity - 1] = symbol;
	}

	char get(unsigned short index) {
		if (index < capacity) {
			return arr[index];
		}
		else return '~';
	}

	void delElement(unsigned short index) {
		if (index < capacity) {
			for (int j = index; j < capacity - 1; j++) {
				arr[j] = arr[j + 1];
			}
			capacity--;
			arr = (char *)realloc(arr, capacity * sizeof(char));
		}
	}

	void replace(unsigned short index, char symbol) {
		if (index < capacity) {
			arr[index] = symbol;
		}
	}

	void delAll() {
		free(arr);
		arr = NULL;
		capacity = 0;
	}

	unsigned short getCapacity() {
		return capacity;
	}

};

struct mData {
	int data;
	mData *next, *prev;
};

class deque {
	mData *head, *tail;
	unsigned short size;
public:
	deque() {
		head = NULL;
		tail = NULL;
		size = 0;
	}
	~deque() {
		mData *del = head;
		while (del != NULL) {
			head = head->next;
			delete del;
			del = head;
		}
		head = NULL;
		tail = NULL;
	};

	void pushFront(int data) {
		if (head != NULL) {
			mData *a = new mData;
			a->data = data;
			a->next = head;
			a->prev = NULL;
			head->prev = a;
			head = a;
		}
		else {
			mData *a = new mData;
			head = a;
			tail = a;
			head->data = data;
			head->next = NULL;
			head->prev = NULL;
		}
	}

	int popFront() {
		if (head != NULL) {
			int buffer = head->data;
			mData *del = head;
			head = head->next;
			head->prev = NULL;
			delete del;
			size--;
			return buffer;
		}
		else {
			return 0;
		}
	}

	void pushBack(int data) {
		if (tail != NULL) {
			mData *a = new mData;
			a->data = data;
			a->prev = tail;
			tail->next = a;
			tail = a;
			tail->next = NULL;
		}
		else {
			mData *a = new mData;
			tail = a;
			head = a;
			tail->data = data;
			tail->next = NULL;
			tail->prev = NULL;
		}
	}

	int popBack() {
		if (tail != NULL) {
			int buffer = tail->data;
			mData *del = tail;
			tail = tail->prev;
			tail->next = NULL;
			delete del;
			return buffer;
		}
		else {
			return 0;
		}
	}

	bool isEmpty() {
		if (head == NULL) {
			return true;
		}
		else return false;
	}

	void clear() {
		mData *del = head;
		while (del != NULL) {
			head = head->next;
			delete del;
			del = head;
		}
		head = NULL;
		tail = NULL;
	}
};

short priority(char symbol) {
	switch (symbol) {
	case '^': return 3; break;
	case '*':
	case '/': return 2; break;
	case '+':
	case '-': return 1; break;
	default: break;
	}
	return 0;
}

void stringOutput(dArray &stack, char *enterS, short &stacktop, short stringtop) { 

	for (int i = 0; i <= stringtop; i++) {
		if (enterS[i])
		if (isdigit(enterS[i])) cout << enterS[i];
		if (enterS[i] == '(') {
			stack.add(enterS[i]);
			stacktop++;
		}
		if (enterS[i] == ')') {
			cout << " ";
			while (stack.get(stacktop) != '(') {
				cout << stack.get(stacktop);
				stack.delElement(stacktop);
				stacktop--;
			}
			stack.delElement(stacktop);
			stacktop--;
		}
		if (isoperator(enterS[i])) {
			while ((priority(stack.get(stacktop)) >= priority(enterS[i]))) {
				cout << stack.get(stacktop);
				stack.delElement(stacktop);
				stacktop--;
			}
			stack.add(enterS[i]);
			stacktop++;
		}
		if (enterS[i] == space) cout << enterS[i];
	}
	while (stacktop != -1) {
		cout << " " << stack.get(stacktop);
		stack.delElement(stacktop);
		stacktop--;
	}
}

int main() {
	dArray stack;
	short stacktop = -1, enterStringTop = -1;
	char enterString[75] = {0};

	while (true) {
		stacktop = -1; enterStringTop = -1;
		for (int i = 0; i < 75; i++) {
			enterString[i] = 0;
		}
		system("cls");
		cout << "Input instructions - all numbers and operators must be separated by a space," << endl;
		cout << "except for '(' and ')': you must not put a space after '(' and before ')';" << endl;
		cout << "input must not start with a space; input must not end with a space; " << endl;
		cout << "input must not contain two or more spaces in  a row" << endl;
		cout << "Example: 13 + 4 * 232 / (1 - 5) ^ 2" << endl;
		cout << endl;
		cout << "Enter infix expression (escape - exit, enter - input, backspace - erase) " << endl;
		enterStringHandler(enterString, enterStringTop);
		if (inputIsCorrect(enterString, enterStringTop)) {
			cout << "\n\nPostfix form:" << endl;
			stringOutput(stack, enterString, stacktop, enterStringTop);
			cout << endl;
			system("pause");
		}
		else {
			cout << "\n\nIncorrect input, try again";
			Sleep(sleepTime);
		}
	}
	return 0;
}