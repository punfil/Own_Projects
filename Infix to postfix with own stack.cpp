#include <iostream>

template <typename T>
struct stack_struct {
	stack_struct* next;
	T character;
};

template <typename X>
class stack {
	stack_struct <X> whole_stack;
	int size;
public:
	stack() {
		this->size = 0;
		this->whole_stack.next = nullptr;
	}
	bool empty() const {
		return this->size <= 0;
	}
	void pop() {
		if (this->empty()) {
			return;
		}
		void* ptr = this->whole_stack.next;
		this->whole_stack.next = this->whole_stack.next->next;
		free(ptr);
		this->size--;
	}
	void push(char value) {
		stack_struct <X>* ptr = (stack_struct <X>*)malloc(sizeof(stack_struct <X>));
		if (ptr == nullptr) {
			std::cout << "Error allocating memory"; return;
		}
		ptr->character = value;
		ptr->next = this->whole_stack.next;
		this->whole_stack.next = ptr;
		this->size++;
	}
	X top() const {
		return this->whole_stack.next->character;
	}
	
	~stack() {
		while (!this->empty()) {
			this->pop();
		}
	}
};

#define MAX_LINE_SIZE 100

//B is operand on stack, a on input
bool isbHigher(char a, char b) {
	if (a == '+' or a == '-') {
		if (b == '*' || b == '/' || b == '^') {
			return true;
		}
	}
	else if (a == '*' || a == '/'){
		if (b == '^')
			return true;
	}
	return false;
}

int main() {
	//Program zamieniajacy notacje zwykla na ONP
	char wyrazenie_wejsciowe[MAX_LINE_SIZE];
	char delimiter[] = " ";
	std::string wyrazenie_wyjsciowe;
	stack <char>operands;
	fgets(wyrazenie_wejsciowe, MAX_LINE_SIZE, stdin);
	wyrazenie_wejsciowe[strcspn(wyrazenie_wejsciowe, "\n")] = 0;
	char* strtok_buffer = strtok(wyrazenie_wejsciowe, delimiter);
	while (strtok_buffer!=NULL) {
		if (isdigit(strtok_buffer[0])) {
			wyrazenie_wyjsciowe += strtok_buffer;
		}
		else if (*strtok_buffer == '(') {
			operands.push(*strtok_buffer);
		}
		else if (*strtok_buffer == ')') {
			while (operands.top() != '(') {
				wyrazenie_wyjsciowe += operands.top();
				operands.pop();
			}
			operands.pop();
		}
		else {
			while (!operands.empty() && isbHigher(*strtok_buffer, operands.top())) {
				wyrazenie_wyjsciowe += operands.top();
				operands.pop();
			}
			operands.push(*strtok_buffer);
		}
		strtok_buffer = strtok(NULL, delimiter);
	}
	while (!operands.empty()) {
		wyrazenie_wyjsciowe += operands.top();
		operands.pop();
	}
	std::cout << wyrazenie_wyjsciowe;
	return 0;
}
