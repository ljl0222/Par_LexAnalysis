#include "ParAnalyst.h"

bool operator <(const Symbol& a, const Symbol& b)
{
	return a.content < b.content;
}

bool operator ==(const Symbol& a, const Symbol& b)
{
	return a.content == b.content;
}

bool operator <(const Item& a, const Item& b)
{
	return a.idProduction < b.idProduction;
}

bool operator ==(const Item& a, const Item& b)
{
	return a.idProduction == b.idProduction;
}

Symbol::Symbol(string content, bool Vt)
{
	this->content = content;
	this->Vt = Vt;
}

Symbol::Symbol()
{
	// 这里啥也不做
}

Production::Production()
{
	// 这里啥也不做
}

Item::Item(int posPoint, Symbol expect, int idPro)
{
	this->posPoint = posPoint;
	this->expect = expect;
	this->idProduction = idPro;
}

Status ParAnalyst::openInputProductions(string path)
{
	inputProductions.open(path, ios::in);
	if (!inputProductions.is_open())
	{
		cerr << "打开输入文件" << path << "失败" << endl;
		exit(ERROR);
	}
	return OK;
}

ParAnalyst::ParAnalyst()
{
	openInputProductions("inputProductions.txt");
}

ParAnalyst::ParAnalyst(string path)
{
	openInputProductions(path);
}

// 读产生式
Status ParAnalyst::getProductions()
{
	int id = 0;
	string tempString;

	// 不断的读产生式
	while (inputProductions >> tempString)
	{
		Production tempProduction;
		tempProduction.id = id;
		tempProduction.lcontent = Symbol(tempString, false);
		inputProductions >> tempString; // 读掉assign语句
		char temp[MAXLINECNT];
		inputProductions.getline(temp, MAXLINECNT, '\n');
		stringstream s(temp);
		while (s >> tempString)
		{
			tempProduction.rcontent.push_back(Symbol(tempString, isVt(tempString)));
		}
		Productions.push_back(tempProduction); // 放回项目组当中
		id++;
	}

	return OK;
}

// 输出读到的产生式（test）
Status ParAnalyst::showProductions()
{
	cout << setw(10) << "ID" << setw(20) << "LEFT" << setw(10)  << "   " << "RIGHT" << endl;
	vector<Production>::iterator itProduction;
	for (itProduction = Productions.begin(); itProduction != Productions.end(); itProduction++)
	{
		cout << setw(10) << itProduction->id  << "   " << setw(20) << itProduction->lcontent.content << "   ";
		vector<Symbol>::iterator itSymbol;
		for (itSymbol = itProduction->rcontent.begin(); itSymbol != itProduction->rcontent.end(); itSymbol++)
			cout << itSymbol->content << ' ';
		cout << endl;
	}
	return OK;
}

Status ParAnalyst::initFirst()
{
	bool isAddFirst = false;

	First[Symbol("#", true)].insert(Symbol("#", true)); // 这里将#加入

	// 这里，重复这样的过程直至First集合不发生变化
	while (1)
	{
		isAddFirst = false;
		// 这里需要遍历每个项目的每个Symbol
		vector<Production>::iterator itProduction;
		for (itProduction = Productions.begin(); itProduction != Productions.end(); itProduction++)
		{
			vector<Symbol>::iterator itSymbol;
			for (itSymbol = itProduction->rcontent.begin(); itSymbol != itProduction->rcontent.end(); itSymbol++)
			{
				/*
				 * 如果这里的右部符号是终结符，将其加入，直接结束遍历该项目的右部符号即可(break)
				 */
				if (itSymbol->Vt == true)
				{
					if (First[itProduction->lcontent].insert(*itSymbol).second == true)
						isAddFirst = true;
					break;
				}

				/*
				 * 如果这里的右部符号是非终结符，需要分类
				 * 若起始符号有eps，继续遍历后面的符号，看看要不要加入left的first
				 * 若没有eps，完成加入这个右部符号后，直接结束遍历该项目的右部符号，转入下一个项目(break)
				 */
				else
				{
					bool isEps = false;
					set<Symbol>::iterator itFirst;

					// 遍历一个右部符号的first集合
					for (itFirst = First[*itSymbol].begin(); itFirst != First[*itSymbol].end(); itFirst++)
					{
						if (itFirst->content == "EPS")
						{
							isEps = true;
						}
						else // 这里else是为了不加入eps
						{
							if (First[itProduction->lcontent].insert(*itFirst).second == true)
								isAddFirst = true;
						}
					}

					// 如果没有eps，那么就不需要继续遍历右部符号串
					if (isEps == false)
						break;
				}


			} // end of 遍历项目

			// 如果这里，在这个项目能够遍历到最后一个符号，则说明需要加入eps作为该左部符号的First集合
			if (itSymbol == itProduction->rcontent.end())
			{
				if (First[itProduction->lcontent].insert(Symbol("EPS", true)).second == true)
					isAddFirst = true;
			}

		}// end of 遍历产生式

		// 如果First集不变，就跳出while
		if (!isAddFirst)
			break;
	}// end of while

	return OK;
}

//Status ParAnalyst::initFollow()
//{
//	bool isAddFollow = false;
//
//	// 首先将#加入起始符号的Follow集合
//	Follow[Productions[0].lcontent].insert(Symbol("#", true));
//
//	// 重复这样的过程直至Follow集不变（不增加）
//	while (1)
//	{
//		isAddFollow = false;
//
//		// 首先遍历每个项目的每个右部符号
//		vector<Production>::iterator itProduction;
//		for (itProduction = Productions.begin(); itProduction != Productions.end(); itProduction++)
//		{
//			vector<Symbol>::iterator itSymbol;
//			for (itSymbol = itProduction->rcontent.begin(); itSymbol != itProduction->rcontent.end(); itSymbol++)
//			{
//				// 这里，以itSymbol为基准，开始去找这个符号右边的符号
//				vector<Symbol>::iterator itNextSymbol;
//				// 遍历itSymbol的右部符号
//				for (itNextSymbol = itSymbol + 1; itNextSymbol != itProduction->rcontent.end(); itNextSymbol++)
//				{
//
//				}
//
//
//			}// end of 遍历右部符号
//		}// end of 遍历项目
//	}// end of while
//}

Status ParAnalyst::showFirst()
{
	map<Symbol, set<Symbol>>::iterator itFirst;
	for (itFirst = First.begin(); itFirst != First.end(); itFirst++)
	{
		cout << itFirst->first.content << "的First集合为{";
		set<Symbol>::iterator itSet;
		for (itSet = itFirst->second.begin(); itSet != itFirst->second.end(); itSet++)
			cout << itSet->content << ' ';
		cout << "}" << endl;
	}
	return OK;
}

set<Symbol> ParAnalyst::getExpect(list<Symbol> S)
{
	set<Symbol> retSet;
	bool isAddFirst = false;

	while (1)
	{
		isAddFirst = false;
		list<Symbol>::iterator itList;
		for (itList = S.begin(); itList != S.end(); itList++)
		{
			/*
			 * 如果这里的右部符号是终结符，将其加入，直接结束遍历该项目的右部符号即可(break)
			 */
			if (itList->Vt == true)
			{
				if (retSet.insert(*itList).second == true)
					isAddFirst = true;
				break;
			}

			/*
			 * 如果这里的右部符号是非终结符，需要分类
			 * 若起始符号有eps，继续遍历后面的符号，看看要不要加入left的first
			 * 若没有eps，完成加入这个右部符号后，直接结束遍历该项目的右部符号，转入下一个项目(break)
			 */
			else
			{
				bool isEps = false;
				set<Symbol>::iterator itFirst;

				// 遍历一个右部符号的first集合
				for (itFirst = First[*itList].begin(); itFirst != First[*itList].end(); itFirst++)
				{
					if (itFirst->content == "EPS")
					{
						isEps = true;
					}
					else // 这里else是为了不加入eps
					{
						if (retSet.insert(*itFirst).second == true)
							isAddFirst = true;
					}
				}

				// 如果没有eps，那么就不需要继续遍历右部符号串
				if (isEps == false)
					break;
			}
		}// end of 遍历展望串
		// 如果这里，在这个项目能够遍历到最后一个符号，则说明需要加入eps作为该左部符号的First集合
		if (itList == S.end())
		{
			if (retSet.insert(Symbol("EPS", true)).second == true)
				isAddFirst = true;
		}

		// 如果First集不变，就跳出while
		if (!isAddFirst)
			break;
	}// end of while

	return retSet;
}

// 由一个状态产生一个状态集合（其实是求闭包的过程？
I ParAnalyst::Closure(Item item)
{
	I ret;

	// 如果是规约项目
	// 如果点后面是终结符
	if (item.posPoint == Productions[item.idProduction].rcontent.size() || Productions[item.idProduction].rcontent[item.posPoint].Vt)
	{
		ret.Items.insert(item);
	}
	// 点后面是非终结符
	else
	{
		ret.Items.insert(item);
		vector<Production>::iterator itProduction;
		for (itProduction = Productions.begin(); itProduction != Productions.end(); itProduction++)
		{
			if (itProduction->lcontent == Productions[item.idProduction].rcontent[item.posPoint])
			{
				// 这里需要将找到的产生式都加入I，但是这里需要求一下展望符
				list<Symbol> SymList; //贝塔 a

				// 可能size会有问题
				// 将右边的产生式依次放入SymList，作为获得展望串的入口参数
				for (int cnt = 1; item.posPoint + cnt < Productions[item.idProduction].rcontent.size(); cnt++)
				{
					SymList.push_back(Productions[item.idProduction].rcontent[cnt]);
				}
				SymList.push_back(item.expect); //记得放a

				set<Symbol> retSet; //获得的展望串
				retSet = getExpect(SymList);

				// 将这些展望符依次加入I（ret）中
				set<Symbol>::iterator itSymbol;
				for (itSymbol = retSet.begin(); itSymbol != retSet.end(); itSymbol++)
				{
					I temp = Closure(Item(0, *itSymbol, itProduction->id));
					
					set<Item>::iterator itItem;
					for (itItem = temp.Items.begin(); itItem != temp.Items.end(); itItem++)
					{
						ret.Items.insert(*itItem);
					}
				}
			}
		}
	}

	return ret;
}

Status ParAnalyst::createDFA()
{
	DFA.status.push_back(Closure(Item(0, Symbol("#", true), 0)));

	cout << endl;

	list<I>::iterator itI;
	for (itI = DFA.status.begin(); itI != DFA.status.end(); itI++)
	{
		set<Item>::iterator itS;
		for (itS = itI->Items.begin(); itS != itI->Items.end(); itS++)
		{
			cout << "("  <<itS->idProduction << "," << itS->expect.content << "," << itS->posPoint << ")" << endl;
			cout << "end of Items" << endl;
		}
		cout << "end of Status" << endl;
	}

	return OK;
}