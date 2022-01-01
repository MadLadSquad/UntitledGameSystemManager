#pragma once
#include <vector>
#include <string>
#include <thread>
#include <unistd.h>

namespace UGM::Core::Utilities
{
    [[maybe_unused]] std::string toLower(const char* str);
    [[maybe_unused]] std::string toLower(const std::string& str);
    [[maybe_unused]] void toLower(std::string& str);

    template<typename T>
    struct [[maybe_unused]] Vector2
    {
        Vector2() = default;
        explicit Vector2(const T& mult)
        {
            x = mult;
            y = mult;
        }
        [[maybe_unused]] Vector2(const T& xx, const T& yy)
        {
            x = xx;
            y = yy;
        }
        T x{};
        T y{};
    };

    template<typename T>
    struct Vector3
    {
        Vector3() = default;
        explicit Vector3(const T& mult)
        {
            x = mult;
            y = mult;
            z = mult;
        }
        [[maybe_unused]] Vector3(const T& xx, const T& yy, const T& zz)
        {
            x = xx;
            y = yy;
            z = zz;
        }
        T x{};
        T y{};
        T z{};
    };

    template<typename T>
    struct [[maybe_unused]] Vector4
    {
        Vector4() = default;
        explicit Vector4(const T& mult)
        {
            x = mult;
            y = mult;
            z = mult;
            w = mult;
        }
        [[maybe_unused]] Vector4(const T& xx, const T& yy, const T& zz, const T& ww)
        {
            x = xx;
            y = yy;
            z = zz;
            w = ww;
        }
        T x{};
        T y{};
        T z{};
        T w{};
    };

    template<typename T>
    using Vector = Vector3<T>;

    /**
     * @brief Given a reference to a vector of strings, an array of c string containing a nullptr terminated shell command
     * and a hint of weather to use threads this function will fork the current process, execute a command and load its
     * stdin and stderr output buffers(which can be offloaded to a thread) into a the vector for standard strings and
     * return a pid_t pointing to the child process
     * @param lineBuffer An array to hold every line of text
     * @param command A command that needs to be run on the child process via exec
     * @param bUsingThreads If bUsing threads is set to true then the loading of the string buffer will be offloaded to a separate thread
     * @param thread A pointer to a std::thread
     * @return The process ID of the child process, needed so that when used with multithreading users can kill the process after they join the thread!
     */
    [[maybe_unused]] pid_t loadLineByLineFromPID(std::vector<std::string>& lineBuffer, char* const* command, bool bUsingThreads = false, std::thread* thread = nullptr);

    /**
     * @brief The ScriptRunner class is a wrapper that executes a program and writes its stderr and stdout output to a
     * unix pipe, which can be used to read the input from stderr and stdout
     */
    class [[maybe_unused]] ScriptRunner
    {
    public:
        /**
         * @brief Will initialize the runner with the command that was passed as a parameter. It will create a unix pipe,
         * fork the current process, execute the command in the child process, then enable a bool to signal readiness
         * for update
         * @param cmd The command to run
         */
        [[maybe_unused]] void init(char* const* cmd);
        /**
         * @brief This update function reads everything that came from the pipe and add it to the line buffer accordingly,
         * because this is a direct read, it needs to be rate-limited as to not slow down rendering of the GUI, which is one
         * of the reasons the "updateBufferSize" function exists
         * @param bFirst This bool is used to check if we're running on an initial update
         */
        [[maybe_unused]] void update(bool bFirst = false);
        /**
         * @brief This update function is used to make performance faster by moving buffer clearing and resizing to a
         * different frame, between 2 regular updates
         */
        [[maybe_unused]] void updateBufferSize();
        [[maybe_unused]] void destroy();
        [[maybe_unused]] std::vector<std::string>& data();
        // Returns if the process is valid
        [[maybe_unused]] [[nodiscard]] bool valid() const;
        [[maybe_unused]] [[nodiscard]] pid_t& getPID();
    private:
        std::vector<std::string> lineBuffer; // The buffer of lined messages used for displaying text line by line
        pid_t pid = 0; // The process ID of the child process
        int pipefd[2]; // The unix pipe file descriptors

        std::string stringBuffer; // The temporary string buffer that is used on every update call
        bool bCanUpdate = false; // A bool used to check if we're ready for an update as to not update without actually having a valid pipe or pid
        bool bValid = true;
    };

    void execandwait(char* const* command);
}