/*============================================================================*/
/* ListCursor_t.hpp - template for list cursor                                */
/*============================================================================*/

#pragma once

#include "DataStructures.h"

namespace DataStructures
{
  template <class OBJECT> class List_t;
  
  template <class OBJECT>
  class ListCursor_t //: public Object
  {
  private:
    List_t<OBJECT>* m_pList;    // the list
    OBJECT*         m_pObject;  // current node

  public:
    ListCursor_t(List_t<OBJECT>* pList);
    
    void vBegin(void);
    void vEnd(void);
    void vFirst(void);
    void vNext(void);
    OBJECT* pObject(void);
  };
}

////////////////////////////////////////////////////////////////////////////////

using namespace DataStructures;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
ListCursor_t<OBJECT>::ListCursor_t(List_t<OBJECT>* pList)
{
  m_pList   = pList;
  m_pObject = nullptr;
}

/*----------------------------------------------------------------------------*/
/* begin iterating list                                                       */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void ListCursor_t<OBJECT>::vBegin(void)
{
  ASSERT(m_pList);
  
  m_pList->vReadLock();
}

/*----------------------------------------------------------------------------*/
/* end iterating list                                                         */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void ListCursor_t<OBJECT>::vEnd(void)
{
  ASSERT(m_pList);
  
  m_pList->vReadUnlock();
}

/*----------------------------------------------------------------------------*/
/* set at first object in list                                                */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void ListCursor_t<OBJECT>::vFirst(void)
{
  ASSERT(m_pList);
  
  m_pObject = m_pList->pHead();
}

/*----------------------------------------------------------------------------*/
/* move to next object in list                                                */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void ListCursor_t<OBJECT>::vNext(void)
{
  if (m_pObject)
    m_pObject = m_pObject->pNext();
}

/*----------------------------------------------------------------------------*/
/* get current object                                                         */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* ListCursor_t<OBJECT>::pObject(void)
{
  return m_pObject;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/