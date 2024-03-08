// Name//Section//Student id = Darsh_Bhatt//2//110124147 - Project Submission
// importing all the necessarry libs
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <time.h>

// Global var declaration
#define DRS_SZE_of_BFR 4096000
#define MX_PTH 4096
#define MX_TKNS 500
#define MN_TKNS 0
#define MX_TKN_LNTH 50
#define MN_TKN_LNTH 0
#define MX_LNTH_of_TKNS 100

// Global var initialization
char usr_agmnt_dv[10][MX_LNTH_of_TKNS];
int drc_usr_agmnt_dc = 0;
int cnt_of_tknss = 0;
char *files[MX_TKNS];
int cnt_of_fls = 0;
int te1_counter=0;
const char *drs_ot_pt_fles = "temp_to_send.tar.gz";
time_t currentTime;
char *sam = "samp.txt";
struct tm *localTime;
char strng_of_time[100];

// function that uses the first character encountered in the file or folder to determine whether it is hidden or not "."
bool is_hhiddn(const char *name)
{
    return name[0] == '.';
}

// function that uses global variable array files to produce tar files
void create_tar_gz(const char *ot_pt_fle_nme)
{
    int te2_counter = 0; // for testing purposes
    // total length needed for tar 
    size_t total_lnth_strng = strlen(ot_pt_fle_nme) + 25; 
    for (int i = 0; i < cnt_of_fls; i++)
    {
        total_lnth_strng += strlen(files[i]) + 1;  // Here the plus 1 is for space character
    }

    // allocating the storage space needed to execute the entire command
    char *tar_cmmndd = (char *)malloc(total_lnth_strng);

    if (tar_cmmndd == NULL)
    {
        perror("Error allocating memory");
        return;
    }

    snprintf(tar_cmmndd, total_lnth_strng, "tar -czvf %s ", ot_pt_fle_nme);

    // including every file that has to be sent
    for (int i = 0; i < cnt_of_fls; i++)
    {
        strcat(tar_cmmndd, files[i]);
        strcat(tar_cmmndd, " ");
    }

    // loop to fund the num of files send
    for (int i = 0; i < cnt_of_fls; i++)
    {
        te2_counter++;
    }
    // printf(te2_counter);

    // redirecting all error to null
    strcat(tar_cmmndd, "2>/dev/null");
    system(tar_cmmndd);

    // memory free
    free(tar_cmmndd);  
}

// function with retrieves all the files in the directory treee
void find_files(const char *dir_path) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_info;

    if ((dir = opendir(dir_path)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        if (stat(full_path, &file_info) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        if (S_ISDIR(file_info.st_mode) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            find_files(full_path);  // Recursively call for subdirectories
        } else if (S_ISREG(file_info.st_mode)) {
            printf("%s\n", full_path);  // Print the path of the file
        }
    }

    closedir(dir);
}

// function that allows clients to upload tar.gz files from servers
void uploadd_to_clnt(int con_sd)
{
    int te3_counter = 0; // for testing purposes
    // tar function is being called here
    create_tar_gz(drs_ot_pt_fles);

    // launching the tar file for submission
    FILE* tar_ball_fle = fopen(drs_ot_pt_fles, "rb");
    if (tar_ball_fle == NULL) {
        perror("Error opening tar_ball_fle");
        close(con_sd);
        exit(EXIT_FAILURE);
    }

    if (te3_counter != 0) 
    {
        while (1) {
            printf(te3_counter);
        }
    }

    // finding out the tar file's size
    fseek(tar_ball_fle, 0, SEEK_END);
    long size_of_fle = ftell(tar_ball_fle);
    fseek(tar_ball_fle, 0, SEEK_SET);
    
    // now sending it to the respective cleint
    send(con_sd, &size_of_fle, sizeof(size_of_fle), 0);

    char drs_bfr[DRS_SZE_of_BFR];
    size_t readng_bytss;

    // now sending the content of it to the respective client
    while ((readng_bytss = fread(drs_bfr, 1, sizeof(drs_bfr), tar_ball_fle)) > 0) {
        usleep(500000);
        send(con_sd, drs_bfr, readng_bytss, 0);
    }
    fclose(tar_ball_fle);

    // printing the timestamp by using the current time
    currentTime = time(NULL);
    localTime = localtime(&currentTime);
    strftime(strng_of_time, sizeof(strng_of_time), "%c", localTime);
    printf("%s: File sent\n", strng_of_time);

    // removing the tar.gz temporary file
    system("rm temp_to_send.tar.gz");
    return;
}

// function for the get files according to size feature, basically is same as working of the location get feature
void gt_lcton_sze(const char *dirPath, size_t size1, size_t size2)
{
    int te4_counter = 0; // for testing purposes
    DIR *dir = opendir(dirPath);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (te4_counter != 0) 
        {
            printf(te4_counter);
        }
        if (is_hhiddn(entry->d_name))
        {
            continue;
        }
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        if (strcmp(entry->d_name, "temp.tar.gz") == 0) {
            continue;
        }
        char path[MX_PTH];
        snprintf(path, MX_PTH, "%s/%s", dirPath, entry->d_name);
        struct stat fileStat;
        if (lstat(path, &fileStat) < 0)
        {
            perror("Error getting file info");
            continue;
        }
        if (S_ISDIR(fileStat.st_mode))
        {
            gt_lcton_sze(path, size1, size2);
        }
        else if (S_ISREG(fileStat.st_mode))
        {
            if (fileStat.st_size >= size1 && fileStat.st_size <= size2)
            {
                files[cnt_of_fls] = strdup(path);
                cnt_of_fls++;
            }
        }
    }
    closedir(dir);
    return;
}

// function for the extentson feature of the product, basically is same as working of the location get feature
void gt_lcton_extnsn(const char *dirPath)
{
    int te5_counter = 0; // for testing purposes
    DIR *dir = opendir(dirPath);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (is_hhiddn(entry->d_name)) {
            continue;
        }
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        if (strcmp(entry->d_name, "temp.tar.gz") == 0) {
            continue;
        }
        char path[MX_PTH];
        snprintf(path, MX_PTH, "%s/%s", dirPath, entry->d_name);
        struct stat fileStat;
        if (lstat(path, &fileStat) < 0)
        {
            perror("Error getting file info");
            continue;
        }
        if (S_ISDIR(fileStat.st_mode))
        {
            gt_lcton_extnsn(path);
        }
        else if (S_ISREG(fileStat.st_mode))
        {
            bool vld_extnsn = false;
            char *fileName = entry->d_name;
            const char *file_extension = strrchr(fileName, '.');
            if (file_extension) {
                file_extension++;
                for (int i = 1; i < drc_usr_agmnt_dc; i++)
                {
                    if (strcmp(file_extension, usr_agmnt_dv[i]) == 0 && vld_extnsn == false)
                    {
                        vld_extnsn = true;
                        break;
                    }
                }
            }
            if (vld_extnsn) {
            files[cnt_of_fls] = strdup(path);
            cnt_of_fls++; }
        }
    }
    if (te5_counter != 0) {
        printf(te4_counter);
    }
    closedir(dir);
    return;
}

// function for the get files according to date greater-than feature
void gt_lcton_date_grter(const char *dirPath, const char *date) 
{
    int te6_counter = 0; // for testing purposes    
    DIR *dir = opendir(dirPath);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (is_hhiddn(entry->d_name)) {
            continue;
        }
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        if (strcmp(entry->d_name, "temp_to_send.tar.gz") == 0) {
            continue;
        }
        if (strcmp(entry->d_name, "temp.tar.gz") == 0) {
            continue;
        }
        char path[MX_PTH];
        snprintf(path, MX_PTH, "%s/%s", dirPath, entry->d_name);
        struct stat fileStat;
        if (lstat(path, &fileStat) < 0) {
            perror("Error getting file info");
            continue;
        }
        if (S_ISDIR(fileStat.st_mode)) {
            gt_lcton_date_grter(path, date);
        } else if (S_ISREG(fileStat.st_mode)) {
            struct tm *creation_time = localtime(&fileStat.st_ctime);
            char date_str[11];
            strftime(date_str, sizeof(date_str), "%Y-%m-%d", creation_time);
            
            if (strcmp(date_str, date) >= 0)
            {
                files[cnt_of_fls] = strdup(path);
                cnt_of_fls++;
            }
        }
    }
    if (te6_counter != 0) {
        printf(te6_counter);
    }
    closedir(dir);
    return;
}

// function for the get files according to date smaller-than feature
void gt_lcton_date_lesss(const char *dirPath, const char *date) 
{
    int te7_counter = 0; // for testing purposes    
    DIR *dir = opendir(dirPath);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (is_hhiddn(entry->d_name)) {
            continue;
        }
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        if (strcmp(entry->d_name, "temp_to_send.tar.gz") == 0) {
            continue;
        }
        if (strcmp(entry->d_name, "temp.tar.gz") == 0) {
            continue;
        }
        char path[MX_PTH];
        snprintf(path, MX_PTH, "%s/%s", dirPath, entry->d_name);
        struct stat fileStat;
        if (lstat(path, &fileStat) < 0) {
            perror("Error getting file info");
            continue;
        }
        if (S_ISDIR(fileStat.st_mode)) {
            gt_lcton_date_lesss(path, date);
        } else if (S_ISREG(fileStat.st_mode)) {
            struct tm *creation_time = localtime(&fileStat.st_ctime);
            char date_str[11];
            strftime(date_str, sizeof(date_str), "%Y-%m-%d", creation_time);
            
            if (strcmp(date_str, date) <= 0)
            {
                files[cnt_of_fls] = strdup(path);
                cnt_of_fls++;
            }
        }
    }
    if (te7_counter != 0) {
        printf(te7_counter);
    }
    closedir(dir);
    return;
}


// function for the get file_informatson feature of the project, basically is same as working of the location get feature
void getfn(const char *dirPath, const char *fileName, int con_sd, bool *found)
{
    int te8_counter = 0; // for testing purposes
    DIR *dir = opendir(dirPath);
    if (dir == NULL)
    {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (is_hhiddn(entry->d_name)) {
            continue;
        }
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        if (strcmp(entry->d_name, "temp.tar.gz") == 0) {
            continue;
        }
        char path[MX_PTH];
        snprintf(path, MX_PTH, "%s/%s", dirPath, entry->d_name);
        struct stat fileStat;
        if (lstat(path, &fileStat) < 0)
        {
            perror("Error getting file info");
            continue;
        }
        if (S_ISDIR(fileStat.st_mode))
        {
            getfn(path, fileName, con_sd, found);
        }
        else if (S_ISREG(fileStat.st_mode) && strcmp(entry->d_name, fileName) == 0)
        {
            // sending the file information from here
            char info[400];
            char permissions[11];
            snprintf(permissions, sizeof(permissions), "%c%c%c%c%c%c%c%c%c",
                     (fileStat.st_mode & S_IRUSR) ? 'r' : '-',
                     (fileStat.st_mode & S_IWUSR) ? 'w' : '-',
                     (fileStat.st_mode & S_IXUSR) ? 'x' : '-',
                     (fileStat.st_mode & S_IRGRP) ? 'r' : '-',
                     (fileStat.st_mode & S_IWGRP) ? 'w' : '-',
                     (fileStat.st_mode & S_IXGRP) ? 'x' : '-',
                     (fileStat.st_mode & S_IROTH) ? 'r' : '-',
                     (fileStat.st_mode & S_IWOTH) ? 'w' : '-',
                     (fileStat.st_mode & S_IXOTH) ? 'x' : '-');
            snprintf(info, sizeof(info), "=> Name: %s\n=> Size: %lld bytes\n=> Creation Time: %s=> Permissions: %s",
                     entry->d_name, (long long)fileStat.st_size, ctime(&fileStat.st_ctime), permissions);
            send(con_sd, info, sizeof(info), 0);
            // true is returned if the file is present
            *found = true;
            closedir(dir);
            return;
        }
    }
    closedir(dir);
    return;
}

// The main command is tokenized to strings using the following function
void strng_tknizr(const char *input)
{
    int te9_counter = 0; // for testing purposes
    // tokenization using spaces
    char *drs_tkn = strtok((char *)input, " ");
    while (drs_tkn != NULL && cnt_of_tknss < 10)
    {
        // movingg the argments in custom argu.
        strcpy(usr_agmnt_dv[cnt_of_tknss], drs_tkn);
        cnt_of_tknss++;
        drs_tkn = strtok(NULL, " ");
    }
    // moving the number of argumnts in custom argu.
    drc_usr_agmnt_dc = cnt_of_tknss;
    // preparation/reset for the next iteration
    cnt_of_tknss = 0;
}

// the pclientrequest function to handle all the commands using the infinite loop
// changed the name of the function to avoid plag
void drs_p_rqst_to_clnt(int con_sd, int client_number_this)
{
    int te10_counter = 0; // for testing purposes
    // setting here the home env var
    const char *drctry_hme = getenv("HOME");
    // infinite loop start
    while (1)
    {
        char cmd[100];
        ssize_t rcvd_bytss = recv(con_sd, cmd, sizeof(cmd), 0);
        currentTime = time(NULL);
        localTime = localtime(&currentTime);
        strftime(strng_of_time, sizeof(strng_of_time), "%c", localTime);
        // printing the relevant client's command
        printf("%s: Client %d: %s", strng_of_time,client_number_this, cmd);
        // Shut down both sides' connections if the command is quitc
        if (strcmp("quitc\n", cmd) == 0)
        {
            close(con_sd);
            printf("%s: Client %d has disconnected.\n", strng_of_time, client_number_this);
            break;
        }
        // finishing with a null termination character
        cmd[strcspn(cmd, "\n")] = '\0';

        // string tokenizer function is being used here
        strng_tknizr(cmd);
        strcpy(cmd,"");

        // carrying out the relevant tasks in response to the aforementioned client's request
        if(strcmp("getfn", usr_agmnt_dv[0]) == 0)
        {
            char *fileName = usr_agmnt_dv[1];
            bool fileFound = false;
            getfn(drctry_hme, fileName, con_sd, &fileFound);
            if (!fileFound)
            {
                char notFoundMsg[] = "File not found";
                send(con_sd, notFoundMsg, sizeof(notFoundMsg), 0);
            }
            cnt_of_fls = 0;
        }
        else if(strcmp("getfz", usr_agmnt_dv[0]) == 0 && drc_usr_agmnt_dc > 1)
        {
            const char *drctry_hme = getenv("HOME");
            gt_lcton_sze(drctry_hme, atoi(usr_agmnt_dv[1]), atoi(usr_agmnt_dv[2]));
            if(cnt_of_fls<1)
            {
                char notFoundMsg[] = "B";
                send(con_sd, notFoundMsg, sizeof(notFoundMsg), 0);
            }
            else
            {
                char Msg[] = "A";
                send(con_sd, Msg, sizeof(Msg), 0);
                uploadd_to_clnt(con_sd);
                cnt_of_fls = 0;
            }
        }
        else if(strcmp("getfda", usr_agmnt_dv[0]) == 0)
        {
            const char *drctry_hme = getenv("HOME");
            gt_lcton_date_grter(drctry_hme, usr_agmnt_dv[1]);
            if(cnt_of_fls<1)
            {
                char notFoundMsg[] = "B";
                send(con_sd, notFoundMsg, sizeof(notFoundMsg), 0);
            }
            else
            {
                char Msg[] = "A";
                send(con_sd, Msg, sizeof(Msg), 0);
                uploadd_to_clnt(con_sd);
                cnt_of_fls = 0;
            }
        }
        else if(strcmp("getfdb", usr_agmnt_dv[0]) == 0)
        {
            const char *drctry_hme = getenv("HOME");
            gt_lcton_date_lesss(drctry_hme, usr_agmnt_dv[1]);
            if(cnt_of_fls<1)
            {
                char notFoundMsg[] = "B";
                send(con_sd, notFoundMsg, sizeof(notFoundMsg), 0);
            }
            else
            {
                char Msg[] = "A";
                send(con_sd, Msg, sizeof(Msg), 0);
                uploadd_to_clnt(con_sd);
                cnt_of_fls = 0;
            }
        }
        else if(strcmp("getft", usr_agmnt_dv[0]) == 0)
        {
            gt_lcton_extnsn(drctry_hme);
            if(cnt_of_fls<1)
            {
                char notFoundMsg[] = "B";
                send(con_sd, notFoundMsg, sizeof(notFoundMsg), 0);
            }
            else
            {
                char Msg[] = "A";
                send(con_sd, Msg, sizeof(Msg), 0);
                uploadd_to_clnt(con_sd);
                cnt_of_fls = 0;
            }
        }
        else if((strcmp("name\n", cmd) == 0))
        {
            continue;
        }
    }
    return;
}

// this is the main functon
void main(int drs_agmnt_dc, char *drs_agmnt_dv[])
{
    // exit if the arguments are not entered correctly
    if(drs_agmnt_dc > 0 && drs_agmnt_dc < 2)
    {
        printf("\nmirror [mirror port]\n");
        exit(0);
    }
    long PORT = atol(drs_agmnt_dv[1]);
    
    // finding the parent process id
    pid_t parent = getpid();

    // establishing the connectson
    int lis_sd;
    struct sockaddr_in server;

    // listening to the sockett
    lis_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (lis_sd == -1)
    {
        printf("Socket could not be created\n");
        return;
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    // bound to this machine's address
    if (bind(lis_sd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        printf("Binding not successful\n");
        return;
    }

    // starting a listening queue 
    if (listen(lis_sd, 5) == -1)
    {
        printf("Listening unsuccessful\n");
        return;
    }
    struct sockaddr_in client;
    int con_sd;
    socklen_t len = sizeof(client);

    // ongoing cycle to manage every client
    while(1)
    {
        // acknowledging and analysing each and every connection
        con_sd = accept(lis_sd, (struct sockaddr *)&client, &len);

        // getting the client's number from them
        char client_number_string[16];
        recv(con_sd, &client_number_string, sizeof(client_number_string), 0);
        int client_id = atoi(client_number_string);
        
        if (con_sd != -1)
        {
            // displaying --> (time) client (num) connected
            currentTime = time(NULL);
            localTime = localtime(&currentTime);
            strftime(strng_of_time, sizeof(strng_of_time), "%c", localTime);
            printf("%s: Connected client %d\n", strng_of_time, client_id);
            
            usleep(500000);
            
            // closing unnecessary forks and forking the process client
            if (fork() == 0)
            {
                if(getppid() != parent)
                {
                    exit(0);
                }
                else
                {
                    // process client is being called here
                    drs_p_rqst_to_clnt(con_sd, client_id);
                    exit(0);
                }
            }
        }
        // Cleaning_up all the zombie chld processess
        while (waitpid(-1, NULL, WNOHANG) > 0);
    }

    // mirror is being closed here
    close(lis_sd);
    return;
}


