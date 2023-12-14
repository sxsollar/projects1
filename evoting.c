#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int getWinner();
int findCandidateVotes(int cid);
void displayElectionInfoFromFile();

struct CandidateNode {
    int cid;
    char cname[20];
    int votes;
    struct CandidateNode* next;
};

struct Election {
    int year;
    char branch[6];
    int totalVoters;
    struct CandidateNode* candidates;
};

struct StudentNode {
    char userID[15];
    char voteInput;
    struct StudentNode* next;
};

struct Election election;
struct StudentNode* votedStudents = NULL;

void initElection() {
    printf("\nNew Election Initiation:\n");
    printf("\nElections for which Year: ");
    scanf("%d", &election.year);
    printf("Enter branch code:");
    scanf("%s", election.branch);
    printf("Enter max roll no.:");
    scanf("%d", &election.totalVoters);
    election.candidates = NULL;
}

void addCandidate(int cid, const char* cname) {
    struct CandidateNode* newCandidate = (struct CandidateNode*)malloc(sizeof(struct CandidateNode));
    newCandidate->cid = cid;
    strcpy(newCandidate->cname, cname);
    newCandidate->votes = 0;
    newCandidate->next = election.candidates;
    election.candidates = newCandidate;
}

void displayCandidates() {
    printf("List of Candidates:\n");
    struct CandidateNode* current = election.candidates;

    while (current != NULL) {
        printf("%d. %s\n", current->cid, current->cname);
        current = current->next;
    }
}

void vote(char userID[15], char voteInput) {
    struct StudentNode* newVote = (struct StudentNode*)malloc(sizeof(struct StudentNode));
    strcpy(newVote->userID, userID);
    newVote->voteInput = voteInput;
    newVote->next = votedStudents;
    votedStudents = newVote;

    struct CandidateNode* current = election.candidates;
    while (current != NULL) {
        if (current->cid == (voteInput - '0')) {
            current->votes++;
            break;
        }
        current = current->next;
    }
}

const char* findCandidateName(int cid) {
    struct CandidateNode* current = election.candidates;

    while (current != NULL) {
        if (current->cid == cid) {
            return current->cname;
        }
        current = current->next;
    }

    return "Unknown";
}

int findCandidateVotes(int cid) {
    struct CandidateNode* current = election.candidates;

    while (current != NULL) {
        if (current->cid == cid) {
            return current->votes;
        }
        current = current->next;
    }

    return -1; // Candidate not found
}

int getWinner() {
    int maxVotes = -1;
    int winnerCid = -1;

    struct CandidateNode* current = election.candidates;

    while (current != NULL) {
        if (current->votes > maxVotes) {
            winnerCid = current->cid;
            maxVotes = current->votes;
        } else if (current->votes == maxVotes) {
            // It's a tie
            return -1;
        }
        current = current->next;
    }

    return winnerCid;
}

void studentPanel() {
    char userID[15];
    char voteInput;

    while (1) {
        printf("\n\nTo exit, press 0");
        printf("\nEnter user ID: ");
        scanf("%s", userID);

        if (strcmp(userID, "0") == 0) {
            return;  // Exit if the user enters '0'
        }

        displayCandidates();

        printf("\nYour Vote (Enter Number): ");
        scanf(" %c", &voteInput);

        if (voteInput < '1' || voteInput > '0' + election.totalVoters) {
            printf("\nInvalid Vote (Press Enter)");
            getchar();
            continue;
        }

        vote(userID, voteInput);
        printf("\nThanks for your vote!");
        getchar();  // Consume the newline character left in the buffer
    }
}

void writeElectionToFile() {
    FILE* file = fopen("C:/Users/admin/OneDrive/Desktop/project s1/election_info.txt", "w");
    if (file == NULL) {
        perror("Error opening file for writing election information");
        return;
    }

    fprintf(file, "Year: %d\n", election.year);
    fprintf(file, "Branch: %s\n", election.branch);
    fprintf(file, "Total Voters: %d\n\n", election.totalVoters);

    struct CandidateNode* currentCandidate = election.candidates;
    while (currentCandidate != NULL) {
        fprintf(file, "Candidate %d: %s - Votes: %d\n", currentCandidate->cid, currentCandidate->cname, currentCandidate->votes);
        currentCandidate = currentCandidate->next;
    }

    if (fclose(file) != 0) {
        perror("Error closing file");
    }
}

void displayElectionInfoFromFile() {
    FILE* file = fopen("C:/Users/admin/OneDrive/Desktop/project s1/election_info.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading election information.\n");
        return;
    }
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Remove newline characters and display each line
        strtok(buffer, "\n");
        printf("%s\n", buffer);
    }
    if (getWinner() != -1) {
        printf("\nWinner: Candidate %d - %s\n", getWinner(), findCandidateName(getWinner()));
    } else {
        printf("\nIt's a tie!\n");
    }
    
    fclose(file);
}

int main() {
    initElection();
    addCandidate(1, "Petros");
    addCandidate(2, "Poghos");

    while (1) {
        printf("\n\t\t\t   1.Student panel \n\t\t\t   2.Admin panel \n\t\t\t   3.Exit \n\t\t\t   Option:");
        char input;
        scanf(" %c", &input);

        switch (input) {
            case '1':
                studentPanel();
                break;
            case '2':
                writeElectionToFile();
                displayElectionInfoFromFile();
                break;
            case '3':
                {
                struct CandidateNode* currentCandidate = election.candidates;
                while (currentCandidate != NULL) {
                    struct CandidateNode* nextCandidate = currentCandidate->next;
                    free(currentCandidate);
                    currentCandidate = nextCandidate;  
                } 
                }
                return 0;
            default:
                printf("\nInvalid option");
        }
    }
    return 0;
}
