#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h> // for seeding

// Important function
void dataCreate(char matchOver);
int dataCollect();
void displayData();

// utilities functiion
void isOver();
void wicketAll();
void secondInning();
void getScore(int *tempRunPtr);
int randomNum();

// global variables
int score = 0;
int wicket = 0;
int over = 0;
int ball = 0;
int inning = 1;
int totalOver;

int team1run;

char team1[40], team2[40];

// file pointers
FILE *appendAndReadFp = NULL;

// main function [function starts here]
int main()
{
    printf("\n\n------------------ Cricket App ------------------\n\n");
    printf("Enter the Batsmans team Name: ");
    scanf("%s", &team1);
    printf("Enter the Bowlers team Name: ");
    scanf("%s", &team2);
    printf("Enter the total overs: ");
    scanf("%d", &totalOver);

    // runs a function named dataCollect, which runs recursively
    dataCollect();
    return 0;
}

// Cricket data collector (takes data recursively)
int dataCollect()
{
    // helper variable for state management
    char tempBallC[2];
    // to state manage
    int tempBall;
    // for wide / no ball of [wrong ball]
    char ballStatus[2];
    // for wicket or not
    char tempWicketStatus[2];
    // current run store
    int tempRun;

    // ask user to insert value
    while (!(strcmp(tempBallC, "0") == 0 || strcmp(tempBallC, "1") == 0))
    {
        printf("\n\n Enter ball (0 for wrong ball) (1 for right ball): ");
        scanf("%s", &tempBallC);
    }
    // converting string to num as switch statement won't take string as an parameter.
    // Alternatively, using characters would have done the trick.
    tempBall = atoi(tempBallC);

    // switching based on values
    switch (tempBall)
    {
    // if 0 was pressed.
    case 0:
    {
        char wideBallComment[][100] = {
            "!!! That one slipped way down the leg side. Wide ball called by the umpire !!!",
            "!!! The bowler loses control there, and it's a wide ball. Extra run for the batting side !!!",
            "!!! A wide delivery outside off-stump. The batsman doesn't have to play at that !!!"};

        char noBallComment[][105] = {
            "!!! Oh, that's a costly mistake by the bowler. No-ball! The batsman gets a free hit now !!!",
            "!!! The bowler overstepped, and it's a no-ball. The batsman will be looking to capitalize on this !!!",
            "!!! A no-ball! That's not what the bowling side needed. Extra delivery and a free hit coming up !!!"};

        // generating random index for comment
        int randomIndex = randomNum();
        while (!(strcmp(strlwr(ballStatus), "w") == 0 || strcmp(strlwr(ballStatus), "n") == 0))
        {
            printf("\n Enter wide (W) or No Ball(N): ");
            scanf("%s", &ballStatus);
        }

        getScore(&tempRun);

        if (strcmp(ballStatus, "W") == 0)
        {
            printf("\n\n COMMENT:- %s", wideBallComment[randomIndex]);
        }
        else
        {
            printf("\n\n COMMENT:- %s", noBallComment[randomIndex]);
        }
        printf("\n\n");
    }
    break;

        // if 1 was pressed.
    case 1:
    {
        // validating input
        while (!(strcmp(strlwr(tempWicketStatus), "w") == 0 || strcmp(strlwr(tempWicketStatus), "n") == 0))
        {
            printf("\n (W) for wicket \t (N) for no wicket : ");
            scanf("%s", &tempWicketStatus);
        }
        ball++;

        if (strcmp(strlwr(tempWicketStatus), "w") == 0)
        {
            printf("\n\n Comment:- What a loss for team %s", team1);
            int a = randomNum();
            wicket++;
            wicketAll();
        }

        getScore(&tempRun);
        // check if the over ended
        isOver();
    }
    break;

    // if neither, However getting to this statement is impossible as we have validated input
    default:
        printf("\n\n Invalid input");
        break;
    }

    // if 2nd inning ended
    if (inning > 2)
    {
        // when 2nd inning ends, 'inning' will increased by 1 to ensure inning ended.
        // But we can't display 3rd inning in our page so inning will be decreased by 1(meaning inning now is 2);
        inning--;
        // one last time data Creation
        dataCreate('y');

        // resetting inning value
        inning = 0;
        // this whole function ends
        return 0;
    }

    // if inning is still running
    else
    {
        dataCreate('n');
        return dataCollect();
    }
}

// creates a js (module) file containing all the cricket data for displaying in Html site.
void dataCreate(char matchOver)
{
    long currentPosition;
    appendAndReadFp = fopen("score.txt", "a+");

    // Check if the file was opened successfully
    if (appendAndReadFp == NULL)
    {
        printf("Unable to open the file.\n");
        exit(1);
    }

    fprintf(appendAndReadFp, "\n");
    currentPosition = ftell(appendAndReadFp); // ftell Basically stores pointer position, - used in reading appended data

    fprintf(appendAndReadFp, "\t Current Inning ----->  %d\n", inning);
    fprintf(appendAndReadFp, "\t Total Over To play ->  %d \n", totalOver);
    fprintf(appendAndReadFp, "\t %s (batsman) Vs %s (bowler) \n", team1, team2);
    fprintf(appendAndReadFp, "\t %d Runs In %d.%d over with %d wickets \n", score, over, ball, wicket);

    if (matchOver == 'y')
    {
        fprintf(appendAndReadFp, "\n\t --------------- Match Result ---------------  \n");
        if (team1run > score)
        {
            fprintf(appendAndReadFp, "\n\t %s Won the match with %d runs \n", team2, team1run - score);
        }
        else if (team1run == score)
        {
            fprintf(appendAndReadFp, "\n\t Match Draw \n");
        }
        else
        {
            fprintf(appendAndReadFp, "\n\t %s Won the match with %d wickets \n", team1, 10 - wicket);
        }
        fprintf(appendAndReadFp, "\n\t --------------------------------------------  \n");
    }

    fclose(appendAndReadFp);

    displayData(currentPosition);
}

// # display function
void displayData(long currentPosition)
{
    printf("\n");
    appendAndReadFp = fopen("score.txt", "r");
    if (appendAndReadFp == NULL)
    {
        printf("Unable to open the file.\n");
        exit(1);
    }

    // Set the file pointer to the stored position
    fseek(appendAndReadFp, currentPosition, SEEK_SET);

    printf("\n\t--- Displaying Data ---\n\n");
    char ch;
    while ((ch = fgetc(appendAndReadFp)) != EOF)
    {
        putc(ch, stdout);
    }
    // Close the file when done
    fclose(appendAndReadFp);
}

// # utility function()
// checks if over ends
void isOver()
{
    if (ball == 6)
    {
        over++;
        ball = 0;
        // has over reached maximum over (e.g. 20 over in T20 match Or 50 over in ODI match)
        if (over == totalOver)
        {
            inning++;

            // if 2nd inning began then run  secondInning() :-[swap teams name]
            inning == 2 ? secondInning() : 0;
        }
        // ball = 0;
    }
}

// checks for wicket
void wicketAll()
{
    if (wicket == 10)
    {
        inning++;
        // if 2nd inning began then run  secondInning() :-[swap teams name]
        inning == 2 ? secondInning() : 0;
        over = 0;
        score = 0;
        wicket = 0;
    }
}

// second Inning match function
void secondInning()
{
    dataCreate('n');
    char temp[20];
    strcpy(temp, team1);
    strcpy(team1, team2);
    strcpy(team2, temp);

    printf("\n\n----------Second Inning Started----------\n\n");

    team1run = score;
    over = 0;
    score = 0;
    wicket = 0;

    printf("%d", team1run);
}

void getScore(int *tempRunPtr)
{
    printf("\n Run scored: ");
    scanf("%d", tempRunPtr);

    if (*tempRunPtr > 7)
    {
        printf("\n Invalid run Entered, Enter Run scored:) -> ");
        scanf("%d", tempRunPtr);
    }
    score += *tempRunPtr;
}

// Generate Random Number
int randomNum()
{
    srand(time(NULL));
    int randomNum = rand() % 3; // 0-2
    return randomNum;
}