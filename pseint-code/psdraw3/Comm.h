#ifndef COMM_H
#define COMM_H

bool Connect(int port, int id);

bool SendUpdate(int action);

bool SendHelp();

void CloseComm();

void ReadComm();

void NotifyModification();

#endif

