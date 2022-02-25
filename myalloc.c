// Caleb Knight
// CS 344
// Project 8
// Programming Exercise in C that uses previous projects 
// to allocate, free and coalesce memory

// gcc -Wall -Wextra -Wno-deprecated-declarations -o myalloc myalloc.c
// This ^ is what I used to compile in command line

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define ALIGNMENT 16   // Must be power of 2
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x) - 1) & (ALIGNMENT - 1)))
#define PADDED_SIZE(x) ((x) + GET_PAD(x))
#define PTR_OFFSET(p, offset) ((void*)((char *)(p) + (offset)))


struct block *head = NULL;

struct block {
	struct block *next;
    int size;     // Bytes
    int in_use;   // Boolean
};

// Split_Space(current_node, requested_size):
    // If current_node big enough to split:
        // Add a new struct block with the remaining unused space
        // Wire it into the linked list


void split_space(struct block *n, int bytes)
{
	struct block *new_node = PTR_OFFSET(n, PADDED_SIZE(bytes) + PADDED_SIZE(sizeof(struct block))); // create new node
	new_node -> size = n -> size - (PADDED_SIZE(bytes) + (PADDED_SIZE(sizeof(struct block)))); // declares start of new block
	n -> size = PADDED_SIZE(bytes); // sets up next
	new_node -> in_use = 0; // declare not in use
	new_node -> next = n -> next; // set value of next to NULL
	n -> next = new_node; // make the next node new
}

void *myalloc(int bytes)
{
	void *h = sbrk(1024);

	size_t padded_block_size = PADDED_SIZE(sizeof(struct block));

	if(head == NULL) {
		head = h;
		head -> next = NULL;
		head -> size = 1024 - padded_block_size;
		head -> in_use = 0;
	}
	struct block *n = head;
	int required_space = PADDED_SIZE(bytes) + PADDED_SIZE(sizeof(struct block)) + 16;

		//find_space(size);
	while(n != NULL) {
		if(n -> size >= bytes && n -> in_use == 0){
			if (n -> size >= required_space){
				split_space(n, bytes);

				n -> in_use = 1;
				return PTR_OFFSET(n, PADDED_SIZE(sizeof(struct block)));
			}
		}
		n = n -> next;
	}
	return NULL;
}


// Need to implement this, was getting implicit error
// void find_space(int bytes) 
// {
// 	bytes = bytes + GET_PAD(bytes);
// 	while(n != NULL){
// 		if(n -> size >= bytes && n -> in_use == 0){
// 			split_space(n, bytes);
// 			n -> in_use = 1;
// 			return PTR_OFFSET(n, PADDED_SIZE(sizeof(struct block)));
// 		}
// 		n = n -> next;
// 	}
// 	return NULL;
// }

void myfree(void *p) 
{
	struct block *b = PTR_OFFSET(p, -PADDED_SIZE(sizeof(struct block)));
	b->in_use = 0;
	struct block *curr = head;
	while (curr->next != NULL) { // loop until end of list
		if(curr->in_use == 0 && curr->next->in_use == 0) { // if the current and the next are NOT in use
			curr->size += curr->next->size + PADDED_SIZE(sizeof(struct block));
			curr->next = curr->next->next;
		}
		else{
			curr = curr->next;
		}
	}
	// struct block *h = head;
	// if ((int)p == (int)(h - (GET_PAD(sizeof(struct block))) + 1)){
	// 	h -> in_use = 0;
	// } h = h -> next;		
}


void print_data(void)
{
	struct block *b = head;

	if (b == NULL) {
		printf("[empty]\n");
		return;
	}

	while (b != NULL) {
        // Uncomment the following line if you want to see the pointer values
        // printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
		printf("[%d,%s]", b->size, b->in_use? "used": "free");
		if (b -> next != NULL) {
			printf(" -> ");
		}

		b = b -> next;
	}

	printf("\n");
}

int main(void) 
{


	void *p, *q, *r, *s;

p = myalloc(10); print_data();
q = myalloc(20); print_data();
r = myalloc(30); print_data();
s = myalloc(40); print_data();

myfree(q); print_data();
myfree(p); print_data();
myfree(s); print_data();
myfree(r); print_data();


// void *p, *q;

// p = myalloc(10); print_data();
// q = myalloc(20); print_data();

// myfree(q); print_data();
// myfree(p); print_data();

// void *p, *q;

// p = myalloc(10); print_data();
// q = myalloc(20); print_data();

// myfree(p); print_data();
// myfree(q); print_data();
// void *p;

// p = myalloc(10); print_data();

// myfree(p); print_data();

	// void *p;

	// myalloc(10);     print_data();
	// p = myalloc(20); print_data();
	// myalloc(30);     print_data();
	// myfree(p);       print_data();
	// myalloc(40);     print_data();
	// myalloc(10);     print_data();

 //     void *p;

 //    p = myalloc(512);
 //    print_data();

 //    myfree(p);
 //    print_data();

    // myalloc(10); print_data();
    // myalloc(20); print_data();
    // myalloc(30); print_data();
    // myalloc(40); print_data();
    // myalloc(50); print_data();
	// void *p;

	// print_data();
	// p = myalloc(64);
	// print_data();

	// void *p;

	// print_data();
	// p = myalloc(16);
	// print_data();
	// p = myalloc(16);
	// printf("%p\n", p);
}