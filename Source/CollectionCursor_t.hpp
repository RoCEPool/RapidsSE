/*============================================================================*/
/* CollectionCursor_t.hpp -- Template for Collection Cursor                   */
/*============================================================================*/
#pragma once

namespace Structures
{
  template <class OBJECT> class Collection_t;
  
  template <class OBJECT>
  class CollectionCursor_t //: public Object
  {
  private:
    Collection_t<OBJECT>* m_pCollection; // the collection
    bool    m_bHasLock; // does the cursor have the lock?
    OBJECT* m_pObject;  // current node

    // must be last
    Footer m_xFooter;
    
  public:
    CollectionCursor_t(Collection_t<OBJECT>* pCollection);
    
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
CollectionCursor_t<OBJECT>::CollectionCursor_t(Collection_t<OBJECT>* pCollection)
{
  m_pCollection = pCollection;
  m_pObject = nullptr;
  m_bHasLock = false;
  
  _checkInitialisedSize_(sizeof(CollectionCursor_t<OBJECT>));
}

/*----------------------------------------------------------------------------*/
/* begin iterating collection                                                 */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void CollectionCursor_t<OBJECT>::vBegin(void)
{
  ASSERT(m_pCollection);
  ASSERT(! m_bHasLock);
  
  m_pCollection->vReadLock();
  m_bHasLock = true;
}

/*----------------------------------------------------------------------------*/
/* end iterating collection                                                   */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void CollectionCursor_t<OBJECT>::vEnd(void)
{
  ASSERT(m_pCollection);
  
  if (m_bHasLock) {
    m_pCollection->vReadUnlock();
    m_bHasLock = false;
  }
  
  ASSERT(! m_bHasLock);
}

/*----------------------------------------------------------------------------*/
/* set at first object in collection                                          */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void CollectionCursor_t<OBJECT>::vFirst(void)
{
  ASSERT(m_pCollection);
  ASSERT(m_bHasLock);
   
  m_pObject = m_pCollection->pHead();
}

/*----------------------------------------------------------------------------*/
/* move to next object in collection                                          */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void CollectionCursor_t<OBJECT>::vNext(void)
{
  ASSERT(m_pCollection);
  ASSERT(m_bHasLock);
  
  if (m_pObject)
    m_pObject = m_pObject->pNextNode();
}

/*----------------------------------------------------------------------------*/
/* get current object                                                         */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* CollectionCursor_t<OBJECT>::pObject(void)
{
  ASSERT(m_pCollection);
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
Footer* CollectionCursor_t<OBJECT>::aFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
