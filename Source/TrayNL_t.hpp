/*============================================================================*/
/* TrayNL_t.hpp - double-headed vector template                             */
/*============================================================================*/
#pragma once

namespace Structures
{  
  template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
  class TrayNL_t : public Object
  {
  protected:
    u8     m_nHeadItems;       // number of items in vec from the front
    u8     m_nTailItems;       // number of items currently in the vector
    ITEM*  m_pItem[MAX_ITEMS]; // vector of items

    // must be last
    Footer m_xFooter;

  public:
  
    void* operator new(size_t);
    void  operator delete(void*);

    TrayNL_t(void);
    ~TrayNL_t(void) noexcept(false);

    ITEM*& operator [](SUB nItemNo);

    i8    iAllotHead(void);
    i8    iAllotTail(void);

    void  vClear(void);
    i8    nHeads(void);
    i8    nItems(void);
    ITEM* aGet(SUB nItemNo);
    void  vReset(void);
    i8    nTails(void);

  private:
    Footer* aFooter(void);
  };
  
  template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
  TrayNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>* _pTrayNL(void* aAddr);
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
TrayNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>* Structures::_pTrayNL(void* aAddr)
{
  TrayNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>* pVector =
    reinterpret_cast<TrayNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>*>(aAddr);
  
  RASSERT_TYPE(pVector,VEC_CN);
  
  return pVector;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
void* TrayNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(VEC_CN);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
void TrayNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::operator delete(void* pVectorAddress)
{
  TrayNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>* pVector =
    _pTrayNL<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>(pVectorAddress);
  
  HeapMem::vFree(pVector);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
TrayNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::TrayNL_t(void)
: Object(VEC_CN)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  m_nHeadItems = 0;
  
  SUB nItemNo;
  for (nItemNo = (i8)0; nItemNo < MAX_ITEMS; nItemNo++)
    m_pItem[(i8)nItemNo] = nullptr;
  
  _setFooter_();
  
  // compute size of this vector proper (in case it is inherited)
  i8 nBytes = sizeof(TrayNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>);
  
  _checkInitialisedSize_(nBytes);
  
  _postcondition_(_MAGIC_,VEC_CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
TrayNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::~TrayNL_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  SUB nItemNo;
  for (nItemNo = (i8)0; (i8)nItemNo < (i8)m_nHeadItems; nItemNo++) {
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
ITEM*& TrayNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::operator [](SUB nItemNo)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  //ASSERT(nItemNo >= 0 and nItemNo < MAX_ITEMS);
  
  return m_pItem[nItemNo];
}

/*----------------------------------------------------------------------------*/
/* clear all items                                                            */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
void TrayNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::vClear(void)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  for (SUB nItemNo = 0; nItemNo < m_nHeadItems; nItemNo++)
    m_pItem[nItemNo] = nullptr;
  
  m_nHeadItems = 0;
}

/*----------------------------------------------------------------------------*/
/* get number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
i8 TrayNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::nItems(void)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  ASSERT(m_nHeadItems >= 0 && m_nHeadItems <= MAX_ITEMS);
  
  return m_nHeadItems;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
ITEM* TrayNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::aGet(SUB nItemNo)
{
  _precondition_(_MAGIC_,VEC_CN,_FOOTER_);
  
  ASSERT(nItemNo >= 0 && nItemNo < m_nHeadItems);
  
  ITEM* pItem = m_pItem[nItemNo];
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN VEC_CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
Footer* TrayNL_t<VEC_CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::aFooter(void)
{
  _precondition_(_MAGIC_,VEC_CN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
