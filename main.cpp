#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable : 4996)

struct Node {
    int value;
    Node* prev;
    Node* next;
};

class Number {
    int sign;
    long length;
    Node* head;
    Node* tail;

public:
    Number() {
        sign = '+';
        length = 0;
        head = new Node;
        tail = new Node;
        head->value = 'h';
        head->prev = nullptr;
        head->next = tail;
        tail->value = 't';
        tail->prev = head;
        tail->next = nullptr;
    }
    ~Number() {
        Node* tmp = head;
        while (tmp != nullptr) {
            tmp = head->next;
            delete head;
            head = tmp;
        }
        // free(&head);
        // free(&tail);
    }
    int getSign() {
        return sign;
    }
    long getLenght() {
        return length;
    }
    Node* getHead() {
        return head;
    }
    Node* getTail() {
        return tail;
    }
    void setSign(char s) {
        sign = s;
    }
    void addFirst(int v) {
        Node* tmpHeadNext = head->next;
        Node* newNode = new Node;
        newNode->value = v;
        newNode->prev = head;
        newNode->next = tmpHeadNext;
        head->next = newNode;
        tmpHeadNext->prev = newNode;
    }
    void addLast(int v) {
        Node* tmpTailPrev = tail->prev;
        Node* newNode = new Node;
        newNode->value = v;
        newNode->next = tail;
        newNode->prev = tmpTailPrev;
        tail->prev = newNode;
        tmpTailPrev->next = newNode;
    }
    void printNumber() {
        Node* tmpTail = tail->prev;
        if (sign == '-') printf("-");
        while (tmpTail->prev != nullptr) {
            printf("%d", tmpTail->value);
            tmpTail = tmpTail->prev;
        }
        printf("\n");
    }
    void countElements() {
        Node* tmpHead = head->next;
        long counter = 0;
        while (tmpHead->next != nullptr) {
            counter++;
            tmpHead = tmpHead->next;
        }
        length = counter;
    }
    void removeZeros() {
        Node* tmpTail = tail->prev;
        while (tmpTail->prev != nullptr) {
            if (tmpTail->value != 0) return;
            else {
                tmpTail->next->prev = tmpTail->prev;
                tmpTail = tmpTail->prev;
                free(tmpTail->next);
                tmpTail->next = tail;
            }
        }
    }
};

void printAllNumbers(Number** numbers, int amountOfNumbers);
Number* cmpSignMin(Number* a, Number* b);
Number* cmpSignMax(Number* a, Number* b);
Number* cmpLengthMin(Number* a, Number* b);
Number* cmpLengthMax(Number* a, Number* b);
Number* cmpValuesMin(Number* a, Number* b);
Number* cmpValuesMax(Number* a, Number* b);
void quickSort(Number** numbers, int start, int end);
int partition(Number** numbers, int start, int end);
Number* compareNumbers(Number* n1, Number* n2);

int main(void) {

    int amountOfNumbers;
    scanf("%d ", &amountOfNumbers);
    Number** numbers = new Number*[amountOfNumbers];
    for (int i = 0; i < amountOfNumbers; i++) {
        char c;
        Number* n = new Number();
        while ((c = getchar()) != '\n') {
            if (c == '-') {
                n->setSign(c);
                continue;
            }
            else if (c == '+') continue;
            n->addFirst(atoi(&c));
        }
        // n->removeZeros();
        n->countElements();
        numbers[i] = n;
        // if (checkIfZero(n)) printf("ZERO\n");
    }
    quickSort(numbers, 0, amountOfNumbers - 1);
    // printf("OUTPUT\n");
    printAllNumbers(numbers, amountOfNumbers);

    for (int i = 0; i < amountOfNumbers; i++) {
        delete numbers[i];
    }
    
    delete[] numbers;

    return 0;
}

/*
Quicksort - implementacja
def QuickSort(A, p, r):
if (p < r) :
q = Partition(A, p, r)
QuickSort (A, p, q)
QuickSort (A, q+1, r)
QuickSort(Tablica, 0, N-1)
*/

void quickSort(Number** numbers, int start, int end) {
    if (start < end) {
        int p = partition(numbers, start, end);
        quickSort(numbers, start, p - 1);
        quickSort(numbers, p + 1, end);
    }
}

int partition(Number** numbers, int start, int end) {
    Number* pivot = numbers[end];
    int i = start;
    for (int j = start; j <= end-1; j++) {
        if (numbers[j] != compareNumbers(numbers[j], numbers[end])) {
            Number* tmp = numbers[j];
            numbers[j] = numbers[i];
            numbers[i] = tmp;
            i++;
        }
    }
    Number* tmp = numbers[i];
    numbers[i] = numbers[end];
    numbers[end] = tmp;
    // printAllNumbers(numbers, 8);
    // printf("\n");
    return i;
}

Number* compareNumbers(Number* n1, Number* n2) {
    Number* max = n1;
        if (cmpSignMax(max, n2) == nullptr) {
            if (max->getSign() == '+') {
                if (cmpLengthMax(max, n2) == nullptr) {
                    max = cmpValuesMax(max, n2);
                }
                else max = cmpLengthMax(max, n2);
            }
            else {
                if (cmpLengthMin(max, n2) == nullptr) {
                    max = cmpValuesMin(max, n2);
                }
                else max = cmpLengthMin(max, n2);
            }
        }
        else max = cmpSignMax(max, n2);
    return max;
}

void printAllNumbers(Number** numbers, int amountOfNumbers) {
    for (int i = 0; i < amountOfNumbers; i++) {
        numbers[i]->printNumber();
    }
}

Number* cmpSignMin(Number* a, Number* b) {
    if (a->getSign() > b->getSign()) return a;
    else if (a->getSign() < b->getSign()) return b;
    else return nullptr;
}

Number* cmpSignMax(Number* a, Number* b) {
    if (a->getSign() < b->getSign()) return a;
    else if (a->getSign() > b->getSign()) return b;
    else return nullptr;
}

Number* cmpLengthMin(Number* a, Number* b) {
    if (a->getLenght() < b->getLenght()) return a;
    else if (a->getLenght() > b->getLenght()) return b;
    else return nullptr;
}

Number* cmpLengthMax(Number* a, Number* b) {
    if (a->getLenght() > b->getLenght()) return a;
    else if (a->getLenght() < b->getLenght()) return b;
    else return nullptr;
}

Number* cmpValuesMin(Number* a, Number* b) {
    Node* tmpA = a->getTail()->prev;
    Node* tmpB = b->getTail()->prev;

    while (true) {
        if (tmpA->prev == nullptr && tmpB->prev == nullptr) return a;
        else if (tmpA->value > tmpB->value) return b;
        else if (tmpA->value < tmpB->value) return a;
        tmpA = tmpA->prev;
        tmpB = tmpB->prev;
    }
}

Number* cmpValuesMax(Number* a, Number* b) {
    Node* tmpA = a->getTail()->prev;
    Node* tmpB = b->getTail()->prev;

    while (true) {
        if (tmpA->prev == nullptr && tmpB->prev == nullptr) return a;
        else if (tmpA->value < tmpB->value) return b;
        else if (tmpA->value > tmpB->value) return a;
        tmpA = tmpA->prev;
        tmpB = tmpB->prev;
    }
}
