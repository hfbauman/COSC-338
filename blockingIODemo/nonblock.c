#include <stdio.h>
#include <unistd.h>
#include <string.h>


// buffer into which we will write, and only
// actually "flush" the data, via write,
// when enough data has accumulated
#define BUFFER_SIZE 100
static char large_buffer[BUFFER_SIZE];
static int buffer_index = 0;

// xwrite is a wrapper around write, with the same interface,
// but defers the expensive "write" system call for
// later
ssize_t xwrite(int fd,  const  void * buf, size_t count){
  if (buffer_index + count > BUFFER_SIZE){
    write(STDOUT_FILENO, large_buffer, buffer_index);
    buffer_index = 0;
  }
  memcpy(&large_buffer[buffer_index], buf, count);
  buffer_index += count;
}

int main(int argc, char* argv[]){

  char buffer[10];
  ssize_t numberRead;
  while ( (numberRead = read(STDIN_FILENO, buffer, 10)) > 0){
    xwrite(STDOUT_FILENO, buffer, numberRead);
  }
  return 0;

}
