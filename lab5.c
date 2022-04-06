#include "lab5.h"

Vnode* find_node(Graph* gr, char* station){
    int i = 0;
    Vnode* find_node;               // parameter to find the node
    Vnode* node = NULL;             // store the node
    while (i < gr->count){
        find_node = gr->adj_list[i];
        if (strcmp(station,find_node->station) == 0){
            node = find_node;
            return node;
        }
        i++;
    }
    return NULL;
}

void initializing_graph(Graph* gr){
    Vnode* station_node;
    int i = 0;
    while(i < gr->count){
        gr->adj_list[i]->cost = 0;
        gr->adj_list[i]->visited = 0;
        gr->adj_list[i]->prev = NULL;
        i++;
    }
}

void setting_values(Graph* gr, Vnode* start, char* final_dest){
    // getting the edge series
    Enode* edge_series = start->edges;

    // it is the final node
    if (edge_series == NULL){ return; }

    // it is not the final node
    if (edge_series != NULL){
        // loop all the edge
        while (edge_series != NULL){
            // update the value of the cost in the dest
            // find the dest
            Vnode* dest;
            dest = find_node(gr,edge_series->vertex);
            //update the value
            int new_cost;
            new_cost = start->cost + edge_series->weight;
            // not arrived
            if (dest->visited == 0){
                dest->cost = new_cost;
                dest->visited = 1;
                dest->prev = start;
            }
            // arrived
            if (dest->visited != 0){
                if (dest->cost > new_cost){
                    dest->cost = new_cost;
                    dest->prev = start;
                }
            }
            // recursion if the dest is not the final dest
            if (strcmp(dest->station,final_dest)!=0){
                setting_values(gr,dest,final_dest);
            }
            edge_series = edge_series->next;
        }
    }
}





char **plan_route(Graph *gr, char *start, char *dest){
    // find the start and dest node
    Vnode* start_node;
    Vnode* dest_node;
    start_node = find_node(gr,start);
    dest_node = find_node(gr,dest);
    if (start_node == NULL){return NULL;}
    if (dest_node == NULL){return NULL;}

    // initialize graph
    initializing_graph(gr);

    // setting values
    setting_values(gr,start_node,dest);

    // if the rountine does not exist
    if (dest_node->visited == 0){return NULL; }

    // getting trail
    int i = 0;
    char** my_routine = malloc(sizeof(char*)*1);

    while (strcmp(start,dest_node->station)!=0){
        my_routine[i] = dest_node->station;
        dest_node = dest_node->prev;
        i++;
        my_routine = realloc(my_routine,sizeof(char*)* (i+1));
    }
    my_routine[i] = start_node->station;
    return my_routine;
}

void add(Graph *gr, char *station){
    // checking if the station already exists
    Vnode* my_node;
    my_node = find_node(gr,station);
    if (my_node != NULL){return;}
    
    // if not exist: initialize
    Vnode* add_node = malloc(sizeof(Vnode));
    strcpy(add_node->station,station);
    add_node->cost = 0;
    add_node->visited = 0;
    add_node->edges = NULL;
    add_node->prev = NULL;

    int count = gr->count;
    Vnode** adj_nodes = gr->adj_list;
    adj_nodes = realloc(adj_nodes,sizeof(Vnode*)*(count+1));
    adj_nodes[count] = add_node;
    gr->adj_list = adj_nodes;
    gr->count = count +1;
}


Enode* create_new_edge(char* dest,int weight){
    Enode* my_new_edge = malloc(sizeof(Enode));
    strcpy(my_new_edge->vertex,dest);
    my_new_edge->weight = weight;
    my_new_edge->next = NULL;
    return my_new_edge;
}

void update(Graph *gr, char *start, char *dest, int weight){
    // weight != 0 case
    if (weight != 0){
        // find the star and dest station
        Vnode* find_start = NULL;       // store the start node
        Vnode* find_dest = NULL;        // store the dest node
        find_dest = find_node(gr,dest);
        find_start = find_node(gr,start);

        // create start and dest if not find
        if (find_start == NULL){
            add(gr,start);
            find_start = find_node(gr,start);
        }
        if (find_dest == NULL){
            add(gr,dest);
        }

        // update the edge
        Enode* find_edge;
        find_edge = find_start->edges;
        // case where the edge is not empty
        if (find_start->edges != NULL){
            // for node before the last one
            while (find_edge->next != NULL){
                if (strcmp(find_edge->vertex, dest) == 0){
                    find_edge->weight = weight;
                    return;
                }
                find_edge = find_edge->next;
            }
            // for the last one
            if (strcmp(find_edge->vertex, dest) == 0){
                find_edge->weight = weight;
                return;
            }
        
            // the edge does not exist
            Enode* new_edge;
            new_edge = create_new_edge(dest,weight);
            find_edge->next = new_edge;
        }

        // empty case
        if (find_start->edges == NULL){
            Enode* new_edge;
            new_edge = create_new_edge(dest,weight);
            find_start->edges = new_edge;    
        }
    }

    // weight == 0 case
    if (weight == 0){
        Vnode* find_start;
        find_start = find_node(gr,start);

        // case where start node does not exist, the empty edge set
        if (find_start == NULL){return;}
        if (find_start->edges == NULL){return;}

        // go over the edges
        Enode* go_over;
        go_over = find_start->edges;
        Enode* free_this;
        
        if (strcmp(find_start->edges->vertex,dest)==0){
            free_this = find_start->edges;
            find_start->edges = find_start->edges->next;
            free(free_this);

        }

        while (go_over->next != NULL){
            if (strcmp(go_over->next->vertex,dest)==0){
                free_this = go_over->next;
                go_over->next = go_over->next->next;
                free(free_this);
            }
            else{
                go_over = go_over->next;
            }
        }
    }
}

void disrupt(Graph *gr, char *station){
    for (int i = 0; i < gr->count; i++){
        update(gr,gr->adj_list[i]->station,station,0);
        update(gr,station,gr->adj_list[i]->station,0);
    }
    int hit = 0;
    for (int j = 0; j < gr->count;j++){
        if (hit != 0){
            gr->adj_list[j-1] = gr->adj_list[j];
        }

        if (strcmp(gr->adj_list[j]->station,station)==0){
            hit = 1;
        }
    }
    gr->adj_list = realloc(gr->adj_list,sizeof(Vnode*)*(gr->count-1));
    gr->count--;
}