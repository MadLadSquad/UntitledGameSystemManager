#include "Commands.hpp"
#include <cstdlib>
#include <yaml.h>
#include <pwd.h>
#include <UVKLog.h>
#include <cstring>
#include <unistd.h>
#include <Core/Defines.hpp>

void UGM::Managers::GUI::restart(bool bAll, char* name)
{

}

void UGM::Managers::GUI::poweroff(bool bAll, char* name)
{

}

void UGM::Managers::GUI::poweron(bool bAll, char* name)
{

}

void UGM::Managers::GUI::newContainer()
{
    // Who cares if the program stops while the wizard is running, it's even better since we don't have to worry about
    // collisions
    system("ugm-gui-installer");
}

void UGM::Managers::GUI::deleteContainer(char* name)
{

}

void UGM::Managers::GUI::execProgram(char* containerName, char* command)
{

}

void UGM::Managers::GUI::genscript(char* containerName, char* program)
{
    auto* passwd = getpwuid(geteuid());

    std_filesystem::path path = std::string("/home/") + std::string(passwd->pw_name) + std::string("/.config/UntitledLinuxGameManager/scripts/") + std::string(containerName) + "-" + std::string(program) + ".sh";
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

void UGM::Managers::GUI::pin(char* containerName, char* program)
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

void UGM::Managers::GUI::unpin(char* containerName, char* program)
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