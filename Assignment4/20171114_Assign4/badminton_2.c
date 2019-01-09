#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

long long int n,p=0,r=0,temp=0,temp_p=0,temp_r=0;
pthread_t person1[10000];
pthread_cond_t cond1;
pthread_cond_t cond2;
pthread_cond_t cond_main;
pthread_mutex_t mutex_player;
pthread_mutex_t mutex_arrive;
pthread_mutex_t mutex_refree;
pthread_mutex_t mutex;
long long int warm=0;
long long int batra=0;
long long int total_player;
long long int total_refree;
long long int game_num=0;
double prob_player,prob_refree,prob_rand;

void enterAcademyplayer(long long int num)
{
    printf("Player[%lld] enters the academy\n",num);
}

void meetOrganizerplayer(long long int num)
{
    printf("Player[%lld] meets organizer\n",num);
}

void enterCourt(long long int num)
{
    printf("Player[%lld] enters court\n",num);
}

void warmup(long long num)
{
    printf("Player[%lld] warming up\n",num);
    sleep(1);
    
}


void *person(void *arg)
{
    long long int num = *(long long int * )arg;
    enterAcademyplayer(num); 
    
    pthread_mutex_lock(&mutex_player);
    temp_p++;
    pthread_cond_wait(&cond1, &mutex_player);
    warm++;
    pthread_mutex_unlock(&mutex_player);
       
    
    meetOrganizerplayer(num);
    //sleep(1);
    enterCourt(num);
    //sleep(1);
    warmup(num);

    if(warm>=2)
    {
        warm=0;
        
        game_num++;
        printf("-----------Game Number[%lld] started---------\n",game_num);
        pthread_cond_signal(&cond_main);
        
    }

    
    
    //printf("player=%lld temp_r=%lld temp_p=%lld\n",warm,temp_p,temp_r);
    //sleep(1);

    
    //printf("player=%lld   temp_r=%lld temp_p=%lld\n",warm,temp_p,temp_r);


    
}

void enterAcademyrefree(long long int num)
{
    printf("Refree[%lld] enters the academy\n",num);
}

void meetOrganizerrefree(long long int num)
{
    printf("Refree[%lld] meets organizer\n",num);
}

void enterCourtRefree(long long int num)
{
    printf("Refree[%lld] enters court\n",num);
}

void equipment(long long int num)
{
    printf("Refree[%lld] checking equipment\n",num);
    sleep(0.5);
    
}

void *refree_func(void *arg)
{
    long long int num = *(long long int * )arg;
    enterAcademyrefree(num);
    
    pthread_mutex_lock(&mutex_refree);
    temp_r++;
    pthread_cond_wait(&cond2, &mutex_refree);
    warm++;
    pthread_mutex_unlock(&mutex_refree);

    //printf("signal received refree\n");
    meetOrganizerrefree(num);
    //sleep(1);
    enterCourtRefree(num);
    //sleep(1);
    equipment(num);

    if(warm>=2)
    {
        warm=0;
        game_num++;
        printf("-----------Game Number[%lld] started---------\n",game_num);
        pthread_cond_signal(&cond_main);
        
    }
    
    //printf("Refree=%lld   temp_r=%lld temp_p=%lld\n",warm,temp_p,temp_r);
    //sleep(0.5);
    
    //printf("Refree=%lld   temp_r=%lld temp_p=%lld\n",warm,temp_p,temp_r);
}

long long int calculate()
{
    prob_player=(double)(2*total_player)/(double)(2*total_player+total_refree);
    prob_refree=(double)(total_refree)/(double)(2*total_player+total_refree);
    prob_rand=(double)(rand())/(double)RAND_MAX;
    if(prob_rand<prob_player)
    {
        total_player--;
        return 1;
        
    }
    else
    {
        total_refree--;
        return -1;
        
    }    
}

void *main_person(void *arg)
{
    pthread_mutex_lock(&mutex_arrive);
    long long int err,i,gaurav=0,probability;
    double prob_range1,prob_range2;
    for(i=0;i<3*n;i++)
    {
        /*prob_range1=(double)(2*total_player)/(double)(total_player+total_refree);
        prob_range2=(double)(total_refree)/(double)(total_player+total_refree);

        printf("%lf",(double)rand()/RAND_MAX);
        */
        probability=calculate();
        if(probability==1)
        {
            
            p++;
            //temp_p++; 
            err = pthread_create(&person1[i],NULL,person,&p);
            if(err)
            {
                perror("Unable to create person\n");
            }
            
              
            
        }
        else if(probability==-1)
        {
            //printf("in refree\n");
            
            r++;
            //temp_r++;
            err=pthread_create(&person1[i],NULL,refree_func,&r);
            if(err)
            {
                perror("Unable to create person\n");
            }
            
            
            
        }
        sleep(rand()%3);
        //gaurav++;
    }
    pthread_mutex_unlock(&mutex_arrive);
}

int main()
{
    long long int i,j,k,err;
    printf("Enter Number of Teams: ");
    scanf("%lld",&n);
    //printf("%lld\n",p);
    
    pthread_mutex_init(&mutex, NULL);
    total_player=2*n;
    pthread_mutex_init(&mutex_refree, NULL);
    pthread_mutex_init(&mutex_player, NULL);    
    total_refree=n;
    pthread_mutex_init(&mutex_arrive, NULL);
    pthread_cond_init(&cond1, NULL);
    srand(time(NULL));
    pthread_cond_init(&cond2, NULL);
    pthread_cond_init(&cond_main,NULL);

    pthread_t main_person_thread;
    err=pthread_create(&main_person_thread,NULL,main_person,NULL);
    if(err)
    {
        perror("Unable to start game\n");
    }
   
    while(1)
    {
        //printf("temp_p=%lld temp_r=%lld\n",p,r);
        if(temp_p>=2 && temp_r>=1)
        {
            //printf("%lld\n",batra);
            //printf("%lld",warm);
            pthread_mutex_lock(&mutex);
            temp_p--;
            temp_p--;
            temp_r--;
            
            //sleep(0.5);
            pthread_cond_signal(&cond1);
            pthread_cond_signal(&cond1);
            pthread_cond_signal(&cond2);
            //printf("---------Main Wait-------------\n");
            pthread_cond_wait(&cond_main, &mutex);
            

            batra+=3;
            pthread_mutex_unlock(&mutex);
            
            
            
        }
        if((batra)>=3*n)
        {
            break;
        }
        
    }
    
    /*pthread_cond_broadcast(&cond1);
    pthread_cond_broadcast(&cond2);*/

    printf("All Teams Have Played\n");

    for(i=0;i<3*n;i++)
    {
        pthread_join(person1[i],NULL);

    }

    pthread_join(main_person_thread,NULL);
}