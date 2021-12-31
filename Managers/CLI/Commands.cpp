#include "Commands.hpp"
#include <iostream>
#include <yaml.h>
#include <Core/Utilities.hpp>
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>

void UGM::Managers::CLI::help()
{
    std::cout << R"(Untitled Linux Game Manager - A game system manager for linux
Commands:
    --help/help/-h/-H - help

    --new/new/-N/-n <container-name> - creates a new container

    --exec/exec <container-name> - executes your current shell as bash
    --exec/exec <container-name> <command> - executes a shell command on the container specified

    --poweroff/poweroff/stop/--stop <container-name> - powers off a container
    --poweroff/poweroff/stop/--stop --all/all - powers off all containers

    --poweron/poweron/start/--start <container-name> - powers on a container
    --poweron/poweron/start/--start --all/all -- powers on all containers

    --restart/restart <container-name> - restarts the container
    --restart/restart --all/all - restarts all containers

    --list/list/-l/-L - lists all containers and their pinned applications
    --genscript/genscript/-g/-G <container-name> <program> - will generate a program start script for easy access
    --pin/pin/-p/-P <container-name> <program> - will pin a program for easy access
)" << std::endl;

    std::cout << "Program developed and maintained by MadLad Squad!" << std::endl;
}

void UGM::Managers::CLI::restart(bool bAll, char* name)
{
    char* const args[] = { (char*)"lxc", (char*)"restart", name, nullptr };
    if (bAll)
    {

    }
    else
        UGM::Core::Utilities::execandwait(args);
}

void UGM::Managers::CLI::poweroff(bool bAll, char* name)
{
    char* const args[] = { (char*)"lxc", (char*)"stop", name, nullptr };
    if (bAll)
    {

    }
    else
        UGM::Core::Utilities::execandwait(args);
}

void UGM::Managers::CLI::poweron(bool bAll, char* name)
{
    char* const args[] = { (char*)"lxc", (char*)"start", name, nullptr };
    if (bAll)
    {

    }
    else
        UGM::Core::Utilities::execandwait(args);
}

void UGM::Managers::CLI::list()
{

}

void UGM::Managers::CLI::execBash(char* containerName)
{
    // Who the fuck cares if this
    char* const args[] = { (char*)"lxc", (char*)"exec", containerName, (char*)"-- bash", nullptr };
    execvp(args[0], args);
}

void UGM::Managers::CLI::execProgram(char* containerName, char* command)
{
    // construct a really large string to run our command
    std::string cmd = (std::string("lxc exec ") + std::string(containerName) + " -- bash -c 'su ubuntu -c " + std::string(command) + "' &> /tmp/" + command + ".log & disown");
    // Ugly ass const cast right here
    char* const args[] = { (char*)"bash", (char*)"-c", const_cast<char*>(cmd.c_str()), nullptr };
    UGM::Core::Utilities::execandwait(args);
}

void UGM::Managers::CLI::genscript(char* containerName, char* program)
{
    std::ofstream out("~/.config/UntitledLinuxGameManager/scripts/program.sh");
    out << "#!/bin/bash" << std::endl;
    out << R"(if [ "$1" == "-q" ] || [ "$1" == "-Q" ] || [ "$1" == "quiet" ] || [ "$1" == "--quiet" ]; then)" << std::endl;
    out << "    lxc exec " << containerName << " -- bash -c 'su ubuntu -c '" << program << "'' &> /tmp/" << program << ".log & disown" << std::endl;
    out << "else" << std::endl;
    out << "    lxc exec " << containerName << " -- bash -c 'su ubuntu -c '" << program << "''" << std::endl;
    out << "fi" << std::endl;
    out.close();
}

void UGM::Managers::CLI::newContainer(char* name)
{
    char* const args[] = { (char*)"bash", (char*)"-c", (char*)"ugm-cli-install", nullptr };
    UGM::Core::Utilities::execandwait(args);
}

void UGM::Managers::CLI::pin(char* containerName, char* program)
{

}