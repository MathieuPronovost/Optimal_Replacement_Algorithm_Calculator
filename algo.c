/*
 * INF3172
 * Algorithmes de remplacements des pages et threads
 *
 * Auteur : Mathieu Pronovost PROM18118300
 * Version : 8 avril 2013
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"

struct memoire_physique *algo_optimal (struct ref_processus *processus, int nbCadres) {
    struct memoire_physique *reponse = malloc(sizeof(struct memoire_physique));
    reponse->algo = OPTIMAL;
    reponse->cadres = malloc(sizeof(struct cadre) * nbCadres);
    reponse->nbre_cadres = nbCadres;
    reponse->nbre_defauts_pages = -1;

    int contenuCadre[1000][2];
    int max = 0;
    int posMax = 0;
    int check = 0;
    int j = 0;
    int i = 0;

    for (j = 0; j < nbCadres; j++) {
        contenuCadre[j][0] = 0;
        contenuCadre[j][1] = 999999;
    }

    // Parcourt toutes les références
    for (i = 0; i < processus->nbre_ref; i++) {
        max = 0;
        posMax = 0;
        check = 0;
        // Parcourt tous les cadres
        for (j = 0; j < nbCadres; j++) {
            if (contenuCadre[j][0] == processus->references[i]) {
                check = 1;
            }
            if (contenuCadre[j][1] > max) {
                max = contenuCadre[j][1];
                posMax = j;
            }
        }
        if (check != 1) {
            reponse->nbre_defauts_pages++;
            contenuCadre[posMax][0] = processus->references[i];
            contenuCadre[posMax][1] = 999999;
            int k = processus->nbre_ref - 1;
            while (k > i) {
                if (processus->references[i] == processus->references[k]) {
                    contenuCadre[posMax][1] = k;
                }
                k--;
            }
        }
    }
    return reponse;
}

struct memoire_physique *algo_horloge (struct ref_processus *processus, int nbCadres) {
    struct memoire_physique *reponse = malloc(sizeof(struct memoire_physique));
    reponse->algo = HORLOGE;
    reponse->cadres = malloc(sizeof(struct cadre) * nbCadres);
    reponse->nbre_cadres = nbCadres;
    reponse->nbre_defauts_pages = 0;

    int j = 0;
    int i = 0;
    int pos = 0;
    int stop = 0;

    for (j = 0; j < nbCadres; j++) {
        reponse->cadres[j].page = 0;
        reponse->cadres[j].R = '0';
    }

    for (i = 0; i < processus->nbre_ref; i++) {
        if (pos > (nbCadres-1)) {
            pos = 0;
        }
        stop = 0;
        for (j = 0; j < nbCadres; j++) {
            if (processus->references[i] == reponse->cadres[j].page) {
                reponse->cadres[j].R = '1';
                stop = 1;
            }
        }
        while (stop == 0) {
            if (reponse->cadres[pos].R == '1') {
                reponse->cadres[pos].R = '0';
                if (pos >= (nbCadres-1)) {
                    pos = 0;
                } else {
                    pos++;
                }
            } else {
                reponse->nbre_defauts_pages++;
                stop = 1;
                reponse->cadres[pos].page = processus->references[i];
                reponse->cadres[pos].R = '1';
                pos++;
            }
        }
    }
    return reponse;
}

struct memoire_physique *algo_vieillissement (struct ref_processus *processus, int nbCadres, int cycle) {
    struct memoire_physique *reponse = malloc(sizeof(struct memoire_physique));
    reponse->algo = VIEILLISSEMENT;
    reponse->cadres = malloc(sizeof(struct cadre) * nbCadres);
    reponse->nbre_cadres = nbCadres;
    reponse->nbre_defauts_pages = 0;

    int contenuCadre[1000][2];
    int pos = 0;
    int vieux = 0;
    int stop = 0;
    int j = 0;
    int i = 0;

    for (j = 0; j < nbCadres; j++) {
        contenuCadre[j][0] = 0;
        contenuCadre[j][1] = cycle + 2;
    }

    for (i = 0; i < processus->nbre_ref; i++) {
        vieux = 0;
        stop = 0;
        for (j = 0; j < nbCadres; j++) {
            if (processus->references[i] == contenuCadre[j][0]) {
                contenuCadre[j][1] = 0;
                stop = 1;
            }
            if (contenuCadre[j][1] > vieux) {
                vieux = contenuCadre[j][1];
                pos = j;
            }
        }

        if (stop == 0) {
            reponse->nbre_defauts_pages++;
            contenuCadre[pos][0] = processus->references[i];
            contenuCadre[pos][1] = 0;
            for (j = 0; j < nbCadres; j++) {
                if (contenuCadre[j][1] != (cycle + 2)) {
                    contenuCadre[j][1]++;
                }
            }
        }
    }

    // Transfert contenuCadre dans cadres
    for (j = 0; j < nbCadres; j++) {
        reponse->cadres[j].page = contenuCadre[j][0];
        if (contenuCadre[j][1] == 1) {
            reponse->cadres[j].R = 128;
        } else if (contenuCadre[j][1] == 2) {
            reponse->cadres[j].R = 64;
        } else if (contenuCadre[j][1] == 3) {
            reponse->cadres[j].R = 32;
        } else if (contenuCadre[j][1] == 4) {
            reponse->cadres[j].R = 16;
        } else if (contenuCadre[j][1] == 5) {
            reponse->cadres[j].R = 8;
        } else if (contenuCadre[j][1] == 6) {
            reponse->cadres[j].R = 4;
        } else if (contenuCadre[j][1] == 7) {
            reponse->cadres[j].R = 2;
        } else if (contenuCadre[j][1] == 8) {
            reponse->cadres[j].R = 1;
        }
    }
    return reponse;
}
