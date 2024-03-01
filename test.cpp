#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <format>
#include "LexemInfo.h"

vector <LexemInfo> ErrorVector;
vector <LexemInfo> TokenVector;
vector <string> ConstVector;
vector <string> IdentifierVector;

int main()
{
    std::cout << setw(25) << "Table 'Lexem'" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "| Line | Column |    Id    |   Lexem  |" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    ifstream f;
    f.open("test.txt");
    if (!f.is_open())
    {
        std::cout << "Error: cannot open the file" << endl;
    }
    else
    {
        int lineStartOfComment = 0, columnStartOfComment = 0;
        char s;
        bool flag = true;
        int ind;
        int indSecond;
        LexemInfo token;
        string comment = "";
        int line = 1, column = 1;
        int category;
        while (!f.eof())
        {
            if (flag)
            {
                s = f.get();
                ind = (int)s;
            }
            else
            {
                flag = true;
            }
            if (ind >= 0)
            {
                if (ind <= 127)
                {
                    category = attributes[ind];
                    switch (category)
                    {
                    case 0:
                        if (s == '\n')
                        {
                            line++;
                            column = 1;
                        }
                        else if (s == '\t')
                        {
                            column = column - (column - 1) % 4 + 4;
                        }
                        else
                        {
                            column++;
                        }
                        token.lexem = "";
                        continue;
                    case 1:
                        token.line = line;
                        token.column = column;
                        token.text = "Number";
                        do
                        {
                            token.lexem += s;
                            s = f.get();
                            ind = (int)s;
                            if (ind >= 0)
                            {
                                if (ind <= 127)
                                {
                                    category = attributes[ind];
                                }
                                else
                                {
                                    category = 6;
                                }
                            }
                            else
                            {
                                if (ind == -1)
                                {
                                    category = 0;
                                    break;
                                }
                                else
                                {
                                    category = 6;
                                }
                            }
                            column++;
                        } while (category == 1);
                        if (category == 0 || category == 3 || category == 41 || category == 42 || category == 5)
                        {
                            int isConstInVector = SearchConst(token.lexem);

                            if (isConstInVector == 1)
                            {
                                token.id = 501 + ConstVector.size();
                                ConstVector.push_back(token.lexem);
                                TokenVector.push_back(token);
                            }
                            else
                            {
                                token.id = isConstInVector;
                                TokenVector.push_back(token);
                            }

                            if (token.id > 1000)
                            {
                                token.text = "const id error";
                                ErrorVector.push_back(token);
                            }

                            PrintInfo(token);
                        }
                        else
                        {
                            do
                            {
                                token.lexem += s;
                                s = f.get();
                                ind = (int)s;
                                if (ind >= 0)
                                {
                                    if (ind <= 127)
                                    {
                                        category = attributes[ind];
                                    }
                                    else
                                    {
                                        category = 6;
                                    }
                                }
                                else
                                {
                                    if (ind == -1)
                                        break;
                                    else
                                        category = 6;
                                }
                                column++;
                            } while (category != 0 && category != 3 && category != 41 && category != 42 && category != 5);

                            if (category != 0)
                                token.text = "number contains invalid characters";
                            ErrorVector.push_back(token);
                        }
                        token.lexem = "";
                        flag = false;
                        continue;
                    case 2:
                        token.line = line;
                        token.column = column;
                        token.text = "Identifier";
                        do
                        {
                            token.lexem += s;
                            s = f.get();
                            ind = (int)s;
                            if (ind >= 0)
                            {
                                if (ind <= 127)
                                {
                                    category = attributes[ind];
                                }
                                else
                                {
                                    category = 6;
                                }
                            }
                            else
                            {
                                if (ind == -1)
                                {
                                    category = 0;
                                    break;
                                }
                                else
                                {
                                    category = 6;
                                }
                            }
                            column++;
                        } while (category == 1 || category == 2);
                        if (category == 0 || category == 3 || category == 41 || category == 42 || category == 5)
                        {
                            int isKeyword = IsKeyword(token.lexem);
                            int isIdentifierInVector = SearchIdentifier(token.lexem);
                            int isIdentifierInTable = SearchIdentifierTable(token.lexem);

                            if (isKeyword != 1)
                            {
                                token.id = isKeyword;
                                TokenVector.push_back(token);
                            }
                            else if (isIdentifierInTable != 1)
                            {
                                token.id = isIdentifierInTable;
                                TokenVector.push_back(token);
                            }
                            else if (isIdentifierInVector == 1)
                            {
                                token.id = 1002 + IdentifierVector.size();
                                IdentifierVector.push_back(token.lexem);
                                TokenVector.push_back(token);
                            }
                            else
                                token.id = isIdentifierInVector;

                            PrintInfo(token);
                        }
                        else
                        {
                            do
                            {
                                token.lexem += s;
                                s = f.get();
                                ind = (int)s;
                                if (ind >= 0)
                                {
                                    if (ind <= 127)
                                    {
                                        category = attributes[ind];
                                    }
                                    else
                                    {
                                        category = 6;
                                    }
                                }
                                else
                                {
                                    if (ind == -1)
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        category = 6;
                                    }
                                }
                                column++;
                            } while (category != 0 && category != 3 && category != 41 && category != 42 && category != 5);
                            token.text = "identifier number contains invalid characters";
                            ErrorVector.push_back(token);
                        }
                        token.lexem = "";
                        flag = false;
                        continue;
                    case 3:
                        token.line = line;
                        token.column = column;
                        token.text = "Delimiter1";
                        token.lexem += s;
                        token.id = ind;
                        column++;
                        PrintInfo(token);
                        TokenVector.push_back(token);
                        token.lexem = "";
                        continue;
                    case 42:
                        token.line = line;
                        token.column = column;
                        token.lexem += s;
                        s = f.get();
                        ind = (int)s;
                        column++;
                        category = attributes[ind];

                        if (category == 0 || category == 1 || category == 2 ) {
                            token.id = 101;
                            token.text = "Delimiter1";
                            TokenVector.push_back(token);
                            PrintInfo(token);
                            f.unget();
                        }
                        else {
                            column++;
                            token.lexem += s;
                            int DelimetrTable = SearchDelimetrTable(token.lexem);
                            if (DelimetrTable != 1) {
                                if (DelimetrTable == 301)
                                {
                                    token.id = DelimetrTable;
                                    token.text = "Delimiter2";
                                    TokenVector.push_back(token);
                                    PrintInfo(token);
                                }
                                else
                                {
                                    token.id = DelimetrTable;
                                    token.text = "Delimiter2";
                                    TokenVector.push_back(token);
                                    PrintInfo(token);
                                }
                            }
                            else {
                                token.text = "invalid multi-character separator";
                                ErrorVector.push_back(token);
                            }
                        }
                        token.lexem = "";
                        continue;
                    case 43:
                        token.line = line;
                        token.column = column;
                        token.lexem += s;
                        s = f.get();
                        ind = (int)s;
                        column++;
                        category = attributes[ind];

                        if (category == 0 || category == 1 || category == 2) {
                            token.id = 102;
                            token.text = "Delimiter1";
                            TokenVector.push_back(token);
                            PrintInfo(token);
                            f.unget();
                        }
                        else {
                            column++;
                            token.lexem += s;
                            int DelimetrTable = SearchDelimetrTable(token.lexem);
                            if (DelimetrTable != 1) {
                                token.id = DelimetrTable;
                                token.text = "Delimiter2";
                                TokenVector.push_back(token);
                                PrintInfo(token);

                            }
                            else {
                                token.text = "invalid multi-character separator";
                                ErrorVector.push_back(token);
                            }
                        }
                        token.lexem = "";
                        continue;

                    case 5:

                        token.lexem += s;
                        s = f.get();
                        ind = (int)s;
                        if (ind >= 0 && ind <= 127)
                        {
                            category = attributes[ind];
                        }
                        else
                        {
                            category = 6;
                        }
                        column++;
                        if (s == '*')
                        {
                            token.line = line;
                            token.column = column - 1;
                            token.lexem += s;
                            bool isFindStar = false;
                            bool isEndComment = false;

                            while (ind != -1)
                            {

                                s = f.get();
                                ind = (int)s;
                                token.lexem += s;
                                if (s == '*')
                                {
                                    isFindStar = true;
                                    column++;
                                }
                                else if (s == ')' && isFindStar)
                                {
                                    isEndComment = true;
                                    column++;
                                    break;
                                }
                                else
                                {
                                    isFindStar = false;
                                    if (s == '\n')
                                    {
                                        line++;
                                        column = 1;
                                    }
                                    else if (s == '\t')
                                    {
                                        column = column - (column - 1) % 4 + 4;
                                    }
                                    else
                                    {
                                        column++;
                                    }
                                }
                            }
                            if (isEndComment)
                            {
                                token.lexem = "";
                            }
                            else
                            {
                                for (int i = 0; i < token.lexem.size(); i++)
                                {
                                    if (token.lexem[i] != '\n')
                                        comment += token.lexem[i];
                                    else
                                        break;
                                }
                                token.lexem = comment;
                                token.text = "invalid comment";
                                ErrorVector.push_back(token);
                                token.lexem = "";
                            }
                        }
                        else
                        {
                            token.lexem += comment;
                            token.text = "invalid comment";
                            ErrorVector.push_back(token);
                            token.lexem = "";
                            flag = false;
                        }
                        continue;
                    case 6:
                        token.line = line;
                        token.column = column;
                        token.text = "Error";
                        do
                        {
                            token.lexem += s;
                            s = f.get();
                            ind = (int)s;
                            if (ind >= 0 && ind <= 127)
                            {
                                category = attributes[ind];
                            }
                            else
                            {
                                if (ind == -1)
                                {
                                    break;
                                }
                                else
                                {
                                    category = 6;
                                }
                            }
                            column++;
                        } while (category != 0 && category != 3 && category != 41 && category != 42 && category != 5);
                        token.text = "invalid symbol";
                        ErrorVector.push_back(token);
                        token.lexem = "";
                        flag = false;
                        continue;
                    }
                }
                else
                {
                    category = 6;
                    flag = false;
                }
            }
            else
            {
                if (ind == -1)
                {
                    break;
                }
                else
                {
                    category = 6;
                    flag = false;
                }
            }
        }
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "\n" << std::endl;
        std::cout << setw(50) << "Table 'Lexer error'" << std::endl;
        std::cout << "----------------------------------------------------------------------------------------" << std::endl;
        std::cout << "| Line | Column |  Error Type |                                   Error text|   Lexem  |" << std::endl;
        std::cout << "----------------------------------------------------------------------------------------" << std::endl;
        for (int i = 0; i < ErrorVector.size(); i++)
        {
            PrintError(ErrorVector[i]);
        }
        std::cout << "----------------------------------------------------------------------------------------" << std::endl;

    }
}

void PrintInfo(LexemInfo lexemInfo)
{
    std::cout << "|" << setw(6) << lexemInfo.line << "|" << setw(8) << lexemInfo.column << "|" << setw(10) << lexemInfo.id << "|" << setw(10) << lexemInfo.lexem << "|" << std::endl;
}

void PrintError(LexemInfo errorInfo)
{
    std::cout << "|" << setw(6) << errorInfo.line << "|" << setw(8) << errorInfo.column << "|Lexer error: |" << setw(45) << errorInfo.text << "|" << setw(10) << errorInfo.lexem << "|"  << std::endl;
}

int IsKeyword(string keyword)
{
    for (int i = 0; i < 6; i++)
        if (Keywords[i] == keyword)
            return 401 + i;

    return 1;
}

int SearchConst(string strConst)
{
    for (int i = 0; i < ConstVector.size(); i++)
        if (ConstVector[i] == strConst)
            return 501 + i;

    return 1;
}

int SearchIdentifierTable(string Identifiers)
{
    for (int i = 0; i < 2; i++)
        if (tableIdentifiers[i] == Identifiers)
            return 1001 + i;

    return 1;
}

int SearchIdentifier(string strIdentifier)
{
    for (int i = 0; i < IdentifierVector.size(); i++)
        if (IdentifierVector[i] == strIdentifier)
            return 1002 + i;

    return 1;
}

int SearchDelimetrTable(string Delimetr)
{
    for (int i = 0; i < 6; i++)
        if (tableDelimetr[i] == Delimetr)
            return 301 + i;

    return 1;
}