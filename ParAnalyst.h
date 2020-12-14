#pragma once

#include "common.h"

class Symbol
{
public:
	string content;
	bool Vt;
	Symbol(string content, bool Vt);
	Symbol();
	// ���������<����������ʵ����map��keyΪclass�Ľṹ
	friend bool operator <(const Symbol& a, const Symbol& b);

	friend bool operator ==(const Symbol& a, const Symbol& b);
};

class Production
{
public:
	int id;
	Symbol lcontent;
	vector<Symbol> rcontent;
	Production();
};

class Item
{
public:
	int posPoint; // ע�����λ��
	Symbol expect; // չ����
	int idProduction; // ����ʽid
	Item(int posPoint, Symbol expect, int idProduction);
	friend bool operator <(const Item& a, const Item& b);

	friend bool operator ==(const Item& a, const Item& b);

};

class I // ��Ŀ��
{
public:
	set<Item> Items;
};

typedef pair<int, Symbol> GOTO;


class DFA
{
public:
	list<I> status;
	map<GOTO, int> gotos; //��ǰ״̬����GOTO����ǰ̬����symbol���������mapӳ���gotos��int��
};

enum Behave { shift, reduct, accept, error };
class Behavior
{
public:
	Behave bh;
	int nextS;
	Behavior();
	Behavior(Behave bh, int s);

};

class ParAnalyst
{
private:
	ifstream inputProductions;
	// ifstream inputLex;
	vector<Production> Productions;
	map<Symbol, set<Symbol>> First;
	map<Symbol, set<Symbol>> Follow;
	DFA DFA;
	map<GOTO, Behavior> ActionTb;
	map<GOTO, int> GotoTb;
	stack<int> StatusStack;
	stack<Symbol> SymbolStack;
public:
	ParAnalyst();
	ParAnalyst(string path);
	Status openInputProductions(string path);
	Status getProductions();
	Status showProductions();
	Status initFirst();
	// Status initFollow();
	Status showFirst();
	Status createDFA();
	I Closure(Item item);
	set<Symbol> getExpect(list<Symbol> S);
	Status outputAction();
	Status outputActionToFile();
	Status outputGoto();
	Status outputGotoToFile();
	Status LRAnalyse(list<Token> LexRes);
	Status outputStatusStack();
	Status outputSymbolStack();
	Status outputStack(int cnt);
	Status outputStackToFile(int cnt);
};


