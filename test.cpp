#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <format>
#include "LexemInfo.h"

vector <Lexem> ErrorVector;
vector <Lexem> TokenVector;
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
        char currentSymbol;
        bool flag = true;
        int index;
        Lexem token;
        string comment = "";
        int line = 1, column = 1;
        int category;

        while (!f.eof())
        {
            if (flag)
            {
                currentSymbol = f.get();
                index = (int)currentSymbol;
            }
            else
            {
                flag = true;
            }
            if (index >= 0)
            {
                if (index <= 127)
                {
                    category = attributes[index];
                    switch (category)
                    {
                    case 0:
                        if (currentSymbol == '\n')
                        {
                            line++;
                            column = 1;
                        }
                        else if (currentSymbol == '\t')
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
                            token.lexem += currentSymbol;
                            currentSymbol = f.get();
                            index = (int)currentSymbol;
                            if (index >= 0)
                            {
                                if (index <= 127)
                                {
                                    category = attributes[index];
                                }
                                else
                                {
                                    category = 6;
                                }
                            }
                            else
                            {
                                if (index == -1)
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
                                token.lexem += currentSymbol;
                                currentSymbol = f.get();
                                index = (int)currentSymbol;
                                if (index >= 0)
                                {
                                    if (index <= 127)
                                    {
                                        category = attributes[index];
                                    }
                                    else
                                    {
                                        category = 6;
                                    }
                                }
                                else
                                {
                                    if (index == -1)
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
                            token.lexem += currentSymbol;
                            currentSymbol = f.get();
                            index = (int)currentSymbol;
                            if (index >= 0)
                            {
                                if (index <= 127)
                                {
                                    category = attributes[index];
                                }
                                else
                                {
                                    category = 6;
                                }
                            }
                            else
                            {
                                if (index == -1)
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
                            int isKeyword = SearchTableKeyword(token.lexem);
                            int isIdentifierInTable = SearchTableIdentifier(token.lexem);
                            int isIdentifierInVector = Identifier(token.lexem);
                           
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
                                token.id = 1003 + IdentifierVector.size();
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
                                token.lexem += currentSymbol;
                                currentSymbol = f.get();
                                index = (int)currentSymbol;
                                if (index >= 0)
                                {
                                    if (index <= 127)
                                    {
                                        category = attributes[index];
                                    }
                                    else
                                    {
                                        category = 6;
                                    }
                                }
                                else
                                {
                                    if (index == -1)
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
                        token.lexem += currentSymbol;
                        token.id = index;
                        column++;
                        PrintInfo(token);
                        TokenVector.push_back(token);
                        token.lexem = "";
                        continue;
                    case 42:
                        token.line = line;
                        token.column = column;
                        token.lexem += currentSymbol;
                        currentSymbol = f.get();
                        index = (int)currentSymbol;
                        column++;
                        category = attributes[index];

                        if (category == 0 || category == 1 || category == 2 ) {
                            token.id = 301;
                            token.text = "Delimiter2";
                            TokenVector.push_back(token);
                            PrintInfo(token);
                            f.unget();
                        }
                        else {
                            column++;
                            token.lexem += currentSymbol;
                            int DelimetrTable = SearchTableDelimiter(token.lexem);
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
                        token.lexem += currentSymbol;
                        currentSymbol = f.get();
                        index = (int)currentSymbol;
                        column++;
                        category = attributes[index];

                        if (category == 0 || category == 1 || category == 2) {
                            token.id = 302;
                            token.text = "Delimiter3";
                            TokenVector.push_back(token);
                            PrintInfo(token);
                            f.unget();
                        }
                        else {
                            column++;
                            token.lexem += currentSymbol;
                            int DelimetrTable = SearchTableDelimiter(token.lexem);
                            if (DelimetrTable != 1) {
                                token.id = DelimetrTable;
                                token.text = "Delimiter3";
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

                        token.lexem += currentSymbol;
                        currentSymbol = f.get();
                        index = (int)currentSymbol;
                        if (index >= 0 && index <= 127)
                        {
                            category = attributes[index];
                        }
                        else
                        {
                            category = 6;
                        }
                        column++;
                        if (currentSymbol == '*')
                        {
                            token.line = line;
                            token.column = column - 1;
                            token.lexem += currentSymbol;
                            bool isFindStar = false;
                            bool isEndComment = false;

                            while (index != -1)
                            {

                                currentSymbol = f.get();
                                index = (int)currentSymbol;
                                token.lexem += currentSymbol;
                                if (currentSymbol == '*')
                                {
                                    isFindStar = true;
                                    column++;
                                }
                                else if (currentSymbol == ')' && isFindStar)
                                {
                                    isEndComment = true;
                                    column++;
                                    break;
                                }
                                else
                                {
                                    isFindStar = false;
                                    if (currentSymbol == '\n')
                                    {
                                        line++;
                                        column = 1;
                                    }
                                    else if (currentSymbol == '\t')
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
                            token.lexem += currentSymbol;
                            currentSymbol = f.get();
                            index = (int)currentSymbol;
                            if (index >= 0 && index <= 127)
                            {
                                category = attributes[index];
                            }
                            else
                            {
                                if (index == -1)
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
                if (index == -1)
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

void PrintInfo(Lexem lexem)
{
    std::cout << "|" << setw(6) << lexem.line << "|" << setw(8) << lexem.column << "|" << setw(10) << lexem.id << "|" << setw(10) << lexem.lexem << "|" << std::endl;
}

void PrintError(Lexem Error)
{
    std::cout << "|" << setw(6) << Error.line << "|" << setw(8) << Error.column << "|Lexer error: |" << setw(45) << Error.text << "|" << setw(10) << Error.lexem << "|"  << std::endl;
}

int SearchTableKeyword(string Keyword)
{
    for (int i = 0; i < 7; i++)
        if (tableKeywords[i] == Keyword)
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

int SearchTableIdentifier(string Identifiers)
{
    for (int i = 0; i < 2; i++)
        if (tableIdentifiers[i] == Identifiers)
            return 1001 + i;

    return 1;
}

int Identifier(string strIdentifier)
{
    for (int i = 0; i < IdentifierVector.size(); i++)
        if (IdentifierVector[i] == strIdentifier)
            return 1003 + i;

    return 1;
}

int SearchTableDelimiter(string Delimiter)
{
    for (int i = 0; i < 3; i++)
        if (tableDelimiter[i] == Delimiter)
            return 303 + i;

    return 1;
}