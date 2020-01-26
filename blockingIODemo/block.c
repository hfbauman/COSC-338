#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]){

  char buffer[10];
  ssize_t numberRead;
  while ( (numberRead = read(STDIN_FILENO, buffer, 10)) > 0){
    // write is a blocking system call, which requires
    // OS overhead per call
    write(STDOUT_FILENO, buffer, numberRead);
  }
  return 0;

}
