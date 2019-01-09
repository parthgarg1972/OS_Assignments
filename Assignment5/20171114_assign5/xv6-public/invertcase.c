#include "types.h"
#include "stat.h"
#include "user.h"

void invertcase(char *word)
{
    int i;
    
    for(i=0;word[i]!='\0';i++)
    {
        if(word[i]>=97 && word[i]<=122)
        {
            word[i]=word[i]-32;
        }
        else if(word[i]>=65 && word[i]<=90)
        {
            word[i]=word[i]+32;
        }
    }
    printf(1,"%s ",word);
}

int main(int argc, char *argv[])
{
    int i;

    if(argc<=1)
    {
        printf(1,"invertcase: error\n");
        exit();
    }

    for(i=1;i<argc;i++)
    {
        invertcase(argv[i]);
    }
    printf(1,"\n");
    exit();

}
