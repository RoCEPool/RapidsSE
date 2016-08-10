/*============================================================================*/
/* Colleciton_t.hpp - template for collection mixin                           */
/*============================================================================*/

#pragma once

#include "DataStructures.h"

namespace DataStructures
{
  template <class OBJECT> 
  class Collection_t
  {
  private:
    Lock    m_zLock;   // lock
    i8      m_iCount;  // number of objects
    OBJECT* m_pHead;   // head node
    OBJECT* m_pTail;   // tail node
    OBJECT* m_pFree;   // free chain

  public:
    void* operator new(size_t);
    
    Collection_t(void);
        
    OBJECT* pAllocObject(void);
    void    vFreeObject(OBJECT* pObject);
    
    void    vAppend(OBJECT*& pNode);
    void    vDump(const char* aTitle);
    void    vInsert(OBJECT*& pNode);
    OBJECT* pPop(void);
    void    vRemove(OBJECT*& pNode);

    void    vReadLock(void);
    void    vReadUnlock(void);
    
    OBJECT* pHead(void);
    OBJECT* pTail(void);
  };
}

////////////////////////////////////////////////////////////////////////////////

using namespace DataStructures;

extern Heap* gpSmallHeap;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void* Collection_t<OBJECT>::operator new(size_t iSize)
{
  vFatal("fatal: Collection_t class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
Collection_t<OBJECT>::Collection_t(void)
{
  m_zLock.vCheckInit();
  m_iCount = 0;
  m_pHead  = nullptr;
  m_pTail  = nullptr;
  m_pFree  = nullptr;
}

/*----------------------------------------------------------------------------*/
/* allocate object & append to collection                                     */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* Collection_t<OBJECT>::pAllocObject(void)
{
  m_zLock.vGrabWrite();

  OBJECT* pObject = nullptr;
  
  if (m_pFree) {
    OBJECT* pObject = m_pFree;
    m_pFree = m_pFree->pNext();
  }
  else {
    // this is an assumption
    pObject = reinterpret_cast<OBJECT*>(gpSmallHeap->pAllocObject());
  }
  
  m_zLock.vDropWrite();
  
  vAppend(pObject);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* dump each object in collection                                             */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void Collection_t<OBJECT>::vDump(const char* aTitle)
{
  vReadLock();
  
  CollectionCursor_t<OBJECT> xCursor(this);
  
  vTrace("%s\n",aTitle);
  for(xCursor.vFirst(); xCursor.pObject(); xCursor.vNext()) {
    OBJECT* pObject = xCursor.pObject();
    pObject->vDump();
  }
  
  vReadUnlock();
}

/*----------------------------------------------------------------------------*/
/* remove object from collection and push onto free chain                     */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void Collection_t<OBJECT>::vFreeObject(OBJECT* pObject)
{
  vRemove(pObject);
  
  m_zLock.vGrabWrite();
  
  pObject->vNext(m_pFree);
  m_pFree = pObject;
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* append object to end of list                                               */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void Collection_t<OBJECT>::vAppend(OBJECT*& pNode)
{
  m_zLock.vGrabWrite();
  
  if (m_pTail)
    m_pTail->vNext(pNode);
  
  pNode->vPrev(m_pTail);
  pNode->vNext(nullptr);
  
  m_pTail = pNode;
  
  m_iCount++;
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* insert object at head of list                                              */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void Collection_t<OBJECT>::vInsert(OBJECT*& pNode)
{
  ASSERT( ! pNode->pPrev() );
  ASSERT( ! pNode->pNext() );
  
  m_zLock.vGrabWrite();
  
  if (m_pHead)
    m_pTail->vHead(pNode);
  
  pNode->vPrev(nullptr);
  pNode->vNext(m_pHead);
  
  m_pHead = pNode;
  
  m_iCount++;
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* pop object from head of list                                               */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* Collection_t<OBJECT>::pPop(void)
{
  m_zLock.vGrabWrite();
  
  if ( ! m_pHead ) {
    ASSERT( ! m_iCount );
    m_zLock.vDropWrite();
    return nullptr;
  }

  OBJECT* pNode = m_pHead;

  m_pHead = m_pHead->pNext();
  if (m_pHead)
    m_pHead->vPrev(nullptr);
  else
    m_pTail = nullptr;
  
  m_iCount--;
  
  m_zLock.vDropWrite();
  
  pNode->vPrev(nullptr);
  pNode->vNext(nullptr);
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* remove object from head of list                                            */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void Collection_t<OBJECT>::vRemove(OBJECT*& pNode)
{
  m_zLock.vGrabWrite();
  
  OBJECT* pPrev = pNode->pPrev();
  OBJECT* pNext = pNode->pNext();

  if (pPrev)
    pPrev->vNext(pNext);

  if (pNext)
    pNext->vPrev(pPrev);
  
  if (pNode == m_pHead)
    m_pHead = pNext;
  
  if (pNode == m_pTail)
    m_pTail = pPrev;
  
  m_iCount--;
  
  m_zLock.vDropWrite();
  
  pNode->vPrev(nullptr);
  pNode->vNext(nullptr);
}

/*----------------------------------------------------------------------------*/
/* get read lock on collection                                                */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void Collection_t<OBJECT>::vReadLock(void)
{
  m_zLock.vGrabRead();
}

/*----------------------------------------------------------------------------*/
/* release read lock on collection                                            */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void Collection_t<OBJECT>::vReadUnlock(void)
{
  m_zLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* get head object in list                                                    */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* Collection_t<OBJECT>::pHead(void)
{
  return m_pHead;
}

/*----------------------------------------------------------------------------*/
/* get last object in list                                                    */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* Collection_t<OBJECT>::pTail(void)
{
  return m_pTail;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/