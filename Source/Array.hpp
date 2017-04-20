/*============================================================================*/
/* Array.hpp - Array template                                                 */
/*============================================================================*/
#pragma once

namespace Structures
{  
  template <eCN ARRAY_CN, class ITEM, i8 MAX_ITEMS>
  class Array : public Object
  {
  private:
    i8    m_nItems;           // number of actual items
    ITEM  m_xItem[MAX_ITEMS]; // array of items (not proper objects)
  
    // must be last
    Footer m_xFooter;

  public:
  
    void* operator new(size_t);
    void  operator delete(void*);

    Array(void);
    ~Array(void);
  
    void  vAdd(ITEM pObject);
    void  vClear(void);
    void  vInsert(ITEM pObject);
    i8    nItems(void);
    ITEM  xGet(i8 iItem);
  
    Footer* pFooter(void);
  };
  
  template <eCN ARRAY_CN, class ITEM, i8 MAX_ITEMS>
  Array<ARRAY_CN, ITEM, MAX_ITEMS>* _pArray(void* aAddr);
}

////////////////////////////////////////////////////////////////////////////////

using namespace Structures;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
template <eCN ARRAY_CN, class ITEM, i8 MAX_ITEMS>
Array<ARRAY_CN, ITEM, MAX_ITEMS>* Structures::_pArray(void* aAddr)
{
  Array<ARRAY_CN, ITEM, MAX_ITEMS>* pArray =
    reinterpret_cast<Array<ARRAY_CN, ITEM, MAX_ITEMS>*>(aAddr);
  
  RASSERT( !pArray || ARRAY_CN == pArray->eClass() );
  
  return pArray;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN ARRAY_CN, class ITEM, i8 MAX_ITEMS>
void* Array<ARRAY_CN, ITEM, MAX_ITEMS>::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(ARRAY_CN);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN ARRAY_CN, class ITEM, i8 MAX_ITEMS>
void Array<ARRAY_CN, ITEM, MAX_ITEMS>::operator delete(void* pArrayAddress)
{
  Array<ARRAY_CN, ITEM, MAX_ITEMS>* pArray =
  //reinterpret_cast<Array<ARRAY_CN, ITEM, MAX_ITEMS>*>(pVecAddress);
    _pArray<ARRAY_CN, ITEM, MAX_ITEMS>(pArrayAddress);
  
  Heap::vFree(pArray);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN ARRAY_CN, class ITEM, i8 MAX_ITEMS>
Array<ARRAY_CN, ITEM, MAX_ITEMS>::Array(void)
: Object(ARRAY_CN)
{
  _precondition_(_ADDRESS_,_MAGIC_,ARRAY_CN,_FOOTER_);
  
  m_nItems = 0;
  
  for (i8 iItem = 0; iItem < MAX_ITEMS; iItem++)
    m_xItem[iItem] = (ITEM)0;
  
  _setFooter_();
  
  _checkInitialised_(ARRAY_CN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,ARRAY_CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN ARRAY_CN, class ITEM, i8 MAX_ITEMS>
Array<ARRAY_CN, ITEM, MAX_ITEMS>::~Array(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,ARRAY_CN,_FOOTER_);
  
//  for (i8 iItem = 0; iItem < m_nItems; iItem++) {
//    ITEM xItem = m_xItem[iItem];
//    delete xItem;
//  }
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add an item                                                                */
/*----------------------------------------------------------------------------*/
template <eCN ARRAY_CN, class ITEM, i8 MAX_ITEMS>
void Array<ARRAY_CN, ITEM, MAX_ITEMS>::vAdd(ITEM xItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,ARRAY_CN,_FOOTER_);
  
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
template <eCN ARRAY_CN, class ITEM, i8 MAX_ITEMS>
void Array<ARRAY_CN, ITEM, MAX_ITEMS>::vClear(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,ARRAY_CN,_FOOTER_);
  
  for (i8 iItem = 0; iItem < m_nItems; iItem++) {
    m_xItem[iItem] = (ITEM)0;
  }
    
  m_nItems = 0;
}

/*----------------------------------------------------------------------------*/
/* insert an item as first                                                    */
/*----------------------------------------------------------------------------*/
template <eCN ARRAY_CN, class ITEM, i8 MAX_ITEMS>
void Array<ARRAY_CN, ITEM, MAX_ITEMS>::vInsert(ITEM xItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,ARRAY_CN,_FOOTER_);
  
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
    
    MemLib::vMove(aDst,aSrc,nBytes);
  }
  
  // set item
  m_xItem[0] = xItem;
}

/*----------------------------------------------------------------------------*/
/* get number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <eCN ARRAY_CN, class ITEM, i8 MAX_ITEMS>
i8 Array<ARRAY_CN, ITEM, MAX_ITEMS>::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,ARRAY_CN,_FOOTER_);
  
  ASSERT(m_nItems >= 0 && m_nItems < MAX_ITEMS);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN ARRAY_CN, class ITEM, i8 MAX_ITEMS>
ITEM Array<ARRAY_CN, ITEM, MAX_ITEMS>::xGet(i8 iItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,ARRAY_CN,_FOOTER_);
  
  ASSERT(iItem >= 0 && iItem < m_nItems);
  
  ITEM xItem = m_xItem[iItem];
  
  return xItem;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN ARRAY_CN, class ITEM, i8 MAX_ITEMS>
Footer* Array<ARRAY_CN, ITEM, MAX_ITEMS>::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,ARRAY_CN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
