#include <iostream>
#include <unistd.h>   // For pipe(), fork(), read(), write(), close()
#include <sys/types.h> // For pid_t
#include <sys/wait.h>  // For wait()
#include <cstring>     // For strlen()

using namespace std;

int main() {
    int fd[2]; // File descriptors for the pipe

    if (pipe(fd) == -1) { // Check if pipe creation fails
        cerr << "Pipe failed!" << endl;
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed!" << endl;
        return 1;
    }

    if (pid == 0) { // Child process
        close(fd[1]); // Close write end
        char buffer[100];

        ssize_t bytesRead = read(fd[0], buffer, sizeof(buffer));
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0'; // Null-terminate string
            cout << "Child received: " << buffer << endl;
        } else {
            cerr << "Child: No data received" << endl;
        }

        close(fd[0]); // Close read end
    } else { // Parent process
        close(fd[0]); // Close read end
        sleep(5); // Artificial delay to create a deadlock scenario

        const char* message = "Hello from parent!";
        write(fd[1], message, strlen(message) + 1);
        close(fd[1]); // Close write end

        wait(NULL); // Wait for child to complete
    }

    return 0;
}
