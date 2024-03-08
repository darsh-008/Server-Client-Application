// Name//Section//Student id = Darsh_Bhatt//2//110124147 - Project Submission
// importing all the necessarry libs
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

// Global var declaration
#define DRS_SZE_BFR 4096000
#define DRS_SZE_BFR_LW 4096
#define MX_TKNS 10
#define MN_TKNS 0
#define MX_TKN_LNTH 50
#define MN_TKN_LNTH 0

// Global var initialization
typedef char ARY_TKN[MX_TKNS][MX_TKN_LNTH];
ARY_TKN argv_user;
int usr_agmnt_dc;
int usr_vlid_or_nt = 1;
int tkn_cnt_n = 0;
int cntr_in_n =1;

// funtion for file open
FILE* openFileForWrite(const char* nme_of_fle, int socket) {
    FILE* file = fopen(nme_of_fle, "wb");
    if (file == NULL) {
        perror("Error opening tar file");
        close(socket);  // Closing the socket
        exit(EXIT_FAILURE);
    }
    return file;
}

// feature that allows the client to download the temp.tar.gz
void clnt_sde_dwnld(int con_sd)
{
    long sze_of_fle;
    int t1_counter=0;
    char drs_bufr[DRS_SZE_BFR];
    size_t bytesReceived;
    ssize_t byts_rcivd;
    recv(con_sd, &sze_of_fle, sizeof(sze_of_fle), 0);

    // error handling
    if (byts_rcivd == -1) {
         perror("Error in filesoze");
    }
    // writing the contents of a new tar file
    FILE* temp_tar_gz = openFileForWrite("temp.tar.gz", con_sd);

    // launching the animation for downloading
    long byts_dwnlded = 0;
    int indx_of_animtion = 0;
    char crs_of_animtion[] = {'-', '\\', '|', '/'};
    char crs_of_dot_animtion[] = {'-', '-', '|', '-', '-'};

    // take the ccontennts and add them to the document.
    while (sze_of_fle > 0 && (bytesReceived = recv(con_sd, drs_bufr, sizeof(drs_bufr), 0)) > 0) {
        // sleep(10);
        fwrite(drs_bufr, 1, bytesReceived, temp_tar_gz);
        sze_of_fle -= bytesReceived;
        byts_dwnlded += bytesReceived;
        t1_counter= t1_counter+1;

        // Printing theanimation
        printf("\rDownloading: %c", crs_of_animtion[indx_of_animtion]);
        fflush(stdout);
        indx_of_animtion = (indx_of_animtion + 1) % 4;
    }

    fclose(temp_tar_gz);
    printf("\n>> File is received\n");
}

// funtion to close the socket
void terminte_scket(int socket) {
    if (close(socket) == -1) {
        perror("Error in closing the socket");
        exit(EXIT_FAILURE);
    }
}

// functon used to tokenize the command to strings which will be used in the command handler function
void tknizr_strng(const char *input)
{
    int t2_counter=0;
    // tokenization using spaces
    char *drs_tcken = strtok((char *)input, " ");
    while (drs_tcken != NULL && tkn_cnt_n < 10)
    {
        // movingg the argments in custom agmnt_dv
        strcpy(argv_user[tkn_cnt_n], drs_tcken);
        tkn_cnt_n++;
        drs_tcken = strtok(NULL, " ");
    }
    // moving the number of argumnts in custom argu.
    usr_agmnt_dc = tkn_cnt_n;
    // preparation/reset for the next iteration
    tkn_cnt_n = 0;
}

// the capability to confirm and process each client command as needed
void drs_Hndlr_of_Cmnds(int client_sd)
{
    int t3_counter=0;
    while (1)
    {
        char cmmndd[100];
        int t4_counter=0;
        printf("\nWhat should be delivered to the server, in your opinion? \n");
        fgets(cmmndd, sizeof(cmmndd), stdin);
        write(client_sd, cmmndd, sizeof(cmmndd));

        // Shut down both sides' connections if the command is quitc
        if (strcmp("quitc\n", cmmndd) == 0)
        {
            close(client_sd); // Connection close with to the server
            exit(0); // loop exit 
        }
        else if (strcmp("name\n", cmmndd) == 0)
        {
           printf("Darsh\n");
           continue;
        }
        // finishing with a null termination character
        cmmndd[strcspn(cmmndd, "\n")] = '\0';

        // string tokenizer function is being used here
        tknizr_strng(cmmndd);
        strcpy(cmmndd,"");

        // carrying out the relevant tasks in response to the aforementioned client's request
        if((strcmp("getfn", argv_user[0]) == 0) && (usr_agmnt_dc == 2))
        {
            char info[400];
            recv(client_sd, info, sizeof(info), 0);
            printf("\n%s\n", info);
        }
        else if((strcmp("getfz", argv_user[0]) == 0) && (usr_agmnt_dc < 4))
        {
            char Msg[2];
            recv(client_sd, Msg, sizeof(Msg), 0);
            if(strcmp(Msg, "A") == 0)
            {
                printf("Downloading available files. Wait!\n");
                clnt_sde_dwnld(client_sd);
            }
            else
            {
                printf("no files found\n");
            }
        }

        else if((strcmp("getfda", argv_user[0]) == 0) && (usr_agmnt_dc < 3))
        {
            char Msg[2];
            recv(client_sd, Msg, sizeof(Msg), 0);
            if(strcmp(Msg, "A") == 0)
            {
                printf("Downloading available files. Wait!\n");
                clnt_sde_dwnld(client_sd);
            }
            else
            {
                printf("nothing to clnt_sde_dwnld, sorry!\n");
            }
        }

        else if((strcmp("getfdb", argv_user[0]) == 0) && (usr_agmnt_dc < 3))
        {
            char Msg[2];
            recv(client_sd, Msg, sizeof(Msg), 0);
            if(strcmp(Msg, "A") == 0)
            {
                printf("Downloading available files. Wait!\n");
                clnt_sde_dwnld(client_sd);
            }
            else
            {
                printf("nothing to clnt_sde_dwnld, sorry!\n");
            }
        }

        else if((strcmp("getft", argv_user[0]) == 0) & (usr_agmnt_dc < 4))
        {
            char Msg[2];
            recv(client_sd, Msg, sizeof(Msg), 0);
            if(strcmp(Msg, "A") == 0)
            {
                printf("Downloading available files. Wait!\n");
                clnt_sde_dwnld(client_sd);
            }
            else
            {
                printf("nothing to clnt_sde_dwnld, sorry!\n");
            }
        }

        else
        {
            printf("Please enter the command properly.\n");
        }
    }
}

// this is the main functon
void main(int agmnt_dc, char *agmnt_dv[])
{
    // exit if the arguments are not entered correctly
    if(agmnt_dc > 0 && agmnt_dc < 3)
    {
        printf("\nclient [server ip address] [server port]\n");
        exit(0);
    }
    long PP_ORT = atol(agmnt_dv[2]);
    char *IP_of_SRVR = agmnt_dv[1];
    int client_sd;

    // establishing the connectson
    socklen_t len;
    struct sockaddr_in servAdd;
    if ((client_sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("error in the socket creation");
        exit(1);
    }
    servAdd.sin_family = AF_INET;
    servAdd.sin_port = htons(PP_ORT);
    if (inet_pton(AF_INET, IP_of_SRVR, &servAdd.sin_addr) < 0)
    {
        perror("error in inet_pton()");
        exit(2);
    }

    if (connect(client_sd, (struct sockaddr *) &servAdd, sizeof(servAdd)) < 0)
    {
        perror("error in connect()");
        exit(3);
    }

    // receiving the client number from the server
    char nmbr_clnt_strngg[16];
    recv(client_sd, &nmbr_clnt_strngg, sizeof(nmbr_clnt_strngg), 0);
    int client_id = atoi(nmbr_clnt_strngg);
    
    // here I am checking to recieve M or S for the bytes to check mirror and server connection
    char SRVR_MROR;
    ssize_t byts_rcivd = recv(client_sd, &SRVR_MROR, sizeof(SRVR_MROR), 0);
    if (byts_rcivd == sizeof(SRVR_MROR))
    {
        // if server
        if (SRVR_MROR == 'A')
        {
            // here it is starting the command running into
            drs_Hndlr_of_Cmnds(client_sd);
        }
        // if mirror
        else if (SRVR_MROR == 'B')
        {
            // it is recieving the port numbere and mirrorr address
            int prt_of_mrorr;
            byts_rcivd = recv(client_sd, &prt_of_mrorr, sizeof(prt_of_mrorr), 0);
            if (byts_rcivd == sizeof(prt_of_mrorr))
            {
                char drs_i_p_mrorr[20];
                recv(client_sd, &drs_i_p_mrorr, sizeof(drs_i_p_mrorr), 0);
                // dissconnecting connection to main server
                close(client_sd); 

                // establishing a new socket connection to the mirror server
                if ((client_sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                {
                    perror("error in the socket creation");
                    exit(1);
                }
                servAdd.sin_family = AF_INET;
                servAdd.sin_port = htons(prt_of_mrorr);
                if (inet_pton(AF_INET, drs_i_p_mrorr, &servAdd.sin_addr) < 0)
                {
                    perror("error in inet_pton()");
                    exit(2);
                }

                // connecting to the mirror
                if (connect(client_sd, (struct sockaddr *) &servAdd, sizeof(servAdd)) < 0)
                {
                    perror("error in connect()");
                    exit(3);
                }

                // transferring the client number to the mirror
                send(client_sd, &nmbr_clnt_strngg, sizeof(nmbr_clnt_strngg), 0);
                // handling commands
                drs_Hndlr_of_Cmnds(client_sd);
            }
        } 
    }
}


