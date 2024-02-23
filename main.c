#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include <stdint.h>
#define k1 1024
#define dimAuto 512
#define INFINITY 4294967295
#define heapCapacity 25000
#include <math.h>

int unused;;
double A;
uint16_t precalc;

typedef struct stazione_s{
    uint32_t km;
    uint32_t automobili[dimAuto];
    uint32_t nAuto;
    struct stazione_s * next;

    uint32_t dist;
    struct stazione_s * prev_g; //previous in the shortest path

    int heap_i;
}stazione_t;

void aggiungiAuto(stazione_t * s, uint32_t autonomia){
    uint32_t * automobili = s->automobili;
    uint32_t tmp, i;
    if(autonomia > automobili[0]){
        tmp = automobili[0];
        automobili[0] = autonomia;
    }
    else{
        tmp = autonomia;
    }
    for(i = 1; automobili[i] != 0; i++){
    }
    automobili[i] = tmp;
    s->nAuto++;
}

uint16_t calcolaKeyKm(uint32_t km){
    return km * precalc;//% 262143;
}

void aggiungiStazione(stazione_t ** hashmap, uint32_t km){
    stazione_t * s = malloc(sizeof(stazione_t));
    uint32_t num_auto, autonomia, i, key;
    s->km = km;
    s->next = NULL;
    unused = scanf("%u", &num_auto);
    //int * autos = malloc(sizeof(uint32_t *num_auto*2);
    for(i=0; i < dimAuto ; i++){
        s->automobili[i] = 0;
    }
    s->nAuto = num_auto;
    for(i=0; i<num_auto; i++){
        unused = scanf("%u", &autonomia);
        aggiungiAuto(s, autonomia);
    }
    key = calcolaKeyKm(km);
    if(hashmap[key] == NULL){
        hashmap[key] = s;
    }
    else{
        s->next = hashmap[key];
        hashmap[key] = s;
    }
}

typedef struct q_node_s{
    stazione_t * stazione;
    struct q_node_s * next;
}q_t;

typedef struct min_heap_s{
    stazione_t * heap[heapCapacity];
    int size;
}min_heap_t;
min_heap_t MinHeap;
min_heap_t BstKm;
int isMaggiore(stazione_t * s1, stazione_t * s2){
    if(s1->dist != s2->dist) {
        if (s1->dist > s2->dist)
            return 1;
        else
            return 0;
    }  //(MinHeap.heap[i/2]->dist > MinHeap.heap[i]->dist) || ((MinHeap.heap[i/2]->dist <= MinHeap.heap[i]->dist) && (MinHeap.heap[i/2]->km > s2->km))
    if(s1->km > s2->km)
        return 1;
    else
        return 0;
}
uint32_t alt;
stazione_t * tmp_heap;
void aggiungiAHeap(stazione_t * s){
    MinHeap.size++;
    int i = MinHeap.size;
    MinHeap.heap[i] = s;
    while(i > 1 && isMaggiore(MinHeap.heap[i/2], MinHeap.heap[i]) == 1){
        tmp_heap = MinHeap.heap[i/2];
        MinHeap.heap[i/2] = MinHeap.heap[i];
        MinHeap.heap[i] = tmp_heap;
        i = i/2;
    }
}

void minHeapify(int i){
    int l = 2*i;
    int r = l+1;
    int smallest = i;
    if (l <= MinHeap.size && isMaggiore(MinHeap.heap[i], MinHeap.heap[l]) == 1) {
        smallest = l;
    }
    if (r <= MinHeap.size && isMaggiore(MinHeap.heap[smallest], MinHeap.heap[r])) {
        smallest = r;
    }
    if (smallest != i){
        tmp_heap = MinHeap.heap[i];
        MinHeap.heap[i] = MinHeap.heap[smallest];
        MinHeap.heap[smallest]->heap_i = i;
        MinHeap.heap[smallest] = tmp_heap;
        tmp_heap->heap_i = smallest;
        minHeapify(smallest);
    }
}
void aggiusta_dopo_nuova_distanza(int i){

    while (i > 1 && isMaggiore(MinHeap.heap[i/2], MinHeap.heap[i]))
    {
        tmp_heap = MinHeap.heap[i];
        MinHeap.heap[i] = MinHeap.heap[i/2];
        MinHeap.heap[i]->heap_i = i;
        MinHeap.heap[i/2] = tmp_heap;
        i = i/2;
        MinHeap.heap[i]->heap_i = i; // i/2
    }
}
uint32_t sott;
uint32_t ris_ind_bst;
uint32_t calcolaIndiceRadiceBst(uint32_t inizio, uint32_t fine){
    sott = fine-inizio;
    if(sott % 2 == 0){
        ris_ind_bst = sott / 2;
    }
    else{
        ris_ind_bst = sott / 2 + 1;
    }
    ris_ind_bst += inizio;
    return ris_ind_bst;
}

void maxHeapifyBst(int len, int i){
    int l = 2*i;
    int r = l+1;
    int max = i;
    if (l <= len && BstKm.heap[i]->km < BstKm.heap[l]->km) {
        max = l;
    }
    if (r <= len && BstKm.heap[max]->km < BstKm.heap[r]->km) {
        max = r;
    }
    if (max != i){
        tmp_heap = BstKm.heap[i];
        BstKm.heap[i] = BstKm.heap[max];
        BstKm.heap[max] = tmp_heap;

        maxHeapifyBst(len, max);
    }
}

void heapSort(){
    int i, len;

    for(i = BstKm.size/2; i >= 1; i--){
        maxHeapifyBst(BstKm.size, i);
    }

    len = BstKm.size;
    for(i = BstKm.size; i >= 2; i--){
        tmp_heap = BstKm.heap[i];
        BstKm.heap[i] = BstKm.heap[1];
        BstKm.heap[1] = tmp_heap;

        len--;
        maxHeapifyBst(len, 1);
    }
}

//sicuro sempre presente
uint32_t cercaValoreBst(uint32_t km){
    uint32_t inizio = 0;
    uint32_t fine = BstKm.size;
    uint32_t radice;
    while(1 != 0) {
        radice = calcolaIndiceRadiceBst(inizio, fine);
        if (BstKm.heap[radice]->km == km) {
            return radice;
        }

        if (km < BstKm.heap[radice]->km) {
            fine = radice - 1;
        }
        else {
            inizio = radice;
        }
    }
}

void calcolaPercorsoDirettoOttimizzato(uint32_t partenza_km, uint32_t arrivo_km, stazione_t ** hashmap){
    /*
     * 1. Riempi l'insieme di nodi Q(lista in ordine crescente o decrescente) e inizializza dist e prev_g,
     *      solo dalla partenza/arrivo all'arrivo/partenza
     *      da inserire in ordine o da ordinare
     * 2. Dijkstra
     * 3. Print del percorso
     */
    MinHeap.size = 0;

    int flag = 0;
    stazione_t * partenza;
    for(partenza = hashmap[calcolaKeyKm(partenza_km)]; partenza->km != partenza_km; partenza = partenza->next){
    }
    partenza->dist = 0;
    partenza->prev_g = NULL;
    aggiungiAHeap(partenza);
    stazione_t * arrivo;
    for(arrivo = hashmap[calcolaKeyKm(arrivo_km)]; arrivo->km != arrivo_km; arrivo = arrivo->next){
    }
    arrivo->prev_g = NULL;

    uint32_t i;
    stazione_t * s;
    for(i = 0; i < 65536; i++){
        for(s = hashmap[i]; s != NULL; s=s->next){
            if(s->km <= arrivo_km && s->km > partenza_km) {
                s->dist = (uint32_t) INFINITY;
                aggiungiAHeap(s);
            }
        }
    }

    //Inizializzazione BST
    BstKm.size = MinHeap.size;
    for(i = 1; i <= MinHeap.size; i++){
        BstKm.heap[i] = MinHeap.heap[i];
        MinHeap.heap[i]->heap_i = i;
    }
    heapSort();

    partenza->dist = 0;
    flag = 1;
    stazione_t * u;
    uint32_t u_km, u_auto, bst_size;
    bst_size = BstKm.size;
    while(MinHeap.size > 0 && flag == 1){
        // u <- min_dist(Q)
        u = MinHeap.heap[1];
        //Q = Q \ {u}
        MinHeap.heap[1] = MinHeap.heap[MinHeap.size];
        MinHeap.heap[1]->heap_i = 1;
        MinHeap.size--;
        minHeapify(1);

        if(u->dist != (uint32_t) INFINITY){
            u_km = u->km;
            u_auto = u->automobili[0];
            alt = u->dist + 1;
            for(int k = cercaValoreBst(u_km) + 1; k <= bst_size; k++){
                if(BstKm.heap[k]->km - u_km <= u_auto){
                    if(alt < BstKm.heap[k]->dist){
                        BstKm.heap[k]->dist = alt;
                        BstKm.heap[k]->prev_g = u;
                        aggiusta_dopo_nuova_distanza(BstKm.heap[k]->heap_i);
                    }
                }
                else{
                    break;
                }
            }
        }
        else{
            flag = 0;
        }

        if(arrivo->prev_g != NULL){
            flag = 0;
        }
    }

    q_t * d;
    q_t * e;
    if(arrivo->prev_g == NULL){
        printf("nessun percorso\n");
    }
    else{
        q_t * percorso = malloc(sizeof(q_t));
        percorso->next = NULL;
        percorso->stazione = arrivo;
        for(s = arrivo->prev_g; s != NULL; s = s->prev_g){
            e = malloc(sizeof (q_t));
            e->next = percorso;
            e->stazione = s;
            percorso = e;
        }
        d = percorso;
        printf("%u", percorso->stazione->km);
        percorso = percorso->next;
        free(d);
        while(percorso != NULL){
            d = percorso;
            printf(" %u", percorso->stazione->km);
            percorso = percorso->next;
            free(d);
        }
        printf("\n");
    }
}
void calcolaPercorsoInversoOttimizzato(uint32_t partenza_km, uint32_t arrivo_km, stazione_t ** hashmap){
    /*
     * 1. Riempi l'insieme di nodi Q(lista in ordine crescente o decrescente) e inizializza dist e prev_g,
     *      solo dalla partenza/arrivo all'arrivo/partenza
     *      da inserire in ordine o da ordinare
     * 2. Dijkstra
     * 3. Print del percorso
     */
    MinHeap.size = 0;

    int flag = 0;
    stazione_t * partenza;
    for(partenza = hashmap[calcolaKeyKm(partenza_km)]; partenza->km != partenza_km; partenza = partenza->next){
    }
    partenza->dist = 0;
    partenza->prev_g = NULL;
    aggiungiAHeap(partenza);
    stazione_t * arrivo;
    for(arrivo = hashmap[calcolaKeyKm(arrivo_km)]; arrivo->km != arrivo_km; arrivo = arrivo->next){
    }
    arrivo->prev_g = NULL;

    uint32_t i;
    stazione_t * s;
    for(i = 0; i < 65536; i++){
        for(s = hashmap[i]; s != NULL; s=s->next){
            if(s->km >= arrivo_km && s->km < partenza_km) {
                s->dist = (uint32_t) INFINITY;
                //s->prev_g = NULL;
                aggiungiAHeap(s);
            }
        }
    }


    //Inizializzazione BST
    BstKm.size = MinHeap.size;
    for(i = 1; i <= MinHeap.size; i++){
        BstKm.heap[i] = MinHeap.heap[i];
        MinHeap.heap[i]->heap_i = i;
    }
    heapSort();


    stazione_t * u;
    flag = 1;
    uint32_t u_km, u_auto;
    while(MinHeap.size > 0 && flag == 1){
        // u <- min_dist(Q)
        u = MinHeap.heap[1];
        //Q = Q \ {u}
        MinHeap.heap[1] = MinHeap.heap[MinHeap.size];
        u->heap_i = -1;
        MinHeap.heap[1]->heap_i = 1;
        MinHeap.size--;
        minHeapify(1);

        if(u->dist != (uint32_t)INFINITY){
            u_km = u->km;
            u_auto = u->automobili[0];
            alt = u->dist + 1;
            for(int k = cercaValoreBst(u_km) - 1; k >= 1; k--){
                if(BstKm.heap[k]->heap_i != -1) {
                    if (u_km - BstKm.heap[k]->km <= u_auto) {
                        if (alt < BstKm.heap[k]->dist) {
                            BstKm.heap[k]->dist = alt;
                            BstKm.heap[k]->prev_g = u;
                            aggiusta_dopo_nuova_distanza(BstKm.heap[k]->heap_i);
                        }
                    }else {
                        break;
                    }
                }
            }
        }
        else{
            flag = 0;
        }

        if(arrivo->prev_g != NULL){
            flag = 0;
        }
    }

    q_t * d, * e;
    if(arrivo->prev_g == NULL){
        printf("nessun percorso\n");
    }
    else{
        q_t * percorso = malloc(sizeof(q_t));
        percorso->next = NULL;
        percorso->stazione = arrivo;
        for(s = arrivo->prev_g; s != NULL; s = s->prev_g){
            e = malloc(sizeof (q_t));
            e->next = percorso;
            e->stazione = s;
            percorso = e;
        }
        d = percorso;
        printf("%u", percorso->stazione->km);
        percorso = percorso->next;
        free(d);
        while(percorso != NULL){
            d = percorso;
            printf(" %u", percorso->stazione->km);
            percorso = percorso->next;
            free(d);
        }
        printf("\n");
    }
}

int main() {
    A = (sqrt(5)-1)/2;
    precalc = (uint16_t) ((double)A * (((uint32_t )1 << 16)));

    uint32_t dim_hashmap = 65536;
    stazione_t ** hashmap = malloc(sizeof(stazione_t*) * dim_hashmap);
    //freopen("../open_110.txt", "r", stdin);
    //freopen("../out.txt", "w", stdout);
    char comando[50];
    stazione_t * stazioni = NULL;
    stazione_t * s; // uso come tmp
    uint32_t km, autonomia, arrivo, partenza, i = 0, tmp, key, flag;
    for(i = 0; i < dim_hashmap; i++){
        hashmap[i] = NULL;
    }
    i = 0;
    while(scanf("%s", comando) != EOF){
        i++;
        if(strcmp(comando,"aggiungi-stazione") == 0){
            s = stazioni;
            unused = scanf("%u", &km);
            key = calcolaKeyKm(km);
            flag = 1;
            for(stazione_t * e = hashmap[key]; e != NULL && flag==1; e = e->next){
                if(e->km == km){
                    flag = 0;
                }
            }
            if(flag == 1){
                aggiungiStazione(hashmap, km);
                printf("aggiunta\n");
            }
            else {
                printf("non aggiunta\n");
            }
        }
        else if(strcmp(comando,"demolisci-stazione") == 0) {
            s = stazioni;
            unused = scanf("%u", &km);
            key = calcolaKeyKm(km);
            flag = 1;
            stazione_t *e, *p;
            if (hashmap[key] != NULL){
                if (hashmap[key]->km == km) {
                    e = hashmap[key];
                    hashmap[key] = hashmap[key]->next;
                    free(e);
                    printf("demolita\n");
                }
                else{
                    flag = 1;
                    p = hashmap[key];
                    for(e = hashmap[key]->next; e != NULL; e = e->next){
                        if(e->km == km){
                            flag = 0;
                            break;
                        }
                        p = e;
                    }
                    if(flag == 0){
                        p->next = e->next;
                        free(e);
                        printf("demolita\n");
                    }
                    else{
                        printf("non demolita\n");
                    }
                }
            }
            else{
                printf("non demolita\n");
            }
        }
        else if(strcmp(comando,"aggiungi-auto") == 0){
            unused = scanf("%u", &km);
            unused = scanf("%u", &autonomia);

            s = stazioni;
            key = calcolaKeyKm(km);
            flag = 1;
            stazione_t * e;
            for(e = hashmap[key]; e != NULL; e = e->next){
                if(e->km == km){
                    flag = 0;
                    break;
                }
            }
            if(flag == 0){
                aggiungiAuto(e, autonomia);
                printf("aggiunta\n");
            }
            else {
                printf("non aggiunta\n");
            }
        }
        else if(strcmp(comando,"rottama-auto") == 0){
            unused = scanf("%u", &km);
            unused = scanf("%u", &autonomia);
            key = calcolaKeyKm(km);
            flag = 1;
            stazione_t * e;
            for(e = hashmap[key]; e != NULL; e = e->next){
                if(e->km == km){
                    flag = 0;
                    break;
                }
            }
            if(flag == 0){
                s = e;
                uint32_t * automobili = s->automobili;

                if(s->nAuto == 0){
                    printf("non rottamata\n");
                }
                else{
                    for(i = 0; i<dimAuto && automobili[i] != autonomia; i++){
                    }
                    if(i<dimAuto && automobili[i] == autonomia){
                        automobili[i] = 0;
                        s->nAuto--;
                        if(i == 0 && s->nAuto != 0){
                            for(i = 1; i<dimAuto; i++){
                                if(automobili[0] < automobili[i]){
                                    tmp = automobili[0];
                                    automobili[0] = automobili[i];
                                    automobili[i] = tmp;
                                }
                            }
                        }
                        printf("rottamata\n");
                    }
                    else{
                        printf("non rottamata\n");
                    }
                }
            }
            else {
                printf("non rottamata\n");
            }
        }
        else if(strcmp(comando,"pianifica-percorso") == 0){
            unused = scanf("%u", &partenza);
            unused = scanf("%u", &arrivo);
            if(partenza == arrivo){
                printf("%u\n", arrivo);
            }
            else if(arrivo > partenza){
                calcolaPercorsoDirettoOttimizzato(partenza, arrivo, hashmap);
            }
            else{
                calcolaPercorsoInversoOttimizzato(partenza, arrivo, hashmap);
            }
        }
    }
    return 0;
}
