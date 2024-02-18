#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#define ERROR_USAGE 1
#define ERROR_FILE_OPEN 2
#define ERROR_MEMORY 3
#define INTERACTION_ERROR 4
#define IO_ERROR 5
#define TEST_ERROR 6

struct CustomException : public std::exception
{
    std::string message;
    explicit CustomException(const char * message) :message(message){}
    explicit CustomException(const std::string& message) :message(message){}
    const char* what() const noexcept override
    {
        return message.c_str();
    }
};

struct UnknownCommand : public CustomException
{
    explicit UnknownCommand(const std::string &cmd) : CustomException("Unknown command " + cmd){}
    explicit UnknownCommand(char cmd): CustomException(std::string(1,cmd)){}
};
using integer=long long;

std::ostream& operator<<(std::ostream  & stream, const std::vector<int> &P)
{
    for(int i=0;i<P.size();i++)
        stream << (i?" ":"") << P[i];
    return stream;
}

std::string get_environment_variable(const char* varname)
{
    char *var = getenv(varname);
    if(!var)
        return "";
    return var;
}

bool is_valid_query(int i,int j,int n)
{
    return i > 0 && i <= n && j > 0 && j <= n && i != j;
}

int main(int argc, char **argv)
{
    std::string debug_filename= get_environment_variable("DEBUG");
    std::string non_debug=get_environment_variable("NDEBUG");
    if(argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " TEST_FILE";
        return ERROR_USAGE;
    }
    const std::string filename(argv[1]);
    std::ifstream file(filename);
    std::ofstream debug_file;
    if(non_debug.empty())
        debug_file.open(debug_filename);
    try
    {
        int queries=0;
        integer S=0;
        int n;
        file >> n;
        std::vector<int> P(n);
        for(auto &p:P)
        {
            file >> p;
            S+=p;
        }
        std::cout << n << std::endl;
        char command;
        while(std::cin >> command)
        {
            if(command=='?')
            {
                queries++;
                int i=0,j=0;
                std::cin >> i >> j;
                if(!is_valid_query(i,j,n))
                {
                    std::cerr << "Invalid query: ? "  << i << ' ' << j << std::endl;
                    return INTERACTION_ERROR;
                }
                debug_file << "Received Input: ? " << i << ' ' << j << std::endl;
                auto SUM=P[i-1]+P[j-1];
                std::cout << SUM << std::endl;
                debug_file << "Sent Output " << SUM << std::endl;
            }
            else if(command == '!')
            {
                integer S_guess;
                std::cin >> S_guess;
                if(S_guess!=S)
                {
                    std::cerr << "Solution guessed the wrong sum!\n";
                    std::cerr << "Expected: " << S << '\n';
                    std::cerr << "Found:    " << S_guess << '\n';
                    std::cerr << "Array: " << P << '\n';
                    std::cerr << std::flush;
                }
                else
                    std::cerr << "Solution guessed the correct sum! Number of queries: " << queries << std::endl;
                return 0;
            }
            else
            {
                std::cerr << "Unknown command: " << command << std::endl;
                return INTERACTION_ERROR;
            }
        }
    }
    catch (std::exception &exception)
    {
        std::cerr << "Failed to read data from stream/file. Reason: " << exception.what() << std::endl;
        return IO_ERROR;
    }
    std::cerr << "Encountered premature end of file" << std::endl;
    return IO_ERROR;
}