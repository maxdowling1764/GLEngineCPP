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

Mesh parse_obj(std::string& objPath)
{
    fstream newfile;
    Mesh m = Mesh();
    newfile.open(objPath, ios::in);  // open a file to perform write operation using file object

    if (newfile.is_open()) {   //checking whether the file is open
        string curr_line;
        string curr_head;
        string subtoken;
        stringstream subtokens(curr_line);
        
        
        newfile.close(); //close the file object.
    }
    return m;
}