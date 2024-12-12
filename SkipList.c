#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SKIP_LIST_P 0.5
struct node{
    int val;
    int level;
    struct node* next;
    struct node* down;
}typedef Node;

int randLevel(){ //return the level of a node
    int num = 1;
    srand((unsigned int)time(NULL));
    while((double)rand() / RAND_MAX < SKIP_LIST_P){ //SKIP_LIST_P is 0.5, the left side generates a number in (0,1) randomly
        num++;
    }
    return num;
}

//list[] is an array to store the skip list, and each element list[i] represents a level of the skip list
Node* Search(Node* list[], int val, int level){   //level represents the valid number of levels in list[]
    Node* pre = list[level]->next;
    Node* cur = pre->next;
    while(1){
        if(pre->level == 1){ //When the list comes to level 1
            while(pre->val != val){ 
                pre = pre->next;
                if(pre->val > val){ //When we cannot find the exact value
                    return NULL;
                }
            }
            return pre; //Find the exact value
        }
        if(pre->val == val){ //A lucky situation
            while(pre->level != 1){
                pre = pre->down;
            }
            return pre;
        }
        else if(cur == NULL || pre->val < val && cur->val > val){ //go down
            pre = pre->down;
            cur = pre->next;
        }
        else{ //forward
            cur = cur->next;
            pre = pre->next;
        }
    }

}

void Insert(Node* list[], int val, int level) {
    int newNodeLevel = randLevel();

    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->val = val;
    newNode->level = newNodeLevel;
    newNode->next = NULL;
    newNode->down = NULL;


    for (int i = 1; i <= newNodeLevel; i++) {
        newNode->down = list[i]; 
        list[i] = newNode;
    }

    for (int i = newNodeLevel; i >= 1; i--) {
        Node* pre = list[i];
        Node* cur = pre->next;
        while (cur != NULL && cur->val < val) {
            pre = cur;
            cur = cur->next;
        }
        newNode->next = cur; 
        pre->next = newNode;
    }
}


void Delete(Node* list[], int val, int level) {
    Node** update = (Node**)malloc(sizeof(Node*) * level);
    for (int i = 1; i <= level; i++) {
        update[i] = list[i];
    }

    for (int i = level ; i >= 1; i--) {
        Node* pre = update[i];
        Node* cur = pre->next;
        while (cur != NULL && cur->val < val) {
            pre = cur;
            cur = cur->next;
        }

        if (cur != NULL && cur->val == val) {
            pre->next = cur->next;
            update[i] = pre;
        } else {
            update[i] = NULL;
        }
    }

    for (int i = 1; i <= level; i++) {
        if (update[i] != NULL) {
            Node* toDelete = update[i]->next;
            update[i]->next = toDelete->next; 
            free(toDelete); 
        }
    }

    free(update);
}

