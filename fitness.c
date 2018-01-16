#include "start.h"

/*
	Some example fitness methods. Modify fitness() to call desired fitness evaluator
*/


fitval pairs(candidate* cand){
	fitval f = 1;
	int i;
	for(i = 0; i < INFOLEN - 1; i++)
		if(cand->info[i] == cand->info[i+1])
			f++;
	return f;
}

fitval streaks(candidate* cand){
	fitval f = 1;
	int i;
	int s = 0;
	for(i = 0; i < INFOLEN - 1; i++)
		if(cand->info[i] == cand->info[i+1])
			s++;
		else{
			f+=s*s;
			s=0;
		}
	return f;
}

fitval sumsquares(candidate* cand){
	fitval f = 2;
	int i;
	for(i = 0; i < INFOLEN; i++)
		f += (cand->info[i] * cand->info[i]);
	return f;
}

fitval zerostreak(candidate* cand){
	fitval f = 1;
	int i;
	for(i = 0; i < INFOLEN; i++){
		if(cand->info[i] == 0)
			f++;
		else
			break;
	}
	return f;
}

fitval zero(candidate* cand){
	fitval f = 1;
	int i;
	for(i = 0; i < INFOLEN; i++){
		if(cand->info[i] == 0)
			f++;
	}
	return f;
}

fitval zero_mult(candidate* cand){
	fitval f = 1;
	int i;
	for(i = 0; i < INFOLEN; i++){
		if(cand->info[i] == 0)
			f*=2;
	}
	return f;
}


fitval fitness(candidate* cand){
	return zerostreak(cand);
}
