#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "leak_detector_c.h"

typedef struct{//monster
    int id;
    char *type;
    char *name;
    int population;
} monster;

typedef struct{//region
    char *name;
    int nmonsters;
    int total_population;
    monster **monsters;
} region;

typedef struct{//itinerary
    int nregions;
    region **regions;
    int captures;
} itinerary;

typedef struct{//trainer
    char *name;
    itinerary *visits;
} trainer;

/*---------------Functions-----------------*/

//This function removes the Null character that is at the end of every string.
void remove_crlf(char *s){
    char *t = s + strlen(s);
    t--;

    while ((t >= s) && (*t == '\n' || *t == '\r')){
        *t = '\0';
        t--;
    }
}
/*This function scans the line of the input file then uses the function "remove_crlf" to remove the 
Null character and keeps going until a line doesnt equal '\0' */
void get_next_nonblank_line(FILE *ifp, char *s, int max_length){
    s[0] = '\0';

    while (s[0] == '\0'){
        fgets(s, max_length, ifp);
        remove_crlf(s);
    }
}
/*fill_monsters gets the input from the "read_monsters" function and fills the value 
into the values in structures*/
void fill_monster(monster *m, char *name, char *type, int population){
    m->name = strdup(name);
    m->type = strdup(type);
    m->population = population;
}
/*fill_regions gets the input from the "read_regions" function and fills the value 
into the values in structures*/
void fill_regions(region *r, char *name, int nmonsters){
    r->name = strdup(name);
    r->nmonsters = nmonsters;
    (r->monsters) = malloc(r->nmonsters * sizeof(monster *));
}
/*fill_itinerary gets the input from the "read_itinerry" function and fills the value 
into the values in structures*/
void fill_itinerary(itinerary *it, int captures, int nregions){
    it->captures = captures;
    it->nregions = nregions;
    (it->regions) = malloc(it->nregions * sizeof(region *));
}
/*clear_monster makes the values in "fills" function NULL which is equal to 0 which means they get freed.*/
void clear_monster(monster *m){
    if(m->name != NULL){
        free(m->name);
        m->name = NULL;
    }

    if(m->type != NULL){
        free(m->type);
        m->type = NULL;
    }

    m->population = 0;
}
/*clear_region makes the values in "fills" function NULL which is equal to 0 which means they get freed.*/
void clear_region(region *r){
    if(r->name != NULL){
        free(r->name);
        r->name = NULL;
    }
    if(r->monsters !=NULL){
        free(r->monsters);
        r->monsters = NULL;
    }

    r->nmonsters = 0;
}
/*clear_itinerary makes the values in "fills" function NULL which is equal to 0 which means they get freed.*/
void clear_itinerary(itinerary *it){
    if(it->regions !=NULL){
        free(it->regions);
        it->regions = NULL;
    }
    it->nregions = 0;
    it->captures = 0;
}
/*clear_trainer makes the values in "fills" function NULL which is equal to 0 which means they get freed.*/
void clear_trainer(trainer *t){
    if(t->name != NULL){
        free(t->name);
        t->name = NULL;
    }
    if(t->visits != NULL){
        free(t->visits);
        t->visits = NULL;
    }

}
/*The "new_monster_array" function allocates space for the values in the structures. Which basically makes it 
an array of characters for each value of the structure.*/
monster *new_monster_array(int nmonsters){
    monster *m = calloc(nmonsters,sizeof(monster));// calloc(allocates the memory of # of monsters)

    return m;
}
/*The "array_of_regions" function allocates space for the values in the structures. Which basically makes it 
an array of characters for each value of the structure.*/
region *array_of_regions(int nregions){
    region *r = calloc(nregions,sizeof(region));

    return r;
}
/*The "array_of_itinerary" function allocates space for the values in the structures. Which basically makes it 
an array of characters for each value of the structure.*/
itinerary *array_of_itinerary(int nregions){
    itinerary *it = calloc(nregions,sizeof(itinerary));

    return it;
}
/*The "trainer_array" function allocates space for the values in the structures. Which basically makes it 
an array of characters for each value of the structure.*/
trainer *trainer_array(int ntrainers){
    trainer *t = calloc(ntrainers,sizeof(trainer));

    return t;
}

void dispose_monster_array(monster *m, int nmonsters){
    for(int i = 0; i < nmonsters; i++){
        clear_monster(m + i);
    }
    free(m);
}

void dispose_region_array(region *r,int nregions){
    for(int i = 0; i < nregions; i++){
        clear_region(r + i);
    }
    free(r);
}

void dispose_itinerary(itinerary *it){
    
    for(int i = 0; i < it->nregions; i++){
        clear_itinerary(it + i);
    }

}
/*the "get_number_of_monsters" gets the total number amount of monsters from the input file and it returns it into
the main function.*/
int get_number_of_monsters(FILE *ifp){
    char amount_of_characters_in_line[128];
    int num;

    get_next_nonblank_line(ifp,amount_of_characters_in_line,127);
    sscanf(amount_of_characters_in_line, "%d", &num);

    return num;
}
/*the "get_number_of_monsters" gets the total number amount of regions from the input file and returns it into the 
main function.*/
int get_number_of_regions(FILE *ifp){
    char nbr_of_characters[128];
    int number_of_regions;

    get_next_nonblank_line(ifp,nbr_of_characters,127);
    sscanf(nbr_of_characters,"%d",&number_of_regions);

    return number_of_regions;
}
/*the "get_number_trainers" gets the total number amount of trainers from the input file and returns it into the 
main function.*/
int get_number_trainers(FILE *ifp){
     char different_lines[128];
     char number_of_trainers[128];
     int ntrainer;

     get_next_nonblank_line(ifp,different_lines,127);
     sscanf(different_lines,"%s",number_of_trainers);
     ntrainer = atoi(number_of_trainers);

     return ntrainer;
}
/*The function "read_monsters" gets the three different values from the inpout file. Which are the name of the monsters,
type, and the total amount of monsters in that population*/
void read_monster(FILE *ifp, monster *m){
    char different_lines[128];
    char name[128];
    char type[128];
    char population_string[128];
    int  population;

    get_next_nonblank_line(ifp,different_lines,128);
    sscanf(different_lines,"%s %s %s",name,type,population_string);
    population = atoi(population_string);
    
    fill_monster(m,name,type,population);
}
/*"filltotal_population" this assigns the value totalpopulation that you get from the fuction "read_reagions" and inputs it 
into the integer in regions named "total_population". */
void filltotal_poopulation(region *r, int totalpopulation){
    r->total_population = totalpopulation;
}
/*read_reagions reads certain lines in the input file that get turned into specific values like "name_of_region",and "amount_of_mon". 
This function also calculates the total population in the specific region that the input file is giving us*/
void read_regions(FILE *ifp, region *r,monster *m,int monster_index){
    char different_lines[128];
    char buffering_string[128];
    char name_of_region[128];
    char amount_of_mon_string[128];
    int populationcount;
    int totalpopulation = 0;
    int amount_of_mon;
    int i;
    int j;

    get_next_nonblank_line(ifp,different_lines,127);
    sscanf(different_lines,"%s",name_of_region);

    get_next_nonblank_line(ifp,different_lines,127);
    sscanf(different_lines,"%s",amount_of_mon_string);
    amount_of_mon = atoi(amount_of_mon_string);

    fill_regions(r, name_of_region,amount_of_mon);
    
    for(i = 0;i < amount_of_mon; i++){
        get_next_nonblank_line(ifp,buffering_string,127);
        sscanf(buffering_string,"%s",buffering_string);

        for(j = 0; j < monster_index; j++){
            if(strcmp(buffering_string, m[j].name)== 0){
                (r->monsters[i]) = m + j;
                populationcount = m[j].population;
                totalpopulation += populationcount;
            }
        }
    }
    filltotal_poopulation(r, totalpopulation);
}

/*int math_equation(monster *m,region *r,itinerary *i){
     double amount_of_monsters_given_to_trainers;
    
     amount_of_monsters_given_to_trainers = (m->population/r->total_population) * i->captures;

     return amount_of_monsters_given_to_trainers;   
}*/

/*"read_itinerary" reads didferent lines from the input file and assigns them to values like "amount_of_captures_array",and "amount_of_regions_array". And this function
reads which regions the trainer is going to go to.*/
void read_itinerary(FILE *ifp,itinerary *it,region *r,int region_index){
     char different_lines[128];
     char buffering_string[128];
     char amount_of_captures_array[128];
     char amount_of_regoins_array[128];
     int nregions;
     int ncaptures;
     int i;
     int j;

     get_next_nonblank_line(ifp,different_lines,127);
     sscanf(different_lines,"%s",amount_of_captures_array);
     ncaptures = atoi(amount_of_captures_array);

     get_next_nonblank_line(ifp,different_lines,127);
     sscanf(different_lines,"%s",amount_of_regoins_array);
     nregions = atoi(amount_of_regoins_array);

     fill_itinerary(it,ncaptures,nregions);

     for(i = 0; i < nregions; i++){
        get_next_nonblank_line(ifp,buffering_string,127);
        sscanf(buffering_string,"%s",buffering_string); 

        for(j = 0; j < region_index; j++){
            if(strcmp(buffering_string, r[j].name)== 0){
                (it->regions[i]) =  r + j;
            }    
        }
    }
}

/*the function "fill_trainer" gets the values from "read_trainer" and assigns them to values in the structure trainer.*/
void fill_trainer(FILE *ifp,trainer *t,char *name,region *r, int nregions){
    t->visits = calloc(1, sizeof(itinerary));
    t->name = strdup(name);
    read_itinerary(ifp, t->visits,r, nregions);
}

void dispose_trainers(trainer *t, int ntrainers){
    dispose_itinerary(t->visits);
    for(int i = 0; i < ntrainers; i++){
        clear_trainer(t + i);
    }
    free(t);
}
/*This function gets the name of the trainer from the input file and assigns it the vaule to "trainer_name"*/
void read_trainer(FILE *ifp,trainer *t,region *r, int nregions){
     char different_lines[128];
     char trainer_name[128];

     get_next_nonblank_line(ifp,different_lines,127);
     sscanf(different_lines,"%s",trainer_name);

     fill_trainer(ifp,t,trainer_name,r,nregions);
}

/* This code is from me testing out the vaules that we got from the input files.
void print_monster_into_output(FILE *ofp, monster *m){
    fprintf(ofp, "Monster: %s Type: %s Population: %d\n",
            m->name, m->type, m->population);    
}

void print_region_into_output(FILE *ofp, region *r){
    int i;

    fprintf(ofp,"%s \n%d amount of monsters\n",
            r->name,r->nmonsters);

    for(i = 0;i < r->nmonsters; i++){
        fprintf(ofp,"%s\n", (*(r->monsters + i))->name);
    }
}

void print_trainers(FILE *ofp,trainer *t){
    fprintf(ofp,"%s\n",
            t->name);
}

void print_itinerary(FILE *ofp, itinerary *it){
    int i;

    fprintf(ofp,"%d captures and %d regions\n",
            it->captures,it->nregions);
    for(i = 0; i < it->nregions; i++){
        fprintf(ofp,"%s\n",(*(it->regions + i))->name);
    }
}*/

/*The function "printfinaldata" basically prints out all the information that we got from the input file and it also does the calculation 
for the amount of monsters that the trainers catch.*/
void print_to_outputfile(FILE *ofp, trainer *t, int ntrainer){
    int i;
    int j;
    int k;
    double total_number_of_cmonsters;

    /*This is a nested for loop which will print out the trainer names,the region names, and the amount of captures per monsters.*/
    for(i=0; i < ntrainer; i++){ //This for loop will print all the names of the trainers 
        fprintf(ofp, "%s\n\n",(t+i)->name);

        for(j=0; j < (t+i)->visits->nregions;j++){
            fprintf(ofp,"%s\n",(t+i)->visits->regions[j]->name);
            for(k=0;k < (t+i)->visits->regions[j]->nmonsters;k++){
                total_number_of_cmonsters =(((double)(t+i)->visits->regions[j]->monsters[k]->population/(double)(t+i)->visits->regions[j]->total_population)* (double)(t+i)->visits->captures);
                if(total_number_of_cmonsters > 0.5){
                fprintf(ofp,"%.0f %s\n",round(total_number_of_cmonsters), (t+i)->visits->regions[j]->monsters[k]->name);
                }
            }
            fprintf(ofp,"\n");
        }
    }
}

/*----------------------------------------*/
int main(void){
    atexit(report_mem_leak);//this function is for memeory leak.
    FILE *ifp;
    FILE *ofp;
    int nmonsters;
    int nregions;
    int ntrainers;
    monster *monsters;
    region *regions;
    itinerary *trainer_itinerary;
    trainer *trainers;
    int i;

    
    ifp = fopen("input.txt", "r");//This opens the input file  
    ofp = fopen("output.txt", "w");//This opens the output file

    nmonsters = get_number_of_monsters(ifp);//gets the total amount of monsters 
    monsters = new_monster_array(nmonsters);//makes an array named monsters from the total amount of monsters 

    for (i = 0; i < nmonsters; i++){/*This reads "read_monster" as many times the total nmonsters since we made
    an array called monsters with the number of nmonsters*/
        read_monster(ifp, monsters + i);
    }

    nregions = get_number_of_regions(ifp);//This gets the total amount of regions 
    regions = array_of_regions(nregions);//makes an array named regions from the total amount of regions
    

    for (i = 0; i < nregions; i++){/*This reads "read_regions" as many times the total nregions since we made 
    an array called regions with the number of nregions*/
        read_regions(ifp, regions + i,monsters,nmonsters);
    }

    ntrainers = get_number_trainers(ifp);//This gets the total amount of trainers 
    trainers = trainer_array(ntrainers);//makes an array named trainers from the total amount of trainers

    for(i = 0; i < ntrainers; i++){ /*This read "read_trainer" as many times the total ntrainers since we made
    an array called regions with the number of ntrainers*/
        read_trainer(ifp,trainers + i,regions,nregions);
    }

    print_to_outputfile(ofp,trainers,ntrainers);/*This calls this function and transfers two different vaules which 
    are trainers and ntrainers*/


    /*These three fuction will dispose the normal values and the pointers that we made inside the main function*/
    dispose_monster_array(monsters, nmonsters);
    dispose_region_array(regions,nregions);
    dispose_trainers(trainers,ntrainers);


    /*This will close the two files the we opened which are the "input.txt" and "output.txt".*/
    fclose(ifp);
    fclose(ofp);
}