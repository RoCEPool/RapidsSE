/*============================================================================*/
/* ListItem_t.hpp -- Template for List Item                                   */
/*============================================================================*/
#pragma once

namespace Structures
{
  template <class ITEM>
  class ListItem_t
  {
  private:
    ITEM* m_pPrevItem;   // prev item
    ITEM* m_pNextItem;   // next item
  
  public:
    void* operator new(size_t);
    
    ListItem_t<ITEM>(void);
    
    ITEM* pNextItem(void);
    void  vNextItem(ITEM* pNext);
    ITEM* pPrevItem(void);
    void  vPrevItem(ITEM* pPrev);
  };
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void* ListItem_t<OBJECT>::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: ListItem_t class cannot be new'd"_Stz);
  
  NEVER_REACH;
}

/*---------------------------------------------------------------------------*/
/* constructor                                                               */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
ListItem_t<OBJECT>::ListItem_t(void)
{
  m_pNextItem = nullptr;
  m_pPrevItem = nullptr;
  
  _checkInitialisedSize_(sizeof(ListItem_t<OBJECT>));
}

/*---------------------------------------------------------------------------*/
/* get next item                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* ListItem_t<OBJECT>::pNextItem(void)
{
  return m_pNextItem;
}

/*---------------------------------------------------------------------------*/
/* set next item                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
void ListItem_t<OBJECT>::vNextItem(OBJECT* pNextItem)
{
  m_pNextItem = pNextItem;
}

/*---------------------------------------------------------------------------*/
/* get prev item                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* ListItem_t<OBJECT>::pPrevItem(void)
{
  return m_pPrevItem;
}

/*---------------------------------------------------------------------------*/
/* set prev item                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
void ListItem_t<OBJECT>::vPrevItem(OBJECT* pPrevItem)
{
  m_pPrevItem = pPrevItem;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
