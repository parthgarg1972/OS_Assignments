#include <stdio.h>
long long int a[100009];
long long int temp[100009];
long long int n;

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

long long int mergesort(long long int low,long long high)
{
	long long int mid;
	if(low!=high)
	{
		mid=(low+high)/2;
		mergesort(low,mid);
		mergesort(mid+1,high);
		merge(low,mid,high);
	}
}

int main()
{
	long long int i,j,k;
	scanf("%lld",&n);
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
}