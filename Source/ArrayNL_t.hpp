/*============================================================================*/
/* ArrayNL_t.hpp - ArrayNL_t template                                         */
/*============================================================================*/
#pragma once

namespace Structures
{  
  template <eCN CN, class ITEM, i8 MAX_ITEMS>
  class ArrayNL_t : public Object
  {
  private:
    i8    m_nItems;           // number of actual items
    ITEM  m_xItem[MAX_ITEMS]; // array of items (not proper objects)
  
    // must be last
    Footer m_xFooter;

  public:
  
    void* operator new(size_t);
    void* operator new(size_t, void* aAddr);
    void  operator delete(void*);

    ArrayNL_t(void);
    ~ArrayNL_t(void) noexcept(false);
  
    ITEM& operator [](i8 iItemNo);
  
    void  vAdd(ITEM pObject);
    void  vClear(void);
    void  vInsert(ITEM pObject);
    i8    nItems(void);
    void  vItems(i8 nItems);
    ITEM  xGet(i8 iItem);
    i8    iMaxItems(void);
  
    Footer* aFooter(void);
  };
  
  template <eCN CN, class ITEM, i8 MAX_ITEMS>
  ArrayNL_t<CN, ITEM, MAX_ITEMS>* _pArrayNL_t(void* aAddr);
}

using namespace Structures;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, i8 MAX_ITEMS>
ArrayNL_t<CN, ITEM, MAX_ITEMS>* Structures::_pArrayNL_t(void* aAddr)
{
  ArrayNL_t<CN, ITEM, MAX_ITEMS>* pArrayNL_t =
    reinterpret_cast<ArrayNL_t<CN, ITEM, MAX_ITEMS>*>(aAddr);
  
  RASSERT_TYPE(pArrayNL_t,CN);
  
  return pArrayNL_t;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, i8 MAX_ITEMS>
void* ArrayNL_t<CN, ITEM, MAX_ITEMS>::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, i8 MAX_ITEMS>
void* ArrayNL_t<CN, ITEM, MAX_ITEMS>::operator new(size_t, void* aAddr)
{
  return aAddr;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, i8 MAX_ITEMS>
void ArrayNL_t<CN, ITEM, MAX_ITEMS>::operator delete(void* pArrayNL_tAddress)
{
  ArrayNL_t<CN, ITEM, MAX_ITEMS>* pArrayNL_t =
  //reinterpret_cast<ArrayNL_t<CN, ITEM, MAX_ITEMS>*>(pVecAddress);
    _pArrayNL_t<CN, ITEM, MAX_ITEMS>(pArrayNL_tAddress);
  
  HeapMem::vFree(pArrayNL_t);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, i8 MAX_ITEMS>
ArrayNL_t<CN, ITEM, MAX_ITEMS>::ArrayNL_t(void)
: Object(CN)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_nItems = 0;
  
  for (i8 i_Item = 0; i_Item < MAX_ITEMS; i_Item++)
    m_xItem[i_Item] = (ITEM)0;
  
  _setFooter_();
  
  _checkInitialised_(CN);
  
  _postcondition_(_MAGIC_,CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, i8 MAX_ITEMS>
ArrayNL_t<CN, ITEM, MAX_ITEMS>::~ArrayNL_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
//  for (i8 i_Item = 0; i_Item < m_nItems; i_Item++) {
//    ITEM xItem = m_xItem[i_Item];
//    delete xItem;
//  }
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* [] operator                                                                */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, i8 MAX_ITEMS>
ITEM& ArrayNL_t<CN, ITEM, MAX_ITEMS>::operator [](i8 iItemNo)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  ASSERT(iItemNo >= 0 and iItemNo < MAX_ITEMS);
  
  return m_xItem[iItemNo];
}

/*----------------------------------------------------------------------------*/
/* add an item                                                                */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, i8 MAX_ITEMS>
void ArrayNL_t<CN, ITEM, MAX_ITEMS>::vAdd(ITEM xItem)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  // check we have a free slot
  ASSERT(m_nItems >= 0 && m_nItems < MAX_ITEMS - 1);
  
  // get item number
  i8 iItem = m_nItems++;
  ASSERT(m_nItems > 0 && m_nItems < MAX_ITEMS);
  
  // there shouldnt already be a item
  ASSERT( ! m_xItem[iItem] );
  
  // set item
  m_xItem[iItem] = xItem;
}

/*----------------------------------------------------------------------------*/
/* clear all items                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, i8 MAX_ITEMS>
void ArrayNL_t<CN, ITEM, MAX_ITEMS>::vClear(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  for (i8 i_Item = 0; i_Item < m_nItems; i_Item++) {
    m_xItem[i_Item] = (ITEM)0;
  }
    
  m_nItems = 0;
}

/*----------------------------------------------------------------------------*/
/* insert an item as first                                                    */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, i8 MAX_ITEMS>
void ArrayNL_t<CN, ITEM, MAX_ITEMS>::vInsert(ITEM xItem)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  // check we have a free slot
  ASSERT(m_nItems >= 0 && m_nItems < MAX_ITEMS - 1);
  
  // get item number
  i8 iItem = m_nItems++;
  ASSERT(m_nItems > 0 && m_nItems < MAX_ITEMS);
  
  // there shouldnt already be a item
  ASSERT( ! m_xItem[iItem] );
  
  // if there are items in the array then move array up
  if (iItem) {
    char* aSrc = reinterpret_cast<char*>(&m_xItem[0]);
    char* aDst = reinterpret_cast<char*>(&m_xItem[1]);
    char* aLst = pAdd<char*>(&m_xItem[iItem],sizeof(ITEM));
    i8 nBytes = nSub(aLst,aDst);
    
    MemLib::vMove(aDst,aSrc,nBytes,CN);
  }
  
  // set item
  m_xItem[0] = xItem;
}

/*----------------------------------------------------------------------------*/
/* get number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, i8 MAX_ITEMS>
i8 ArrayNL_t<CN, ITEM, MAX_ITEMS>::nItems(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  ASSERT(m_nItems >= 0 && m_nItems < MAX_ITEMS);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* set number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, i8 MAX_ITEMS>
void ArrayNL_t<CN, ITEM, MAX_ITEMS>::vItems(i8 nItems)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  ASSERT(nItems >= 0 && nItems < MAX_ITEMS);
  
  m_nItems = nItems;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, i8 MAX_ITEMS>
ITEM ArrayNL_t<CN, ITEM, MAX_ITEMS>::xGet(i8 iItem)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  ASSERT(iItem >= 0 && iItem < m_nItems);
  
  ITEM xItem = m_xItem[iItem];
  
  return xItem;
}

/*----------------------------------------------------------------------------*/
/* get maximum number of items                                                */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, i8 MAX_ITEMS>
i8 ArrayNL_t<CN, ITEM, MAX_ITEMS>::iMaxItems(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return MAX_ITEMS;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, i8 MAX_ITEMS>
Footer* ArrayNL_t<CN, ITEM, MAX_ITEMS>::aFooter(void)
{
  _precondition_(_MAGIC_,CN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
