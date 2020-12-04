#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>

using namespace std;

// һЩ����
const int MAXLINECNT = 1024;
const int MAXCOMMENTCNT = 2048;

// ��������״̬
typedef int Status;
const int ERROR = -1;
const int OK = 0;

const int ENDINPUT = -2;

// �ʷ���������
typedef enum
{
	// �ؼ���
	// int void if else while return
	INT, VOID, IF, ELSE, WHILE, RETURN,

	// ��ʶ��(������)
	ID, 

	// ����
	NUM,

	// ��ֵ
	// =
	ASSIGN,

	// ���
	// +  -      *    /    ==      >   >=   <   <=   !=
	PLUS, MINUS, MUL, DIV, EQUAL, GT, GTE, LT, LTE, NE,

	// ���
	// ;
	SEMICOLON,

	// �ָ���
	// ,
	COMMA,

	// ע�ͷ�
	// /*      */         //        /* */
	LPCOMMENT, RPCOMMENT, LCOMMENT, PCOMMENT,

	// ����
	// (      )
	LBRACKET, RBRACKET,

	// ������
	// {    }
	LBRACE, RBRACE,

	// ����
	// \n
	NEXTL,

	// ������
	// eof
	ENDFILE
} LexType;

// ��Ԫ�鶨��
typedef pair<LexType, string> Token;

// һЩ�򵥵ĺ�������
bool isNum(char c);
bool isAlpha(char c);
string transTokenFirst(Token t);