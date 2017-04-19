/*============================================================================*/
/* Vectortor.hpp - Vectortor template                                               */
/*============================================================================*/
#pragma once

namespace Structures
{  
  template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
  class Vector : public Object
  {
  private:
    i8     m_nItems;           // number of actual items
    ITEM*  m_pItem[MAX_ITEMS]; // vector of items
  
    // must be last
    Footer m_xFooter;

  public:
  
    void* operator new(size_t);
    void  operator delete(void*);

    Vector(void);
    ~Vector(void);
  
    void  vAdd(ITEM* pObject);
    void  vClear(void);
    void  vInsert(ITEM* pObject);
    i8    nItems(void);
    ITEM* pGet(i8 iItem);
    void  vRemove(ITEM* pObject);
  
    Footer* pFooter(void);
  };
  
  template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
  Vector<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>* _pVector(void* aAddr);
}

////////////////////////////////////////////////////////////////////////////////

using namespace Structures;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
Vector<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>* Structures::_pVector(void* aAddr)
{
  Vector<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>* pVector =
    reinterpret_cast<Vector<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>*>(aAddr);
  
  RASSERT( !pVector || VEC_CN == pVector->eClass() );
  
  return pVector;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
void* Vector<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(VEC_CN);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
void Vector<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>::operator delete(void* pVectorAddress)
{
  Vector<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>* pVector =
  //reinterpret_cast<Vector<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>*>(pVectorAddress);
    _pVector<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>(pVectorAddress);
  
  Heap::vFree(pVector);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
Vector<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>::Vector(void)
: Object(VEC_CN)
{
  _precondition_(_ADDRESS_,_MAGIC_,VEC_CN,_FOOTER_);
  
  m_nItems = 0;
  
  for (i8 iItem = 0; iItem < MAX_ITEMS; iItem++)
    m_pItem[iItem] = nullptr;
  
  _setFooter_();
  
  _checkInitialised_(VEC_CN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,VEC_CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
Vector<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>::~Vector(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,VEC_CN,_FOOTER_);
  
  for (i8 iItem = 0; iItem < m_nItems; iItem++) {
    ITEM* pItem = m_pItem[iItem];
    delete pItem;
  }
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add an item                                                                */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
void Vector<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>::vAdd(ITEM* pItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,VEC_CN,_FOOTER_);
  
  // check we have a free slot
  ASSERT(m_nItems >= 0 && m_nItems < MAX_ITEMS - 1);
  
  // get item number
  i8 iItem = m_nItems++;
  ASSERT(m_nItems > 0 && m_nItems < MAX_ITEMS);
  
  // there shouldnt already be a item
  ASSERT( ! m_pItem[iItem] );
  
  // set item
  m_pItem[iItem] = pItem;
}

/*----------------------------------------------------------------------------*/
/* clear all items                                                            */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
void Vector<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>::vClear(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,VEC_CN,_FOOTER_);
  
  m_nItems = 0;
}

/*----------------------------------------------------------------------------*/
/* insert an item as first                                                    */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
void Vector<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>::vInsert(ITEM* pItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,VEC_CN,_FOOTER_);
  
  // check we have a free slot
  ASSERT(m_nItems >= 0 && m_nItems < MAX_ITEMS - 1);
  
  // get item number
  i8 iItem = m_nItems++;
  ASSERT(m_nItems > 0 && m_nItems < MAX_ITEMS);
  
  // there shouldnt already be a item
  ASSERT( ! m_pItem[iItem] );
  
  // if there are items in the array then move array up
  if (iItem) {
    char* aSrc = reinterpret_cast<char*>(&m_pItem[0]);
    char* aDst = reinterpret_cast<char*>(&m_pItem[1]);
    char* aLst = pAdd<char*>(&m_pItem[iItem],sizeof(ITEM*));
    i8 nBytes = nSub(aLst,aDst);
    
    MemLib::vMove(aDst,aSrc,nBytes);
  }
  
  // set item
  m_pItem[0] = pItem;
}

/*----------------------------------------------------------------------------*/
/* get number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
i8 Vector<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,VEC_CN,_FOOTER_);
  
  ASSERT(m_nItems >= 0 && m_nItems < MAX_ITEMS);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
ITEM* Vector<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>::pGet(i8 iItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,VEC_CN,_FOOTER_);
  
  ASSERT(iItem >= 0 && iItem < m_nItems);
  
  ITEM* pItem = m_pItem[iItem];
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* remove an item                                                             */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
void Vector<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>::vRemove(ITEM* pItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,VEC_CN,_FOOTER_);
  
  // check we have items
  ASSERT(m_nItems >= 0);
  
  // find the item
  i8 iItem;
  
  for (iItem=0; iItem < m_nItems; iItem++) {
    if (m_pItem[iItem] == pItem)
      break;
  }

  // item should be there
  ASSERT(m_pItem[iItem] );
  
  // reduce no of items
  m_nItems--;
  
  // last item removed, nothing to do
  if (iItem == m_nItems)
    return;
  
  // move array up
  char* aSrc = reinterpret_cast<char*>(&m_pItem[iItem+1]);
  char* aDst = reinterpret_cast<char*>(&m_pItem[iItem]);
  char* aLst = pAdd<char*>(&m_pItem[m_nItems-1],sizeof(ITEM*));
  i8 nBytes = nSub(aLst,aDst);
  
  MemLib::vMove(aDst,aSrc,nBytes);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
Footer* Vector<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,VEC_CN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
