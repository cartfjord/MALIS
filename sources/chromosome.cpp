/*****************************************************************************/
/*
* Routines to manipulate chromosomes
*
* MALIS Lab Session
*
/*****************************************************************************/

#include <assert.h>
#include "tsp_utils.h"
#include "chromosome.h"
#include <algorithm>
#include <iostream>
using namespace std;

#define _DEBUG
/*****************************************************************************/
/* Print a chromosome */
void chromosome_print(Chromosome * c){
	int i;

	printf("  Chromosome: fitness=%5.2f, cities:",c->fitness);
	for(i=0; i<n_cities; i++){
		printf(" %3d",c->gene[i]);
	}
	printf("\n");
}

/*****************************************************************************/
/* Print a chromosome in a file */
void chromosome_print_file(FILE *out, Chromosome * c){
	int i;

	fprintf(out,"  Chromosome: fitness=%5.2f, cities:",c->fitness);
	for(i=0; i<n_cities; i++){
		fprintf(out," %3d",c->gene[i]);
	}
	fprintf(out,"\n");
}

/*****************************************************************************/
/* Initialize a chromosome randomly:
- create an ordered sequence
- randomly swap genes many times
*/
void chromosome_random(Chromosome * c){
	int i, i1, i2, g;

	/* create a ordered sequence of cities */ 
	for(i=0; i<n_cities; i++) {
		c->gene[i]=i; 
	}

	/* Now, swap random cities in the sequence */
	for(i=0; i<n_cities*10; i++){
		i1 = random_number(0,n_cities-1);
		i2 = random_number(0,n_cities-1);
		g = c->gene[i1];
		c->gene[i1] = c->gene[i2];
		c->gene[i2] = g;
	}

	c->fitness = chromosome_compute_fitness(c);
}

/*****************************************************************************/
/* Computes the fitness (round-trip length) of a chromosome */
double chromosome_compute_fitness(Chromosome * c){

	c->fitness = tsp_round_trip_length(c->gene);
	return c->fitness;
}

/*****************************************************************************/
/* This routine implements the mutation operator */
void chromosome_mutation(Chromosome * c) {
	/* invert a subsequence of this chromosome */
	int first, last;		/* beginning and end of subsequence */

	/* BEGIN IMPLEMENTATION */
	/* Generate random positions for the beginning and end of the subsequence */
	/* Reverse the chosen subsequence of cities */
	/* END IMPLEMENTATION */
	first = random_number(0,n_cities-1);
	last = random_number(first,n_cities-1);

	int *tmp_arr = new int[last-first+1];

	int j = 0;
	for(int i = last; i >= first; i--){
		tmp_arr[j] = c->gene[i];
		j++;
	}

	memcpy(c->gene+first,tmp_arr,(last-first+1)*sizeof(int));

	/* always keep fitness updated */
	c->fitness = chromosome_compute_fitness(c);

	// For debugging: check that each city appears exactly once
#ifdef _DEBUG
	{
		int check[MAX_N_CITIES];
	    int i, city;
	    for(i=0; i<n_cities; i++) { 
		    int city = c->gene[i];
		    assert(city < n_cities);
		    assert(city >= 0);
		    check[city]=1; 
	    }
	    for(city=0;city<n_cities;city++) { 
		    assert(check[city]==1); 
	    }
	}
#endif
}

/*****************************************************************************/
/* This routine implements the crossover operator */

int gene_get_next_gene(Chromosome * father, int * copied){
	for(int father_pos = 0; father_pos < n_cities ; father_pos++){
		int  = true;
		for(int copied_pos = 0; copied_pos < n_cities; copied_pos++){
			if(father->gene[father_pos] == copied[copied_pos]) test = false;
		}
	}

	return -1;
}

void chromosome_crossover(Chromosome * mother, Chromosome * father, Chromosome * offspring){
	int first, last;			/* beginning and end of mother's subsequence */
	int copied[MAX_N_CITIES]; /* array to remember which genes have been already copied to the offspring */
	int offspring_pos, father_pos, copied_pos;

	/* initialize Offspring does not yet contain any genes. */
	for(int i=0; i<n_cities; i++) copied[i]=0;

	/* BEGIN IMPLEMENTATION */
	/* Generate random positions for the beginning and end of the subsequence */
	first = random_number(0,n_cities-1);
	last = random_number(first,n_cities-1);
	copied_pos = last - first + 1;
	
	for(int i = 0 ; i < n_cities ; i++) offspring->gene[i] = -1;

	cout << "First pos: " << first << endl;
	cout << "Last pos: " << last << endl;
	/* Copy the mother's sub-sequence to the offspring in the same position */
	memcpy(offspring->gene+first,mother->gene+first,(last-first+1)*sizeof(int));
	/* update the "copied" array to keep track of which cities have been copied to the offspring */
	memcpy(copied,mother->gene+first,(last-first+1)*sizeof(int));

	/* END IMPLEMENTATION */

	/* Now, fill up the remaining positions of the offspring with the cities
	from the father's chromosome while conserving the ordering */

	father_pos = 0;
	for(offspring_pos = 0 ; offspring_pos < n_cities ; offspring_pos++){
		if ( offspring->gene[offspring_pos] == -1
				/* BEGIN IMPLEMENTATION */
				/* test if offspring_pos is pointing to an empty position in offspring */

				/* END IMPLEMENTATION */
			)
		{
			/* BEGIN IMPLEMENTATION */
			/* offspring_pos is an empty position, fill it with the next gene in the father's chromosome */
			int next_gene = gene_get_next_gene(father,copied);
			if(next_gene != -1){
				offspring->gene[offspring_pos] = next_gene;
				copied[copied_pos] = next_gene;
				copied_pos++;
			}
			/* update the 'copied' array */
			/* END IMPLEMENTATION */

			/* check that positions remain within allowed range */
			assert(offspring_pos < n_cities && offspring_pos >= 0);
			assert(father_pos < n_cities && father_pos >= 0);
		}
	}

	/* Sanity check: All the cities should appear exactly once in the
	offspring, otherwise something went wrong in the algorithm */
	for(int i=0; i<n_cities; i++){
		cout << copied[i] << " ";
		//assert(copied[i]==1);
	}
	cout << endl;
	/* always keep fitness updated */
	offspring->fitness = chromosome_compute_fitness(offspring);
}
