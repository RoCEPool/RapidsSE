/*============================================================================*/
/* TrayNL_t.hpp - double-headed tray of items template                        */
/*============================================================================*/
#pragma once

namespace Structures
{  
  template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
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

    ITEM*& operator [](i8 nItemNo);

    i8    iAllotHead(void);
    i8    iAllotTail(void);

    void  vClear(void);
    i8    nHeads(void);
    i8    nItems(void);
    ITEM* aGet(i8 nItemNo);
    void  vReset(void);
    i8    nTails(void);

  private:
    Footer* aFooter(void);
  };
  
  template <eCN TRAY_CN, class ITEM,  i8 MAX_ITEMS>
  TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>* _pTrayNL(void* aAddr);
}

////////////////////////////////////////////////////////////////////////////////

using namespace Structures;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>* Structures::_pTrayNL(void* aAddr)
{
  TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>* pVector =
    reinterpret_cast<TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>*>(aAddr);
  
  RASSERT_TYPE(pVector,TRAY_CN);
  
  return pVector;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
void* TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(TRAY_CN);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
void TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::operator delete(void* pVectorAddress)
{
  TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>* pVector =
    _pTrayNL<TRAY_CN,ITEM,MAX_ITEMS>(pVectorAddress);
  
  HeapMem::vFree(pVector);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::TrayNL_t(void)
: Object(TRAY_CN)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  m_nHeadItems = 0;
  
  i8 nItemNo;
  for (nItemNo = (i8)0; nItemNo < MAX_ITEMS; nItemNo++)
    m_pItem[(i8)nItemNo] = nullptr;
  
  _setFooter_();
  
  // compute size of this vector proper (in case it is inherited)
  i8 nBytes = sizeof(TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>);
  
  _checkInitialisedSize_(nBytes);
  
  _postcondition_(_MAGIC_,TRAY_CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::~TrayNL_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 nItemNo;
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
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
ITEM*& TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::operator [](i8 nItemNo)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  //ASSERT(nItemNo >= 0 and nItemNo < MAX_ITEMS);
  
  return m_pItem[nItemNo];
}

/*----------------------------------------------------------------------------*/
/* clear all items                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
void TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::vClear(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  for (i8 nItemNo = 0; nItemNo < m_nHeadItems; nItemNo++)
    m_pItem[nItemNo] = nullptr;
  
  m_nHeadItems = 0;
}

/*----------------------------------------------------------------------------*/
/* get number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::nItems(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  ASSERT(m_nHeadItems >= 0 && m_nHeadItems <= MAX_ITEMS);
  
  return m_nHeadItems;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
ITEM* TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::aGet(i8 nItemNo)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
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
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
Footer* TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::aFooter(void)
{
  _precondition_(_MAGIC_,TRAY_CN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
