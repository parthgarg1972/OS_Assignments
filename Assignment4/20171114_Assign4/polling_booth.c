#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
long long int n;

typedef struct Evm Evm;
typedef struct Voter Voter;


typedef struct Booth
{
    long long int id;
    pthread_t booth_thread_id;
    long long int evm;
    long long int max_slots_in_evm;
    long long int number_of_voters;
    long long int done_voters;
    Evm ** evms;
    Voter ** voters;
    pthread_mutex_t mutex_booth;
    pthread_cond_t cond1;
    pthread_cond_t cond2;
}Booth;

struct Evm
{
    long long int id;
    long long int slots;
    long long int check;
    pthread_t evm_thread_id;
    Booth *booth_related;
};

struct Voter
{
    long long int id;
    Booth *booth_related;
    long long int status;
    Evm *evm_related;
    pthread_t voter_thread_id;

};

void *voter_thread(void *args)
{
    Voter *voter = (Voter *)args;
    Booth *booth = voter->booth_related; 

    printf("Voter[%lld] of Booth[%lld] is waiting for EVM\n",voter->id+1,booth->id+1);

    pthread_cond_t *cond1 = &(booth->cond1);
    pthread_mutex_t *mutex = &(booth->mutex_booth);    
    pthread_cond_t *cond2 = &(booth->cond2);

    
    
    pthread_mutex_lock(mutex);
    voter->status=1;

    while(voter->status == 1)
    {
        pthread_cond_wait(cond1,mutex);
    }
    pthread_mutex_unlock(mutex);
    Evm *evm = voter->evm_related;
    

    

    pthread_mutex_lock(mutex);
    while(evm->check==0)
    {
        pthread_cond_wait(cond1,mutex);
    }
    
    evm->slots--;

    pthread_cond_broadcast(cond2);
    printf("Booth[%lld] evm[%lld] voter[%lld] has casted his vote\n",booth->id+1,evm->id+1,voter->id+1);

    pthread_mutex_unlock(mutex);

    return NULL;



}

void *evm_thread(void *args)
{
    Evm *evm = (Evm*)args;
    Booth *booth = evm->booth_related;
    long long int i,j,k,l,m,slots;
    pthread_cond_t *cond1 = &(booth->cond1);
    pthread_mutex_t *mutex = &(booth->mutex_booth);
    pthread_cond_t *cond2 = &(booth->cond2);

    

    while(1)
    {
        pthread_mutex_lock(mutex);

        if(booth->done_voters == booth->number_of_voters)
        {
            pthread_mutex_unlock(mutex);
            break;
        }

        pthread_mutex_unlock(mutex);
        slots=rand()%(booth->max_slots_in_evm)+1;
        k=0;
        
        pthread_mutex_lock(mutex);
        evm->check=0;
        evm->slots=slots;
        printf("EVM[%lld] of booth[%lld] has %lld slots free.\n",evm->id+1,booth->id+1,evm->slots);
        pthread_mutex_unlock(mutex);

        while(1)
        {
            if(k>=slots)break;
            else
            {
                i=rand()%booth->number_of_voters;
                pthread_mutex_lock(mutex);
                
                if(booth->voters[i]->status==1)
                {
                    booth->voters[i]->evm_related=evm;
                    booth->voters[i]->status=2;
                    printf("Voter[%lld] of Booth[%lld] is alloted EVM[%lld].\n",i+1,booth->id+1,evm->id+1);
                    booth->done_voters+=1;
                    k++;
                }
                if(booth->done_voters == booth->number_of_voters)
                {
                    pthread_mutex_unlock(mutex);
                    break;
                }
                pthread_mutex_unlock(mutex);
                
            }


        }
        if(k==0)break;
        pthread_mutex_lock(mutex);
       

        
        evm->slots=k;
        printf("Booth[%lld]'s EVM[%lld] has started the voting phase.\n",booth->id+1,evm->id+1);
        sleep(1);    
        evm->check=1;   
        pthread_cond_broadcast(cond1);
        while(evm->slots)
            pthread_cond_wait(cond2,mutex);
        pthread_mutex_unlock(mutex);



        printf("--------------Booth[%lld]'s EVM[%lld] has finished voting phase.-------------------\n",booth->id+1,evm->id+1);
        sleep(1);

    }

    printf("----------Booth[%lld]'s EVM[%lld] Finished-------------\n",booth->id+1,evm->id+1);

    return NULL;



}





Evm *evm_initialize(long long int id,Evm *evm,Booth *booth)
{
    evm=(Evm *)malloc(sizeof(Evm));

    evm->id=id;
    evm->slots=0;
    evm->booth_related=booth;
    evm->check=0;
    return evm;
}

Voter *voter_initialize(long long int id,Voter *voter,Booth *booth)
{
    voter=(Voter *)malloc(sizeof(Voter));

    voter->id=id;
    voter->booth_related=booth;
    voter->evm_related=booth->evms[id];
    voter->status=0;
    return voter;
}

Booth *booth_initialize(long long int id,long long int number_evm,long long int number_of_voters,long long int max_slots,Booth *booth)
{
    booth=(Booth *)malloc(sizeof(Booth));
    booth->id=id;
    
    

    booth->evms=(Evm **)malloc(sizeof(Evm*)*number_evm);
    booth->evm=number_evm;
    booth->voters=(Voter **)malloc(sizeof(Voter*)*number_of_voters);

    pthread_mutex_init(&booth->mutex_booth, NULL);
    booth->number_of_voters=number_of_voters;
    pthread_cond_init(&booth->cond2, NULL);
    pthread_cond_init(&booth->cond1,NULL);

    
    booth->max_slots_in_evm=max_slots;
    booth->done_voters=0;
    return booth;


}

void *booth_thread(void *args)
{
    Booth *booth = (Booth *)args;
    long long int i,j,k,l,m;
    //printf("%lld %lld %lld\n",booth->evm,booth->number_of_voters,booth->max_slots_in_evm);

    for(i=0;i<booth->evm;i++)
    {
        booth->evms[i]=evm_initialize(i,booth->evms[i],booth);
    }
    for(i=0;i<booth->number_of_voters;i++)
    {
        booth->voters[i]=voter_initialize(i,booth->voters[i],booth);
    }

    for(i=0;i<booth->evm;i++)
    {
        //printf("in\n");
        k=pthread_create(&(booth->evms[i]->evm_thread_id),NULL,evm_thread,booth->evms[i]);
        if(k)
        {
            perror("Failed to create thread");
        }
    }
    for(i=0;i<booth->number_of_voters;i++)
    {
        k=pthread_create(&(booth->voters[i]->voter_thread_id),NULL,voter_thread,booth->voters[i]);
        if(k)
        {
            perror("Failed to create thread");
        }
    }
    for(i=0;i<booth->evm;i++)
    {
        pthread_join(booth->evms[i]->evm_thread_id,NULL);
    }
    for(i=0;i<booth->number_of_voters;i++)
    {
        pthread_join(booth->voters[i]->voter_thread_id,NULL);
    }

    

    for(int i=0; i<booth->number_of_voters; i++)
        free(booth->voters[i]);

    for(int i=0; i<booth->evm; i++)
        free(booth->evms[i]);    

    printf("-----------Booth[%lld] has Finished------------\n",booth->id+1);

    
          
    free(booth->evms);
    free(booth->voters);

    return NULL;
    



}





int main()
{
    long long int i,j,k,l,m;
    printf("Enter number of Booths: ");
    scanf("%lld",&n);

    long long int *evm = (long long int *)malloc(sizeof(long long int)*n);
    long long int *max_slots_in_evm=(long long int*)malloc(sizeof(long long int)*n);
    long long int *number_of_voters=(long long int*)malloc(sizeof(long long int)*n);

    for(i=0;i<n;i++)
    {
        printf("Enter number of EVMs for Booth %lld:",i+1);
        scanf("%lld",&evm[i]);

        printf("Enter number of Voters for Booth %lld:",i+1);
        scanf("%lld",&number_of_voters[i]);

        printf("Enter max slots evm for booth %lld(less than or equal to 10):",i+1);
        scanf("%lld",&max_slots_in_evm[i]);
    }
    Booth **all_booths=(Booth **)malloc(sizeof(Booth*)*n);

    printf("Start of Elections\n");
    
    for(i=0;i<n;i++)
    {
        all_booths[i]=booth_initialize(i,evm[i],number_of_voters[i],max_slots_in_evm[i],all_booths[i]);
    }

    for(i=0;i<n;i++)
    {
        k=pthread_create(&(all_booths[i]->booth_thread_id),NULL,booth_thread,all_booths[i]);
        if(k)
        {
            perror("Failed to create thread");
        
        }
    }

    for(i=0;i<n;i++)
    {
        pthread_join(all_booths[i]->booth_thread_id,NULL);
    }

    printf("End of Elections\n");
    free(evm);
    free(number_of_voters);
    free(max_slots_in_evm);
    for(i=0;i<n;i++)
    {
        free(all_booths[i]);
    }
    free(all_booths);



}