/* 
 * File:  huffman.c
 * Author: Jake Day
 * Class: COEN 12
 * Instructor: Atkinson
 * Section: T 5:15-8pm
 * Created on May 17, 2016, 5:38 PM
 * Description: This program uses Huffman coding to compress text files.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include "tree.h"
#include "pack.h"

#define SIZE 256
#define p(x) (x - 1) / 2 //parent location
#define l(x) x * 2 + 1 //left location
#define r(x) x * 2 + 2 //right location

typedef struct tree TREE;
int counts[SIZE + 1];
TREE *leaves[SIZE + 1];
TREE *heap[SIZE + 1];

void readFile(char *input);
void createHeap();
void insert(TREE *t, int i);
void print(TREE *t);
TREE *delete(int count);

int main(int argc, char* argv[]) {
    int count = 0;
    TREE *left, *right, *node;
    
    //checks to make sure an input and output file are specified
    if (argc != 3) {
        fprintf(stderr, "Improper number of arguments.\n");
        EXIT_FAILURE;
    }

    readFile(argv[1]);

    //fills an array of tree nodes with each character's count
    for (int i = 0; i <= SIZE; i++) {
        if (counts[i] != 0) {
            leaves[i] = createTree(counts[i], NULL, NULL);
            count++;
        }
    }

    //creates a tree for the EOF marker
    leaves[SIZE] = createTree(0, NULL, NULL);
    count++;

    createHeap();

    //repeatedly removes the two minimum nodes until it creates a Huffman tree at heap[0]
    while (count > 1) {
        left = delete(count);
        count--;
        right = delete(count);
        count--;
        node = createTree(getData(left) + getData(right), left, right);
        insert(node, count);
        count++;
    }

    //prints the ASCII value, count, and encoding of each character
    for (int i = 0; i <= SIZE; i++) {
        if (leaves[i] != NULL) {
            if (isprint(i) != 0) {
                printf("'%c': ", i);
            } else {
                printf("%03o: ", i);
            }
            printf("%d ", getData(leaves[i]));
            print(leaves[i]);
            printf("\n");
        }
    }

    pack(argv[1], argv[2], leaves);
    
    destroyTree(heap[0]);

    EXIT_SUCCESS;
}

/*
 * Reads a text file and inserts each ASCII character's count into an array
 */
void readFile(char *input) {
    int c;
    FILE *file;

    file = fopen(input, "r");

    if (file == NULL) {
        fprintf(stderr, "File not found.\n");
        EXIT_FAILURE;
    }

    while ((c = fgetc(file)) != EOF) {
        counts[c]++;
    }

    fclose(file);
}

/*
 * Creates the initial binary heap of leaves
 */
void createHeap() {
    int j = 0;
    
    for (int i = 0; i <= SIZE; i++) {
        if (leaves[i] != NULL) {
            insert(leaves[i], j);
            j++;
        }
    }
}

/*
 * Places a tree into the priority queue 
 */
void insert(TREE *t, int i) {
    while (i > 0 && getData(heap[p(i)]) > getData(t)) {
        heap[i] = heap[p(i)];
        i = p(i);
    }

    heap[i] = t;
}

/*
 * Prints the binary representation of a leaf
 */
void print(TREE *t) {
    if (getParent(t) != NULL) {
        print(getParent(t));
        if (getLeft(getParent(t)) == t) {
            printf("0");
        } else {
            printf("1");
        }
    }
}

/*
 * Returns the minimum value in a binary heap
 */
TREE *delete(int count) {
    int i, child;
    TREE *min, *node;

    min = heap[0];
    node = heap[count - 1];

    i = 0;

    while (l(i) < count - 1) {
        child = l(i);
        if (r(i) < count - 1 && getData(heap[l(i)]) > getData(heap[r(i)])) {
            child = r(i);
        }
        if (getData(heap[child]) < getData(node)) {
            heap[i] = heap[child];
            i = child;
        } else {
            break;
        }
    }

    heap[i] = node;

    return min;
}