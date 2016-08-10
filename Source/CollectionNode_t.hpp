/*============================================================================*/
/* CollectionNode_t.hpp - template for collection node mixin                  */
/*============================================================================*/

#pragma once

#include "DataStructures.h"

namespace DataStructures
{
  template <class OBJECT>
  class CollectionNode_t
  {
    OBJECT* m_pPrev;   // prev node
    OBJECT* m_pNext;   // next node
    
  public:
    void* operator new(size_t);
    
    CollectionNode_t<OBJECT>(void);
    
    OBJECT* pNext(void);
    void    vNext(OBJECT* pNext);
    OBJECT* pPrev(void);
    void    vPrev(OBJECT* pPrev);
  };
}

////////////////////////////////////////////////////////////////////////////////

using namespace DataStructures;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void* CollectionNode_t<OBJECT>::operator new(size_t iSize)
{
  vFatal("fatal: ListNode_t class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*---------------------------------------------------------------------------*/
/* constructor                                                               */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
CollectionNode_t<OBJECT>::CollectionNode_t(void)
{
  m_pNext = nullptr;
  m_pPrev = nullptr;
}

/*---------------------------------------------------------------------------*/
/* get next node                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* CollectionNode_t<OBJECT>::pNext(void)
{
  return m_pNext;
}

/*---------------------------------------------------------------------------*/
/* set next node                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
void CollectionNode_t<OBJECT>::vNext(OBJECT* pNext)
{
  m_pNext = pNext;
}

/*---------------------------------------------------------------------------*/
/* get prev node                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* CollectionNode_t<OBJECT>::pPrev(void)
{
  return m_pPrev;
}

/*---------------------------------------------------------------------------*/
/* set prev node                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
void CollectionNode_t<OBJECT>::vPrev(OBJECT* pPrev)
{
  m_pPrev = pPrev;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/