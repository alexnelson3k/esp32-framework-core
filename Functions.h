#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>

void initializeSystem();
void registerModules();
void processCoreTasks();

String formatUptime(unsigned long ms);
String boolToString(bool value);

void safeRestart();

#endif