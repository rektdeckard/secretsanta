#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Person structure definition
struct person {
    char name[30];
    char emailAddress[30];
    char secretSanta[30];
};

// Read person prototype
void readPerson(struct person *, int);

// Derangement function prototype
void derange(struct person *, int);

// Print function prototype for diagnostic purposes. should be disbled for obscurity
void printPerson(struct person, int);

// Email function prototype
void email(struct person, int);


// Main function
int main(void) {
    int i, n;
    char term, em, pr;
    printf("\e[1;1H\e[2J");
    printf("\n        *~~*~~* SECRET SANTA *~~*~~*\n");
    printf("--------------------------------------------\n");
    printf("Enter number of players: ");
    // Sanitized input
    if(scanf("%2d%c", &n, &term) != 2 || term !='\n') {
        printf("Invalid entry. Exiting.\n");
        return 1;
    }
    struct person * members = malloc(n * sizeof(struct person));
    for(i = 0; i < n; i++) {
        readPerson(&members[i], i);
    }

    derange(members, n);

    // Print?
    printf("--------------------------------------------\n");
    while((pr != 'y') && (pr != 'Y') && (pr != 'n') && (pr != 'N')) {
        printf("Print results? [y/n]: ");
        scanf(" %c", &pr);
        if((pr == 'y') || (pr == 'Y')) {
            for(i = 0; i < n; i++) {
                printPerson(members[i], i);
            }
        } else if((pr != 'n') && (pr != 'N')) {
            printf("Invalid entry. ");
        }
    }

 
    // Email?
    printf("--------------------------------------------\n");
    while((em != 'y') && (em != 'Y') && (em != 'n') && (em != 'N')) {
        printf("Email results? [y/n]: ");
        scanf(" %c", &em);
        if((em == 'y') || (em == 'Y')) {
            for(i = 0; i < n; i++) {
                email(members[i], i);
            }
        } else if((em != 'n') && (em != 'N')) {
            printf("Invalid entry. ");
        }
    }

    free(members);
    return 0;
}

void readPerson(struct person * p, int n) {
    printf("Player %d\n", n+1);
    printf("  Enter player name: ");
    scanf("%29s", p->name);
    printf("  Enter Player email address: ");
    scanf("%29s", p->emailAddress);
}

void derange(struct person * p, int n) {
    srand(time(NULL));  // initialize PRNG
    printf("--------------------------------------------\n");
    printf("Randomizing...\n");
    int guesses[n];
    int i, j, r;
    for(i = 0; i < n; i++) {
        guesses[i] = -1;
    }
    for(i = 0; i < n; i++) {
        r = (rand() % n);
        if(r == i) {
            i = -1;
            printf("  That won't work.\n");
        } else {
            for(j = 0; j < n; j++) {
                if(r == guesses[j]) {
                    i = -1;
		    guesses[j] = -1;
                    printf("  Already used...\n");
                }
            }
            if(i == -1) continue;
            guesses[i] = r;
            for(j = 0; j < 30; j++) {
                p[i].secretSanta[j] = p[r].name[j];
            }
        }
    }
    printf("  FINALLY!\n");    
}

void printPerson(struct person p, int n) {
    printf("--------------------------------------------\n");
    printf("Player %d\n", n+1);
    printf("  Name: %s\n", p.name);
    printf("  Email address: %s\n", p.emailAddress);
    printf("  Secret Santa: %s\n", p.secretSanta);
}

void email(struct person p, int i) {
    char send[512];
    sprintf(send, "echo \"Hello %s! Your Secret Santa this year is %s!\" | mutt -s \"Secret Santa 2018\" %s", p.name, p.secretSanta, p.emailAddress);
    system(send);
}
