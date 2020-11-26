/*============================================================================*/
/* LatticeNL_t.hpp -- Lattice of Pointers template                            */
/*============================================================================*/
#pragma once

namespace Structures
{  
  template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
  class LatticeNL_t : public Object
  {  
  protected:
    u8     m_nItems;             // number of items currently in the lattice
    i8     m_i_Free;             // index of first free
    
    union {
      ITEM*  m_pItem[MAX_ITEMS];      // array of pointers to items (proper objects)
      i8     m_i_NextFree[MAX_ITEMS]; // array of next free item element
    };
    
    // must be last
    Footer m_xFooter;

  public:
  
    void* operator new(size_t);
    void  operator delete(void*);

    LatticeNL_t(void);
    ~LatticeNL_t(void) noexcept(false);

    ITEM*& operator [](i8 i_Item);

    i8    iAdd(ITEM* pItem);
    void  vClear(void);
    ITEM* pGet(i8 i_Item);
    bool  bIsItem(i8 i_Item);
    i8    nItems(void);
    i8    iNext(i8 i_Item);
    void  vRemoveItem(i8 i_Item);
    
  private:
  
    Footer* aFooter(void);
  };
  
  template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
  LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>* _pLatticeNL(void* aAddr);
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>* Structures::_pLatticeNL(void* aAddr)
{
  LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>* pVector =
    reinterpret_cast<LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>*>(aAddr);
  
  RASSERT_TYPE(pVector,LAT_CN);
  
  return pVector;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
void* LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(LAT_CN);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
void LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::operator delete(void* aAddress)
{
  LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>* pLattice =
    _pLatticeNL<LAT_CN,ITEM,MAX_ITEMS>(aAddress);
  
  HeapMem::vFree(pLattice);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::LatticeNL_t(void)
: Object(LAT_CN)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  m_nItems = 0;
  m_i_Free = 0;
  
  for (i8 i_Item = 0; i_Item < MAX_ITEMS; i_Item++)
    m_i_NextFree[i_Item] = i_Item + 1;
  m_i_NextFree[MAX_ITEMS - 1] = C_UNKNOWN;

  _setFooter_();
  
  // compute size of this lattice proper (in case it is inherited)
  i8 nBytes = sizeof(LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>);
  
  _checkInitialisedSize_(nBytes);
  
  _postcondition_(_MAGIC_,LAT_CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::~LatticeNL_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  for (u8 i_Item = (i8)0; (i8)i_Item < (i8)m_nItems; i_Item++) {
    ITEM* pItem = m_pItem[i_Item];
    delete pItem;
  }
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* [] operator                                                                */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
ITEM*& LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::operator [](i8 i_Item)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  
  return m_pItem[i_Item];
}

/*----------------------------------------------------------------------------*/
/* add an item                                                                */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
i8 LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::iAdd(ITEM* pItem)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  ASSERT(m_i_Free >= 0 && m_i_Free < MAX_ITEMS);
  
  // get item number
  i8 i_Item = m_i_Free;

  if (C_UNKNOWN == i_Item)
    return C_UNKNOWN;
  
  m_i_Free = m_i_NextFree[i_Item];
  ASSERT(m_i_Free >= 0 && m_i_Free < MAX_ITEMS);
  
  m_pItem[i_Item] = pItem;
  
  m_nItems++;
  
  return i_Item;
}

/*----------------------------------------------------------------------------*/
/* clear all items                                                            */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
void LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::vClear(void)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  m_nItems = 0;
  m_i_NextFree = 0;
  
  for (i8 i_Item = 0; i_Item < MAX_ITEMS; i_Item++)
    m_i_NextFree[i_Item] = i_Item + 1;
  m_i_NextFree[MAX_ITEMS - 1] = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
ITEM* LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::pGet(i8 i_Item)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  ASSERT(i_Item >= 0 && i_Item < MAX_ITEMS);
  
  ITEM* pItem = m_pItem[i_Item];
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* is elememt an item (or free)                                               */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
bool LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::bIsItem(i8 i_Item)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  if (gpBufferPool->bValidAddress(m_pItem[i_Item]))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
i8 LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::nItems(void)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  ASSERT(m_nItems >= 0 && m_nItems <= MAX_ITEMS);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* get next i_item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
i8 LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::iNext(i8 i_Item)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  i8 i_Next = i_Item + 1;
  
  for (;;) {
  
    if (i_Next >= MAX_ITEMS)
      return C_UNKNOWN;
  
    if (bIsItem(i_Next))
      return i_Next;
    
    i_Next++;
  }
}

/*----------------------------------------------------------------------------*/
/* remove the given item                                                      */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
void LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::vRemoveItem(i8 i_Item)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  if (871 == i_Item)
    vDebugPoint();
  
  ASSERT(i_Item >= 0 && i_Item < MAX_ITEMS);
  ASSERT(m_i_Free >= 0 && m_i_Free < MAX_ITEMS);

  m_i_NextFree[i_Item] = m_i_Free;
  m_i_Free = i_Item;
  ASSERT(m_i_Free >= 0 && m_i_Free < MAX_ITEMS);

  m_nItems--;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
Footer* LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::aFooter(void)
{
  _precondition_(_MAGIC_,LAT_CN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
