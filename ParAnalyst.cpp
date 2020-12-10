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
	// ����ɶҲ����
}

Production::Production()
{
	// ����ɶҲ����
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
		cerr << "�������ļ�" << path << "ʧ��" << endl;
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

// ������ʽ
Status ParAnalyst::getProductions()
{
	int id = 0;
	string tempString;

	// ���ϵĶ�����ʽ
	while (inputProductions >> tempString)
	{
		Production tempProduction;
		tempProduction.id = id;
		tempProduction.lcontent = Symbol(tempString, false);
		inputProductions >> tempString; // ����assign���
		char temp[MAXLINECNT];
		inputProductions.getline(temp, MAXLINECNT, '\n');
		stringstream s(temp);
		while (s >> tempString)
		{
			tempProduction.rcontent.push_back(Symbol(tempString, isVt(tempString)));
		}
		Productions.push_back(tempProduction); // �Ż���Ŀ�鵱��
		id++;
	}

	return OK;
}

// ��������Ĳ���ʽ��test��
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

	First[Symbol("#", true)].insert(Symbol("#", true)); // ���ｫ#����

	// ����ظ������Ĺ���ֱ��First���ϲ������仯
	while (1)
	{
		isAddFirst = false;
		// ������Ҫ����ÿ����Ŀ��ÿ��Symbol
		vector<Production>::iterator itProduction;
		for (itProduction = Productions.begin(); itProduction != Productions.end(); itProduction++)
		{
			vector<Symbol>::iterator itSymbol;
			for (itSymbol = itProduction->rcontent.begin(); itSymbol != itProduction->rcontent.end(); itSymbol++)
			{
				/*
				 * ���������Ҳ��������ս����������룬ֱ�ӽ�����������Ŀ���Ҳ����ż���(break)
				 */
				if (itSymbol->Vt == true)
				{
					if (First[itProduction->lcontent].insert(*itSymbol).second == true)
						isAddFirst = true;
					break;
				}

				/*
				 * ���������Ҳ������Ƿ��ս������Ҫ����
				 * ����ʼ������eps��������������ķ��ţ�����Ҫ��Ҫ����left��first
				 * ��û��eps����ɼ�������Ҳ����ź�ֱ�ӽ�����������Ŀ���Ҳ����ţ�ת����һ����Ŀ(break)
				 */
				else
				{
					bool isEps = false;
					set<Symbol>::iterator itFirst;

					// ����һ���Ҳ����ŵ�first����
					for (itFirst = First[*itSymbol].begin(); itFirst != First[*itSymbol].end(); itFirst++)
					{
						if (itFirst->content == "EPS")
						{
							isEps = true;
						}
						else // ����else��Ϊ�˲�����eps
						{
							if (First[itProduction->lcontent].insert(*itFirst).second == true)
								isAddFirst = true;
						}
					}

					// ���û��eps����ô�Ͳ���Ҫ���������Ҳ����Ŵ�
					if (isEps == false)
						break;
				}


			} // end of ������Ŀ

			// �������������Ŀ�ܹ����������һ�����ţ���˵����Ҫ����eps��Ϊ���󲿷��ŵ�First����
			if (itSymbol == itProduction->rcontent.end())
			{
				if (First[itProduction->lcontent].insert(Symbol("EPS", true)).second == true)
					isAddFirst = true;
			}

		}// end of ��������ʽ

		// ���First�����䣬������while
		if (!isAddFirst)
			break;
	}// end of while

	return OK;
}

//Status ParAnalyst::initFollow()
//{
//	bool isAddFollow = false;
//
//	// ���Ƚ�#������ʼ���ŵ�Follow����
//	Follow[Productions[0].lcontent].insert(Symbol("#", true));
//
//	// �ظ������Ĺ���ֱ��Follow�����䣨�����ӣ�
//	while (1)
//	{
//		isAddFollow = false;
//
//		// ���ȱ���ÿ����Ŀ��ÿ���Ҳ�����
//		vector<Production>::iterator itProduction;
//		for (itProduction = Productions.begin(); itProduction != Productions.end(); itProduction++)
//		{
//			vector<Symbol>::iterator itSymbol;
//			for (itSymbol = itProduction->rcontent.begin(); itSymbol != itProduction->rcontent.end(); itSymbol++)
//			{
//				// �����itSymbolΪ��׼����ʼȥ����������ұߵķ���
//				vector<Symbol>::iterator itNextSymbol;
//				// ����itSymbol���Ҳ�����
//				for (itNextSymbol = itSymbol + 1; itNextSymbol != itProduction->rcontent.end(); itNextSymbol++)
//				{
//
//				}
//
//
//			}// end of �����Ҳ�����
//		}// end of ������Ŀ
//	}// end of while
//}

Status ParAnalyst::showFirst()
{
	map<Symbol, set<Symbol>>::iterator itFirst;
	for (itFirst = First.begin(); itFirst != First.end(); itFirst++)
	{
		cout << itFirst->first.content << "��First����Ϊ{";
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
			 * ���������Ҳ��������ս����������룬ֱ�ӽ�����������Ŀ���Ҳ����ż���(break)
			 */
			if (itList->Vt == true)
			{
				if (retSet.insert(*itList).second == true)
					isAddFirst = true;
				break;
			}

			/*
			 * ���������Ҳ������Ƿ��ս������Ҫ����
			 * ����ʼ������eps��������������ķ��ţ�����Ҫ��Ҫ����left��first
			 * ��û��eps����ɼ�������Ҳ����ź�ֱ�ӽ�����������Ŀ���Ҳ����ţ�ת����һ����Ŀ(break)
			 */
			else
			{
				bool isEps = false;
				set<Symbol>::iterator itFirst;

				// ����һ���Ҳ����ŵ�first����
				for (itFirst = First[*itList].begin(); itFirst != First[*itList].end(); itFirst++)
				{
					if (itFirst->content == "EPS")
					{
						isEps = true;
					}
					else // ����else��Ϊ�˲�����eps
					{
						if (retSet.insert(*itFirst).second == true)
							isAddFirst = true;
					}
				}

				// ���û��eps����ô�Ͳ���Ҫ���������Ҳ����Ŵ�
				if (isEps == false)
					break;
			}
		}// end of ����չ����
		// �������������Ŀ�ܹ����������һ�����ţ���˵����Ҫ����eps��Ϊ���󲿷��ŵ�First����
		if (itList == S.end())
		{
			if (retSet.insert(Symbol("EPS", true)).second == true)
				isAddFirst = true;
		}

		// ���First�����䣬������while
		if (!isAddFirst)
			break;
	}// end of while

	return retSet;
}

// ��һ��״̬����һ��״̬���ϣ���ʵ����հ��Ĺ��̣�
I ParAnalyst::Closure(Item item)
{
	I ret;

	// ����ǹ�Լ��Ŀ
	// �����������ս��
	if (item.posPoint == Productions[item.idProduction].rcontent.size() || Productions[item.idProduction].rcontent[item.posPoint].Vt)
	{
		ret.Items.insert(item);
	}
	// ������Ƿ��ս��
	else
	{
		ret.Items.insert(item);
		vector<Production>::iterator itProduction;
		for (itProduction = Productions.begin(); itProduction != Productions.end(); itProduction++)
		{
			if (itProduction->lcontent == Productions[item.idProduction].rcontent[item.posPoint])
			{
				// ������Ҫ���ҵ��Ĳ���ʽ������I������������Ҫ��һ��չ����
				list<Symbol> SymList; //���� a

				// ����size��������
				// ���ұߵĲ���ʽ���η���SymList����Ϊ���չ��������ڲ���
				for (int cnt = 1; item.posPoint + cnt < Productions[item.idProduction].rcontent.size(); cnt++)
				{
					SymList.push_back(Productions[item.idProduction].rcontent[cnt]);
				}
				SymList.push_back(item.expect); //�ǵ÷�a

				set<Symbol> retSet; //��õ�չ����
				retSet = getExpect(SymList);

				// ����Щչ�������μ���I��ret����
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