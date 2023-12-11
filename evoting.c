#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void displayResult() {
    int winnerCid = getWinner();

    if (winnerCid != -1) {
        printf("\nWinner is Candidate %d with %d votes\n", winnerCid, findCandidateVotes(winnerCid));
    } else {
        printf("\nIt's a tie!\n");
    }

    printf("\nFull Result:\n");
    struct CandidateNode* current = election.candidates;
    int totalVotedNow = 0;

    while (current != NULL) {
        totalVotedNow += current->votes;
        printf("Candidate %d -> %d votes\n", current->cid, current->votes);
        current = current->next;
    }

    printf("\nVoting Percentage: %d %%\n\n", (totalVotedNow * 100) / election.totalVoters);
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

void adminPanel() {
    int choice;
    while (1) {
        printf("\nAdmin Panel:\n");
        printf("1. Display Candidates\n");
        printf("2. Display Election Result\n");
        printf("3. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayCandidates();
                break;
            case 2:
                displayResult();
                break;
            case 3:
                return;
            default:
                printf("Invalid option. Please try again.\n");
        }
    }
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
        printf("\nThanks for your vote! (Press Enter)");
        getchar();  // Consume the newline character left in the buffer
    }
}

int main() {
    initElection();
    addCandidate(1, "Candidate 1");
    addCandidate(2, "Candidate 2");

    while (1) {
        printf("\n\t\t\t   1.Student panel \n\t\t\t   2.Admin panel \n\t\t\t   3.Exit \n\t\t\t   Option:");
        char input;
        scanf(" %c", &input);

        switch (input) {
            case '1':
                studentPanel();
                break;
            case '2':
                adminPanel();
                break;
            case '3':
                return 0;
            default:
                printf("\nInvalid option");
        }
    }
    return 0;
}

