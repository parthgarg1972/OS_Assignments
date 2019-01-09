#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

char ch1[10000000],ch2[10000000],original[10000000];
char extra_1[10000000],extra_2[10000000],extra_3[10000000];

long long int checker(long long int f1,long long int f2)
{
	long long int i,count_1=0,flag=0,count_2=0,limit=1000000,left,i_2,j,a;
	size_t size_read;
	
	count_1=lseek(f1,0,SEEK_END);
	count_2=lseek(f2,0,SEEK_END);
	//count=ftell(f1);
	
	if(count_1!=count_2)
	{
		//printf("hello\n");
		return 1;
	}
	i=count_1;

	
	lseek(f2,0,SEEK_SET);

	i_2=limit;
	while(i>limit)
	{
		read(f2,&original,limit);
		lseek(f2,i_2,0);
		lseek(f1,i-limit,SEEK_SET);
		read(f1,&ch1,limit);

		for(j=0;j<limit;j++)
		{
			if(ch1[j]>=97 && ch1[j]<=122)
			{
				ch1[j]-=32;
			}
			else if(ch1[j]>=65 && ch1[j]<=90)
			{
				ch1[j]+=32;
			}
			ch2[limit-j-1] = ch1[j];
		}

		for(a=0;a<limit;a++)
		{
			if(ch2[a]!=original[a])
			{
				return 1;
			}
		}



		i=i-limit;
		i_2=i_2+limit;
	}
	//printf("hello\n");

	left=count_1-i_2+limit;
	lseek(f2,-left,SEEK_END);
	read(f2,&extra_1,left);
	lseek(f1,0,0);
	read(f1,&extra_2,left);


	for(i=0;i<left;i++)
	{
		if(97 <= extra_2[i] && extra_2[i] <= 122)
		{
			extra_2[i]-=32;
		}
		else if(65 <= extra_2[i] && extra_2[i] <= 90)
		{
			extra_2[i]+=32;
		}
		extra_3[left-i-1] = extra_2[i];
	}

	for(i=0;i<left;i++)
	{
		if(extra_3[i]!=extra_1[i])
		{
			return 1;
		}
	}


	return 0;
}

int main(int argc, char *argv[])
{
	long long int i,j,k,l,m,n,p,flag;

	struct stat status;
	struct stat f_status;
	struct stat sym_link;
	long long f1,f2;

	symlink("./Assignment/final.txt","symlink");

	if(stat("./Assignment",&status)==0)
	{
		write(1,"Checking whether the directory has been created: Yes\n",53);
	}
	else
	{
		write(1,"Checking whether the directory has been created: No\n",53);
	}

	if(stat("./Assignment/final.txt",&f_status)==0)
	{
		write(1,"Checking whether the file has been created: Yes\n",48);
	}
	else
	{
		write(1,"Checking whether the file has been created: No\n",47);
	}

	if(stat("symlink",&sym_link)==0)
	{

		if(S_ISLNK(sym_link.st_mode)==0)
		{
			//write(1,str[4],strlen(str[4]));
			write(1,"Checking whether the symlink has been created: Yes\n",52);
		}
		else
		{
			//write(1,str[5],strlen(str[5]));
			write(1,"Checking whether the symlink has been created: No\n",51);
		}
	}

	if(stat("./Assignment",&status)==0 && stat("./Assignment/final.txt",&f_status)==0)
	{
		f1=open("./Assignment/final.txt",O_RDONLY);
		f2=open(argv[1],O_RDONLY);

		flag=checker(f1,f2);

		if(flag==0)
		{
			write(1,"Checking whether file contents have been reversed and case-inverted: Yes\n",73);
		}
		else if(flag==1)
		{
			write(1,"Checking whether file contents have been reversed and case-inverted: No\n",72);
		}
	}
	else
	{
		write(1,"Checking whether file contents have been reversed and case-inverted: No\n",72);
	}

	if(stat("./Assignment/final.txt",&f_status)==0)
	{
		if((f_status.st_mode & S_IRUSR) != 0)
		{
			write(1,"User has read permission on file: Yes\n",38);
		}
		else
		{
			write(1,"User has read permission on file: No\n",37);
		}

		if((f_status.st_mode & S_IWUSR) != 0)
		{
			write(1,"User has write permission on file: Yes\n",40);
		}
		else
		{
			write(1,"User has write permission on file: No\n",39);
		}

		if((f_status.st_mode & S_IXUSR) != 0)
		{
			write(1,"User has execute permission on file: Yes\n",41);
		}
		else
		{
			write(1,"User has execute permission on file: No\n",40);
		}

		if((f_status.st_mode & S_IRGRP) != 0)
		{
			write(1,"Group has read permission on file: Yes\n",39);
		}
		else
		{
			write(1,"Group has read permission on file: No\n",38);
		}

		if((f_status.st_mode & S_IWGRP) != 0)
		{
			write(1,"Group has write permission on file: Yes\n",40);
		}
		else
		{
			write(1,"Group has write permission on file: No\n",39);
		}

		if((f_status.st_mode & S_IXGRP) != 0)
		{
			write(1,"Group has execute permission on file: Yes\n",42);
		}
		else
		{
			write(1,"Group has execute permission on file: No\n",41);
		}

		if((f_status.st_mode & S_IROTH) != 0)
		{
			write(1,"Others has read permission on file: Yes\n",40);
		}
		else
		{
			write(1,"Others has read permission on file: No\n",39);
		}

		if((f_status.st_mode & S_IWOTH) != 0)
		{
			write(1,"Others has write permission on file: Yes\n",41);
		}
		else
		{
			write(1,"Others has write permission on file: No\n",40);
		}

		if((f_status.st_mode & S_IXOTH) != 0)
		{
			write(1,"Others has execute permission on file: Yes\n",43);
		}
		else
		{
			write(1,"Others has execute permission on file: No\n",42);
		}


	}
	else
	{
		write(1,"User has read permission on file: No\n",37);
		write(1,"User has write permission on file: No\n",39);
		write(1,"User has execute permission on file: No\n",40);
		write(1,"Group has read permission on file: No\n",38);
		write(1,"Group has write permission on file: No\n",39);
		write(1,"Group has execute permission on file: No\n",41);
		write(1,"Others has read permission on file: No\n",39);
		write(1,"Others has write permission on file: No\n",40);
		write(1,"Others has execute permission on file: No\n",42);
	}

	if(stat("./Assignment",&status)==0)
	{
		if((status.st_mode & S_IRUSR) != 0)
		{
			write(1,"User has read permission on directory: Yes\n",38+5);
		}
		else
		{
			write(1,"User has read permission on directory: No\n",37+5);
		}

		if((status.st_mode & S_IWUSR) != 0)
		{
			write(1,"User has write permission on directory: Yes\n",40+5);
		}
		else
		{
			write(1,"User has write permission on directory: No\n",39+5);
		}

		if((status.st_mode & S_IXUSR) != 0)
		{
			write(1,"User has execute permission on directory: Yes\n",41+5);
		}
		else
		{
			write(1,"User has execute permission on directory: No\n",40+5);
		}

		if((status.st_mode & S_IRGRP) != 0)
		{
			write(1,"Group has write permission on directory: Yes\n",40+5);
		}
		else
		{
			write(1,"Group has write permission on directory: No\n",39+5);
		}

		if((status.st_mode & S_IWGRP) != 0)
		{
			write(1,"Group has write permission on directory: Yes\n",40+5);
		}
		else
		{
			write(1,"Group has write permission on directory: No\n",39+5);
		}

		if((status.st_mode & S_IXGRP) != 0)
		{
			write(1,"Group has execute permission on directory: Yes\n",42+5);
		}
		else
		{
			write(1,"Group has execute permission on directory: No\n",41+5);
		}

		if((status.st_mode & S_IROTH) != 0)
		{
			write(1,"Others has read permission on file: Yes\n",40+5);
		}
		else
		{
			write(1,"Others has read permission on file: No\n",39+5);
		}

		if((status.st_mode & S_IWOTH) != 0)
		{
			write(1,"Others has write permission on file: Yes\n",41+5);
		}
		else
		{
			write(1,"Others has write permission on file: No\n",40+5);
		}

		if((status.st_mode & S_IXOTH) != 0)
		{
			write(1,"Others has execute permission on directory: Yes\n",43+5);
		}
		else
		{
			write(1,"Others has execute permission on directory: No\n",42+5);
		}
	}
	else
	{
		write(1,"User has read permission on directory: No\n",37+5);
		write(1,"User has write permission on directory: No\n",39+5);
		write(1,"User has execute permission on directory: No\n",40+5);
		write(1,"Group has write permission on directory: No\n",39+5);
		write(1,"Group has write permission on directory: No\n",40+5);
		write(1,"Group has execute permission on directory: No\n",41+5);
		write(1,"Others has read permission on file: No\n",39+5);
		write(1,"Others has write permission on file: No\n",40+5);
		write(1,"Others has execute permission on directory: No\n",42+5);
	}


}