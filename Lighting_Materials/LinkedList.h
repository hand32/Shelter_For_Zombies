//	Linked List (Header)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr

#pragma once
#include <iostream>
template <class TYPE>
void IpInsertSort(TYPE *data, int size){
    int i, k;
	TYPE t;

    for(i=1 ; i<size ; i++){
        t = data[i];

        for(k=i-1 ; k>=0 ; k--){
            if(data[k] > t)
                data[k+1] = data[k];
            else
                break;
        }
        data[k+1] = t;
    }
}

template <class TYPE1, class TYPE2>
void IpInsertSort(TYPE1 *data, int size, TYPE2 (*Fn)(TYPE1)){
    int i, k;
	TYPE1 t;

    for(i=1 ; i<size ; i++){
        t = data[i];

        for(k=i-1 ; k>=0 ; k--){
            if(Fn(data[k]) > Fn(t))
                data[k+1] = data[k];
            else
                break;
        }
        data[k+1] = t;
    }
}

template <class TYPE>
struct CNode4IpList {
	TYPE data;
	CNode4IpList *next;
	CNode4IpList *prev;
	
	CNode4IpList () {
		next = prev = NULL;
	}
	CNode4IpList(TYPE d, CNode4IpList *n, CNode4IpList *p) : data(d), next(n), prev(p) {
	}
};

template <class TYPE>
struct CNode4IpBinTree {
	TYPE data;
	CNode4IpBinTree *left;
	CNode4IpBinTree *right;
	
	CNode4IpBinTree () {
		left = right = NULL;
	}
	CNode4IpBinTree (TYPE d, CNode4IpBinTree *l, CNode4IpBinTree* r) : data(d), left(l), right(r) {
	}
};

template <class TYPE>
class CIpList {
private:
	CNode4IpList<TYPE> *m_pHead;
	CNode4IpList<TYPE> *m_pTail;
	int m_nCnt;
public:
	CIpList();
	~CIpList();
	void DeleteAll();
	bool IsEmpty();
	int GetSize();
	void InsertHead(TYPE Data);
	void InsertTail(TYPE Data);
	void DeleteHead();
	void DeleteTail();
	void DeletePrevious(CNode4IpList<TYPE> *pCurr);
	CNode4IpList<TYPE> *GetHead();
	TYPE &GetDataForward(CNode4IpList<TYPE> **pCurr);
	TYPE &GetDataBackward(CNode4IpList<TYPE> **pCurr);
};

template<class TYPE>
class CIpBinTree {
public:
	CNode4IpBinTree<TYPE> *m_pRoot;
public:
	CIpBinTree();
	~CIpBinTree();
	void DeleteAll();
	void PostorderDelete(CNode4IpBinTree<TYPE> *pRoot);

	void Inorder(CNode4IpBinTree<TYPE> *pRoot, CIpList<TYPE> *pList);
	void Preorder(CNode4IpBinTree<TYPE> *pRoot, CIpList<TYPE> *pList);
	void Postorder(CNode4IpBinTree<TYPE> *pRoot, CIpList<TYPE> *pList);
};

template <class TYPE>
CIpList<TYPE>::CIpList()
{
	m_pHead = NULL;
	m_pTail = NULL;

	m_nCnt = 0;
}

template <class TYPE>
CIpList<TYPE>::~CIpList()
{
	DeleteAll();
}

template <class TYPE>
void CIpList<TYPE>::DeleteAll()
{
	CNode4IpList<TYPE> *Curr = m_pHead, *Temp;

	while(Curr)
	{
		Temp = Curr;
		Curr = Curr->next;

		delete Temp;
	}

	m_pHead = NULL;
	m_pTail = NULL;

	m_nCnt = 0;
}

template <class TYPE>
CNode4IpList<TYPE> *CIpList<TYPE>::GetHead()
{
	return m_pHead;
}

template <class TYPE>
TYPE &CIpList<TYPE>::GetDataForward(CNode4IpList<TYPE> **pCurr)
{
	TYPE &Data = (*pCurr)->data;

	*pCurr = (*pCurr)->next;
	return Data;
}

template <class TYPE>
TYPE &CIpList<TYPE>::GetDataBackward(CNode4IpList<TYPE> **pCurr)
{
	TYPE &Data = (*pCurr)->data;

	*pCurr = (*pCurr)->prev;
	return Data;
}

template <class TYPE>
bool CIpList<TYPE>::IsEmpty()
{
	if(m_nCnt == 0) return true;

	return false;
}

template <class TYPE>
int CIpList<TYPE>::GetSize()
{
	return m_nCnt;
}

template <class TYPE>
void CIpList<TYPE>::InsertHead(TYPE Data)
{
	CNode4IpList<TYPE> *Temp = new CNode4IpList<TYPE>;
	Temp->data = Data;
	Temp->next = Temp->prev = NULL;

	if(m_pHead == NULL)
	{
		m_pHead = Temp;
		m_pTail = Temp;
	}
	else
	{
		m_pHead->prev = Temp;
		Temp->next = m_pHead;
		m_pHead = Temp;
	}

	m_nCnt++;
}

template <class TYPE>
void CIpList<TYPE>::InsertTail(TYPE Data)
{
	CNode4IpList<TYPE> *Temp = new CNode4IpList<TYPE>;
	Temp->data = Data;
	Temp->next = Temp->prev = NULL;

	if(m_pTail == NULL)
	{
		m_pHead = Temp;
		m_pTail = Temp;
	}
	else
	{
		m_pTail->next = Temp;
		Temp->prev = m_pTail;
		m_pTail = Temp;
	}

	m_nCnt++;
}

template <class TYPE>
void CIpList<TYPE>::DeleteHead()
{	
	if(!IsEmpty())
	{
		CNode4IpList<TYPE> *Temp = m_pHead;

		if(m_pHead == m_pTail)
		{
			m_pHead = NULL;
			m_pTail = NULL;
		}
		else
		{
			m_pHead = m_pHead->next;
			m_pHead->prev = NULL;
		}

		delete Temp;

		m_nCnt--;		
	}
}

template <class TYPE>
void CIpList<TYPE>::DeleteTail()
{
	if(!IsEmpty())
	{
		CNode4IpList<TYPE> *Temp = m_pTail;

		if(m_pHead == m_pTail)
		{
			m_pHead = NULL;
			m_pTail = NULL;
		}
		else
		{
			m_pTail = m_pTail->prev;
			m_pTail->next = NULL;
		}

		delete Temp;

		m_nCnt--;		
	}
}

template <class TYPE>
void CIpList<TYPE>::DeletePrevious(CNode4IpList<TYPE> *pCurr)
{
	if(IsEmpty()) return;

	if(pCurr == NULL)
	{
		DeleteTail();
		return;
	}

	TYPE &Data = pCurr->data;

	pCurr = pCurr->prev;
	if(pCurr)
	{
		if(pCurr == m_pHead && pCurr == m_pTail)
			DeleteHead();
		else if(pCurr == m_pHead)
			DeleteHead();
		else if(pCurr == m_pTail)
			DeleteTail();
		else
		{
			CNode4IpList<TYPE> *pPrev = pCurr->prev, *pNext = pCurr->next;

			pPrev->next = pCurr->next;
			pNext->prev = pCurr->prev;

			delete pCurr;

			m_nCnt--;
		}
	}
}
