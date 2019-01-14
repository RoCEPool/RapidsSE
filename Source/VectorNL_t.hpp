/*============================================================================*/
/* Vector.hpp - vector template                                               */
/*============================================================================*/
#pragma once

namespace Structures
{  
  template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
  class VectorNL_t : public Object
  {
  //friend class TokenVec;
  
  protected:
    u8     m_nItems;           // number of items currently in the vector
    ITEM*  m_pItem[MAX_ITEMS]; // vector of items (proper objects)
  
    // must be last
    Footer m_xFooter;

  public:
  
    void* operator new(size_t);
    void  operator delete(void*);

    VectorNL_t(void);
    ~VectorNL_t(void) noexcept(false);

    ITEM*& operator [](SUB nItemNo);

    void  vAdd(ITEM* pObject);
    void  vClear(void);
    bool  bContains(ITEM* pObject);
    void  vInsert(ITEM* pObject);
    i8    nItems(void);
    ITEM* pGet(SUB nItemNo);
    void  vRemove(ITEM* pObject);
  
    Footer* aFooter(void);
  };
  
  template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
  VectorNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>* _pVector_NL(void* aAddr);
}

////////////////////////////////////////////////////////////////////////////////

using namespace Structures;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
VectorNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>* Structures::_pVector_NL(void* aAddr)
{
  VectorNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>* pVector =
    reinterpret_cast<VectorNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>*>(aAddr);
  
  RASSERT_TYPE(pVector,VEC_CN);
  
  return pVector;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
void* VectorNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(VEC_CN);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
void VectorNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::operator delete(void* pVectorAddress)
{
  VectorNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>* pVector =
    _pVector_NL<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>(pVectorAddress);
  
  HeapMem::vFree(pVector);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
VectorNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::VectorNL_t(void)
: Object(VEC_CN)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  m_nItems = 0;
  
  SUB nItemNo;
  for (nItemNo = (i8)0; nItemNo < MAX_ITEMS; nItemNo++)
    m_pItem[(i8)nItemNo] = nullptr;
  
  _setFooter_();
  
  // compute size of this vector proper (in case it is inherited)
  i8 nBytes = sizeof(VectorNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>);
  
  _checkInitialisedSize_(nBytes);
  
  _postcondition_(_MAGIC_,VEC_CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
VectorNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::~VectorNL_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  SUB nItemNo;
  for (nItemNo = (i8)0; (i8)nItemNo < (i8)m_nItems; nItemNo++) {
    ITEM* pItem = m_pItem[nItemNo];
    delete pItem;
  }
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* [] operator                                                                */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
ITEM*& VectorNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::operator [](SUB nItemNo)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  //ASSERT(nItemNo >= 0 and nItemNo < MAX_ITEMS);
  
  return m_pItem[nItemNo];
}

/*----------------------------------------------------------------------------*/
/* add an item                                                                */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
void VectorNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::vAdd(ITEM* pItem)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  // check we have a free slot
  ASSERT(m_nItems >= 0 && m_nItems < MAX_ITEMS);
  
  // get item number
  SUB nItemNo = m_nItems++;
  
  ASSERT(m_nItems > 0 && m_nItems <= MAX_ITEMS);
  
  // there shouldnt already be a item
  ASSERT( ! m_pItem[nItemNo] );
  
  // set item
  m_pItem[nItemNo] = pItem;
}

/*----------------------------------------------------------------------------*/
/* clear all items                                                            */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
void VectorNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::vClear(void)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  for (SUB nItemNo = 0; nItemNo < m_nItems; nItemNo++)
    m_pItem[nItemNo] = nullptr;
  
  m_nItems = 0;
}

/*----------------------------------------------------------------------------*/
/* test if the vector contains the given item                                 */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
bool VectorNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::bContains(ITEM* pItem)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  SUB nItemNo;
  
  for (nItemNo = 0; nItemNo < m_nItems; nItemNo++)
    if (m_pItem[nItemNo] == pItem)
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* insert an item as first                                                    */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
void VectorNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::vInsert(ITEM* pItem)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  // check we have a free slot
  ASSERT(m_nItems >= 0 && m_nItems < MAX_ITEMS - 1);
  
  // get item number
  SUB nItemNo = m_nItems++;
  ASSERT(m_nItems > 0 && m_nItems < MAX_ITEMS);
  
  // there shouldnt already be a item
  ASSERT( ! m_pItem[nItemNo] );
  
  // if there are items in the array then move array up
  if (nItemNo) {
    char* aSrc = reinterpret_cast<char*>(&m_pItem[0]);
    char* aDst = reinterpret_cast<char*>(&m_pItem[1]);
    char* aLst = aAdd<char*>(&m_pItem[nItemNo],sizeof(ITEM*));
    i8 nBytes = nSub(aLst,aDst);
    
    MemLib::vMove(aDst,aSrc,nBytes,VEC_CN);
  }
  
  // set first item
  m_pItem[0] = pItem;
}

/*----------------------------------------------------------------------------*/
/* get number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
i8 VectorNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::nItems(void)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  ASSERT(m_nItems >= 0 && m_nItems <= MAX_ITEMS);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
ITEM* VectorNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::pGet(SUB nItemNo)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  ASSERT(nItemNo >= 0 && nItemNo < m_nItems);
  
  ITEM* pItem = m_pItem[nItemNo];
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* remove the given item                                                      */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
void VectorNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::vRemove(ITEM* pItem)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  // check we have items
  ASSERT(m_nItems >= 0);
  
  // find the item
  SUB nItemNo;
  
  for (nItemNo = 0; nItemNo < m_nItems; nItemNo++) {
    if (m_pItem[nItemNo] == pItem)
      break;
  }

  // item should be there
  ASSERT(pItem == m_pItem[nItemNo] );
  
  // reduce no of items
  m_nItems--;
  
  m_pItem[nItemNo] = nullptr;
  
  // last item removed, nothing to do
  if (nItemNo == m_nItems)
    return;
  
  // move array up
  char* aSrc = reinterpret_cast<char*>(&m_pItem[nItemNo+1]);
  char* aDst = reinterpret_cast<char*>(&m_pItem[nItemNo]);
  char* aLst = aAdd<char*>(&m_pItem[m_nItems-1],sizeof(ITEM*));
  i8 nBytes = nSub(aLst,aDst);
  
  MemLib::vMove(aDst,aSrc,nBytes,VEC_CN);
  
  // clear the last item
  m_pItem[m_nItems] = nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
Footer* VectorNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::aFooter(void)
{
  _precondition_(_MAGIC_,VEC_CN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
