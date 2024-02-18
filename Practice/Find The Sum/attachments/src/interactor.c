#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>

#define ERROR_USAGE 1
#define ERROR_FILE_OPEN 2
#define ERROR_MEMORY 3
#define INTERACTION_ERROR 4
#define IO_ERROR 5
#define TEST_ERROR 6


typedef long long integer;

bool is_valid_query(int i,int j,int n)
{
    return i > 0 && i <= n && j > 0 && j <= n && i != j;
}

bool check_allocation(const void *P,int *err_code)
{
    if(!P)
    {
        fprintf(stderr,"Unable to allocate memory\n");
        fflush(stderr);
        *err_code=ERROR_MEMORY;
        return false;
    }
    return true;
}

bool check_query_validity(int i,int j,int n, int *err_code)
{
    if(!is_valid_query(i,j,n))
    {
        fprintf(stderr,"Invalid query: ? %d %d\n",i,j);
        fflush(stderr);
        *err_code=INTERACTION_ERROR;
        return false;
    }
    return true;
}

void debug_input(FILE * debug_file,int i,int j)
{
    if(debug_file)
    {
        fprintf(debug_file, "Received Input: ? %d %d\n",i,j);
        fflush(debug_file);
    }
}

void debug_output(FILE* debug_file, integer LCM)
{
    if(debug_file)
    {
        fprintf(debug_file, "Sent Output %lld\n",LCM);
        fflush(debug_file);
    }
}

void debug_answer(FILE *debug_file, integer S)
{
    if(debug_file)
    {
        fprintf(debug_file, "Received Input: !%lld\n",S);
        fflush(debug_file);
    }
}

void print_array(FILE *file, int *P,int n)
{
    for(int i=0;i<n;i++)
        fprintf(file,"%s%d",i?" ":"",P[i]);
}

int read_permutation_guess(FILE *file, char *buffer, int size)
{
    fgets(buffer,size,file);
    int permutation_size=0;
    bool inspace=true;
    while(*buffer)
    {
        if(!isspace(*buffer) && inspace)
        {
            inspace=false;
            permutation_size++;
        }
        else if(isspace(*buffer))
            inspace=true;
        ++buffer;
    }
    return permutation_size;
}

int extract_permutation_guess(char* buffer,int *P, int size)
{
    int extracted=0;
    char *start=buffer;
    char *end;
    while(*start && isspace(*start))
        ++start;
    while(*start && extracted < size)
    {
        end=start;
        while(isdigit(*end))
            ++end;
        char tmp=*end;
        *end='\0';
        sscanf(start,"%d",P+extracted);
        extracted++;
        *end=tmp;
        start=end;
        while(*start && isspace(*start))
            ++start;
    }
    return extracted;
}

int main(int argc, char**argv)
{
    int error_code=0;
    int queries=0;

    char* debug_filename= getenv("DEBUG");
    char* non_debug=getenv("NDEBUG");
    if(argc < 2)
    {
        fprintf(stderr,"Usage %s TEST_FILE\n", argv[0]);
        return ERROR_USAGE;
    }
    FILE* file=fopen(argv[1],"r");
    FILE* debug_file=NULL;
    bool use_debug=non_debug==NULL && debug_filename!=NULL;
    if(use_debug)
        debug_file= fopen(debug_filename,"w");
    if(!file)
    {
        fprintf(stderr,"Unable to open file %s\n", argv[1]);
        return ERROR_FILE_OPEN;
    }

    int n;
    integer S=0;
    fscanf(file,"%d",&n);
    int *P = (int*)malloc(sizeof(int)*n);
    if(!check_allocation(P,&error_code))
        return error_code;

    for(int i=0;i<n;i++)
    {
        fscanf(file, "%d", P + i);
        S+=P[i];
    }
    fclose(file);

    printf("%d\n",n);
    fflush(stdout);

    while(!feof(stdin))
    {
        char command=fgetc(stdin);
        if(command=='?')
        {
            int i=0,j=0;
            scanf("%d %d",&i,&j);
            if(!check_query_validity(i,j,n,&error_code))
                goto clean;
            debug_input(debug_file,i,j);
            integer SUM=P[i-1]+P[j-1];
            fprintf(stdout,"%lld\n",SUM);
            fflush(stdout);
            debug_output(debug_file,SUM);
            queries++;
        }
        else if(command=='!')
        {
            integer S_guess;
            scanf("%lld",&S_guess);
            debug_answer(debug_file,S_guess);
            if(S_guess == S)
            {
                fprintf(stderr,"Solution guessed the correct sum! Number of queries: %d\n",queries);
                fflush(stderr);
            }
            else
            {
                fprintf(stderr,"Solution guessed the wrong sum!\n");
                fprintf(stderr, "Expected: %lld\n",S);
                fprintf(stderr, "Found:    %lld\n",S_guess);
                fprintf(stderr,"Array: ");
                print_array(stderr,P,n);
                fprintf(stderr,"\n");
                fflush(stderr);
            }
            goto clean;
        }
        else if(isspace(command))
            continue;
        else
        {
            fprintf(stderr, "Unknown command %c", command);
            error_code=INTERACTION_ERROR;
            goto clean;
        }
    }
    fprintf(stderr,"Encountered premature end of file\n");
    error_code=IO_ERROR;

    clean:
    free(P);
    if(use_debug)
        fclose(debug_file);
    return error_code;
}