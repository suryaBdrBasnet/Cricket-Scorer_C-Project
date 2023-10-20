#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <termios.h>
#include <unistd.h>

#define PASSWORD "bottleOpener"

// imp functions
void adminValidation();
void dataCreate(char matchOver);
int dataCollect();
void displayData();

// utilities functions
void isOver();
void wicketAll();
void secondInning();
void getScore(int *tempRunPtr);
int randomNum();
char *transformLower(char *string);


int score = 0;
int wicket = 0;
int over = 0;
int ball = 0;
int inning = 1;
int totalOver;

int team1run;
char team1[40], team2[40];

// pointers
FILE *appendAndReadFp = NULL;

int main()
{
    char adminAccessUser[40];
    printf("-------- ADMIN ACCESS REQUIRED --------\n");
    printf("Enter your Name: ");
    fgets(adminAccessUser, 40, stdin);
    adminValidation();
    
    appendAndReadFp = fopen("adminLogin.csv", "a");
    fprintf(appendAndReadFp,"Date of Access,Name \n");

    time_t current_time;
    
    time(&current_time);
    char *dateAndTime = ctime(&current_time);  
    dateAndTime[strlen(dateAndTime) -1] = '\0';  

    fprintf(appendAndReadFp, "%s,%s", dateAndTime, adminAccessUser);
    fclose(appendAndReadFp);

    printf("\n------------------ Cricket App ------------------\n\n");
    printf("Enter the Batsmans team Name: ");
    scanf("%s", &team1);
    printf("Enter the Bowlers team Name: ");
    scanf("%s", &team2);
    printf("Enter the total overs: ");
    scanf("%d", &totalOver);

    dataCollect();
    return 0;
}


void adminValidation()
{
    char password[14];
    char ch;

    struct termios old_settings, new_settings;
    tcgetattr(STDIN_FILENO, &old_settings);
    
    new_settings = old_settings; 

    new_settings.c_lflag &= ~(ICANON | ECHO);        
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings); 
    printf("Enter Password:\t");
    for (int i = 0; i < 12; i++)
    {
        ch = getc(stdin);
        password[i] = ch;
        printf("*");
    }
    password[12] = '\0';
    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);

    if (strcmp(password, PASSWORD) != 0)
    {
        printf("\nWrong Password Please Try Again \n\n");
        adminValidation();
    }
    else printf("\n --- USER AUTHORIZED --- \n\n");
}


int dataCollect()
{
    char tempBallC[2];
    int tempBall;
    char ballStatus[2];
    char tempWicketStatus[2];
    int tempRun;

    while (!(strcmp(tempBallC, "0") == 0 || strcmp(tempBallC, "1") == 0))
    {
        printf("\n\n Enter ball (0 for wrong ball) (1 for right ball): ");
        scanf("%s", &tempBallC);
    }
    
    tempBall = atoi(tempBallC);
    
    switch (tempBall)
    {    
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
   
        int randomIndex = randomNum();
        while (!(strcmp(transformLower(ballStatus), "w") == 0 || strcmp(transformLower(ballStatus), "n") == 0))
        {
            printf("\n Enter wide (W) or No Ball(N): ");
            scanf("%s", &ballStatus);
        }

        getScore(&tempRun);

        if (strcmp(ballStatus, "w") == 0)
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

        
    case 1:
    {        
        while (!(strcmp(transformLower(tempWicketStatus), "w") == 0 || strcmp(transformLower(tempWicketStatus), "n") == 0))
        {
            printf("\n (W) for wicket \t (N) for no wicket : ");
            scanf("%s", &tempWicketStatus);
        }
        ball++;

        if (strcmp(transformLower(tempWicketStatus), "w") == 0)
        {
            printf("\n\n Comment:- What a loss for team %s", team1);
            int a = randomNum();
            wicket++;
            wicketAll();
        }

        getScore(&tempRun);   
        isOver();
    }
    break;

    default:
        printf("\n\n Invalid input");
        break;
    }
    
    if (inning > 2)
    {        
        inning--;   
        dataCreate('y');
        inning = 0;
        return 0;
    }

    else
    {
        dataCreate('n');
        return dataCollect();
    }
}

void dataCreate(char matchOver)
{
    long currentPosition;
    appendAndReadFp = fopen("score.txt", "a+");
    
    if (appendAndReadFp == NULL)
    {
        printf("Unable to open the file.\n");
        exit(1);
    }

    fprintf(appendAndReadFp, "\n");
    currentPosition = ftell(appendAndReadFp); 

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


void displayData(long currentPosition)
{
    printf("\n");
    appendAndReadFp = fopen("score.txt", "r");
    if (appendAndReadFp == NULL)
    {
        printf("Unable to open the file.\n");
        exit(1);
    }
    
    fseek(appendAndReadFp, currentPosition, SEEK_SET);

    printf("\n\t--- Displaying Data ---\n\n");
    char ch;
    while ((ch = fgetc(appendAndReadFp)) != EOF)
    {
        putc(ch, stdout);
    }
    
    fclose(appendAndReadFp);
}

void isOver()
{
    if (ball == 6)
    {
        over++;
        ball = 0;
        
        if (over == totalOver)
        {
            inning++;            
            inning == 2 ? secondInning() : 0;
        }
        
    }
}

void wicketAll()
{
    if (wicket == 10)
    {
        inning++;        
        inning == 2 ? secondInning() : 0;
        over = 0;
        score = 0;
        wicket = 0;
    }
}

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

int randomNum()
{
    srand(time(NULL));
    int randomNum = rand() % 3; 
    return randomNum;
}

char *transformLower(char *string)
{
    int i = 0;
    while (string[i] != '\0')
    {
        if (string[i] >= 'A' && string[i] <= 'Z')  string[i] |= 32; 
        i++;
    }
    return string;
}