#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS 10
#define MAX_PIPE 10
#define MAX_CMD_LEN 1024

int cmd_argc;
char *cmd_argv[MAX_ARGS];
char *cmd_pipe[MAX_PIPE][MAX_ARGS];
int pipe_cnt;
int child_running = 0;

void parse_cmd(char *cmd)
{
    cmd_argc = 0;
    char *token = strtok(cmd, " \t\n");
    while (token != NULL && cmd_argc < MAX_ARGS) {
        cmd_argv[cmd_argc++] = token;
        token = strtok(NULL, " \t\n");
    }
    cmd_argv[cmd_argc] = NULL;
}

void parse_pipe(char *cmd)
{
    pipe_cnt = 0;
    char *pipe_token = strtok(cmd, "|");
    while (pipe_token != NULL && pipe_cnt < MAX_PIPE) {
        // Remove any leading/trailing whitespace from the command
        char *cmd_token = strtok(pipe_token, " \t\n");
        int cmd_argc = 0;
        while (cmd_token != NULL && cmd_argc < MAX_ARGS) {
            cmd_pipe[pipe_cnt][cmd_argc++] = cmd_token;
            cmd_token = strtok(NULL, " \t\n");
        }
        cmd_pipe[pipe_cnt][cmd_argc] = NULL;
        pipe_cnt++;
        pipe_token = strtok(NULL, "|");
    }
}

void handle_signal(int sig)
{
    if (child_running) {
        // If a child process is running, send the SIGINT signal to it
        kill(0, SIGINT);
        child_running = 0; // Reset child_running variable to 0
    } else {
        printf("\n");
    }
}

void handle_child_signal(int signal) {
    if (signal == SIGINT) {
        printf("Child process received SIGINT signal, exiting immediately\n");
        exit(0);
    }
}

void run_cmd(char **cmd_argv, int fd_in, int fd_out, int append_out)
{
    pid_t pid = fork();
    if (pid < 0) {
        printf("Error: fork failed\n");
        exit(1);
    } else if (pid == 0) { // child process
        signal(SIGINT, handle_child_signal); // register signal handler function
        child_running = 1; // set child_running variable

        // execute command
        if (strcmp(cmd_argv[0], "ls") == 0) {
            // Handle ls as an external command
            execvp(cmd_argv[0], cmd_argv);
            perror("execvp");
            exit(1);
        }
        else if (strcmp(cmd_argv[0], "copy") == 0) {
            if ((cmd_argc == 4 && strcmp(cmd_argv[1], "-f") == 0) || (cmd_argc == 3)) {
                int fd_src = open(cmd_argv[cmd_argc - 2], O_RDONLY);
                if (fd_src == -1) {
                    perror("open");
                    exit(1);
                }

                int fd_dest = open(cmd_argv[cmd_argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd_dest == -1) {
                    perror("open");
                    exit(1);
                }

            char buffer[1024];
            ssize_t bytes_read;
            while ((bytes_read = read(fd_src, buffer, sizeof(buffer))) > 0) {
            if (write(fd_dest, buffer, bytes_read) != bytes_read) {
            perror("write");
            exit(1);
            }
        }
            if (bytes_read == -1) {
                perror("read");
                exit(1);
            }

            close(fd_src);
            close(fd_dest);

            exit(0);
        } else {
            printf("Usage: copy [-f] <source> <destination>\n");
            exit(1);
        }
    }
    else {
        // Handle any other external command
        execvp(cmd_argv[0], cmd_argv);
        perror("execvp");
        exit(1);
    }
} else { // parent process
    // Wait for child process to complete
    int child_status;
    waitpid(pid, &child_status, 0);

    // Check child process exit status
    if (WIFEXITED(child_status)) {
        int exit_status = WEXITSTATUS(child_status);
        if (exit_status == 0) {
            printf("Command executed successfully.\n");
        } else {
            printf("Command exited with status %d.\n", exit_status);
        }
    } else if (WIFSIGNALED(child_status)) {
        printf("Command exited due to signal %d.\n", WTERMSIG(child_status));
    }
}
    return;
}
int main(int argc, char **argv)
{
    char cmd[MAX_CMD_LEN];

    // Register signal handler function
    signal(SIGINT, handle_signal);

    while (1) {
        // Print the shell prompt
        printf("stshell> ");
        fflush(stdout);

        // Read the user input
        if (fgets(cmd, MAX_CMD_LEN, stdin) == NULL) {
            // End of file or error occurred
            break;
        }

        // Remove newline character from the end of the command
        cmd[strcspn(cmd, "\n")] = '\0';

        // Parse the command into individual commands and pipes
        parse_pipe(cmd);

        // Set up pipes and execute commands
        int fd_in = 0;
        for (int i = 0; i < pipe_cnt; i++) {
            int fd_pipe[2];
            if (pipe(fd_pipe) == -1) {
                perror("pipe");
                exit(1);
            }

            // Execute the command and pass the appropriate file descriptors
            run_cmd(cmd_pipe[i], fd_in, fd_pipe[1], i < pipe_cnt - 1);

            // Close the write end of the pipe
            close(fd_pipe[1]);

            // Set the read end of the pipe as the input for the next command
            fd_in = fd_pipe[0];
        }

        // Wait for all child processes to complete
        while (wait(NULL) > 0) {
            // Do nothing
        }
    }

    return 0;
}