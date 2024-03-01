#pragma once
#include <string>

using namespace std;

int attributes[128] = { 6,6,6,6,6,6,6,6,
                        0,0,0,0,0,0,6,6,
                        6,6,6,6,6,6,6,6,
                        6,6,6,6,6,6,6,6,
                        0,6,6,6,6,6,6,6,    //  !"#$%&'
                        5,5,6,6,6,3,3,6,    // ()*+,-./
                        1,1,1,1,1,1,1,1,    // 01234567
                        1,1,3,3,42,6,43,6,   // 89:;<=>? !!!
                        6,2,2,2,2,2,2,2,    // @ABCDEFG
                        2,2,2,2,2,2,2,2,    // HIJKLMNO
                        2,2,2,2,2,2,2,2,    // PQRSTUVW
                        2,2,2,6,6,6,6,6,    // XYZ[\]^_
                        6,6,6,6,6,6,6,6,    // `abcdefg
                        6,6,6,6,6,6,6,6,    // hijklmno
                        6,6,6,6,6,6,6,6,    // pqrstuvw
                        6,6,6,6,6,6,6,6,    // xyz{|}~⌂
};

const string Keywords[9] = { "PROGRAM", "BEGIN", "END", "WHILE", "ENDWHILE", "DO", "<>", ">=", "<="};
const string tableIdentifiers[2] = { "INTEGER", "FLOAT" };


struct LexemInfo
{
    unsigned int line = 1;
    unsigned int column = 1;
    unsigned int id = 0;
    string lexem = "";
    string text = "";
};

void PrintInfo(LexemInfo lexemInfo);
void PrintError(LexemInfo errorInfo);
int SearchConst(string strConst);
int SearchIdentifier(string strIdentifier);
int IsKeyword(string keyword);
int SearchIdentifierTable(string Identifiers);