/*============================================================================*/
/* VectorNL_t.hpp -- Vector of Pointers template                              */
/*============================================================================*/
#pragma once

namespace BASE
{  
  template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
  class VectorNL_t : public Object
  {
  private:
    i8     m_nItems;           // number of items currently in the vector
    ITEM*  m_paItem[MAX_ITEMS]; // array of items (proper objects)
  
    // must be last
    Footer m_xFooter;

  public:
  
    void* operator new(size_t);
    void  operator delete(void*);

    VectorNL_t(void);
   ~VectorNL_t(void) noexcept(false);

    //ITEM*& operator [](i8 i_Item);

    void  vAdd(ITEM* pItem);
    void  vAppend(ITEM* pItem);
    void  vClear(void);
    bool  bContains(ITEM* pItem);
    void  vCut(i8 i_Item);
    void  vDecItems(void);
    i8    iFind(ITEM* pItem);
    ITEM* pGet(i8 i_Item, bool bDisableBounds=false);
    void  vIncItems(void);
    i8    nItems(void);
    void  vItems(i8 nItems);
    ITEM* pPop(void);
    void  vRemove(ITEM* pItem);
    void  vSet(i8 i_Item, ITEM* pItem);

  private:
  
    Footer* aFooter(void);
  };
  
  template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
  VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>* _pVector_NL(void* aAddr);
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>* BASE::_pVector_NL(void* aAddr)
{
  VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>* pVector =
    reinterpret_cast<VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>*>(aAddr);
  
  RASSERT_TYPE(pVector,VEC_CN);
  
  return pVector;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
void* VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(VEC_CN);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
void VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::operator delete(void* pVectorAddress)
{
  VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>* pVector =
    _pVector_NL<VEC_CN,ITEM,MAX_ITEMS>(pVectorAddress);
  
  Heaps::vFree(pVector);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::VectorNL_t(void)
: Object(VEC_CN)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  m_nItems = 0;
  
  for (i8 i_Item = (i8)0; i_Item < MAX_ITEMS; i_Item++)
    m_paItem[(i8)i_Item] = nullptr;
  
  _setFooter_();
  
  // compute size of this vector proper (in case it is inherited)
  i8 nBytes = sizeof(VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>);
  
  _checkInitialisedSize_(nBytes);
  
  _postcondition_(_MAGIC_,VEC_CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::~VectorNL_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  for (i8 i_Item = (i8)0; (i8)i_Item < (i8)m_nItems; i_Item++) {
    ITEM* pItem = m_paItem[i_Item];
    delete pItem;
  }
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* subscript [] operator                                                      */
/*----------------------------------------------------------------------------*/
//template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
//ITEM*& VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::operator [](i8 i_Item)
//{
//  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
//  
//  //ASSERT(nItemNo >= 0 and nItemNo < MAX_ITEMS);
//  
//  return m_paItem[i_Item];
//}

/*----------------------------------------------------------------------------*/
/* add item as first item in vector                                           */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
void VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::vAdd(ITEM* pItem)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  // check we have a free slot
  ASSERT(m_nItems >= 0 && m_nItems < MAX_ITEMS - 1);
  
  // get item number
  i8 i_Item = m_nItems++;
  ASSERT(m_nItems > 0 && m_nItems < MAX_ITEMS);
  
  // there shouldnt already be a item
  ASSERT( ! m_paItem[i_Item] );
  
  // if there are items in the array then move array up
  if (i_Item) {
    char* aSrc = reinterpret_cast<char*>(&m_paItem[0]);
    char* aDst = reinterpret_cast<char*>(&m_paItem[1]);
    char* aLst = aAdd<char*>(&m_paItem[i_Item],sizeof(ITEM*));
    i8 nBytes = nDifference(aLst,aDst);
    
    MemLib::vMove(aDst,aSrc,nBytes,VEC_CN);
  }
  
  // set first item
  m_paItem[0] = pItem;
}

/*----------------------------------------------------------------------------*/
/* append item to end of vector                                               */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
void VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::vAppend(ITEM* pItem)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  // check we have a free slot
  ASSERT(m_nItems >= 0 && m_nItems < MAX_ITEMS);
  
  // get item number
  i8 i_Item = m_nItems++;
  
  ASSERT(m_nItems > 0 && m_nItems <= MAX_ITEMS);
  
  // there shouldnt already be a item
  ASSERT( ! m_paItem[i_Item] );
  
  // set item
  m_paItem[i_Item] = pItem;
}

/*----------------------------------------------------------------------------*/
/* clear all items                                                            */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
void VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::vClear(void)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  for (i8 i_Item = 0; i_Item < m_nItems; i_Item++)
    m_paItem[i_Item] = nullptr;
  
  m_nItems = 0;
}

/*----------------------------------------------------------------------------*/
/* test if the vector contains the given item                                 */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
bool VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::bContains(ITEM* pItem)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  for (i8 i_Item = 0; i_Item < m_nItems; i_Item++)
    if (m_paItem[i_Item] == pItem)
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* cut item at given subscript                                                */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
void VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::vCut(i8 i_Item)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  if (i_Item < 0 || i_Item > 0)
    return;
  
  // reduce no of items
  m_nItems--;
  
  // clear the cut item
  m_paItem[i_Item] = nullptr;
  
  // last item removed, nothing to do
  if (i_Item == m_nItems)
    return;
  
  // move array up
  char* aSrc = reinterpret_cast<char*>(&m_paItem[i_Item+1]);
  char* aDst = reinterpret_cast<char*>(&m_paItem[i_Item]);
  char* aLst = aAdd<char*>(&m_paItem[m_nItems-1],sizeof(ITEM*));
  i8 nBytes = nDifference(aLst,aDst);
  
  MemLib::vMove(aDst,aSrc,nBytes,VEC_CN);
  
  // clear what was the last item
  m_paItem[m_nItems] = nullptr;
}

/*----------------------------------------------------------------------------*/
/* decrement the number of items                                              */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
void VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::vDecItems(void)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);

  m_nItems--;
}

/*----------------------------------------------------------------------------*/
/* find item                                                                  */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
i8 VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::iFind(ITEM* pItem)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  // find the item
  i8 i_Item;
  
  for (i_Item = 0; i_Item < m_nItems; i_Item++) {
    if (m_paItem[i_Item] == pItem)
      break;
  }

  if (i_Item >= m_nItems)
    return C_UNKNOWN;
    
  return i_Item;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
ITEM* VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::pGet(i8 i_Item, bool bDisableBounds)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  ASSERT(bDisableBounds || (i_Item >= 0 && i_Item < m_nItems));
  UNUSED(bDisableBounds);
 
  ITEM* pItem = m_paItem[i_Item];
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* increment the number of items                                              */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
void VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::vIncItems(void)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);

  m_nItems++;
}

/*----------------------------------------------------------------------------*/
/* get number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
i8 VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::nItems(void)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  ASSERT(m_nItems >= 0 && m_nItems <= MAX_ITEMS);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* set number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
void VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::vItems(i8 nItems)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  ASSERT(m_nItems >= 0 && m_nItems <= MAX_ITEMS);
  
  m_nItems = nItems;
}

/*----------------------------------------------------------------------------*/
/* pop first item                                                             */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
ITEM* VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::pPop(void)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  if (! m_nItems)
    return nullptr;
  
  ITEM* pItem = m_paItem[0];

  vRemove(pItem);
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* remove the given item                                                      */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
void VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::vRemove(ITEM* pItem)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
//  // check we have items
//  ASSERT(m_nItems >= 0);
//
//  // find the item
//  i8 i_Item;
//
//  for (i_Item = 0; i_Item < m_nItems; i_Item++) {
//    if (m_paItem[i_Item] == pItem)
//      break;
//  }

  // find the item
  i8 i_Item = iFind(pItem);
  if (C_UNKNOWN == i_Item)
    return;

  // item should be there
  ASSERT(pItem == m_paItem[i_Item]);
  
//  // reduce no of items
//  m_nItems--;
//
//  m_paItem[i_Item] = nullptr;
//
//  // last item removed, nothing to do
//  if (i_Item == m_nItems)
//    return;
//
//  // move array up
//  char* aSrc = reinterpret_cast<char*>(&m_paItem[i_Item+1]);
//  char* aDst = reinterpret_cast<char*>(&m_paItem[i_Item]);
//  char* aLst = aAdd<char*>(&m_paItem[m_nItems-1],sizeof(ITEM*));
//  i8 nBytes = nDifference(aLst,aDst);
//
//  MemLib::vMove(aDst,aSrc,nBytes,VEC_CN);
//
//  // clear what was the last item
//  m_paItem[m_nItems] = nullptr;
  
  vCut(i_Item);
}

/*----------------------------------------------------------------------------*/
/* set an item                                                                */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
void VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::vSet(i8 i_Item, ITEM* pItem)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
    
  ASSERT(i_Item >= 0 && i_Item < MAX_ITEMS);
  
  // there shouldnt already be a item
  ASSERT( ! m_paItem[i_Item] );
  
  // set item
  m_paItem[i_Item] = pItem;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
Footer* VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::aFooter(void)
{
  _precondition_(_MAGIC_,VEC_CN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
