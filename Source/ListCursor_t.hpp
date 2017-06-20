/*============================================================================*/
/* ListCursor_t.hpp -- Template for List Cursor                               */
/*============================================================================*/
#pragma once

namespace Structures
{
  template <class OBJECT> class List_t;
  
  template <class OBJECT>
  class ListCursor_t //: public Object
  {
  private:
    List_t<OBJECT>* m_pList;    // the list
    OBJECT*         m_pObject;  // current node
    bool            m_bHasLock; // does the cursor have the lock?

    // must be last
    Footer m_xFooter;
    
  public:
    ListCursor_t(List_t<OBJECT>* pList);
    
    void vBegin(void);
    void vEnd(void);
    void vFirst(void);
    void vNext(void);
    OBJECT* pObject(void);

    Footer* aFooter(void);
  };
}

////////////////////////////////////////////////////////////////////////////////

using namespace Structures;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
ListCursor_t<OBJECT>::ListCursor_t(List_t<OBJECT>* pList)
{
  m_pList    = pList;
  m_pObject  = nullptr;
  m_bHasLock = false;
  
  //MemLib::vCheckInitialised(this,sizeof(ListCursor_t<OBJECT>));
  _checkInitialisedSize_(sizeof(ListCursor_t<OBJECT>));
}

/*----------------------------------------------------------------------------*/
/* begin iterating list                                                       */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void ListCursor_t<OBJECT>::vBegin(void)
{
  ASSERT(m_pList);
  ASSERT(! m_bHasLock);
  
  m_pList->vReadLock();
  m_bHasLock = true;
}

/*----------------------------------------------------------------------------*/
/* end iterating list                                                         */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void ListCursor_t<OBJECT>::vEnd(void)
{
  ASSERT(m_pList);
  
  if (m_bHasLock) {
    m_pList->vReadUnlock();
    m_bHasLock = false;
  }
  
  ASSERT(! m_bHasLock);
}

/*----------------------------------------------------------------------------*/
/* set at first object in list                                                */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void ListCursor_t<OBJECT>::vFirst(void)
{
  ASSERT(m_pList);
  ASSERT(m_bHasLock);
 
  m_pObject = m_pList->pHead();
}

/*----------------------------------------------------------------------------*/
/* move to next object in list                                                */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void ListCursor_t<OBJECT>::vNext(void)
{
  ASSERT(m_pList);
  ASSERT(m_bHasLock);
  
  if (m_pObject)
    m_pObject = m_pObject->pNextNode();
}

/*----------------------------------------------------------------------------*/
/* get current object                                                         */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* ListCursor_t<OBJECT>::pObject(void)
{
  ASSERT(m_pList);
  ASSERT(m_bHasLock);
  
  return m_pObject;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
Footer* ListCursor_t<OBJECT>::aFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
