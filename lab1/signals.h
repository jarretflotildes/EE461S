#include "jobs.h"

void registerSignals();
void saveYash(pid_t yash);

void registerJobStack(job **ptr);


static void sig_int(int signo);
static void sig_stp(int signo);
static void sig_chld(int signo);
