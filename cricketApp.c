#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

// Important function
void JsDataCreate();
int dataCollect();


// utilities functiion
void isOver();
void wicketAll();
void secondInning();

// global functions

int score = 0;
int wicket = 0;
int over = 0;
int ball = 0;
int inning = 1;
int totalOver = 20;

char team1[40], team2[40];
FILE *fp = NULL;

// main function [function starts here]
int main()
{
    printf("Enter the batsmans team Name:");
    scanf("%s", &team1);
    printf("Enter the bowlers team Name:");
    scanf("%s", &team2);

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

    printf("\n\n enter ball (0 for wrong ball) (1 for right ball): ");
    scanf("%s", &tempBallC);

    // if value enter is invalid, ask user to try again
    while (!(strcmp(tempBallC, "0") == 0 || strcmp(tempBallC, "1") == 0))
    {
        printf("\n\n enter ball (0 for wrong ball) (1 for right ball): ");
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
        printf("\n wide (W) or No Ball(N): ");
        scanf("%s", &ballStatus);
        

        while (!(strcmp(ballStatus, "W") == 0 || strcmp(ballStatus, "N") == 0))
        {
            printf("\n wide (W) or No Ball(N): ");
            scanf("%s", &ballStatus);
        }

        printf(" Run scored: ");
        scanf("%d", &tempRun);
        score += tempRun;
    }
    break;

// if 1 was pressed.
    case 1:
    {
        printf("\n (W) for wicket \t (N) for no wicket : ");
        scanf("%s", &tempWicketStatus);
        ball++;
        // check if the over ended
        isOver();

        // validating input
        while (!(strcmp(tempWicketStatus, "W") == 0 || strcmp(tempWicketStatus, "N") == 0))
        {
            printf("\n (W) for wicket \t (N) for no wicket : ");
            scanf("%s", &tempWicketStatus);
        }

        if (strcmp(tempWicketStatus, "W") == 0)
        {
            wicket++;
            wicketAll();
        }

        printf(" Run scored: ");
        scanf("%d", &tempRun);
        score += tempRun;
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
        JsDataCreate();
        
        // resetting inning value 
        inning = 0;
        // this whole function ends
        return 0;
    }

    // if inning is still running
    else
    {
        JsDataCreate();
        return dataCollect(inning);
    }
}


// creates a js (module) file containing all the cricket data for displaying in Html site.
void JsDataCreate()
{
    fp = fopen("data.js", "w");
    fprintf(fp, "export class data{\n");
    fprintf(fp, "\tconstructor(){\n");
    fprintf(fp, "\t\t this.score = %d;\n", score);
    fprintf(fp, "\t\t this.wicket = %d;\n", wicket);
    fprintf(fp, "\t\t this.over = %d;\n", over);
    fprintf(fp, "\t\t this.ball = %d;\n", ball);
    fprintf(fp, "\t\t this.team1 = \"%s\";\n", team1);
    fprintf(fp, "\t\t this.team2 = \"%s\";\n", team2);
    fprintf(fp, "\t\t this.inning = %d;\n", inning);
    fprintf(fp, "\t\t this.totalOver = %d;\n", totalOver);
    fprintf(fp, "\t\t }\n");
    fprintf(fp, "\t }");
    fclose(fp);
}



// # utility function()

// checks if over ends
void isOver()
{
    if (ball == 6)
    {
        over++;
        // has over reached maximum over (e.g. 20 over in T20 match Or 50 over in ODI match)
        if (over == totalOver)
        {
            inning++;

            // ternary operator (cool 1 liner if else statement)
            // if 2nd inning began then run  secondInning() :-[swap teams name]
            inning == 2 ? secondInning() : 0;
            over = 0;
        }
        ball = 0;
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
    }
}

// second Inning match function
void secondInning()
{
    char temp[20];
    strcpy(temp, team1);
    strcpy(team1, team2);
    strcpy(team2, temp);
}