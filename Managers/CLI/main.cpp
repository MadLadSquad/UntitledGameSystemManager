#include "Commands.hpp"
#include <Core/Utilities.hpp>
#include <UVKLog.h>

int main(int argc, char** argv)
{
    if (argc < 2 || (argc == 2 && (UGM::Core::Utilities::toLower(argv[1]) == "help" || UGM::Core::Utilities::toLower(argv[1]) == "--help" || UGM::Core::Utilities::toLower(argv[1]) == "-h")))
    {
        UGM::Managers::CLI::help();
        return 0;
    }
    else if (argc == 2)
    {
        if (UGM::Core::Utilities::toLower(argv[1]) == "list" || UGM::Core::Utilities::toLower(argv[1]) == "--list" || UGM::Core::Utilities::toLower(argv[1]) == "-l")
            UGM::Managers::CLI::list();
        return 0;
    }
    else if (argc == 3)
    {
        if (UGM::Core::Utilities::toLower(argv[1]) == "restart" || UGM::Core::Utilities::toLower(argv[1]) == "--restart" || UGM::Core::Utilities::toLower(argv[1]) == "-r")
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
        else if (UGM::Core::Utilities::toLower(argv[1]) == "exec" || UGM::Core::Utilities::toLower(argv[1]) == "--exec")
            UGM::Managers::CLI::execBash(argv[2]);
        else if (UGM::Core::Utilities::toLower(argv[1]) == "--delete" || UGM::Core::Utilities::toLower(argv[1]) == "delete" || UGM::Core::Utilities::toLower(argv[1]) == "-d")
        {
            std::cout << "You're trying to delete a container with name: " << argv[2] << std::endl;
            std::cout << "\x1B[31mYOU CANNOT REVERSE THIS ACTION!!!\x1B[0m" << std::endl;
            std::cout << "To continue enter the following \"Yes, do as I say!\": " << std::flush;

            std::string in;
            std::getline(std::cin, in);

            if (UGM::Core::Utilities::toLower(in.c_str()).find("yes, do as i say!") != std::string::npos)
                UGM::Managers::CLI::deleteContainer(argv[2]);
            else
                logger.consoleLog("Invalid input!", UVK_LOG_TYPE_ERROR);
        }
        else if (UGM::Core::Utilities::toLower(argv[1]) == "--update" || UGM::Core::Utilities::toLower(argv[1]) == "update" || UGM::Core::Utilities::toLower(argv[1]) == "u")
        {

        }
        return 0;
    }
    else if (argc == 4)
    {
        if (UGM::Core::Utilities::toLower(argv[1]) == "exec" || UGM::Core::Utilities::toLower(argv[1]) == "--exec")
            UGM::Managers::CLI::execProgram(argv[2], argv[3]);
        else if (UGM::Core::Utilities::toLower(argv[1]) == "genscript" || UGM::Core::Utilities::toLower(argv[1]) == "--genscript" || UGM::Core::Utilities::toLower(argv[1]) == "-g")
            UGM::Managers::CLI::genscript(argv[2], argv[3]);
        else if (UGM::Core::Utilities::toLower(argv[1]) == "pin" || UGM::Core::Utilities::toLower(argv[1]) == "--pin" || UGM::Core::Utilities::toLower(argv[1]) == "-p")
            UGM::Managers::CLI::pin(argv[2], argv[3]);
        else if (UGM::Core::Utilities::toLower(argv[1]) == "new" || UGM::Core::Utilities::toLower(argv[1]) == "--new" || UGM::Core::Utilities::toLower(argv[1]) == "-n")
        {
            if (UGM::Core::Utilities::toLower(argv[3]) == "n" || UGM::Core::Utilities::toLower(argv[3]) == "nvidia")
                UGM::Managers::CLI::newContainer(argv[2], (char*)"N");
            else if (UGM::Core::Utilities::toLower(argv[3]) == "m" || UGM::Core::Utilities::toLower(argv[3]) == "mesa")
                UGM::Managers::CLI::newContainer(argv[2], (char*)"M");
            else
                logger.consoleLog("Invalid driver type: ", UVK_LOG_TYPE_ERROR, argv[3]);
        }
        else if ((UGM::Core::Utilities::toLower(argv[1]) == "--delete" || UGM::Core::Utilities::toLower(argv[1]) == "delete" || UGM::Core::Utilities::toLower(argv[1]) == "-d") && (UGM::Core::Utilities::toLower(argv[3]) == "-y" || UGM::Core::Utilities::toLower(argv[3]) == "--yes"))
            UGM::Managers::CLI::deleteContainer(argv[2]);
        else if (UGM::Core::Utilities::toLower(argv[1]) == "unpin" || UGM::Core::Utilities::toLower(argv[1]) == "--unpin")
            UGM::Managers::CLI::unpin(argv[2], argv[3]);
        return 0;
    }
}