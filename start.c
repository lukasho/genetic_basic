#include "start.h"

int n; //number of candidates per generation
int g; //number of generations
int p; //chance a given contender will point mutate each generation / 1 million
int c; //chance two contenders cross in a given generation / 1 million

pthread_mutex_t pauser;
candidate* noob;

int fightpipe[2];

void failwith(char* msg){
	fprintf(stderr, "Error: %s. errno %d\n", msg, errno);
	exit(0);
}

unsigned char randchar(){
	return rand() % 256;
}

unsigned char randletter(){
	unsigned int roll = rand() % 52;
	unsigned int base = (roll/26 > 0)?97:65;
	unsigned char ret = base + roll%26;
	return ret;
}

void printinfo(candidate* cand){
	information info = cand->info;
	int i;
	for(i = 0; i < INFOLEN; i++){
		printf("%02x ", info[i]); //print hex v
	}
	printf("\t%lu\n", cand->fitness);
}

candidate* init_cand(){
	int i;
	candidate* ret = malloc(sizeof(candidate));
	ret->info = malloc(sizeof(unsigned char) * (INFOLEN + 1));

	for(i = 0; i < INFOLEN; i++)
		ret->info[i] = randchar();//i<7?randletter():randchar();

	ret->fitness = fitness(ret);
	//printinfo(ret);
	return ret;
}

candidate* candcopy(candidate* oldcan){
	int i = 0;
	candidate* newcand = malloc(sizeof(candidate));
	newcand->info = malloc(sizeof(unsigned char) * (INFOLEN + 1));

	for(i = 0; i < INFOLEN; i++)
		newcand->info[i] = oldcan->info[i];

	newcand->fitness = oldcan->fitness;
	return newcand;
}

void mutate_cands(candidate** cands){
	int i, j;
	int roll;
	int cross; //cross everything after this index
	char tmp; //for swap

	//point mutation
	for(i = 0; i < n; i++){
		roll = rand()%MILLION;
		if(roll < p){
			for(j = 0; j < INFOLEN; j++){
				roll = rand()%INFOLEN;
				if(roll < 1) 
					cands[i]->info[j] = randchar();//(j<7?randletter():randchar());
			}
		}
	}

	//cross mutation
	for(i = 0; i + 1 < n; i+=2){
		roll = rand()%MILLION;
		if(roll < c){
			cross = rand()%(INFOLEN - 1);
			for(j = cross + 1; j < INFOLEN; j++){
				tmp = cands[i]->info[j];
				cands[i]->info[j] = cands[i+1]->info[j];
				cands[i+1]->info[j] = tmp;
			}
		}
	}
	for(i = 0; i < n; i++)
		cands[i]->fitness = fitness(cands[i]);
}

void update_candidates(candidate*** cands){
	int curr_index, i;
	candidate** newcands;

	fitval runningf;
	fitval totalf;
	fitval picknum;

	newcands = malloc(sizeof(candidate) * n); //array of new candidates
	totalf = 0;

	for(i = 0; i < n; i++)
		totalf+=(*cands)[i]->fitness;

	//pick new candidates
	for(i = 0; i < n; i++){
		curr_index = -1;
		runningf = 0;
		picknum = rand() % totalf;
		while(runningf<=picknum){
			curr_index++;
			runningf+=(*cands)[curr_index]->fitness;
		}
		newcands[i] = candcopy((*cands)[curr_index]);
	}
	//free old array
	for(i = 0; i < n; i++){
		free((*cands)[i]->info);
		free((*cands)[i]);
	}
	free(*cands);
	*cands = newcands;
}

candidate* run(){

	int i, j, done = 0;

	candidate** cands; 
	candidate* best_candidate;
	fitval max_fitness;

	cands = malloc(sizeof(candidate*) * n); //array of current candidates
	
	for(i = 0; i < n; i++)
		cands[i] = init_cand();


	
	//g generations
	for(i = 0; i < g; i++){
		
		update_candidates(&cands);
		mutate_cands(cands);

		//loading bar
		done++;
		if(done > g/80){
			fprintf(stderr, "|");
			done = 0;
		}

		pthread_mutex_unlock(&pauser);
	}
	fprintf(stderr, "\n\n");
	
	for(i = 0; i < n; i++)
		printinfo(cands[i]);

	max_fitness = 0;
	for(i = 0; i < n; i++){
		if(cands[i]->fitness > max_fitness) {
			best_candidate = cands[i];
			max_fitness = cands[i]->fitness;
		}
	}
	best_candidate = candcopy(best_candidate);
	
	//cleanup
	for(j = 0; j < n; j++){
		free(cands[j]->info);
		free(cands[j]);
	}
	free(cands);

	return best_candidate;
}




int main(int argc, char** argv){
	
	noob = NULL;
	//int i;
	if(argc!=9 || !argv)
		failwith("Arguments: -n [num] -g [gen] -p [point] -c [cross]");

	pthread_mutex_t tmp = PTHREAD_MUTEX_INITIALIZER;
	pauser = tmp;
	if(pipe(fightpipe) < 0)
		failwith("pipe");

	srand(time(NULL));

	n = atoi(argv[2]);
	g = atoi(argv[4]);
	p = atoi(argv[6]);
	c = atoi(argv[8]);

	//pthread_t threads[NUM_THREADS];

	run();
	//pthread_join(noob_thread, NULL);
	pthread_exit(NULL);
}