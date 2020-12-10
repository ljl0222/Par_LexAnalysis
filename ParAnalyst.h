#pragma once

#include "common.h"

class Symbol
{
public:
	string content;
	bool Vt;
	Symbol(string content, bool Vt);
	Symbol();
	// 重载运算符<，这样才能实例化map中key为class的结构
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
	int posPoint; // 注明点的位置
	Symbol expect; // 展望符
	int idProduction; // 产生式id
	Item(int posPoint, Symbol expect, int idProduction);
	friend bool operator <(const Item& a, const Item& b);

	friend bool operator ==(const Item& a, const Item& b);

};

class I // 项目集
{
public:
	set<Item> Items;
};

class DFA
{
public:
	list<I> status;
};

class ParAnalyst
{
private:
	ifstream inputProductions;
	vector<Production> Productions;
	map<Symbol, set<Symbol>> First;
	map<Symbol, set<Symbol>> Follow;
	DFA DFA;
public:
	ParAnalyst();
	ParAnalyst(string path);
	Status openInputProductions(string path);
	Status getProductions();
	Status showProductions();
	Status initFirst();
	Status initFollow();
	Status showFirst();
	Status createDFA();
	I Closure(Item item);
	set<Symbol> getExpect(list<Symbol> S);
};


