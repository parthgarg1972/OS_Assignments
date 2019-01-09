#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
//long long int *a;
//long long int temp[1000009];
long long int *a;

typedef struct{
    long long int *array;
    long long int left;
    long long int right;
    
} thread_data_t;

int number_threads;
pthread_mutex_t lock;

void swap(long long int i,long long int j)
{
    a[i]=a[i]^a[j];
    a[j]=a[i]^a[j];
    a[i]=a[i]^a[j];
}

void selection_sort(long long int *a,long long int l,long long int r)
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


long long int merge(long long int *a,long long int low,long long int mid,long long int high)
{
    //printf("Merging %lld to %lld\n",low,high);
	long long int i,j,k=0,g;
	i=low;
	j=mid+1;
    long long *temp=(long long int *)malloc((high-low+1)*sizeof(long long int));
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
    free(temp);

}

void *mergesort_threaded(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;

    long long int low=data->left;
    long long int high=data->right;
   
    if((high-low+1)<=5)
    {
        //printf("Calling selection sort(%lld,%lld).\n",low,high);
        selection_sort(data->array,low,high);
        
    }
    else
    {
        long long int mid=(low+high)/2;

        thread_data_t data_thread0;
        data_thread0.left=low;
        data_thread0.right=mid;
        data_thread0.array=data->array;

        

        pthread_t thread_0;
        int err = pthread_create(&thread_0,NULL,mergesort_threaded,&data_thread0);

        int created_thread_0 = 1;
        if(err)
        {
            perror("Failed to create thread\n");
            
        }

        thread_data_t data_thread1;

        data_thread1.left=mid+1;
        data_thread1.right=high;
        data_thread1.array=data->array;

        

        pthread_t thread_1;
        err = pthread_create(&thread_1,NULL,mergesort_threaded,&data_thread1);

        int created_thread_1=1;
        if(err)
        {
            perror("Failed to create thread\n");
            
            
        }

        if(created_thread_0==1)
        {
            pthread_join(thread_0,NULL);
        }
        if(created_thread_1==1)
        {
            pthread_join(thread_1,NULL);
        }

        merge(data->array,low,mid,high);

    }
    pthread_exit(NULL);
    return NULL;

}

void mergesort(long long int *a,long long int low,long long int high)
{
    thread_data_t data;
    data.array=a;
    data.left=low;
    data.right=high;   

    pthread_t thread;
    int p = pthread_create(&thread,NULL,mergesort_threaded,&data);

    if(p)
    {
        perror("Failed to create thread");
        
    }

    pthread_join(thread,NULL);
    return;

}

/*long long int mergesort(long long int low,long long high)
{
	long long int mid;
	if(low!=high)
	{
		mid=(low+high)/2;
		mergesort(low,mid);
		mergesort(mid+1,high);
		merge(low,mid,high);
	}
}*/

int main()
{
	long long int i,j,k,n;
	scanf("%lld",&n);
    a=(long long int *)malloc(n*sizeof(long long int));
	for(i=0;i<n;i++)
	{
		scanf("%lld",&a[i]);
	}
	mergesort(a,0,n-1);

	for(i=0;i<n;i++)
	{
		printf("%lld\n",a[i]);
	}
    free(a);
    pthread_mutex_destroy(&lock);
}