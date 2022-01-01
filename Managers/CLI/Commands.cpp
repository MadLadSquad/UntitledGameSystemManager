#include "Commands.hpp"
#include <iostream>
#include <yaml.h>
#include <Core/Utilities.hpp>
#include <unistd.h>
#include <pwd.h>
#include <Core/Defines.hpp>
#include <UVKLog.h>
#include <cstring>

void UGM::Managers::CLI::help()
{
    std::cout << R"(Untitled Linux Game Manager - A game system manager for linux
Commands:
    --help/help/-h/-H - help

    --new/new/-N/-n <container-name> <nvidia/mesa/n/m> - creates a new container given a name and GPU driver type
    --delete/delete/-D/-d <container-name> - deletes a container
    --delete/delete/-D/-d <container-name> -y - deletes a container but doesn't ask for permission

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
    --unpin/unpin <container-name> <program> - will remove a pin from the container
)" << std::endl;

    std::cout << "Program developed and maintained by MadLad Squad!" << std::endl;
}

void UGM::Managers::CLI::restart(bool bAll, char* name)
{
    if (bAll)
    {
        YAML::Node out;
        try
        {
            auto* passwd = getpwuid(geteuid());
            std::string file = std::string("/home/") + passwd->pw_name + "/.config/UntitledLinuxGameManager/config/layout.yaml";
            out = YAML::LoadFile(file);
        }
        catch (YAML::BadFile&)
        {
            logger.consoleLog("Couldn't open the config file!", UVK_LOG_TYPE_ERROR);
            return;
        }
        auto containers = out["containers"];
        if (containers)
        {
            for (const YAML::Node& a : containers)
            {
                if (a["container"])
                {
                    auto nm = a["container"].as<std::string>();
                    char* const args[] = { (char*)"lxc", (char*)"restart", const_cast<char*>(nm.c_str()), nullptr };
                    UGM::Core::Utilities::execandwait(args);
                }
            }
        }
    }
    else
    {
        char* const args[] = { (char*)"lxc", (char*)"restart", name, nullptr };
        UGM::Core::Utilities::execandwait(args);
    }
}

void UGM::Managers::CLI::poweroff(bool bAll, char* name)
{
    if (bAll)
    {
        YAML::Node out;
        try
        {
            auto* passwd = getpwuid(geteuid());
            std::string file = std::string("/home/") + passwd->pw_name + "/.config/UntitledLinuxGameManager/config/layout.yaml";
            out = YAML::LoadFile(file);
        }
        catch (YAML::BadFile&)
        {
            logger.consoleLog("Couldn't open the config file!", UVK_LOG_TYPE_ERROR);
            return;
        }
        auto containers = out["containers"];
        if (containers)
        {
            for (const YAML::Node& a : containers)
            {
                if (a["container"])
                {
                    auto nm = a["container"].as<std::string>();
                    char* const args[] = { (char*)"lxc", (char*)"stop", const_cast<char*>(nm.c_str()), nullptr };
                    UGM::Core::Utilities::execandwait(args);
                }
            }
        }
    }
    else
    {
        char* const args[] = { (char*)"lxc", (char*)"stop", name, nullptr };
        UGM::Core::Utilities::execandwait(args);
    }
}

void UGM::Managers::CLI::poweron(bool bAll, char* name)
{

    if (bAll)
    {
        YAML::Node out;
        try
        {
            auto* passwd = getpwuid(geteuid());
            std::string file = std::string("/home/") + passwd->pw_name + "/.config/UntitledLinuxGameManager/config/layout.yaml";
            out = YAML::LoadFile(file);
        }
        catch (YAML::BadFile&)
        {
            logger.consoleLog("Couldn't open the config file!", UVK_LOG_TYPE_ERROR);
            return;
        }
        auto containers = out["containers"];
        if (containers)
        {
            for (const YAML::Node& a : containers)
            {
                if (a["container"])
                {
                    auto nm = a["container"].as<std::string>();
                    char* const args[] = { (char*)"lxc", (char*)"start", const_cast<char*>(nm.c_str()), nullptr };
                    UGM::Core::Utilities::execandwait(args);
                }
            }
        }
    }
    else
    {
        char* const args[] = { (char*)"lxc", (char*)"start", name, nullptr };
        UGM::Core::Utilities::execandwait(args);
    }

}

void UGM::Managers::CLI::list()
{
    YAML::Node out;
    try
    {
        auto* passwd = getpwuid(geteuid());
        std::string file = std::string("/home/") + passwd->pw_name + "/.config/UntitledLinuxGameManager/config/layout.yaml";
        out = YAML::LoadFile(file);
    }
    catch (YAML::BadFile&)
    {
        logger.consoleLog("Couldn't open the config file!", UVK_LOG_TYPE_ERROR);
        return;
    }
    auto containers = out["containers"];
    if (containers)
    {
        size_t j = 1;
        for (const YAML::Node& a : containers)
        {
            if (a["container"] && a["pins"])
            {
                std::cout << j << ". " << a["container"].as<std::string>() << ":" << std::endl;
                size_t i = 1;
                for (const auto& p : a["pins"])
                {
                    std::cout << "    " << i << ". " << p.as<std::string>() << std::endl;
                    i++;
                }
                j++;
            }
        }
    }
}

void UGM::Managers::CLI::execBash(char* containerName)
{
    std::string cmd = std::string("lxc exec ") + containerName + " -- bash";
    // The current implementation doesn't work
    char* const args[] = { (char*)"bash", (char*)"-c", const_cast<char*>(cmd.c_str()), nullptr };
    execvp(args[0], args);
}

void UGM::Managers::CLI::execProgram(char* containerName, char* command)
{
    // construct a really large string to run our command
    std::string cmd = (std::string("lxc exec ") + std::string(containerName) + " -- bash -c \"su ubuntu -c '" + std::string(command) + "'\" &> /tmp/" + command + ".log & disown");
    // Ugly ass const cast right here
    char* const args[] = { (char*)"bash", (char*)"-c", const_cast<char*>(cmd.c_str()), nullptr };
    UGM::Core::Utilities::execandwait(args);
}

void UGM::Managers::CLI::genscript(char* containerName, char* program)
{
    auto* passwd = getpwuid(geteuid());

    std_filesystem::path path = std::string("/home/") + std::string(passwd->pw_name) + std::string("/.config/UntitledLinuxGameManager/scripts/") + std::string(program) + ".sh";
    std::ofstream out(path);
    out << "#!/bin/bash" << std::endl;
    out << R"(if [ "$1" == "-q" ] || [ "$1" == "-Q" ] || [ "$1" == "quiet" ] || [ "$1" == "--quiet" ]; then)" << std::endl;
    out << "    lxc exec " << containerName << " -- bash -c \"su ubuntu -c '" << program << "'\" &> /tmp/" << program << ".log & disown" << std::endl;
    out << "else" << std::endl;
    out << "    lxc exec " << containerName << " -- bash -c \"su ubuntu -c '" << program << "'\"" << std::endl;
    out << "fi" << std::endl;
    out.close();

    // Use std::filesystem to change the perms to allow execution of the script, equivalent to chmod +x on unix systems
    std_filesystem::permissions(path, std_filesystem::perms::owner_exec | std_filesystem::perms::group_exec,std_filesystem::perm_options::add);
}

void UGM::Managers::CLI::newContainer(char* name, char* driverType)
{
    auto* passwd = getpwuid(geteuid());
    std::string cmd = std::string("/home/") + passwd->pw_name + std::string("/.config/UntitledLinuxGameManager/scripts/ugm-cli-install.sh");
    char* const args[] = { const_cast<char*>(cmd.c_str()), (char*)"--name", name, (char*)"--driver", driverType, nullptr };
    UGM::Core::Utilities::execandwait(args);
}

void UGM::Managers::CLI::pin(char* containerName, char* program)
{
    YAML::Node out;
    auto* passwd = getpwuid(geteuid());
    std::string file = std::string("/home/") + passwd->pw_name + "/.config/UntitledLinuxGameManager/config/layout.yaml";
    try
    {
        out = YAML::LoadFile(file);
    }
    catch (YAML::BadFile&)
    {
        logger.consoleLog("Couldn't open the config file!", UVK_LOG_TYPE_ERROR);
        return;
    }
    YAML::Node containers = out["containers"];
    if (containers)
    {
        for (YAML::Node a : containers)
        {
            if (a["container"] && a["pins"] && a["container"].as<std::string>() == containerName)
            {
                bool bExists = false;
                for (const auto& p : a["pins"])
                {
                    if (p.as<std::string>() == program)
                    {
                        bExists = true;
                        goto exit_for;
                    }
                }
exit_for:
                if (!bExists)
                {
                    YAML::Node arr = a["pins"];
                    arr.push_back(std::string(program));
                    a["pins"] = arr;
                }
                break;
            }
        }
    }
    out["containers"] = containers;
    std::ofstream fout(file);
    fout << out << std::endl;
    fout.close();
}

void UGM::Managers::CLI::deleteContainer(char* name)
{
    char* const args[] = { (char*)"lxc", (char*)"delete", name, nullptr };
    UGM::Core::Utilities::execandwait(args);
}

void UGM::Managers::CLI::unpin(char* containerName, char* program)
{
    YAML::Node out;
    auto* passwd = getpwuid(geteuid());
    std::string file = std::string("/home/") + passwd->pw_name + "/.config/UntitledLinuxGameManager/config/layout.yaml";
    try
    {
        out = YAML::LoadFile(file);
    }
    catch (YAML::BadFile&)
    {
        logger.consoleLog("Couldn't open the config file!", UVK_LOG_TYPE_ERROR);
        return;
    }
    YAML::Node containers = out["containers"];
    if (containers)
    {
        for (YAML::Node a : containers)
        {
            if (a["container"] && a["pins"] && a["container"].as<std::string>() == containerName)
            {
                bool bExists = false;
                for (const auto& p : a["pins"])
                {
                    if (p.as<std::string>() == program)
                    {
                        bExists = true;
                        goto exit_for2;
                    }
                }
exit_for2:
                if (bExists)
                {
                    YAML::Node arr = a["pins"];
                    auto str = Dump(arr);
                    auto it = str.find(std::string("\n- ") + program);
                    if (it != std::string::npos)
                        str.erase(it, it + strlen(program) + strlen("\n- "));

                    YAML::Node f = YAML::Load(str);
                    a["pins"] = f;
                }
                break;
            }
        }
    }
    out["containers"] = containers;
    std::ofstream fout(file);
    fout << out << std::endl;
    fout.close();
}