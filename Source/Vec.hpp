/*============================================================================*/
/* Vec.hpp - Vector template                                                  */
/*============================================================================*/
#pragma once

namespace Structures
{  
  template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
  class Vec : public Object
  {
  private:
    i8      m_nItems;           // number of actual items
    Object* m_pItem[MAX_ITEMS]; // vector of items
  
    // must be last
    Footer m_xFooter;

  public:
  
    void* operator new(size_t);
    void  operator delete(void*);

    Vec(void);
    ~Vec(void);
  
    void    vAdd(Object* pObject);
    i8      nItems(void);
    Object* pGet(i8 iItem);
  
    Footer* pFooter(void);
  };
  
  template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
  Vec<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>* _pVec(void* aAddr);
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
Vec<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>* Structures::_pVec(void* aAddr)
{
  Vec<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>* pVec =
    reinterpret_cast<Vec<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>*>(aAddr);
  
  RASSERT( !pVec || ITEM_CN == pVec->eClass() );
  
  return pVec;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
void* Vec<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(VEC_CN);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
void Vec<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>::operator delete(void* pVecAddress)
{
  Vec<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>* pVec =
  //reinterpret_cast<Vec<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>*>(pVecAddress);
    _pVec<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>(pVecAddress);
  
  Heap::vFree(pVec);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
Vec<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>::Vec(void)
: Object(VEC_CN)
{
  _precondition_(_ADDRESS_,_MAGIC_,VEC_CN,_FOOTER_);
  
  m_nItems = 0;
  
  for (i8 iItem = 0; iItem < MAX_ITEMS; iItem++)
    m_pItem[iItem] = nullptr;
  
  _setFooter_();
  
  _checkInitialised_(ITEM_CN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,VEC_CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
Vec<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>::~Vec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,VEC_CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add an item                                                                */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
void Vec<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>::vAdd(Object* pItem)
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
/* get number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
i8 Vec<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,VEC_CN,_FOOTER_);
  
  ASSERT(m_nItems >= 0 && m_nItems < MAX_ITEMS);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
Object* Vec<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>::pGet(i8 iItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,VEC_CN,_FOOTER_);
  
  ASSERT(iItem >= 0 && iItem < m_nItems);
  
  Object* pItem = m_pItem[iItem];
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS>
Footer* Vec<VEC_CN, ITEM, ITEM_CN, MAX_ITEMS>::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,VEC_CN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
