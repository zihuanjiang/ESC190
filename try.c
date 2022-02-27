#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Menu {
	int num_items;
	double* item_cost_per_unit;
} Menu;

Menu* create_menu(int num_items,double* item_cost_per_unit){
    Menu* result = malloc(sizeof(Menu));
    result->num_items = num_items;
    result->item_cost_per_unit = item_cost_per_unit;
    return result;
}

void free_menu(Menu** menu){
    Menu* menu2 = *menu;
    free(menu2->item_cost_per_unit);
    free(menu2);
}

int main(){
    int num_items = 2;
    double* item_cost_per_unit = malloc(sizeof(double)*2);
    item_cost_per_unit[0] = 0.02;
    item_cost_per_unit[1] = 0.01;
    Menu* menu = create_menu(num_items,item_cost_per_unit);
    free_menu(&menu);
}