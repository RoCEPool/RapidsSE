/*============================================================================*/
/* ListItem_t.hpp -- Template for List Node                                   */
/*============================================================================*/
#pragma once

namespace Structures
{
  template <class ITEM>
  class ListItem_t
  {
    ITEM* m_pPrevNode;   // prev node
    ITEM* m_pNextNode;   // next node
  
  public:
    void* operator new(size_t);
    
    ListItem_t<ITEM>(void);
    
    ITEM* pNextNode(void);
    void  vNextNode(ITEM* pNext);
    ITEM* pPrevNode(void);
    void  vPrevNode(ITEM* pPrev);
  };
}

////////////////////////////////////////////////////////////////////////////////

using namespace Structures;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void* ListItem_t<OBJECT>::operator new(size_t)
{
  vFatal("fatal: ListItem_t class cannot be new'd"_Stz);
  
  NEVER_REACH;
}

/*---------------------------------------------------------------------------*/
/* constructor                                                               */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
ListItem_t<OBJECT>::ListItem_t(void)
{
  m_pNextNode = nullptr;
  m_pPrevNode = nullptr;
  
  _checkInitialisedSize_(sizeof(ListItem_t<OBJECT>));
}

/*---------------------------------------------------------------------------*/
/* get next node                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* ListItem_t<OBJECT>::pNextNode(void)
{
  return m_pNextNode;
}

/*---------------------------------------------------------------------------*/
/* set next node                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
void ListItem_t<OBJECT>::vNextNode(OBJECT* pNextNode)
{
  m_pNextNode = pNextNode;
}

/*---------------------------------------------------------------------------*/
/* get prev node                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* ListItem_t<OBJECT>::pPrevNode(void)
{
  return m_pPrevNode;
}

/*---------------------------------------------------------------------------*/
/* set prev node                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
void ListItem_t<OBJECT>::vPrevNode(OBJECT* pPrevNode)
{
  m_pPrevNode = pPrevNode;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
