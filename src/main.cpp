#include "../include/LZ77.h"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>


/// Return a text file as a string, "" if failed or empty.
std::string ReadFile(std::string file);

/// Ask a question, read and return user input
template<class T> T ReadInput(std::string ask);



int main()
{
    unsigned lookahead_size;
    unsigned history_size;
    std::string file_path;

    while(1)
    {
        std::cout << std::endl << "*************************************" << std::endl;
        lookahead_size = ReadInput<unsigned>("Please enter the look-ahead buffer size");
        history_size = ReadInput<unsigned>("Please enter the sliding-history buffer size");
        file_path = ReadInput<std::string>("Please enter the the file path");

        if(ReadFile(file_path).length() > 0)
        {
            LZ77 lz(lookahead_size,history_size, file_path);
            std::cout << std::endl << "The resulting LZ77 code is: " << std::endl;
            std::cout << lz.Compress() << std::endl;
        }
    }

    return 0;
}



template<class T> T ReadInput(std::string ask)
{
    T result;
    while(1)
    {
        std::cout << ask << std::endl;
        std::cin >> result;
        if(!std::cin)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else
            break;
    }
    return result;
}

std::string ReadFile(std::string file)
{
    std::string result = "";
    std::ifstream myfile(file);
    if(myfile.is_open())
    {
        std::string line;
        while(getline (myfile, line))
        {
            result.append(line);
        }
        myfile.close();
    }
   return result;
}
