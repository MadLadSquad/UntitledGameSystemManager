#pragma once

namespace UGM::Managers::GUI
{
    void restart(bool bAll = false, char* name = (char*)"none");
    void poweroff(bool bAll = false, char* name = (char*)"none");
    void poweron(bool bAll = false, char* name = (char*)"none");

    void newContainer();
    void deleteContainer(char* name);

    void execProgram(char* containerName, char* command);

    void genscript(char* containerName, char* program);
    void pin(char* containerName, char* program);
    void unpin(char* containerName, char* program);
}