#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char const *argv[])
{
    if(argc == 1)
    {
        check();
        exit();
    }
    else
    {
        printf(1,"ps: error\n");
        exit();
    }
}
