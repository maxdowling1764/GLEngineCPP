#include "ModelParser.h"
#include <sstream> 
using namespace std;

ModelParser::Token parse_token(std::string& token)
{

    if (token == "v")
    {
        return ModelParser::VERT;
    }
    else if (token == "vn")
    {
        return ModelParser::NORM_VERT;
    }
    else if (token == "vt")
    {
        return ModelParser::UV_VERT;
    }
    else if (token == "f")
    {
        return ModelParser::FACE;
    }
    else return ModelParser::NONE;
}

bool ModelParser::parse_vec3(std::istringstream& s, glm::vec3& dest)
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

bool ModelParser::parse_vec2(std::istringstream& s, glm::vec2& dest)
{
    std::string curr_word;
    for (int i = 0; i < 2; i++)
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

bool ModelParser::parse_index(std::istringstream& s, Index& dest)
{    
    unsigned int curr_start = 0;
    std::string substr;
    int i = 0;
    while(std::getline(s, substr, '/') && i < 3)
    {
        try
        {
            dest.indices[i++] = std::stoi(substr) - 1;
        }
        catch (const std::invalid_argument& e)
        {
            std::cerr << "[-] Invalid argument exception while parsing indices: Expected int, got \"" << substr << "\"" <<  std::endl;
            return false;
        }
    }
    // Need to swap since our internal representation has vn before vt
    // OBJ: vp, vt, vn
    // Internal: vp, vn, vt
    std::swap(dest.indices[1], dest.indices[2]);
    return i == 3;
}

bool ModelParser::parse_face(std::istringstream& s, 
                        const std::vector<glm::vec3>& vp, 
                        const std::vector<glm::vec3>& vn,
                        const std::vector<glm::vec2>& vt,
                        std::vector<Vertex>& vertices,
                        std::vector<Index>& indices)
{
    std::string curr_word;
    int i = 0;

    
    while (s >> curr_word && i++ < 3)
    {
        try
        {
            Index curr_idx = Index();
            std::istringstream index_stream(curr_word);
            if (parse_index(index_stream, curr_idx)) 
            {
                unsigned int i_v, i_t, i_n;
                i_v = curr_idx.indices[0];
                i_n = curr_idx.indices[1];
                i_t = curr_idx.indices[2];

                if (i_v < vp.size() && i_n < vn.size() && i_t < vt.size()) 
                {
                    vertices.push_back(Vertex(vp[i_v], vn[i_n], vt[i_t]));
                    indices.push_back(curr_idx);
                }
                else
                {
                    std::cerr << "[-] Failed to parse face - Index out of range\n";
                }
            } 
            else 
            {
                std::cerr << "[-] Failed to parse face - unexpected token: " 
                            << curr_word << std::endl;
                return false;
            }
        }
        catch (const std::invalid_argument& e)
        {
            std::cerr << "[-] Invalid argument exception encountered while parsing OBJ model: " 
                        << e.what() << std::endl;
            return false;
        }
        catch (const std::out_of_range& e)
        {
            std::cerr << "[-] Out of range error encountered while parsing OBJ model: " 
                        << e.what() << std::endl;
            return false;
        }
    }
}

bool ModelParser::parse_obj(const std::string& objPath,
                            std::vector<glm::vec3>& vp,
                            std::vector<glm::vec3>& vn,
                            std::vector<glm::vec2>& vt,
                            std::vector<Vertex>& vertices,
                            std::vector<Index>& indices)
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
        ModelParser::Token parserState = Token::NONE;
        int n_expectedTokens = 0;
        while (wordStream >> currentWord)
        {
            parserState = parse_token(currentWord);
            switch (parserState)
            {
            case Token::VERT:
                glm::vec3 vert(0);
                if (!parse_vec3(wordStream, vert))
                {
                    return false;
                }
                vp.push_back(vert);
                break;
            case Token::NORM_VERT:
                glm::vec3 norm(0);
                if (!parse_vec3(wordStream, norm))
                {
                    return false;
                }
                vn.push_back(norm);
                break;
            case Token::UV_VERT:
                glm::vec2 tex(0);
                if (!parse_vec2(wordStream, tex))
                {
                    return false;
                }
                vt.push_back(tex);
                break;
            case Token::FACE:
                if (!parse_face(wordStream, vp, vn, vt, vertices, indices))
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

bool ModelParser::parse_obj(const std::string& objpath, Mesh& m)
{
    std::vector<glm::vec3> vp = std::vector<glm::vec3>();
    std::vector<glm::vec3> vn = std::vector<glm::vec3>();
    std::vector<glm::vec2> vt = std::vector<glm::vec2>();
    std::vector<Vertex> vertices = std::vector<Vertex>();
    std::vector<Index> indices = std::vector<Index>();
    bool res = parse_obj(objpath, vp, vn, vt, vertices, indices);
    if (res) { m = Mesh(vertices, indices, glm::uvec3(vp.size(), vn.size(), vt.size())); }
    return res;
}