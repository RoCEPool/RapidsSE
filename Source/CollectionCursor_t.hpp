/*============================================================================*/
/* CollectionCursor_t.hpp - template for collection cursor                    */
/*============================================================================*/

#pragma once

#include "DataStructures.h"

namespace DataStructures
{
  template <class OBJECT> class Collection_t;
  
  template <class OBJECT>
  class CollectionCursor_t //: public Object
  {
  private:
    Collection_t<OBJECT>* m_pCollection; // the collection
    OBJECT* m_pObject;     // current node

  public:
    CollectionCursor_t(Collection_t<OBJECT>* pCollection);
    
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
CollectionCursor_t<OBJECT>::CollectionCursor_t(Collection_t<OBJECT>* pCollection)
{
  m_pCollection = pCollection;
  m_pObject = nullptr;
}

/*----------------------------------------------------------------------------*/
/* begin at first object in list                                              */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void CollectionCursor_t<OBJECT>::vFirst(void)
{
  ASSERT(m_pCollection);
  
  m_pObject = m_pCollection->pHead();
}

/*----------------------------------------------------------------------------*/
/* move to next object in list                                                */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void CollectionCursor_t<OBJECT>::vNext(void)
{
  if (m_pObject)
    m_pObject = m_pObject->pNext();
}

/*----------------------------------------------------------------------------*/
/* get current object                                                         */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* CollectionCursor_t<OBJECT>::pObject(void)
{
  return m_pObject;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/