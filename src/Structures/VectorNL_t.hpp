/*============================================================================*/
/* VectorNL_t.hpp -- Vector of Pointers template                              */
/*============================================================================*/
#pragma once

namespace Structures
{  
  template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
  class VectorNL_t : public Object
  {
  private:
    u8     m_nItems;           // number of items currently in the vector
    ITEM*  m_pItem[MAX_ITEMS]; // vector of items (proper objects)
  
    // must be last
    Footer m_xFooter;

  public:
  
    void* operator new(size_t);
    void  operator delete(void*);

    VectorNL_t(void);
    ~VectorNL_t(void) noexcept(false);

    //ITEM*& operator [](u8 i_Item);

    void  vAdd(ITEM* pObject);
    void  vClear(void);
    bool  bContains(ITEM* pObject);
    void  vDecItems(void);
    void  vIncItems(void);
    void  vInsert(ITEM* pObject);
    i8    nItems(void);
    void  vItems(i8 nItems);
    ITEM* pGet(u8 i_Item, bool bDisableBounds=false);
    void  vRemove(ITEM* pObject);
    void  vSet(u8 i_Item, ITEM* pObject);

    Footer* aFooter(void);
  };
  
  template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
  VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>* _pVector_NL(void* aAddr);
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>* Structures::_pVector_NL(void* aAddr)
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
  void* pObject = HeapMem::pAllocate(VEC_CN);
  
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
  
  HeapMem::vFree(pVector);
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
  
  for (u8 i_Item = (i8)0; i_Item < MAX_ITEMS; i_Item++)
    m_pItem[(i8)i_Item] = nullptr;
  
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
  
  for (u8 i_Item = (i8)0; (i8)i_Item < (i8)m_nItems; i_Item++) {
    ITEM* pItem = m_pItem[i_Item];
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
//ITEM*& VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::operator [](u8 i_Item)
//{
//  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
//  
//  //ASSERT(nItemNo >= 0 and nItemNo < MAX_ITEMS);
//  
//  return m_pItem[i_Item];
//}

/*----------------------------------------------------------------------------*/
/* add an item                                                                */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
void VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::vAdd(ITEM* pItem)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  // check we have a free slot
  ASSERT(m_nItems >= 0 && m_nItems < MAX_ITEMS);
  
  // get item number
  u8 i_Item = m_nItems++;
  
  ASSERT(m_nItems > 0 && m_nItems <= MAX_ITEMS);
  
  // there shouldnt already be a item
  ASSERT( ! m_pItem[i_Item] );
  
  // set item
  m_pItem[i_Item] = pItem;
}

/*----------------------------------------------------------------------------*/
/* clear all items                                                            */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
void VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::vClear(void)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  for (u8 i_Item = 0; i_Item < m_nItems; i_Item++)
    m_pItem[i_Item] = nullptr;
  
  m_nItems = 0;
}

/*----------------------------------------------------------------------------*/
/* test if the vector contains the given item                                 */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
bool VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::bContains(ITEM* pItem)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  for (u8 i_Item = 0; i_Item < m_nItems; i_Item++)
    if (m_pItem[i_Item] == pItem)
      return true;

  return false;
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
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
ITEM* VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::pGet(u8 i_Item, bool bDisableBounds)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  ASSERT(bDisableBounds || (i_Item >= 0 && i_Item < m_nItems));
  
  ITEM* pItem = m_pItem[i_Item];
  
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
/* insert an item as first                                                    */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
void VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::vInsert(ITEM* pItem)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  // check we have a free slot
  ASSERT(m_nItems >= 0 && m_nItems < MAX_ITEMS - 1);
  
  // get item number
  u8 i_Item = m_nItems++;
  ASSERT(m_nItems > 0 && m_nItems < MAX_ITEMS);
  
  // there shouldnt already be a item
  ASSERT( ! m_pItem[i_Item] );
  
  // if there are items in the array then move array up
  if (i_Item) {
    char* aSrc = reinterpret_cast<char*>(&m_pItem[0]);
    char* aDst = reinterpret_cast<char*>(&m_pItem[1]);
    char* aLst = aAdd<char*>(&m_pItem[i_Item],sizeof(ITEM*));
    i8 nBytes = nDifference(aLst,aDst);
    
    MemLib::vMove(aDst,aSrc,nBytes,VEC_CN);
  }
  
  // set first item
  m_pItem[0] = pItem;
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
/* remove the given item                                                      */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
void VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::vRemove(ITEM* pItem)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  // check we have items
  ASSERT(m_nItems >= 0);
  
  // find the item
  u8 i_Item;
  
  for (i_Item = 0; i_Item < m_nItems; i_Item++) {
    if (m_pItem[i_Item] == pItem)
      break;
  }

  // item should be there
  ASSERT(pItem == m_pItem[i_Item] );
  
  // reduce no of items
  m_nItems--;
  
  m_pItem[i_Item] = nullptr;
  
  // last item removed, nothing to do
  if (i_Item == m_nItems)
    return;
  
  // move array up
  char* aSrc = reinterpret_cast<char*>(&m_pItem[i_Item+1]);
  char* aDst = reinterpret_cast<char*>(&m_pItem[i_Item]);
  char* aLst = aAdd<char*>(&m_pItem[m_nItems-1],sizeof(ITEM*));
  i8 nBytes = nDifference(aLst,aDst);
  
  MemLib::vMove(aDst,aSrc,nBytes,VEC_CN);
  
  // clear the last item
  m_pItem[m_nItems] = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set an item                                                                */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, i8 MAX_ITEMS>
void VectorNL_t<VEC_CN,ITEM,MAX_ITEMS>::vSet(u8 i_Item, ITEM* pItem)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
    
  ASSERT(i_Item >= 0 && i_Item < MAX_ITEMS);
  
  // there shouldnt already be a item
  ASSERT( ! m_pItem[i_Item] );
  
  // set item
  m_pItem[i_Item] = pItem;
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
