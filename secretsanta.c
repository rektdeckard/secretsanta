#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <curl/curl.h>

#define FROM_ADDR   "<mailbot@tobiasfried.com>"
#define FROM_MAIL   "Secret Santa Mailbot " FROM_ADR

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

CURL *curl;

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
            curl_easy_cleanup(curl);
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

    // Create Payload
    time_t curtime;
    time(&curtime);
    char date[80];
    snprintf(date, 80, "Date: %s\r\n", ctime(&curtime));
    char to[80];
    snprintf(to, 80, "To: %s\r\n", p.emailAddress);
    char from[40] = "From: mailbot@tobiasfried.com\r\n";
    char cc[10] = "Cc: \r\n";
    char subject[40] = "Subject: Secret Santa Bot\r\n\r\n";
    char body[160];
    snprintf(body, 160, "Seasons greetings, %s! Your Secret Santa this year is %s.\r\n", p.name, p.secretSanta);

    const char *payload_text[] = {
        date,
        to,
        from,
        cc,
        subject,
        body,
        NULL
    };

    for (int i = 0; payload_text[i]; ++i) {
        const char *ch = payload_text[i];
        while(*ch) {
            putchar(*ch++);
        }
    }

    struct upload_status {
        int lines_read;
    };

    // Initialize curl
    CURLcode res = CURLE_OK;
    struct curl_slist *recipients = NULL;
    struct upload_status upload_ctx;

    upload_ctx.lines_read = 0;

    curl = curl_easy_init();
    if(curl) {
        // Mailserver
        curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.dreamhost.com");
        curl_easy_setopt(curl, CURLOPT_USERNAME, "mailbot@mail.tobiasfried.com");
        curl_easy_setopt(curl, CURLOPT_USERNAME, "nhdquyT6@wniLwD");

        #ifdef SKIP_PEER_VERIFICATION
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        #endif
        #ifdef SKIP_HOSTNAME_VERIFICATION
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        #endif

        // Set mailfrom: address
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM_ADDR);

        // Add recipient
        recipients = curl_slist_append(recipients, p.emailAddress);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        // Specify payload
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        // Send the message
        res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Free list of recipients
        curl_slist_free_all(recipients);
    }

}
