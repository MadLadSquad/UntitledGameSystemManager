#include "Utilities.hpp"
#include <thread>
#include <iostream>
#include <Core/Defines.hpp>
#include <sys/wait.h>


std::string UGM::Core::Utilities::toLower(const char* str)
{
    std::string ret = str;
    toLower(ret);
    return ret;
}

void UGM::Core::Utilities::toLower(std::string& str)
{
    for (auto& a : str)
        a = tolower(a);
}

std::string UGM::Core::Utilities::toLower(const std::string& str)
{
    std::string ret = str;
    toLower(ret);
    return ret;
}

pid_t UGM::Core::Utilities::loadLineByLineFromPID(std::vector<std::string>& lineBuffer, char* const* command, bool bUsingThreads, std::thread* thread)
{
    int pipefd[2];
    auto pout = pipe(pipefd);

    auto pid = fork();
    if (pid != -1 && pout != -1)
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
                    char buf[STRING_BUFFER_SIZE];
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
                char buf[STRING_BUFFER_SIZE];
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

void UGM::Core::Utilities::ScriptRunner::init(char* const* cmd)
{
    stringBuffer.reserve(STRING_BUFFER_SIZE);
    auto pout = pipe(pipefd);

    pid = fork();
    if (pid != -1 && pout != -1)
    {
        if (pid == 0)
        {
            close(pipefd[0]);

            dup2(pipefd[1], STDOUT_FILENO);
            dup2(pipefd[1], STDERR_FILENO);

            close(pipefd[1]);

            execvp(cmd[0], cmd);
        }
        else
        {
            bCanUpdate = true;
            currentpid = pid;
            signal(SIGCHLD, [](int sig){
                if (currentpid > 0)
                {
                    wait(&currentpid); // Wait for the process to finish
                    currentpid = -1; // Reset the pid
                }
            });
        }

    }
}

void UGM::Core::Utilities::ScriptRunner::update(bool bFirst)
{
    if (bCanUpdate && pid > 0)
    {
        char buf[STRING_BUFFER_SIZE];
        if (bFirst)
            close(pipefd[1]);

        if (read(pipefd[0], buf, sizeof(buf)) != 0)
        {
            std::string tmp = buf;
            tmp.shrink_to_fit();
            stringBuffer += tmp;
            std::string accumulate;
            for (auto& a : stringBuffer)
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

void UGM::Core::Utilities::ScriptRunner::destroy()
{
    bValid = false;
}

std::vector<std::string>& UGM::Core::Utilities::ScriptRunner::data()
{
    return lineBuffer;
}

void UGM::Core::Utilities::ScriptRunner::updateBufferSize()
{
    if (bCanUpdate && pid > 0)
    {
        stringBuffer.clear();
        stringBuffer.reserve(STRING_BUFFER_SIZE);
    }
}

bool UGM::Core::Utilities::ScriptRunner::valid() const
{
    return bValid;
}

pid_t& UGM::Core::Utilities::ScriptRunner::getPID()
{
    return pid;
}

void UGM::Core::Utilities::execandwait(char* const* command)
{
    auto pid = fork();
    if (pid != -1)
    {
        if (pid == 0)
            execvp(command[0], command);
        else
            wait(&pid);
    }
}