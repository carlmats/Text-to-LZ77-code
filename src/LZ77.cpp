#include "../include/LZ77.h"

LZ77::LZ77(const unsigned lookahead_size,
           const unsigned history_size,
           const std::string input_string) : m_lookahead_size(lookahead_size),
                                             m_history_size(history_size),
                                             m_input_string(input_string),
                                             m_input_pointer(lookahead_size)
{
    // Fill the look-ahead buffer with the input string
    for(unsigned i = 0; i < m_lookahead_size; i++)
    {
        if(i < m_input_string.size())
        {
            m_lookahead.push_back(m_input_string.at(i));
        }
        else
        {
            break;
        }
    }
}

const std::string LZ77::Compress()
{
    std::string result = "";
    // While there is characters left to handle
    while(!m_lookahead.empty())
    {
        CodeGroup code_group;
        code_group.next_char = m_lookahead.front();

        // Looking for the start of a match
        for(unsigned i = 0; i < m_history.size(); i++)
        {
            // If we find a beginning of a match
            if(m_lookahead.front() == m_history.at(i))
            {
                // Find the length of the match
                unsigned current = GetMatchLength(i);

                // If the match is the longest for the current lookahead
                if(current > code_group.length)
                {
                    // Create the group
                    MakeGroup(code_group, current, i);
                }
            }
        }
        // Save the longest group to result
        result.append(code_group.ToString());

        // Move the lookahead and sliding-history buffer
        MoveBuffers(code_group.length);
    }

    return result;
}

void LZ77::MakeGroup(CodeGroup& code_group, const unsigned current, const unsigned historyIndex)
{
    // Set the length and offset for the current group
    code_group.length = current;
    code_group.offset = m_history.size() - historyIndex;

    // If there still is a character left in the lookahead after the match
    if(current < m_lookahead.size())
    {
        code_group.next_char = m_lookahead.at(current);
    }
    else
    {
        // If we are at the last character in the input string
        if(m_input_pointer == m_input_string.length())
        {
            code_group.next_char = '$';
        }
        else
        {
            code_group.next_char = m_input_string.at(m_input_pointer);
        }
    }
}


void LZ77::MoveBuffers(const unsigned group_length)
{
    // We move the buffers the same amount of steps as the length of the match
    for(unsigned i = 0; i <= group_length; i++)
    {
        // Push the first character of the lookahead into the history, pop lookahead
        if(m_lookahead.size() > 0 )
        {
            m_history.push_back(m_lookahead.front());
            m_lookahead.pop_front();
        }
        // Make sure we have not reached end of input string
        if(m_input_pointer < m_input_string.length())
        {
            // Push the next character in the input string into our lookahead buffer.
            m_lookahead.push_back(m_input_string.at(m_input_pointer));
            m_input_pointer++;
        }
        // If the history is larger than its maximum size, pop front.
        if(m_history.size() > m_history_size)
        {
            m_history.pop_front();
        }

    }
}

const unsigned LZ77::GetMatchLength(const unsigned historyIndex)
{
    unsigned result = 1;
    // Loop through the lookahead buffer and check for continuous matches
    for(unsigned j = 1; j < m_lookahead.size(); j++)
    {
        // If we still are matching in the history buffer
        if(m_history.size() > historyIndex + j)
        {
            // If the continuous match is broken
            if(m_lookahead.at(j) != m_history.at(historyIndex+j))
            {
                break;
            }
            else
            {
                result++;
            }
        }
        else
        {
             // If the continuous match is broken
             if(m_lookahead.at(j) != m_lookahead.at(historyIndex+j-m_history.size()))
             {
                break;
             }
             else
             {
                result++;
             }
        }
    }
    return result;
}

