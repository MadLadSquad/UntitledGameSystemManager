#pragma once
#include <iostream>

namespace UGM::Managers::CLI
{
    void help();

    void restart(bool bAll = false, char* name = (char*)"none");
    void poweroff(bool bAll = false, char* name = (char*)"none");
    void poweron(bool bAll = false, char* name = (char*)"none");

    void newContainer(char* name, char* driverType);
    void deleteContainer(char* name);

    void execBash(char* containerName);
    void execProgram(char* containerName, char* command);

    void genscript(char* containerName, char* program);
    void list();
    void pin(char* containerName, char* program);
    void unpin(char* containerName, char* program);

    void update(char* containerName, char* drv, bool bGPU);
}