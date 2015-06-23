/*
 * INF3172
 * Algorithmes de remplacements des pages et threads
 *
 * Auteur : Mathieu Pronovost PROM18118300
 * Version : 8 avril 2013
 */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "util.h"

// Arguments qui seront envoyés aux threads
struct argumentsThread {
    struct ref_processus processus;
    int nbCadres;
    int cycle;
};

// Fonctions appelées par chaque thread
void *algoOptimal (void *argumentsAlgo);
void *algoHorloge (void *argumentsAlgo);
void *algoVieillissement (void *argumentsAlgo);

int main (int argc, char *argv []) {
    if (argc != 4) {
        perror("Nombre d'arguments erroné");
        exit(1);
    }
    // Création des threads
    pthread_t algorithmeOptimal;
    pthread_t algorithmeHorloge;
    pthread_t algorithmeVieillissement;

    struct argumentsThread *argumentsAlgorithmes = malloc(sizeof(struct argumentsThread));
    argumentsAlgorithmes->processus = lireFichier(argv[3]);
    argumentsAlgorithmes->nbCadres = atoi(argv[1]);
    argumentsAlgorithmes->cycle = atoi(argv[2]);

    pthread_create(&algorithmeOptimal, NULL, algoOptimal, argumentsAlgorithmes);
    pthread_create(&algorithmeHorloge, NULL, algoHorloge, argumentsAlgorithmes);
    pthread_create(&algorithmeVieillissement, NULL, algoVieillissement, argumentsAlgorithmes);

    struct memoire_physique *affichage = malloc(sizeof(struct memoire_physique));
    void *resultat;

    if (! pthread_join(algorithmeOptimal, &resultat)) {
        affichage = (struct memoire_physique *) resultat;
        print_memoire_physique(*affichage);
    }
    if (! pthread_join(algorithmeHorloge, &resultat)) {
        affichage = (struct memoire_physique *) resultat;
        print_memoire_physique(*affichage);
    }
    if (! pthread_join(algorithmeVieillissement, &resultat)) {
        affichage = (struct memoire_physique *) resultat;
        print_memoire_physique(*affichage);
    }

    return EXIT_SUCCESS;
}

void *algoOptimal (void *argumentsAlgo) {
    struct argumentsThread *algorithme = (struct argumentsThread *) argumentsAlgo;
    return algo_optimal(&algorithme->processus, algorithme->nbCadres);
}

void *algoHorloge (void *argumentsAlgo) {
    struct argumentsThread *algorithme = (struct argumentsThread *) argumentsAlgo;
    return algo_horloge(&algorithme->processus, algorithme->nbCadres);
}

void *algoVieillissement (void *argumentsAlgo) {
    struct argumentsThread *algorithme = (struct argumentsThread *) argumentsAlgo;
    return  algo_vieillissement(&algorithme->processus, algorithme->nbCadres, algorithme->cycle);
}
