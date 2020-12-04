#include "common.h"

bool isNum(char c)
{
	return (c >= '0' && c <= '9') ? true : false;
}

bool isAlpha(char c)
{
	return (c >= 'a' && c <= 'z') ? true : false;
}

// ת��Token�е�First��������
string transTokenFirst(Token t)
{
	const char words[31][10] = {
		// �ؼ���
		// int void if else while return
		"INT", "VOID", "IF", "ELSE", "WHILE", "RETURN",

		// ��ʶ��(������)
		"ID",

		// ����
		"NUM",

		// ��ֵ
		// =
		"ASSIGN",

		// ���
		// +  -      *    /    ==      >   >=   <   <=   !=
		"PLUS", "MINUS", "MUL", "DIV", "EQUAL", "GT", "GTE", "LT", "LTE", "NE",

		// ���
		// ;
		"SEMICOLON",

		// �ָ���
		// ,
		"COMMA",

		// ע�ͷ�
		// /*      */         //        /* */
		"LPCOMMENT", "RPCOMMENT", "LCOMMENT", "PCOMMENT",

		// ����
		// (      )
		"LBRACKET", "RBRACKET",

		// ������
		// {    }
		"LBRACE", "RBRACE",

		// ����
		// \n
		"NEXTL",

		// ������
		// eof
		"ENDFILE"
	};

	string str = words[t.first];

	return str;
}