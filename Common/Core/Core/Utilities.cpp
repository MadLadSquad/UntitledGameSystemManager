#include "Utilities.hpp"
#include <csignal>
#include <thread>

pid_t UGM::Core::Utilities::loadLineByLineFromPID(std::vector<std::string>& lineBuffer, char* const* command, bool bUsingThreads, std::thread* thread)
{
    int pipefd[2];
    pipe(pipefd);

    auto pid = fork();
    if (pid != -1)
    {
        if (pid == 0)
        {
            close(pipefd[0]);

            dup2(pipefd[1], STDOUT_FILENO);
            dup2(pipefd[1], STDERR_FILENO);

            close(pipefd[1]);

            execvp(command[0], command);
        }
        else
        {
            if (bUsingThreads && thread != nullptr)
            {
                *thread = std::thread([&](){
                    std::string buffer;
                    char buf[1024];
                    buffer.reserve(2056);

                    close(pipefd[1]);

                    while (read(pipefd[0], buf, sizeof(buf)) != 0)
                        buffer += std::string(buf);

                    std::string accumulate;
                    for (auto& a : buffer)
                    {
                        if (a == '\n')
                        {
                            lineBuffer.push_back(accumulate);
                            accumulate.clear();
                        }
                        else
                            accumulate += a;
                    }
                });
            }
            else
            {
                std::string buffer;
                char buf[1024];
                buffer.reserve(2056);

                close(pipefd[1]);

                while (read(pipefd[0], buf, sizeof(buf)) != 0)
                    buffer += std::string(buf);

                std::string accumulate;
                for (auto& a : buffer)
                {
                    if (a == '\n')
                    {
                        lineBuffer.push_back(accumulate);
                        accumulate.clear();
                    }
                    else
                        accumulate += a;
                }
            }
        }

    }
    if (!bUsingThreads && pid == 0)
        kill(pid, SIGTERM);

    return pid;
}