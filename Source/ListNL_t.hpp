/*============================================================================*/
/* ListNL_t.hpp -- List of Items (non-locking)                                */
/*============================================================================*/
#pragma once

namespace Structures
{
  template <class ITEM> 
  class ListNL_t : public Object
  {
  protected:
    i8      m_nCount;  // number of objects
    ITEM* m_pHead;   // head node
    ITEM* m_pTail;   // tail node

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ListNL_t(eCN eClassNo);
    
    void    vAppend(ITEM* pNode);
    ITEM* pHead(void);
    i8      nObjects(void);
    ITEM* pPop(void);
    void    vPush(ITEM* pNode);
    void    vRemove(ITEM* pNode);
    ITEM* pTail(void);
    
    Footer* aFooter(void);
  };
}

using namespace Structures;

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void* ListNL_t<ITEM>::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_TASKLIST);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListNL_t<ITEM>::operator delete(void* aAddress)
{
  ListNL_t<ITEM>* pList = reinterpret_cast<ListNL_t<ITEM>*>(aAddress);
  
  HeapMem::vFree(pList);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListNL_t<ITEM>::ListNL_t(eCN eClassNo)
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
 m_nCount = 0;
  m_pHead  = nullptr;
  m_pTail  = nullptr;
  
  _checkInitialisedSize_(sizeof(ListNL_t<ITEM>));
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append object to end of list                                               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListNL_t<ITEM>::vAppend(ITEM* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(NULL == pNode->pPrevNode());
  ASSERT(NULL == pNode->pNextNode());
  
  if (m_pTail)
    m_pTail->vNextNode(pNode);
  
  pNode->vPrevNode(m_pTail);
  pNode->vNextNode(nullptr);
  
  m_pTail = pNode;
  
  if (! m_pHead)
    m_pHead = pNode;
  
  m_nCount++;
}

/*----------------------------------------------------------------------------*/
/* get head object in list                                                    */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListNL_t<ITEM>::pHead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_pHead;
}

/*----------------------------------------------------------------------------*/
/* get number of objects in list                                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
i8 ListNL_t<ITEM>::nObjects(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_nCount;
}

/*----------------------------------------------------------------------------*/
/* pop object from head of list                                               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListNL_t<ITEM>::pPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  if ( ! m_pHead ) {
    ASSERT( ! m_nCount );
    return nullptr;
  }

  ITEM* pNode = m_pHead;

  m_pHead = m_pHead->pNextNode();
  if (m_pHead)
    m_pHead->vPrevNode(nullptr);
  else
    m_pTail = nullptr;
  
  m_nCount--;
  if (m_nCount) {
    ASSERT(m_pHead);
    ASSERT(m_pTail);
  }
  else {
    ASSERT( ! m_pHead );
    ASSERT( ! m_pTail )
  }
  
  pNode->vPrevNode(nullptr);
  pNode->vNextNode(nullptr);
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* push object on top of list                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListNL_t<ITEM>::vPush(ITEM* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(NULL == pNode->pPrevNode());
  ASSERT(NULL == pNode->pNextNode());
  
  pNode->vPrevNode(nullptr);
  pNode->vNextNode(m_pHead);
  
  if (m_pHead)
    m_pHead->vPrev(pNode);
  else
    m_pTail = pNode;
  
  m_pHead = pNode;
  
  m_nCount++;
  ASSERT(m_pHead);
  ASSERT(m_pTail)
}

/*----------------------------------------------------------------------------*/
/* remove object from head of list                                            */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListNL_t<ITEM>::vRemove(ITEM* pNode)
{
  ITEM* pPrev = pNode->pPrevNode();
  ITEM* pNext = pNode->pNextNode();

  if (pPrev)
    pPrev->vNextNode(pNext);

  if (pNext)
    pNext->vPrevNode(pPrev);
  
  if (pNode == m_pHead)
    m_pHead = pNext;
  
  if (pNode == m_pTail)
    m_pTail = pPrev;
  
  m_nCount--;
  
  pNode->vPrevNode(nullptr);
  pNode->vNextNode(nullptr);
}

/*----------------------------------------------------------------------------*/
/* get last object in list                                                    */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListNL_t<ITEM>::pTail(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_pTail;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <class ITEM>
Footer* ListNL_t<ITEM>::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
