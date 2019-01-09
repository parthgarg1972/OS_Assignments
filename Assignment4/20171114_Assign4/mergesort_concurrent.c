#include <stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/types.h>
#include <unistd.h>
#include<sys/stat.h>
#include<sys/wait.h>
long long int *a;
long long int temp[10000009];
long long int n;
int shared_memory_id;

void swap(long long int i,long long int j)
{
    a[i]=a[i]^a[j];
    a[j]=a[i]^a[j];
    a[i]=a[i]^a[j];
}


void selection_sort(int l, int r)
{
  int mi,temp;
  for(int i=l; i<=r; i++)
  {
    mi = i;
    for(int j=i+1; j<=r; j++)
      if(a[mi]>a[j])
        mi = j;

    if(a[mi]!=a[i])
    {        
        swap(mi,i);
    }    

    
  }
}

long long int merge(long long int low,long long int mid,long long int high)
{
	long long int i,j,k=0,g;
	i=low;
	j=mid+1;

	while(i<=mid&&j<=high)
	{
		if(a[i]<=a[j])
		{
			temp[k]=a[i];
			i++;
			k++;
		}
		else
		{
			temp[k]=a[j];
			j++;
			k++;
		}
	}
	while(i<=mid)
	{
		temp[k]=a[i];
		k++;
		i++;
	}
	while(j<=high)
	{
		temp[k]=a[j];
		k++;
		j++;
	}
	for(g=0;g<k;g++)
	{
		a[low+g]=temp[g];
	}

}

void mergesort(long long int low,long long high)
{
    if((high-low)<=5)
    {
        selection_sort(low,high);
        return;
    }
	long long int mid;
    pid_t pid_left, pid_right;
	if(low!=high)
	{
		mid=(low+high)/2;
		/*mergesort(low,mid);
		mergesort(mid+1,high);
		merge(low,mid,high);*/

        pid_left=fork();

        if(pid_left<0)
        {
            perror("ERROR");
            _exit(1);
        }
        else if(pid_left==0)
        {
            mergesort(low,mid);
            exit(1);
        }
        else
        {
            pid_right=fork();

            if(pid_right<0)
            {
                perror("ERROR");
                exit(1);
            }
            else if(pid_right==0)
            {
                mergesort(mid+1,high);
                exit(0);
            }
            else
            {
                int status;
                waitpid(pid_left, &status, 0);
                waitpid(pid_right, &status, 0);
                merge(low, mid, high);

            }
        }
	}
}

int main()
{
	long long int i,j,k;
	scanf("%lld",&n);

    shared_memory_id=shmget(IPC_PRIVATE,(n+5)*sizeof(long long int),IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    if(shared_memory_id<0)
    {
        perror("shmget");
        exit(1);
    }

    a=shmat(shared_memory_id,NULL,0);
    if(!(a))
    {
        perror("shmat");
        exit(1);
    }




	for(i=0;i<n;i++)
	{
		scanf("%lld",&a[i]);
	}
	mergesort(0,n-1);

	for(i=0;i<n;i++)
	{
		printf("%lld\n",a[i]);
	}
    //printf("\n");

    if (shmdt(a) == -1)
    {
       perror("shmdt");
       exit(1);
    }
    if (shmctl(shared_memory_id, IPC_RMID, NULL) == -1)
    {
       perror("shmctl");
       exit(1);
    }
    return 0;
}