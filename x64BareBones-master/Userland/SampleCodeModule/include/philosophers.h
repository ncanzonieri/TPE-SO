#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

int philosopher(int argc, char** argv);
void printStatus();
void printInstructions();
int processInput(char c);
void addPhilo(int i);
void removePhilo(int i);

#endif