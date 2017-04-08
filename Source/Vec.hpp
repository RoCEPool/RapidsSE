/*============================================================================*/
/* Vec.hpp - Vector template                                                  */
/*============================================================================*/
#pragma once

namespace Structures
{  
  template <class OBJECT, eCN CLASSNO, i8 ITEMS>
  class Vec : public Object
  {
  private:
    i8      m_nItems;       // number of actual items
    Object* m_pItem[ITEMS]; // vector of items
  
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
  
  template <class OBJECT, eCN CLASSNO, i8 ITEMS>
  Vec<OBJECT, CLASSNO, ITEMS>* _pVec(void* aAddr);
}

////////////////////////////////////////////////////////////////////////////////

using namespace Structures;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
template <class OBJECT, eCN CLASSNO, i8 ITEMS>
Vec<OBJECT, CLASSNO, ITEMS>* Structures::_pVec(void* aAddr)
{
  Vec<OBJECT, CLASSNO, ITEMS>* pVec =
    reinterpret_cast<Vec<OBJECT, CLASSNO, ITEMS>*>(aAddr);
  
  RASSERT( !pVec || CLASSNO == pVec->eClass() );
  
  return pVec;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <class OBJECT, eCN CLASSNO, i8 ITEMS>
void* Vec<OBJECT, CLASSNO, ITEMS>::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CLASSNO);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <class OBJECT, eCN CLASSNO, i8 ITEMS>
void Vec<OBJECT, CLASSNO, ITEMS>::operator delete(void* pVecAddress)
{
  Vec<OBJECT, CLASSNO, ITEMS>* pVec =
  //reinterpret_cast<Vec<OBJECT, CLASSNO, ITEMS>*>(pVecAddress);
    _pVec<OBJECT, CLASSNO, ITEMS>(pVecAddress);
  
  Heap::vFree(pVec);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class OBJECT, eCN CLASSNO, i8 ITEMS>
Vec<OBJECT, CLASSNO, ITEMS>::Vec(void)
: Object(CLASSNO)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
  m_nItems = 0;
  
  for (i8 iItem = 0; iItem < ITEMS; iItem++)
    m_pItem[iItem] = nullptr;
  
  _setFooter_();
  
  _checkInitialised_(CLASSNO);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <class OBJECT, eCN CLASSNO, i8 ITEMS>
Vec<OBJECT, CLASSNO, ITEMS>::~Vec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add an item                                                                */
/*----------------------------------------------------------------------------*/
template <class OBJECT, eCN CLASSNO, i8 ITEMS>
void Vec<OBJECT, CLASSNO, ITEMS>::vAdd(Object* pItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
  // check we have a free slot
  ASSERT(m_nItems >= 0 && m_nItems < ITEMS - 1);
  
  // get item number
  i8 iItem = m_nItems++;
  ASSERT(m_nItems > 0 && m_nItems < ITEMS);
  
  // there shouldnt already be a item
  ASSERT( ! m_pItem[iItem] );
  
  // set item
  m_pItem[iItem] = pItem;
}

/*----------------------------------------------------------------------------*/
/* get number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <class OBJECT, eCN CLASSNO, i8 ITEMS>
i8 Vec<OBJECT, CLASSNO, ITEMS>::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
  ASSERT(m_nItems >= 0 && m_nItems < ITEMS);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <class OBJECT, eCN CLASSNO, i8 ITEMS>
Object* Vec<OBJECT, CLASSNO, ITEMS>::pGet(i8 iItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
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
template <class OBJECT, eCN CLASSNO, i8 ITEMS>
Footer* Vec<OBJECT, CLASSNO, ITEMS>::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
