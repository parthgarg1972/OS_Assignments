#include<stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
char ch1[10000000],ch2[10000000];
char extra_1[10000000],extra_2[10000000];

int main(int argc, char *argv[])
{
	long long int i,j,k,l,m,n,count=0,limit=10000000;
	
	long long int f1,f2,position;
	size_t size_read;

	struct stat status;

	if(argc != 2)
	{
		write(1,"Usage: ./a.out [filename]\n",26);
		return 0;
	}
	

	if(stat("./Assignment",&status)<0)
	{
		mkdir("./Assignment",0700);
	}

	
	if(f1=open(argv[1],O_RDONLY))
	{	
		f2=open("./Assignment/final.txt",O_WRONLY|O_CREAT,0600);
		position=lseek(f1,0,SEEK_END);
	

		i=position;

		while(i>limit)
		{
			lseek(f1,i-limit,SEEK_SET);
			read(f1,&ch1,limit);

			for(j=0;j<limit;j++)
			{
				if(ch1[j]>=97 && ch1[j]<=122)
				{
					ch1[j]-=32;
				}
				else if(ch1[j]>64 && ch1[j]<91)
				{
					ch1[j]+=32;
				}
				ch2[limit-j-1] = ch1[j];
			}

			write(f2,&ch2,limit);

			i=i-limit;
		}
	

		if(i!=0)
		{
			
			lseek(f1,0,SEEK_SET);
			read(f1,&extra_1,i);

			for(j=0;j<i;j++)
			{
				if(extra_1[j]>=97 && extra_1[j]<=122)
				{
					extra_1[j]-=32;
				}
				else if(extra_1[j]>=65 && extra_1[j]<=90)
				{
					extra_1[j]+=32;
				}

				extra_2[i-j-1]=extra_1[j];
			}
			write(f2,&extra_2,i);
		}
	}

	close(f1);
	close(f2);
	return 0;

	

}
