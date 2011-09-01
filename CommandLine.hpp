
#include <string>
#include <vector>

class Parser;
class CommandLine;

class Parser {
    
    
public:
    CommandLine readCommandLine();
};

class CommandLine {
    std::vector<std::string>* parameters;
    const char ** execv;
public:
    virtual ~CommandLine();
    CommandLine(std::vector<std::string>* parameters);
    const std::vector<std::string>* getParameters();
    char const** execvFormat();
};
