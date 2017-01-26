#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stuinfo
{
	char stuName[10];
	int Age;
} ElemType;

typedef struct node
{
	ElemType data;
	struct node *next;
} ListNode, *ListPtr;

ListPtr CreateList(void)
{
	ListPtr p = (ListPtr)malloc(sizeof(ListNode));
	p->next = NULL;
	return p;
}

void Insert(ListPtr head, char *name, int age)
{
	ListPtr p = (ListPtr)malloc(sizeof(ListNode));
	p->next = NULL;
	strcpy(p->data.stuName, name);
	p->data.Age = age;
	ListPtr temp = head;
	while (temp->next != NULL) {
		temp = temp->next;
	}
	temp->next = p;
	return;
}

void PrintList(ListPtr head)
{
	ListPtr p = head->next;
	while (p != NULL) {
		printf("stuName : %s   Age : %d\n", p->data.stuName, p->data.Age);
		p = p->next;
	}
	return;
}

int main(void)
{
	ListPtr p;
	while (1) {
		printf("1 Create List\n");
		printf("2 Print List\n");
		printf("3 Insert List\n");
		printf("4 Quit List\n");
		char command;
		command = getchar();
		if (command == '\n')
			command = getchar();
		switch(command) {
			case '1' : p = CreateList();
			break;
			case '2' : PrintList(p);
			break;
			case '3' : Insert(p, "newStu", 20);
			break;
			case '4' : 
			return 0;
		}
	}
}