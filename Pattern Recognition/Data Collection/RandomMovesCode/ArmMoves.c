/*
Generates a random list of moves for data collection, with a countdown between each one of 10 seconds
Then stores the list into a text file called "ArmList.txt"
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <time.h>

const char* result(int input);
void printList(int list[]);

void main()
{
    srand(time(0));

    int r=-2;
    int temp=-1;
    int next =-1;
    int list[30] = { };

    for(int i=-1;i<30;i++)
    {
        do{
            temp = rand() % (7);

        } while(r==temp);

        if(i!=-1)
        {
            if(i==29)
            {
                printf("Final Move:\n");
            }
            printf("Now: %s\n",result(r));
            list[i] = r;
        }

        if(i!=29)
        {
            next = temp;
            printf("Next: %s\n",result(next));
            r=next;
        }


        for(int j=10;j>0;j--)
        {
            printf("%d \n",j);
            Sleep(1000);
        }
        system("cls");
    }
    printList(list);

}


const char* result(int input)
{
    char *output = " ";
    switch(input)
    {
        case 0:
        output = "Open Hand";
        break;

        case 1:
        output = "Close Hand";
        break;

        case 2:
        output = "Supination";
        break;

        case 3:
        output = "Pronation";
        break;

        case 4:
        output = "Elbow Extension";
        break;

        case 5:
        output = "Elbow Flex";
        break;

        case 6:
        output = "Rest";
        break;
    }
    return output;
}


void printList(int list[])
{
    FILE *fw = fopen("ArmList.txt","w");
    for (int i=0;i<30;i++)
    {
        fprintf(fw,"%d: %d - %s\n",i+1,list[i]+1,result(list[i]));
    }
}
