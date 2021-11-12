/*============================================================================*/
/* SegmentNL_t.hpp -- Segment of Pointers template                            */
/*============================================================================*/
#pragma once

namespace BASE
{  
  template <class ITEM, i8 MAX_ITEMS>
  class SegmentNL_t
  {
  private:
    u8     m_nItems;           // number of items currently in the vector
    ITEM*  m_paItem[MAX_ITEMS]; // array of items (proper objects)
  
  public:
  
    SegmentNL_t(void);
   ~SegmentNL_t(void) noexcept(false);

    void  vAdd(ITEM* pItem);
    void  vAppend(ITEM* pItem);
    void  vClear(void);
    bool  bContains(ITEM* pItem);
    void  vDecItems(void);
    void  vDeleteAll(void);
    ITEM* pGet(u8 i_Item);
    void  vIncItems(void);
    i8    nItems(void);
    void  vItems(i8 nItems);
    ITEM* pPop(void);
    void  vRemove(u8 i_Item);
    void  vRemove(ITEM* pItem);
    void  vSet(u8 i_Item, ITEM* pItem);

  private:
  
    Footer* aFooter(void);
  };
  
  template <class ITEM, i8 MAX_ITEMS>
  SegmentNL_t<ITEM,MAX_ITEMS>* _pVector_NL(void* aAddr);
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
SegmentNL_t<ITEM,MAX_ITEMS>::SegmentNL_t(void)
{
  m_nItems = 0;
  
  for (u8 i_Item = (i8)0; i_Item < MAX_ITEMS; i_Item++)
    m_paItem[(i8)i_Item] = nullptr;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
SegmentNL_t<ITEM,MAX_ITEMS>::~SegmentNL_t(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add item as first item in vector                                           */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
void SegmentNL_t<ITEM,MAX_ITEMS>::vAdd(ITEM* pItem)
{
  // check we have a free slot
  ASSERT(m_nItems >= 0 && m_nItems < MAX_ITEMS - 1);
  
  // get item number
  u8 i_Item = m_nItems++;
  ASSERT(m_nItems > 0 && m_nItems < MAX_ITEMS);
  
  // there shouldnt already be a item
  ASSERT( ! m_paItem[i_Item] );
  
  // if there are items in the array then move array up
  if (i_Item) {
    char* aSrc = reinterpret_cast<char*>(&m_paItem[0]);
    char* aDst = reinterpret_cast<char*>(&m_paItem[1]);
    char* aLst = aAdd<char*>(&m_paItem[i_Item],sizeof(ITEM*));
    i8 nBytes = nDifference(aLst,aDst);
    
    MemLib::vMove(aDst,aSrc,nBytes,CN_NONE);
  }
  
  // set first item
  m_paItem[0] = pItem;
}

/*----------------------------------------------------------------------------*/
/* append item to end of vector                                               */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
void SegmentNL_t<ITEM,MAX_ITEMS>::vAppend(ITEM* pItem)
{
  // check we have a free slot
  ASSERT(m_nItems >= 0 && m_nItems < MAX_ITEMS);
  
  // get item number
  u8 i_Item = m_nItems++;
  
  ASSERT(m_nItems > 0 && m_nItems <= MAX_ITEMS);
  
  // there shouldnt already be a item
  ASSERT( ! m_paItem[i_Item] );
  
  // set item
  m_paItem[i_Item] = pItem;
}

/*----------------------------------------------------------------------------*/
/* clear all items                                                            */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
void SegmentNL_t<ITEM,MAX_ITEMS>::vClear(void)
{
  for (u8 i_Item = 0; i_Item < m_nItems; i_Item++)
    m_paItem[i_Item] = nullptr;
  
  m_nItems = 0;
}

/*----------------------------------------------------------------------------*/
/* test if the vector contains the given item                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
bool SegmentNL_t<ITEM,MAX_ITEMS>::bContains(ITEM* pItem)
{
  for (u8 i_Item = 0; i_Item < m_nItems; i_Item++)
    if (m_paItem[i_Item] == pItem)
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* decrement the number of items                                              */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
void SegmentNL_t<ITEM,MAX_ITEMS>::vDecItems(void)
{
  m_nItems--;
}

/*----------------------------------------------------------------------------*/
/* delete all items                                                           */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
void SegmentNL_t<ITEM,MAX_ITEMS>::vDeleteAll(void)
{
  while (ITEM* pItem = pPop())
    delete pItem;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
ITEM* SegmentNL_t<ITEM,MAX_ITEMS>::pGet(u8 i_Item)
{
  ASSERT(i_Item >= 0 && i_Item < m_nItems);
 
  ITEM* pItem = m_paItem[i_Item];
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* increment the number of items                                              */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
void SegmentNL_t<ITEM,MAX_ITEMS>::vIncItems(void)
{
  m_nItems++;
}

/*----------------------------------------------------------------------------*/
/* get number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
i8 SegmentNL_t<ITEM,MAX_ITEMS>::nItems(void)
{
  ASSERT(m_nItems >= 0 && m_nItems <= MAX_ITEMS);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* set number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
void SegmentNL_t<ITEM,MAX_ITEMS>::vItems(i8 nItems)
{
  ASSERT(m_nItems >= 0 && m_nItems <= MAX_ITEMS);
  
  m_nItems = nItems;
}

/*----------------------------------------------------------------------------*/
/* remove the given item                                                      */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
void SegmentNL_t<ITEM,MAX_ITEMS>::vRemove(u8 i_Item)
{
  // check we have items
  ASSERT(m_nItems >= 0);
    
  // reduce no of items
  m_nItems--;
  
  m_paItem[i_Item] = nullptr;
  
  // last item removed, nothing to do
  if (i_Item == m_nItems)
    return;
  
  // move array up
  char* aSrc = reinterpret_cast<char*>(&m_paItem[i_Item+1]);
  char* aDst = reinterpret_cast<char*>(&m_paItem[i_Item]);
  char* aLst = aAdd<char*>(&m_paItem[m_nItems-1],sizeof(ITEM*));
  i8 nBytes = nDifference(aLst,aDst);
  
  MemLib::vMove(aDst,aSrc,nBytes,CN_NONE);
  
  // clear the last item
  m_paItem[m_nItems] = nullptr;
}

/*----------------------------------------------------------------------------*/
/* remove the given item                                                      */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
void SegmentNL_t<ITEM,MAX_ITEMS>::vRemove(ITEM* pItem)
{
  // check we have items
  ASSERT(m_nItems >= 0);
  
  // find the item
  u8 i_Item;
  
  for (i_Item = 0; i_Item < m_nItems; i_Item++) {
    if (m_paItem[i_Item] == pItem)
      break;
  }

  // item should be there
  ASSERT(pItem == m_paItem[i_Item]);
  
  vRemove(i_Item);
}

/*----------------------------------------------------------------------------*/
/* pop first item                                                             */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
ITEM* SegmentNL_t<ITEM,MAX_ITEMS>::pPop(void)
{
  if (! m_nItems)
    return nullptr;
 
  ITEM* pItem = m_paItem[0];
  
  vRemove((i8)0);
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* set an item                                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
void SegmentNL_t<ITEM,MAX_ITEMS>::vSet(u8 i_Item, ITEM* pItem)
{
  ASSERT(i_Item >= 0 && i_Item < MAX_ITEMS);
  
  // there shouldnt already be a item
  ASSERT( ! m_paItem[i_Item] );
  
  // set item
  m_paItem[i_Item] = pItem;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
