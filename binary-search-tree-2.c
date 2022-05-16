/*
 * Binary Search Tree #2
 *
 * Data Structures
 *
 * Department of Computer Science
 * at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>



typedef struct node {
	int key;
	struct node *left;
	struct node *right;
} Node;

/* for stack */
#define MAX_STACK_SIZE		20
Node* stack[MAX_STACK_SIZE];			//사용할 스택 생성
int top = -1;

Node* pop();
void push(Node* aNode);

/* for queue */
#define MAX_QUEUE_SIZE		20
Node* queue[MAX_QUEUE_SIZE];			//사용할 큐 생성
int front = -1;
int rear = -1;

Node* deQueue();
void enQueue(Node* aNode);


int initializeBST(Node** h);

/* functions that you have to implement */
void recursiveInorder(Node* ptr);	  /* recursive inorder traversal */
void iterativeInorder(Node* ptr);     /* iterative inorder traversal */
void levelOrder(Node* ptr);	          /* level order traversal */
int insert(Node* head, int key);      /* insert a node to the tree */
int deleteNode(Node* head, int key);  /* delete the node for the key */
int freeBST(Node* head); /* free all memories allocated to the tree */

/* you may add your own defined functions if necessary */


void printStack();



int main()
{
    printf("[-----[Seo han bit]   [2019038043]-----]\n");
    
	char command;
	int key;
	Node* head = NULL;

	do{
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Binary Search Tree #2                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize BST       = z                                       \n");
		printf(" Insert Node          = i      Delete Node                  = d \n");
		printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
		printf(" Level Order (Queue)  = l      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initializeBST(&head);
			break;
		case 'q': case 'Q':
			freeBST(head);
			break;
		case 'i': case 'I':
			printf("Your Key = ");
			scanf("%d", &key);
			insert(head, key);
			break;
		case 'd': case 'D':
			printf("Your Key = ");
			scanf("%d", &key);
			deleteNode(head, key);
			break;

		case 'r': case 'R':
			recursiveInorder(head->left);
			break;
		case 't': case 'T':
			iterativeInorder(head->left);
			break;

		case 'l': case 'L':
			levelOrder(head->left);
			break;

		case 'p': case 'P':
			printStack();
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initializeBST(Node** h) {

	/* if the tree is not empty, then remove all allocated nodes from the tree*/
	if(*h != NULL)
		freeBST(*h);

	/* create a head node */
	*h = (Node*)malloc(sizeof(Node));		//head에 메모리 할당
	(*h)->left = NULL;	/* root */			//head의 left를 NULL로 초기화
	(*h)->right = *h;						//head의 right를 자기 자신을 가리키게 함(사용 X)
	(*h)->key = -9999;

	top = -1;								//top을 -1로 초기화 함

	front = rear = -1;						//front와 rear를 -1로 초기화 함

	return 1;
}



void recursiveInorder(Node* ptr)			//recursive 방식으로 중위 순회
{
	if(ptr) {
		recursiveInorder(ptr->left);
		printf(" [%d] ", ptr->key);
		recursiveInorder(ptr->right);
	}
}

/**
 * textbook: p 224
 */
void iterativeInorder(Node* node)			//iterative 방식으로 중위 순회
{
	for(;;)									//무한 반복문
	{
		for(; node; node = node->left)		//node의 가장 왼쪽까지 전부 push해서 스택에 삽입 node가 NULL이 되면 for루프 종료
			push(node);						
		node = pop();						//전부 스택에 삽입하고 난 뒤에 하나를 pop 해서 node에 초기화 (가장 왼쪽 node였던 값)

		if(!node) break;					//node가 NULL이면 break문을 통해 for문 탈출
		printf(" [%d] ", node->key);		//pop해서 node에 넣었던 node의 key 값을 출력

		node = node->right;					//node를 node의 right로 바꿈
	}
}

/**
 * textbook: p 225
 */
void levelOrder(Node* ptr)					//레벨순서 순회 (같은 레벨을 방문해서 순회 하는 방식) root 방문 -> 왼쪽 자식 방문 -> 오른쪽 자식 방문
{
	// int front = rear = -1;

	if(!ptr) return; /* empty tree */

	enQueue(ptr);							//ptr을 큐에 저장

	for(;;)
	{
		ptr = deQueue();					//큐에 저장된 제일 앞에 값을 하나 ptr에 받음
		if(ptr) {							//ptr이 NULL이 아니면
			printf(" [%d] ", ptr->key);		//ptr의 key값 출력

			if(ptr->left)					//ptr의 left가 NULL이 아니면
				enQueue(ptr->left);			//ptr의 left를 큐에 저장
			if(ptr->right)					//ptr의 right가 NULL이 아니면
				enQueue(ptr->right);		//ptr의 right를 큐에 저장
		}
		else								//ptr이 NULL이면 break
			break;

	}
}


int insert(Node* head, int key)
{
	Node* newNode = (Node*)malloc(sizeof(Node));							//새로운 node를 하나 만들고 메모리 할당
	newNode->key = key;														//사용자로 부터 받아온 key 값을 새로운 node의 key값에 저장
	newNode->left = NULL;													//새로운 node의 left와 right를 모두 NULL로 초기화
	newNode->right = NULL;

	if (head->left == NULL) {												//head의 left가 NULL이면 즉 아직 아무 노드도 없다면
		head->left = newNode;												//새로운 node가 첫 node가 되게 함
		return 1;	
	}

	/* head->left is the root */
	Node* ptr = head->left;													//ptr이 첫 node가 되게 함

	Node* parentNode = NULL;												//parentNode를 NULL로 초기화
	while(ptr != NULL) {													//ptr이 NULL이 될때까지 반복

		
		if(ptr->key == key) return 1;										//ptr의 key값과 사용자로부터 받아온 key값이 같으면 함수 종료

		parentNode = ptr;													//parentNode가 ptr이 되게 함

		if(ptr->key < key)													//ptr의 key값이 받아온 key 값보다 작을 경우
			ptr = ptr->right;												//ptr이 ptr->right가 되게함
		else																//ptr의 key값이 받아온 key 값보다 클 경우
			ptr = ptr->left;												//ptr이 ptr->left가 되게 함
	}

	if(parentNode->key > key)												//parentNode의 key값이 받아온 key값보다 크다면
		parentNode->left = newNode;											//parentNode의 left에 새로운 node가 오게 함
	else																	//parentNode의 key값이 받아온 key값보다 작다면
		parentNode->right = newNode;										//parentNode의 rignt에 새로운 node가 오게 함
	return 1;
}


int deleteNode(Node* head, int key)											
{
	if (head == NULL) {														//head가 NULL인 경우 삭제할게 없다는 내용 출력
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	if (head->left == NULL) {												//head의 left가 NULL인 경우 아무 node도 없다는 말이므로 삭제할게 없다는 내용 출력
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	/* head->left is the root */
	Node* root = head->left;												//root가 head의 left가 되게함



	Node* parent = NULL;													//parent는 NULL로 초기화
	Node* ptr = root;														//ptr이 root가 되게함

	while((ptr != NULL)&&(ptr->key != key)) {								//ptr이 NULL이되거나 ptr의 key값이 받아온 key값과 같을때까지 반복
		if(ptr->key != key) {												//ptr의 key값이 받아온 key값과 다르면

			parent = ptr;													//parent가 ptr이 되게함

			if(ptr->key > key)												//ptr의 key값이 받아온 key값보다 크면
				ptr = ptr->left;											//ptr이 ptr의 left가 되게 함
			else															//ptr의 key값이 받아온 key값보다 작으면
				ptr = ptr->right;											//ptr이 ptr의 right가 되게 함
		}
	}

	if(ptr == NULL)															//ptr이 NULL이면
	{
		printf("No node for key [%d]\n ", key);								//찾는 key값을 가진 node가 없다는 내용 출력
		return -1;
	}

	if(ptr->left == NULL && ptr->right == NULL)								//ptr의 left가 NULL이고 ptr의 right가 NULL이면
	{
		if(parent != NULL) {												//parent가 NULL이 아니면
			if(parent->left == ptr)											//parent의 left가 ptr이 되게함
				parent->left = NULL;										//parent의 left를 NULL이 되게함
			else															//아니면
				parent->right = NULL;										//parent의 right를 NULL로 초기화 함
		} else {															//parent가 NULL이면
			head->left = NULL;												//head의 left를 NULL로 초기화

		}

		free(ptr);															//ptr을 메모리 해제
		return 1;
	}

	if ((ptr->left == NULL || ptr->right == NULL))							//ptr의 left가 NULL이거나 ptr의 right가 NULL이면
	{
		Node* child;														//child 객체 선언
		if (ptr->left != NULL)												//ptr의 left가 NULL이 아니면 
			child = ptr->left;												//child를 ptr의 left가 되게함
		else																//left가 NULL이면
			child = ptr->right;												//child가 ptr의 right가 되게함

		if(parent != NULL)													//parent가 NULL이 아니면
		{
			if(parent->left == ptr)											//parent의 left가 ptr이면
				parent->left = child;										//parent의 left가 child가 되게함
			else															//ptr이 아니면
				parent->right = child;										//parent의 right가 child가 되게함
		} 
		else {																//parent가 NULL이면
			root = child;													//root가 child가 되게함
		}

		free(ptr);															//ptr 메모리 해제
		return 1;
	}

	Node* candidate;														//candidate 객체 선언
	parent = ptr;															//parent가 ptr이 되게 함


	candidate = ptr->right;													//candidate가 ptr의 right가 되게함

	while(candidate->left != NULL)											//candidate의 left가 NULL이 될때까지
	{
		parent = candidate;													//parent가 candidate가 되게 하고
		candidate = candidate->left;										//candidate가 candidate의 left가 되게함
	}

	
	if (parent->right == candidate)											//parent의 right가 candidate랑 같다면
		parent->right = candidate->right;									//parent의 right가 candidate의 right가 되게함
	else																	//다르다면
		parent->left = candidate->right;									//parent의 left가 candidate의 right가 되게함

	ptr->key = candidate->key;												//ptr의 key가 candidate의 key가 되게함

	free(candidate);														//candidate 메모리 해제
	return 1;
}


void freeNode(Node* ptr)
{
	if(ptr) {
		freeNode(ptr->left);												//ptr->left를 인자로 하는 freeNode함수 호출
		freeNode(ptr->right);												//ptr->right를 인자로 하는 freeNode함수 호출
		free(ptr);															//마지막으로 root인 ptr을 메모리 해제
	}
}

int freeBST(Node* head)
{

	if(head->left == head)													//아무노드도 없다면
	{
		free(head);															//head만 메모리 해제
		return 1;
	}

	Node* p = head->left;													//p를 head의 left가 가리키는 곳이 되게 함

	freeNode(p);															//freeNode 함수 호출

	free(head);																//head외의 node들이 다 해제 되고 나면 head도 메모리 해제
	return 1;
}



Node* pop()
{
	if (top < 0) return NULL;												//top이 0보다 작으면 스택안에 아무것도 없으므로 NULL값 리턴
	return stack[top--];													//0보다 크면 stack에서 가장 위에 값 꺼냄
}

void push(Node* aNode)										
{
	stack[++top] = aNode;													//node 하나를 스택에 넣음
}

void printStack()
{
	int i = 0;
	printf("--- stack ---\n");
	while(i <= top)
	{
		printf("stack[%d] = %d\n", i, stack[i]->key);						//스택에 쌓인 값들을 하나씩 출력
	}
}

Node* deQueue()
{
	if (front == rear) {													//front랑 rear랑 같으면 큐 안이 비어있다는 내용 출력하고 NULL값 리턴
		printf("\n....Now Queue is empty!!\n" );
		return NULL;
	}

	front = (front + 1) % MAX_QUEUE_SIZE;									//front를 front+1 한걸 MAX_QUEUE_SIZE로 나눈 나머지 값으로 초기화
	return queue[front];													//queue[front] 리턴
	
}

void enQueue(Node* aNode)
{
	rear = (rear + 1) % MAX_QUEUE_SIZE;										//rear를 rear+1 한걸 MAX_QUEUE_SIZE로 나눈 나머지 값으로 초기화
	if (front == rear) {
		printf("\n....Now Queue is full!!\n");								//front와 rear의 값이 같으면 큐가 꽉찼다는 내용 출력
		return;
	}

	queue[rear] = aNode;													//큐에 node값 넣어주기
}