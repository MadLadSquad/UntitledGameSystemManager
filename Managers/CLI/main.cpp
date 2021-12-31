#include "Commands.hpp"
#include <Core/Utilities.hpp>
#include <UVKLog.h>

int main(int argc, char** argv)
{
    if (argc < 2 || (argc == 2 && (UGM::Core::Utilities::toLower(argv[1]) == "help" || UGM::Core::Utilities::toLower(argv[1]) == "--help" || UGM::Core::Utilities::toLower(argv[1]) == "-h" || UGM::Core::Utilities::toLower(argv[1]) == "-H")))
    {
        UGM::Managers::CLI::help();
        return 0;
    }
    else if (argc == 2)
    {
        if (UGM::Core::Utilities::toLower(argv[1]) == "list" || UGM::Core::Utilities::toLower(argv[1]) == "--list" || UGM::Core::Utilities::toLower(argv[1]) == "-l" || UGM::Core::Utilities::toLower(argv[1]) == "--L")
        {
            UGM::Managers::CLI::list();
        }
        return 0;
    }
    else if (argc == 3)
    {
        if (UGM::Core::Utilities::toLower(argv[1]) == "restart" || UGM::Core::Utilities::toLower(argv[1]) == "--restart" || UGM::Core::Utilities::toLower(argv[1]) == "-r" || UGM::Core::Utilities::toLower(argv[1]) == "--R")
        {
            if (UGM::Core::Utilities::toLower(argv[2]) == "--all" || UGM::Core::Utilities::toLower(argv[2]) == "all")
                UGM::Managers::CLI::restart(true);
            else
                UGM::Managers::CLI::restart(false, argv[2]);
        }
        else if (UGM::Core::Utilities::toLower(argv[1]) == "poweroff" || UGM::Core::Utilities::toLower(argv[1]) == "--poweroff" || UGM::Core::Utilities::toLower(argv[1]) == "stop" || UGM::Core::Utilities::toLower(argv[1]) == "--stop")
        {
            if (UGM::Core::Utilities::toLower(argv[2]) == "--all" || UGM::Core::Utilities::toLower(argv[2]) == "all")
                UGM::Managers::CLI::poweroff(true);
            else
                UGM::Managers::CLI::poweroff(false, argv[2]);
        }
        else if (UGM::Core::Utilities::toLower(argv[1]) == "poweron" || UGM::Core::Utilities::toLower(argv[1]) == "--poweron" || UGM::Core::Utilities::toLower(argv[1]) == "start" || UGM::Core::Utilities::toLower(argv[1]) == "--start")
        {
            if (UGM::Core::Utilities::toLower(argv[2]) == "--all" || UGM::Core::Utilities::toLower(argv[2]) == "all")
                UGM::Managers::CLI::poweron(true);
            else
                UGM::Managers::CLI::poweron(false, argv[2]);
        }
        else if (UGM::Core::Utilities::toLower(argv[1]) == "new" || UGM::Core::Utilities::toLower(argv[1]) == "--new" || UGM::Core::Utilities::toLower(argv[1]) == "-n" || UGM::Core::Utilities::toLower(argv[1]) == "-N")
        {
            UGM::Managers::CLI::newContainer(argv[2]);
        }
        else if (UGM::Core::Utilities::toLower(argv[1]) == "exec" || UGM::Core::Utilities::toLower(argv[1]) == "--exec")
        {
            UGM::Managers::CLI::execBash(argv[2]);
        }
        return 0;
    }
    else if (argc == 4)
    {
        if (UGM::Core::Utilities::toLower(argv[1]) == "exec" || UGM::Core::Utilities::toLower(argv[1]) == "--exec")
        {
            UGM::Managers::CLI::execProgram(argv[2], argv[3]);
        }
        else if (UGM::Core::Utilities::toLower(argv[1]) == "genscript" || UGM::Core::Utilities::toLower(argv[1]) == "--genscript" || UGM::Core::Utilities::toLower(argv[1]) == "-g" || UGM::Core::Utilities::toLower(argv[1]) == "-G")
        {
            UGM::Managers::CLI::genscript(argv[2], argv[3]);
        }
        else if (UGM::Core::Utilities::toLower(argv[1]) == "pin" || UGM::Core::Utilities::toLower(argv[1]) == "--pin" || UGM::Core::Utilities::toLower(argv[1]) == "-p" || UGM::Core::Utilities::toLower(argv[1]) == "-P")
        {
            UGM::Managers::CLI::pin(argv[2], argv[3]);
        }
        return 0;
    }
}