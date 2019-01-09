#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
    int k, n = 1, id,temp=20;
    double x=0, z;

    if(argc < 2)
        printf(1,"hackbench: Error\n");
    else
        n = atoi(argv[1]);

    x = 0;
    id = 0;
    int start_time = uptime();
    for(k=0; k<n; k++)
    {
        id = fork();
        if(id < 0)
            printf(1, "%d failed in fork!\n", getpid());
        else if(id > 0)
        {
            printf(1, "Parent %d creating child %d\n", getpid(), id);
        }
        else
        {
            change(getpid(),temp);
            printf(1, "Child %d created\n", getpid());
            for(z=0;z<800000.0;z+=0.01)
                x = x + 3.14*69.69;
            break;
        }
        temp--;
    }
    for(k=0;k<n;k++)
      wait();

    int finish_time = uptime();

    printf(1,"Total time taken = %d ticks\n",finish_time-start_time);

    exit();
}
