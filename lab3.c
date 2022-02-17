#include "lab3.h"
#include <string.h>

//Add TA party item request to the list
int add_request(struct party_node **head, char *item, double price, char *ta){
    if (strcmp(item,"IDE") == 0) return -1;
    struct party_node *new_node = malloc(sizeof(struct party_node));
    new_node->item = malloc(sizeof(char)*(strlen(item)+1));
    new_node->ta = malloc(sizeof(char)*(strlen(ta)+1));
    strcpy(new_node->item,item);
    strcpy(new_node->ta,ta);
    new_node->price = price;
    new_node->next = *head;
    *head = new_node;
    return 0;
}

//Remove the last item added
void remove_request(struct party_node **head){
    struct party_node* temp = *head;
    struct party_node* next_node = temp->next;
    free(temp->item);
    free(temp->ta);
    free(temp);
    *head = next_node;
}

//Sort party item requests - in place?
void make_sorted(struct party_node **head){
    struct party_node* comparing = NULL;
    int switch_index = 1;

    while (switch_index != 0){
        comparing = *head;
        switch_index = 0;
        while (comparing->next != NULL){
            if (comparing->price < comparing->next->price){
                // remember the value
                char* temp_item = comparing->item;
	            double temp_price = comparing->price;
                char* temp_ta = comparing->ta;
                // switching the value 
                comparing->item = comparing->next->item;
                comparing->price = comparing->next->price;
                comparing->ta = comparing->next->ta;
                comparing->next->item = temp_item;
                comparing->next->price = temp_price;
                comparing->next->ta = temp_ta;
                switch_index++;
            }
            comparing = comparing->next;
        }
    }
}


//Trim list to fit the budget
double finalize_list(struct party_node **head, double budget){
    struct party_node* keep = *head;
    struct party_node* last_one = *head;
    double remain_budget = budget;

    while(keep != NULL){
        if (remain_budget > keep->price){
            remain_budget -= keep->price;
            last_one = keep;
            keep = keep->next;
        }
        else{
            if (keep == *head){
                *head = keep->next;
                last_one = *head;
                free(keep->item);
                free(keep->ta);
                free(keep);
                keep = *head;
            }
            else{
                last_one->next = keep->next;
                free(keep->item);
                free(keep->ta);
                free(keep);
                keep = keep->next;
            }
        }
    }
    return remain_budget;
}



//Print the current list - hope this is helpful!
void print_list(struct party_node *head){
    int count = 1;
    printf("The current list contains:\n");
    while(head!=NULL){
        printf("Item %d: %s, %.2lf, requested by %s\n",
            count, head->item, head->price, head->ta);
        count++;
        head = head->next;
    }
    printf("\n\n");
    return;
}