#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#define MAX 1024 //define max size to use constant memory and prevent overflow
enum state {STRKPT_START, SATT, SQUOTE, PRINT_ATT, PRINT_ELEM, SELEM_START, WHTSPC};

int main()
{
  int ch;
  char str[MAX];

//read in from stdin and remove all spaces
for (int k = 0; k < MAX && (ch = getc(stdin)) != EOF;)
{
   if (!(isspace(ch)))
   {
     str[k] = ch;
     k++;
   }
}
    
    char input[MAX]; //pointer for current input
    strcpy(input, str);
    //fgets(input, MAX, stdin);

    enum state curr = STRKPT_START; //initialize to starting state

    int j = 0;
    char type = 'n';

    while (input[j] != '\0')
    {
        switch(curr)
        {
        case STRKPT_START: //search for start of trkpt tag
            if ((input[j] == '<') && (input[j + 1] == 't' || input[j + 1] == 'T') &&
                    (input[j + 2] == 'r' || input[j + 2] == 'R') && (input[j + 3] == 'k' || input[j + 3] == 'K') &&
                    (input[j + 4] == 'p' || input[j + 4] == 'P') && (input[j + 5] == 't' || input[j + 5] == 'T'))
            {
                curr = SATT; //switch state to start searching for relevant attribute values
                j = j + 6; //skip over trkpt
            }
            else
            {
                j++;
            }
            break;

        case SATT: //search for relevant attributes lat and lon
            if (input[j] == '>') //reached the end of trkpt tag
            {
                curr =  SELEM_START; //switch states to search for relevant elements ele and time
                j++;
            }
            else if (input[j] == 'l' && input[j + 1] == 'a' && input[j + 2] == 't')
            {
                curr =  SQUOTE; //switch states to look for opening quotes of lat
                j = j + 3; //skip over 'lat'
            }
            else if (input[j] == 'l' && input[j + 1] == 'o' && input[j + 2] == 'n')
            {
                curr = SQUOTE; //switch states to look for opening quotes of lon
                j = j + 3; //skip over 'lon'
            }
            else
            {
                j++;
            }
            break;

        case SQUOTE:
            if (input[j] == '"' || input[j] == '\'')
            {
                curr = PRINT_ATT; //if reach open quotes, begin printing out attribute value
            }
            j++;
            break;

        case PRINT_ATT:
            if (input[j] == '"' || input[j] == '\'')
            {
                printf(",");
                curr =  SATT; //switch back to search for attributes
            }
            else
            {
                printf("%c", input[j]);
            }
            j++;
            break;

        case SELEM_START:
            if ((input[j] == '<') && (input[j + 1] == 'e' || input[j + 1] == 'E') &&
                    (input[j + 2] == 'l' || input[j + 2] == 'L') && (input[j + 3] == 'e' || input[j + 3] == 'E'))
            {
                type = 'e';
                curr =  WHTSPC;//state that looks for > after any whitespace
                j = j + 4;
            }
            else if ((input[j] == '<') && (input[j + 1] == 't' || input[j + 1] == 'T') &&
                     (input[j + 2] == 'i' || input[j + 2] == 'I') && (input[j + 3] == 'm' || input[j + 3] == 'M')
                     && (input[j + 4] == 'e' || input[j + 4] == 'E'))
            {
                type = 't';
                curr = WHTSPC;//state that looks for > after any whitespace
                j = j + 5;
            }
            else
            {
              j++;
            }
            break;

        case WHTSPC:
            if(input[j] == '>')
            {
                curr = PRINT_ELEM;
            }
              j++;
            break;

        case PRINT_ELEM:
            if(input[j] == '<' && type == 'e')
            {

                printf(",");
                curr = SELEM_START;
            }
            else if (input[j] == '<' && type == 't')
            {
                printf("\n");
                curr = STRKPT_START;
            }
            else if (input[j] == ',' && type == 't')
            {
                printf("&comma;");
                j++;
            }
            else
            {
                printf("%c", input[j]);
                j++;
            }
            
            break;
        }
    }
}
