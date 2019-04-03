/*============================================================================*/
/* TrayNL_t.hpp - double-headed tray of items template                        */
/*============================================================================*/
#pragma once

namespace Structures
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // nub (the inner tray)
  
  template <eCN TRAY_CN, class ITEM>
  class Nub_t : public Object
  {
  private:
    i8 m_nMaxItems;  // max number of items
    i8 m_nHeadItems; // number of items from the head
    i8 m_nTailItems; // number of items from the tail
    ITEM* m_axaItem; // array of items
  
    i8 m_i_Offset;   // encoding offset
  
    // must be last
    Footer m_xFooter;
  
  public:
    Nub_t(void);
    ~Nub_t(void) noexcept(false);
  
    i8    iAllotHead(void);
    i8    iAllotTail(void);
    i8    nFree(void);
    ITEM* aGet(i8 i_Item);
    i8    nHeads(void);
    i8    nItems(void);
    i8    nMaxItems(void);
    i8    i_Offset(void);
    void  vOffsetInc(i8 iDelta);
    void  vReset(void);
    i8    nTails(void);
  
    void  vSetItems(i8 nMaxItems, ITEM* axaItem);
  
  private:
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tray (the outer tray)

  template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
  class TrayNL_t : public Object
  {
  protected:
    ITEM m_xaItem[MAX_ITEMS]; // array of items
  
    Nub_t<TRAY_CN,ITEM> m_oNub;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    TrayNL_t(void);
    ~TrayNL_t(void) noexcept(false);
  
    // auto conversion
    operator Nub_t<TRAY_CN,ITEM>*(void);
  
    i8    iAllotHead(void);
    i8    iAllotTail(void);
    i8    nFree(void);
    ITEM* aGet(i8 i_Item);
    i8    nHeads(void);
    i8    nItems(void);
    void  vReset(void);
    i8    nTails(void);
  
  private:
    Footer* aFooter(void);
  };
  
  template <eCN TRAY_CN, class ITEM,  i8 MAX_ITEMS>
  TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>* _pNubNL(void* aAddr);
}

using namespace Structures;



////////////////////////////////////////////////////////////////////////////////
///                             NUB (inner tray)                             ///
////////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
Nub_t<TRAY_CN,ITEM>::Nub_t(void)
: Object(TRAY_CN)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  m_nMaxItems  = C_UNKNOWN;
  m_nHeadItems = 0; // number of items from the head
  m_nTailItems = 0; // number of items from the tail
  m_axaItem    = nullptr;  // set of items
  
  m_i_Offset   = 8;
  
  i8 nBytes = sizeof(Nub_t<TRAY_CN,ITEM>);
  _checkInitialisedSize_(nBytes);
  
  _postcondition_(_MAGIC_,TRAY_CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
Nub_t<TRAY_CN,ITEM>::~Nub_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot head item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
i8 Nub_t<TRAY_CN,ITEM>::iAllotHead(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  if (nItems() >= m_nMaxItems)
    return C_UNKNOWN;
  
  i8 i_Item = m_nHeadItems++;
  
  return i_Item;
}

/*----------------------------------------------------------------------------*/
/* allot tail item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
i8 Nub_t<TRAY_CN,ITEM>::iAllotTail(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  if (nItems() >= m_nMaxItems)
    return C_UNKNOWN;
  
  i8 i_Item = m_nMaxItems - 1 - m_nTailItems++;
  
  return i_Item;
}

/*----------------------------------------------------------------------------*/
/* get total number of free items                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
i8 Nub_t<TRAY_CN,ITEM>::nFree(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 nFree = m_nMaxItems - (m_nHeadItems + m_nTailItems);
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
ITEM* Nub_t<TRAY_CN,ITEM>::aGet(i8 i_Item)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  ASSERT(i_Item >= 0 && i_Item < m_nMaxItems);
  
  ITEM* aItem = &m_axaItem[i_Item];
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* get total number of head items                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
i8 Nub_t<TRAY_CN,ITEM>::nHeads(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  return m_nHeadItems;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
i8 Nub_t<TRAY_CN,ITEM>::nItems(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 nItems = m_nHeadItems + m_nTailItems;
  
  return nItems;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
i8 Nub_t<TRAY_CN,ITEM>::i_Offset(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  return m_i_Offset;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
void Nub_t<TRAY_CN,ITEM>::vOffsetInc(i8 iDelta)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  m_i_Offset += iDelta;
}

/*----------------------------------------------------------------------------*/
/* get maximum number of items                                                */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
i8 Nub_t<TRAY_CN,ITEM>::nMaxItems(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  return m_nMaxItems;
}

/*----------------------------------------------------------------------------*/
/* reset all items                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
void Nub_t<TRAY_CN,ITEM>::vReset(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  m_nHeadItems = 0;
  m_nTailItems = 0;
  
  MemLib::vZero(m_axaItem,(sizeof(ITEM) * m_nMaxItems),TRAY_CN);
  
  m_i_Offset = 8;
}

/*----------------------------------------------------------------------------*/
/* get total number of tail items                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
i8 Nub_t<TRAY_CN,ITEM>::nTails(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  return m_nTailItems;
}

/*----------------------------------------------------------------------------*/
/* set item array                                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
void Nub_t<TRAY_CN,ITEM>::vSetItems(i8 nMaxItems, ITEM *axaItem)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  m_nMaxItems = nMaxItems;
  m_axaItem   = axaItem;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
Footer* Nub_t<TRAY_CN,ITEM>::aFooter(void)
{
  _precondition_(_MAGIC_,TRAY_CN);
  
  return &m_xFooter;
}




////////////////////////////////////////////////////////////////////////////////
///                                 TRAY                                     ///
////////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>* Structures::_pNubNL(void* aAddr)
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
  void* pNub = HeapMem::pAllocate(TRAY_CN);
  
  return pNub;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
void TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::operator delete(void* pAddr)
{
  TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>* pNub =
    _pNubNL<TRAY_CN,ITEM,MAX_ITEMS>(pAddr);
  
  HeapMem::vFree(pNub);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::TrayNL_t(void)
: Object(TRAY_CN)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  m_oNub.vSetItems(MAX_ITEMS,&m_xaItem[0]);
  
  vReset();
  
  _setFooter_();
  
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
}

/*----------------------------------------------------------------------------*/
/* get the inner tray                                                         */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::operator Nub_t<TRAY_CN,ITEM>*(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  return &m_oNub;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot head item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::iAllotHead(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 i_Item = m_oNub.iAllotHead();
  
  return i_Item;
}

/*----------------------------------------------------------------------------*/
/* allot tail item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::iAllotTail(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 i_Item = m_oNub.iAllotTail();
  
  return i_Item;
}

/*----------------------------------------------------------------------------*/
/* get total number of free items                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::nFree(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 nFree = m_oNub.nFree();
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
ITEM* TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::aGet(i8 i_Item)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  ITEM* aItem = m_oNub.aGet(i_Item);
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* get total number of head items                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::nHeads(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 nHeads = m_oNub.nHeads();
  
  return nHeads;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::nItems(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 nItems = m_oNub.nItems();
  
  return nItems;
}

/*----------------------------------------------------------------------------*/
/* reset all items                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
void TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::vReset(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  m_oNub.vReset();
}

/*----------------------------------------------------------------------------*/
/* get total number of tail items                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::nTails(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 nTails = m_oNub.nTails();
  
  return nTails;
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
