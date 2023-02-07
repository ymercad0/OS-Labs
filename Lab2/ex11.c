#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
   char command[300];
   //copy the command to download the file
   strcpy(command, "wget -q https://www.c-programming-simple-steps.com/support-files/return-statement.zip");
   //execute the command
   system(command);
   //copy the commmand to list the files
   strcpy(command, "ls | grep return");
   //execute the command
   system(command);
   return(0);
}