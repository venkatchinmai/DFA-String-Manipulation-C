#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct trans
{
    char ini[10];
    char symb[10];
    char fin[10];
} tra[20];
int g_verbose = 0;
char g_dfa_file[512];
int nstates = 0, nalpha = 0, nfinal = 0, ntrans = 0;
char alpha[10][10];
char states[10][10];
char final_states[10][10];
char start_state[10][10];
char token[100];
char input_string[1000];
char nextState[10];
int usage()
{
    const char usage_str[] = "\
./dfa-h -d <dfafile> -v\n\
This is a simple DFA Implementation. The DFA definition\n\
is specified via .dfa file; input string is read from stdin.\n\
In non-verbose mode, print ACCEPT or NOT ACCEPT as the case may be.\n\
-h\n\
print usage\n\
-d <dfafile>\n\
DFA definition file\n\
-v\n\
verbose mode; display machine definition, transitions etc. \n\
";

    printf("\n%s\n", usage_str);
    exit(1);
}
void print_a(char a[10][10], int l)
{
    int i;
    for (i = 0; i < l; i++)
        printf("%s    ", a[i]);
    printf("\n");
}

int file()
{

    FILE *fp;
    //fp=fopen("inputs.txt","r"); //testing only
    fp = fopen(g_dfa_file, "r"); //testing only
    char str[100];
    const char s[2] = ":";
    char *token;

    /*STATES*/
    fgets(str, 100, fp);
    /* get the first token */
    token = strtok(str, s);
    int i;
    token = strtok(NULL, " ");
    /* walk through other s_tokens */
    i = 0;
    while (token != NULL)
    {
        //printf(" %s\n", s_token);
        strcpy(states[i], token);
        i++;
        token = strtok(NULL, " ");
    }
    nstates = i;

    /*Alphabets*/
    fgets(str, 100, fp);
    /* get the first token */
    token = strtok(str, s);
    token = strtok(NULL, " ");
    /* walk through other tokens */
    i = 0;
    while (token != NULL)
    {
        //printf(" %s\n", token);
        strcpy(alpha[i], token);
        i++;
        token = strtok(NULL, " ");
    }
    nalpha = i;

    /*start state*/
    fgets(str, 100, fp);
    /* get the first token */
    token = strtok(str, s);
    token = strtok(NULL, " ");
    /* walk through other tokens */
    i = 0;
    strcpy(start_state, token);

    /*Final states*/
    fgets(str, 100, fp);
    /* get the first token */
    token = strtok(str, s);
    token = strtok(NULL, " ");
    /* walk through other tokens */
    i = 0;
    while (token != NULL)
    {
        //printf(" %s\n", token);
        strcpy(final_states[i], token);
        i++;
        token = strtok(NULL, " ");
    }
    nfinal = i;

    /*transtisionas*/
    i = 0;
    while (fgets(str, 100, fp) != NULL)
    {
        // printf("%s",str);
        token = strtok(str, s);
        token = strtok(NULL, " ");
        strcpy(tra[i].ini, token);
        token = strtok(NULL, " ");
        strcpy(tra[i].symb, token);
        token = strtok(NULL, " ");
        strcpy(tra[i].fin, token);
        i++;
        // printf("%d\n",i);
    }
    ntrans = i;
    if (g_verbose == 1)
    {
        puts("States:");
        print_a(states, nstates);
        puts("Alphabets:");
        print_a(alpha, nalpha);
        puts("Final states:");
        print_a(final_states, nfinal);
        puts("Start state:");
        printf("%s\n", start_state);
        puts("Transitions:");
        for (i = 0; i < ntrans; i++)
        {
            printf("%s\t%s\t%s\n", tra[i].ini, tra[i].symb, tra[i].fin);
        }
    }
    while (1)
    {
        start_machine();
    }
}
void next_state(char cur[10], char input[10])
{
    char *next;
    int i, j;
    for (i = 0; i < ntrans; i++)
    {
        if (!strcmp(tra[i].ini, cur))
        {
            if (!strcmp(tra[i].symb, input))
            {
                strcpy(nextState, tra[i].fin);
                return nextState;
            }
        }
    }
    return -1;
}
void start_machine()
{

    // printf("Enter input String:");
    scanf("%s", input_string);
    //printf("%s",input_string);
    if (strcmp(input_string, "END") == 0)
        exit(1);
    char t[100][10];
    int slen = strlen(input_string);
    int i = 0;
    for (i = 0; i < slen; i++)
    {
        t[i][0] = input_string[i];
        t[i][1] = '\0';
    }
    char cur[10], next[10];
    strcpy(cur, tra[0].ini);
    for (i = 0; i < slen; i++)
    {
        next_state(cur, t[i]);
        strcpy(next, nextState);
        if (g_verbose == 1)
        {
            printf("current state %s : input : %s -> new state  : %s\n", cur, t[i], next);
        }
        strcpy(cur, next);
    }
    int flag = 0;
    for (i = 0; i < nfinal; i++)
    {
        if (!strcmp(cur, final_states[i]))
        {
            flag = 1;
            break;
        }
    }
    if (flag == 1)
    {
        printf("%s ACCEPTED\n", input_string);
    }
    else
    {
        printf("%s REJECTED\n", input_string);
    }
}
int process_args(int argc, char *argv[])
{
    int flags, opt;

    while ((opt = getopt(argc, argv, "d:hv")) != -1)
    {
        switch (opt)
        {
        case 'd':
            strcpy(g_dfa_file, optarg);
            file();
            break;
        case 'h':
            usage();
            break;
        case 'v':
            g_verbose = 1;
            break;
        default: /* '?' */
            usage();
        }
    }

    printf("g_verbose: %d\n", g_verbose);
    printf("g_dfa_file: %s\n", g_dfa_file);
}
int main(int argc, char *argv[])
{
    process_args(argc, argv);
}