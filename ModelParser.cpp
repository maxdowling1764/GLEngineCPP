#include "ModelParser.h"
#include <sstream> 
using namespace std;

ModelParser::TOKEN parse_token(std::string& token)
{
    if (token == "v")
    {
        return ModelParser::VERT;
    }
    else if (token == "f")
    {
        return ModelParser::FACE;
    }
    return ModelParser::NONE;
}

bool ModelParser::parse_vertex(std::istringstream& s, glm::vec3& dest)
{
    std::string curr_word;
    for (int i = 0; i < 3; i++)
    {
        if (s >> curr_word)
        {
            try
            {
                dest[i] = std::stof(curr_word);
            }
            catch (const std::invalid_argument& e)
            {
                std::cerr << "[-] Invalid argument exception encountered while parsing OBJ model: " << e.what() << std::endl;
                return false;
            }
            catch (const std::out_of_range& e)
            {
                std::cerr << "[-] Out of range error encountered while parsing OBJ model: " << e.what() << std::endl;
                return false;
            }
        }
        else
        {
            std::cerr << "[-] Error parsing OBJ model vertex: Expected vec3, got vec" << i << std::endl;
            return false;
        }
    }
    return true;
}

bool ModelParser::parse_face(std::istringstream& s, Mesh& dest)
{
    std::string curr_word;
    for (int i = 0; i < 3; i++)
    {
        if (s >> curr_word)
        {
            try
            {
                /* subtract 1 since obj indices start at 1 instead of 0 */
                dest.m_indices.push_back((unsigned int)std::stoi(curr_word) - 1);
            }
            catch (const std::invalid_argument& e)
            {
                std::cerr << "[-] Invalid argument exception encountered while parsing OBJ model: " << e.what() << std::endl;
                return false;
            }
            catch (const std::out_of_range& e)
            {
                std::cerr << "[-] Out of range error encountered while parsing OBJ model: " << e.what() << std::endl;
                return false;
            }
        }
        else
        {
            std::cerr << "[-] Error parsing OBJ model face: Expected 3 indices, got " << i + 1 << std::endl;
            return false;
        }
    }
}

bool ModelParser::parse_obj(std::string& objPath, Mesh& m)
{
    std::ifstream file(objPath);

    if (!file.is_open()) {   //checking whether the file is open
        std::cerr << "[-] Failed to load OBJ model at path: " << objPath << std::endl;
        return false;
    }

    std::string currentLine;
    while (std::getline(file, currentLine))
    {
        std::istringstream wordStream(currentLine);
        std::string currentWord;
        ModelParser::TOKEN parserState = ModelParser::TOKEN::NONE;
        int n_expectedTokens = 0;
        while (wordStream >> currentWord)
        {
            parserState = parse_token(currentWord);
            switch (parserState)
            {
            case ModelParser::VERT:
                glm::vec3 v(0);
                if (parse_vertex(wordStream, v))
                {
                    m.m_vertices.push_back(v);
                }
                else
                {
                    return false;
                }
                break;
            case ModelParser::FACE:
                if (!parse_face(wordStream, m))
                {
                    return false;
                }
                break;
            default:
                break;
            }
        }
    }
    file.close();
    return true;
}