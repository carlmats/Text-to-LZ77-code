#ifndef LZ77_H
#define LZ77_H

#include <sstream>
#include <string>
#include <queue>

/// LZ77 code group struct
struct CodeGroup{

    unsigned offset;
    unsigned length;
    char next_char;

    CodeGroup() : offset(0),length(0), next_char('$'){}

    std::string ToString()
    {
        // MinGW bug, std::to_string not working
        std::stringstream ss;
        ss << "(" << offset
           << "," << length
           << "," << next_char
           << ")";
        return ss.str();
    }
};

class LZ77
{
    public:
        LZ77(const unsigned lookahead_size, const unsigned history_size, const std::string input_string);
        ~LZ77(){};

        /// Method to start the conversion of the string to LZ77 code
        const std::string Compress();

    private:
        /// Find the length of a match
        const unsigned GetMatchLength(const unsigned i);

        /// Move the buffers
        void MoveBuffers(CodeGroup code_group);

        /// Make the LZ77 code group
        void MakeGroup(CodeGroup& code_group, const unsigned current, const unsigned historyIndex);


        std::deque<char> m_lookahead;
        std::deque<char> m_history;

        const unsigned m_lookahead_size;
        const unsigned m_history_size;

        const std::string m_input_string;

        unsigned m_input_pointer;


};

#endif // LZ77_H
