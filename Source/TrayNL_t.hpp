/*============================================================================*/
/* TrayNL_t.hpp - double-headed tray of items template                        */
/*============================================================================*/
#pragma once

namespace Structures
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // nub (the inner tray)
  
  template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
  class Nub_t : public Object
  {
  private:
    i8 m_nMaxItems;  // max number of items
    i8 m_nHeadItems; // number of items from the head
    i8 m_nTailItems; // number of items from the tail
    DESC* m_pDesc;   // object describing the items
    ITEM_HEAD* m_axaItem; // array of items
  
    i8 m_i_Offset;   // encoding offset
    i8 m_iHash;      // encoding hash

    // must be last
    Footer m_xFooter;
  
  public:
    Nub_t(void);
    ~Nub_t(void) noexcept(false);
  
    ITEM_HEAD* aAllotHead(void);
    i8    iAllotHead(void);
    ITEM_HEAD* aAllotTails(i8 nTails);
    i8    iAllotTail(void);
    DESC* pDesc(void);
    void  vDesc(DESC* pDesc);
    i8    nFree(void);
    ITEM_HEAD* aGet(i8 i_Item);
    i8    nHeads(void);
    i8    iHash(void);
    void  vHashInc(i8 iDelta);
    bool  bHasSpace(i8 nBytes);
    i8    nItems(void);
    i8    nMaxItems(void);
    i8    i_Offset(void);
    void  vOffsetInc(i8 iDelta);
    void  vReset(void);
    i8    nTails(void);
  
    void  vSetItems(i8 nMaxItems, ITEM_HEAD* axaItem);
  
  private:
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tray (the outer tray)

  template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
  class TrayNL_t : public Object
  {
  protected:
    ITEM_HEAD m_xaItem[MAX_ITEMS]; // array of items
  
    Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC> m_oNub;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    TrayNL_t(void);
    ~TrayNL_t(void) noexcept(false);
  
    // auto conversion
    operator Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>*(void);
  
    ITEM_HEAD* aAllotHead(void);
    i8    iAllotHead(void);
    ITEM_HEAD* aAllotTails(i8 nTails);
    i8    iAllotTail(void);
    DESC* pDesc(void);
    void  vDesc(DESC* pDesc);
    i8    nFree(void);
    ITEM_HEAD* aGet(i8 i_Item);
    i8    iHash(void);
    bool  bHasSpace(i8 nBytes);
    i8    nHeads(void);
    i8    nItems(void);
    void  vReset(void);
    i8    nTails(void);
  
  private:
    Footer* aFooter(void);
  };
  
  template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
  TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>* _pNubNL(void* aAddr);
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
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::Nub_t(void)
: Object(TRAY_CN)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  m_nMaxItems  = C_UNKNOWN;
  m_nHeadItems = 0;
  m_nTailItems = 0;
  m_pDesc      = nullptr;
  m_axaItem    = nullptr;
  
  m_i_Offset   = 8;
  m_iHash      = 0;

  i8 nBytes = sizeof(Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>);
  _checkInitialisedSize_(nBytes);
  
  _postcondition_(_MAGIC_,TRAY_CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::~Nub_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot head item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
ITEM_HEAD* Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::aAllotHead(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 i_Item = iAllotHead();
  
  if (C_UNKNOWN == i_Item)
    return nullptr;
  
  ITEM_HEAD* pItem = aGet(i_Item);
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* allot head item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
i8 Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::iAllotHead(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  if (nItems() >= m_nMaxItems)
    return C_UNKNOWN;
  
  i8 i_Item = m_nHeadItems++;
  
  return i_Item;
}

/*----------------------------------------------------------------------------*/
/* allot tail items                                                           */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
ITEM_HEAD* Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::aAllotTails(i8 nTails)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  if (! nTails)
    return nullptr;

  if (! bHasSpace(sizeof(CacheLine) * nTails))
    return nullptr;

  i8 i_CacheLine = C_UNKNOWN;
  
  for (i8 i_Tail = 0; i_Tail < nTails; i_Tail++)
    i_CacheLine = iAllotTail();
  ASSERT(i_CacheLine != C_UNKNOWN);

  ITEM_HEAD* pItem = aGet(i_CacheLine);
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* allot tail item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
i8 Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::iAllotTail(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  if (nItems() >= m_nMaxItems)
    return C_UNKNOWN;
  
  i8 i_Item = m_nMaxItems - 1 - m_nTailItems++;
  
  return i_Item;
}

/*----------------------------------------------------------------------------*/
/* get the item describer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
DESC* Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::pDesc(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  return m_pDesc;
}

/*----------------------------------------------------------------------------*/
/* set the item describer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
void Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::vDesc(DESC* pDesc)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  m_pDesc = pDesc;
}

/*----------------------------------------------------------------------------*/
/* get total number of free items                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
i8 Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::nFree(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 nFree = m_nMaxItems - (m_nHeadItems + m_nTailItems);
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
ITEM_HEAD* Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::aGet(i8 i_Item)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  ASSERT(i_Item >= 0 && i_Item < m_nMaxItems);
  
  ITEM_HEAD* aItem = &m_axaItem[i_Item];
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
i8 Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::iHash(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  return m_iHash;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
void Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::vHashInc(i8 iDelta)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  m_iHash += iDelta;
}

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
bool Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::bHasSpace(i8 nBytes)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);

  i8 nFreeItems = nFree();
  i8 nFreeBytes = nFreeItems * (i8)sizeof(ITEM_HEAD);
  
  if (nFreeBytes < nBytes)
    return false;
  else
    return true;
}

/*----------------------------------------------------------------------------*/
/* get total number of head items                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
i8 Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::nHeads(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  return m_nHeadItems;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
i8 Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::nItems(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 nItems = m_nHeadItems + m_nTailItems;
  
  return nItems;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
i8 Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::i_Offset(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  return m_i_Offset;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
void Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::vOffsetInc(i8 iDelta)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  m_i_Offset += iDelta;
}

/*----------------------------------------------------------------------------*/
/* get maximum number of items                                                */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
i8 Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::nMaxItems(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  return m_nMaxItems;
}

/*----------------------------------------------------------------------------*/
/* reset all items                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
void Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::vReset(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  m_nHeadItems = 0;
  m_nTailItems = 0;
  
  MemLib::vZero(m_axaItem,(sizeof(ITEM_HEAD) * m_nMaxItems),TRAY_CN);
  
  m_i_Offset = 8;
  m_iHash    = 0;
}

/*----------------------------------------------------------------------------*/
/* get total number of tail items                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
i8 Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::nTails(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  return m_nTailItems;
}

/*----------------------------------------------------------------------------*/
/* set item array                                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
void Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::vSetItems(i8 nMaxItems, ITEM_HEAD *axaItem)
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
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC>
Footer* Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>::aFooter(void)
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
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>* Structures::_pNubNL(void* aAddr)
{
  TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>* pVector =
  reinterpret_cast<TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>*>(aAddr);
  
  RASSERT_TYPE(pVector,TRAY_CN);
  
  return pVector;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
void* TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>::operator new(size_t)
{
  void* pNub = HeapMem::pAllocate(TRAY_CN);
  
  return pNub;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
void TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>::operator delete(void* pAddr)
{
  TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>* pNub =
    _pNubNL<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>(pAddr);
  
  HeapMem::vFree(pNub);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>::TrayNL_t(void)
: Object(TRAY_CN)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  m_oNub.vSetItems(MAX_ITEMS,&m_xaItem[0]);
  
  vReset();
  
  _setFooter_();
  
  i8 nBytes = sizeof(TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>);
  _checkInitialisedSize_(nBytes);
  
  _postcondition_(_MAGIC_,TRAY_CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>::~TrayNL_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get the inner tray                                                         */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>::operator Nub_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC>*(void)
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
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>::iAllotHead(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 i_Item = m_oNub.iAllotHead();
  
  return i_Item;
}

/*----------------------------------------------------------------------------*/
/* allot tail item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>::iAllotTail(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 i_Item = m_oNub.iAllotTail();
  
  return i_Item;
}

/*----------------------------------------------------------------------------*/
/* get the item describer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
DESC* TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>::pDesc(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  return m_oNub.pDesc();
}

/*----------------------------------------------------------------------------*/
/* set the item describer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
void TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>::vDesc(DESC* pDesc)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  m_oNub.vDesc(pDesc);
}

/*----------------------------------------------------------------------------*/
/* get total number of free items                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>::nFree(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 nFree = m_oNub.nFree();
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
ITEM_HEAD* TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>::aGet(i8 i_Item)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  ITEM_HEAD* aItem = m_oNub.aGet(i_Item);
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* get total number of head items                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>::iHash(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 iHash = m_oNub.iHash();
  
  return iHash;
}



/*----------------------------------------------------------------------------*/
/* get total number of head items                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>::nHeads(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 nHeads = m_oNub.nHeads();
  
  return nHeads;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>::nItems(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  i8 nItems = m_oNub.nItems();
  
  return nItems;
}

/*----------------------------------------------------------------------------*/
/* reset all items                                                            */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
void TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>::vReset(void)
{
  _precondition_(_MAGIC_,TRAY_CN,_FOOTER_);
  
  m_oNub.vReset();
}

/*----------------------------------------------------------------------------*/
/* get total number of tail items                                             */
/*----------------------------------------------------------------------------*/
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
i8 TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>::nTails(void)
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
template <eCN TRAY_CN, class ITEM_HEAD, class ITEM_TAIL, class DESC, i8 MAX_ITEMS>
Footer* TrayNL_t<TRAY_CN,ITEM_HEAD,ITEM_TAIL,DESC,MAX_ITEMS>::aFooter(void)
{
  _precondition_(_MAGIC_,TRAY_CN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
