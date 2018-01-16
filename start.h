#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#define INFOLEN 32
#define MILLION 1000000



void failwith(char* msg);

typedef unsigned char* information;
typedef unsigned long fitval;


typedef struct candidate_struct{
	information info; // \0 terminated (needed?)
	fitval fitness; //update when info is updated
} candidate;

void free_cand(candidate* cand);



unsigned char randchar();

void printinfo(candidate* cand);
void print_person(candidate* cand);
void putname(candidate* cand);

fitval fitness(candidate* cand);

candidate* fight(candidate* c1, candidate* c2);

void mutate_cands(candidate** cands);
candidate* init_cand();

candidate* candcopy(candidate* oldcan);
