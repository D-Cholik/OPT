#pragma once
#include <string>

using namespace std;

int attributes[128] = { 6,6,6,6,6,6,6,0,    //
                        0,0,0,0,0,0,6,6,    //<-
                        6,6,6,6,6,6,6,6,    //<-
                        6,6,6,6,6,6,6,6,    //<-
                        0,6,6,6,6,6,6,6,    // WS ! " # $ % & '
                        5,5,6,6,6,3,3,6,    // ( ) * + , - . /
                        1,1,1,1,1,1,1,1,    // 0 1 2 3 4 5 6 7
                        1,1,3,3,42,6,43,6,  // 8 9 : ; < = > ? 
                        6,2,2,2,2,2,2,2,    // @ A B C D E F G
                        2,2,2,2,2,2,2,2,    // H I J K L M N O
                        2,2,2,2,2,2,2,2,    // P R S T U V W
                        2,2,2,6,6,6,6,6,    // X Y Z [ \ ] ^ _
                        6,6,6,6,6,6,6,6,    // ` a b c d e f g
                        6,6,6,6,6,6,6,6,    // h i j k l m n o
                        6,6,6,6,6,6,6,6,    // p q r s t u v w
                        6,6,6,6,6,6,6,6,    // x y z { | } ~ ⌂
};

//Зарезервовані ключові слова, ідентифікатори та багатосимволоьні роздільники
const string tableKeywords[7] = { "PROGRAM", "BEGIN", "END", "VAR", "WHILE", "ENDWHILE", "DO"};
const string tableIdentifiers[2] = {"INTEGER", "FLOAT" };
const string tableDelimiter[3] = { "<=", ">=", "<>" };


struct Lexem
{
    unsigned int line = 1;
    unsigned int column = 1;
    unsigned int id = 0;
    string lexem = "";
    string text = "";
};

void PrintInfo(Lexem lexem);
void PrintError(Lexem error);

int SearchConst(string strConst);
int Identifier(string strIdentifier);
int SearchTableKeyword(string Keyword);
int SearchTableIdentifier(string Identifiers);
int SearchTableDelimiter(string Delimetr);
