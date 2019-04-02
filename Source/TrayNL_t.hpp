/*============================================================================*/
/* TrayNL_t.hpp - double-headed tray of items template                        */
/*============================================================================*/
#pragma once

namespace Structures
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  template <eCN TRAY_CN, class ITEM>
  class Tray_t : public Object
  {
  private:
    i8 m_nMaxItems;  // max number of items
    i8 m_nHeadItems; // number of items from the head
    i8 m_nTailItems; // number of items from the tail
    ITEM* m_axaItem; // array of items
  
    // must be last
    Footer m_xFooter;
  
  public:
    Tray_t(void);
    ~Tray_t(void) noexcept(false);
  
    i8    iAllotHead(void);
    i8    iAllotTail(void);
    i8    nFree(void);
    i8    nHeads(void);
    i8    nItems(void);
    ITEM* aGet(i8 i_Item);
    void  vReset(void);
    i8    nTails(void);
  
    void  vSetItems(i8 nMaxItems, ITEM* axaItem);
  
  private:
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
  class TrayNL_t : public Object
  {
  protected:
    ITEM m_xItem[MAX_ITEMS]; // set of items
  
    Tray_t<TRAY_CN,ITEM> m_oTray;
  
    // must be last
    Footer m_xFooter;
  
  public:
  
    void* operator new(size_t);
    void  operator delete(void*);
  
    TrayNL_t(void);
    ~TrayNL_t(void) noexcept(false);
  
    i8    iAllotHead(void);
    i8    iAllotTail(void);
    i8    nFree(void);
    i8    nHeads(void);
    i8    nItems(void);
    ITEM* aGet(i8 i_Item);
    void  vReset(void);
    i8    nTails(void);
    Tray_t<TRAY_CN,ITEM>* pTray(void);
  
  private:
    Footer* aFooter(void);
  };
  
  template <eCN TRAY_CN, class ITEM,  i8 MAX_ITEMS>
  TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>* _pTrayNL(void* aAddr);
}

using namespace Structures;



////////////////////////////////////////////////////////////////////////////////
///                                 tray                                     ///
////////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
Tray_t<TRAY_CN,ITEM>::Tray_t(void)
: Object(TRAY_CN)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  m_nMaxItems  = C_UNKNOWN;
  m_nHeadItems = 0; // number of items from the head
  m_nTailItems = 0; // number of items from the tail
  m_axaItem    = nullptr;  // set of items
  
  i8 nBytes = sizeof(Tray_t<TRAY_CN,ITEM>);
  _checkInitialisedSize_(nBytes);
  
  _postcondition_(_MAGIC_,TRAY_CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
Tray_t<TRAY_CN,ITEM>::~Tray_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public --------------------------------- ---------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot head item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
i8 Tray_t<TRAY_CN,ITEM>::iAllotHead(void)
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
i8 Tray_t<TRAY_CN,ITEM>::iAllotTail(void)
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
i8 Tray_t<TRAY_CN,ITEM>::nFree(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 nFree = m_nMaxItems - ( m_nHeadItems + m_nTailItems);
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* get total number of head items                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
i8 Tray_t<TRAY_CN,ITEM>::nHeads(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  return m_nHeadItems;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
i8 Tray_t<TRAY_CN,ITEM>::nItems(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 nItems = m_nHeadItems + m_nTailItems;
  
  return nItems;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
ITEM* Tray_t<TRAY_CN,ITEM>::aGet(i8 i_Item)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  ASSERT(i_Item >= 0 && i_Item < m_nMaxItems);
  
  ITEM* aItem = &m_axaItem[i_Item];
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* reset all items                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
void Tray_t<TRAY_CN,ITEM>::vReset(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  m_nHeadItems = 0;
  m_nTailItems = 0;
  
  MemLib::vZero(m_axaItem,(sizeof(ITEM) * m_nMaxItems),TRAY_CN);
}

/*----------------------------------------------------------------------------*/
/* get total number of tail items                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
i8 Tray_t<TRAY_CN,ITEM>::nTails(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  return m_nTailItems;
}

/*----------------------------------------------------------------------------*/
/* set item array                                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM>
void Tray_t<TRAY_CN,ITEM>::vSetItems(i8 nMaxItems, ITEM *axaItem)
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
Footer* Tray_t<TRAY_CN,ITEM>::aFooter(void)
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
  void* pTray = HeapMem::pAllocate(TRAY_CN);
  
  return pTray;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
void TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::operator delete(void* pAddr)
{
  TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>* pTray =
  _pTrayNL<TRAY_CN,ITEM,MAX_ITEMS>(pAddr);
  
  HeapMem::vFree(pTray);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::TrayNL_t(void)
: Object(TRAY_CN)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  m_oTray.vSetItems(MAX_ITEMS,&m_xItem[0]);
  
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot head item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::iAllotHead(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 i_Item = m_oTray.iAllotHead();
  
  return i_Item;
}

/*----------------------------------------------------------------------------*/
/* allot tail item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::iAllotTail(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 i_Item = m_oTray.iAllotTail();
  
  return i_Item;
}

/*----------------------------------------------------------------------------*/
/* get total number of free items                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::nFree(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 nFree = m_oTray.nFree();
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* get total number of head items                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::nHeads(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 nHeads = m_oTray.nHeads();
  
  return nHeads;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::nItems(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 nItems = m_oTray.nItems();
  
  return nItems;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
ITEM* TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::aGet(i8 i_Item)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  ITEM* aItem = m_oTray.aGet(i_Item);
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* reset all items                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
void TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::vReset(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  m_oTray.vReset();
}

/*----------------------------------------------------------------------------*/
/* get total number of tail items                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::nTails(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 nTails = m_oTray.nTails();
  
  return nTails;
}

/*----------------------------------------------------------------------------*/
/* get the inner tray                                                         */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM, i8 MAX_ITEMS>
Tray_t<TRAY_CN,ITEM>* TrayNL_t<TRAY_CN,ITEM,MAX_ITEMS>::pTray(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  return &m_oTray;
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
