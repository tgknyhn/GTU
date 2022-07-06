#ifndef CLIENTX_H
#define CLIENTX_H

// Defines
#define SIGNAL 1
#define NO_SIGNAL 0

// Function Declarations
void control_cla(int argc, char* argv[]);
void initializeSignalHandler ();
void initializeClientFIFO();
void sendRequest();
void receiveResponse();

void printTimeAndPID();
void printRequestInfo();
void printResponseInfo();

void handleSIG(int signal);
void removeFifos();
void terminateProgram();

#endif