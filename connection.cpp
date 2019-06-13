#include <iostream>
#include <chrono>
#include <thread>


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <thread>
#include <chrono>

#define SOCK_PATH "./emulator_comm_socket"

int main() {
  int sock, length;

  struct sockaddr_un remote;


  if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  std::cout << "|~| Trying to connect..." << std::endl;

  remote.sun_family = AF_UNIX;
  strcpy(remote.sun_path, SOCK_PATH);
  length = strlen(remote.sun_path) + sizeof(remote.sun_family);
  if (connect(sock, (struct sockaddr *)&remote, length) == -1) {
    perror("connect");
    exit(1);
  }

  std::cout << "|+| Connected." << std::endl;

  std::string wow;

  while(true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    char str[100] = "Testing this message...."; 
    
    if (send(sock, str, strlen(str), 0) == -1) {
      perror("send");
      exit(1);
    }
    
  }

  close(sock);

  return 0;
}
