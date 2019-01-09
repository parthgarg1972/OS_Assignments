#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char const *argv[])
{
    if(argc == 3)
    {
        if(atoi(argv[2]) < 0 || atoi(argv[2]) > 100)
        {
            printf(1,"Error: Invalid priority\n");
            exit();
        }
        change(atoi(argv[1]),atoi(argv[2]));
    }
    else
    {
        printf(1,"set_priority: error\n");
    }
    exit();
}
