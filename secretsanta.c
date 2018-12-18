#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void derange(char *[], int, int);

int main(void) {
    int i, n;
    int m = 30;
    printf("Enter number of participants: ");
    scanf("%d", &n);
    char address[n][30];
    for(i = 0; i < n; i++) { // store addresses
        printf("Enter participant %d email address: ", i+1);
        scanf("%s", address[i]);
    }
    printf("Before Derangement:\n"); // print addresses before derangement
    for(i = 0; i < n; i++) {
        printf("  Address %d is %s.\n", i+1, address[i]);
    }
    derange((char **)address, n, m);
    printf("After Derangement:\n"); // print addresses after derangement
    for(i = 0; i < n; i++) {
        printf("  Address %d is %s.\n", i+1, address[i]);
    }
    return 0;
}

void derange(char *address[], int n, int m) {
    srand(time(NULL)); // initialie rand()
    printf("  Randomizing Addresses...\n");
    int i;
    for(i = 0; i < n; i++) {
        char temp[30];
        temp = address[i]; //breaks
        int r = i;
        while(r == i) {
            r = (rand() % n);
            printf("    Swapping index %d and %d\n", i, r);
        }
        address[i] = address[r]; //wrong
        address[r] = temp;
    }
    printf("  Done\n");
}
