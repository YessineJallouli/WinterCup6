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
#define MAX_BUFFER (1<<21)


typedef long long integer;
integer gcd(integer a,integer b)
{
    if(a<b) return gcd(b,a);
    if(!b) return a;
    return gcd(b,a%b);
}

integer lcm(integer a, integer b)
{
    return a*b / gcd(a,b);
}

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

bool check_permutation_validity(int *P, int n, int *err_code)
{
    bool *B=(bool*) malloc(sizeof(bool)*n);
    bool is_permutation=true;
    for(int i=0;i<n;i++) if(P[i] <= n && P[i] >= 1)
        B[P[i]-1]=true;
    for(int i=0;i<n;i++) if(!B[i])
        is_permutation=false;
    free(B);
    if(!is_permutation)
    {
        fprintf(stderr,"Invalid test file. Array is not a permutation.\n");
        fflush(stderr);
        *err_code=TEST_ERROR;
    }
    return is_permutation;
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

void debug_answer(FILE *debug_file, const char * buffer)
{
    if(debug_file)
    {
        fprintf(debug_file, "Received Input: !%s\n",buffer);
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
    fscanf(file,"%d",&n);
    int *P = (int*)malloc(sizeof(int)*n);
    if(!check_allocation(P,&error_code))
        return error_code;

    for(int i=0;i<n;i++)
        fscanf(file,"%d",P+i);
    fclose(file);
    if(!check_permutation_validity(P,n,&error_code))
        goto clean;

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
            integer LCM=lcm(P[i-1],P[j-1]);
            fprintf(stdout,"%lld\n",LCM);
            fflush(stdout);
            debug_output(debug_file,LCM);
            queries++;
        }
        else if(command=='!')
        {
            char buffer[MAX_BUFFER];
            int guess_size= read_permutation_guess(stdin, buffer, MAX_BUFFER);
            debug_answer(debug_file,buffer);
            int* P_guess = (int*)malloc(sizeof(int)*guess_size);
            if(!check_allocation(P_guess,&error_code))
                goto clean;

            int extracted_size=extract_permutation_guess(buffer,P_guess,guess_size);

            if(extracted_size!=guess_size)
            {
                fprintf(stderr,"Internal error. Error while extracting permutation. Buffer size: %d, Extracted permutation size: %d.\n",guess_size,extracted_size);
                fflush(stderr);
            }
            else if(extracted_size == n && !memcmp(P,P_guess,n*sizeof(int)))
            {
                fprintf(stderr,"Solution guessed the correct permutation! Number of queries: %d\n",queries);
                fflush(stderr);
            }
            else
            {
                fprintf(stderr,"Solution guessed the wrong permutation!\n");
                fprintf(stderr, "Expected: ");
                print_array(stderr, P, n);
                fprintf(stderr, "\nFound:    ");
                print_array(stderr, P_guess, guess_size);
                fprintf(stderr,"\n");
                fflush(stderr);
            }
            free(P_guess);
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