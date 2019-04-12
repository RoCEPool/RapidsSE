/*============================================================================*/
/* TrayNL_t.hpp - double-headed tray of items template                        */
/*============================================================================*/
#pragma once

namespace Structures
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // nub (the inner tray)
  
  template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
  class Nub_t : public Object
  {
  private:
    i8 m_nSpaceUnits; // number of space units
    i8 m_nHeadUnits;  // number of space units allocated from the head
    i8 m_nTailUnits;  // number of space units allocated from the tail

    i8 m_nMaxItems;   // max number of items
    i8 m_nHeadItems;  // number of items allocated from the head
    i8 m_nTailItems;  // number of items allocated from the tail
  
    DESC* m_pDesc;    // object describing the items
  
    TAIL* m_axaUnits;  // array of space units

    i8 m_i_Offset;    // encoding offset
    i8 m_iHash;       // encoding hash

    // must be last
    Footer m_xFooter;
  
  public:
    Nub_t(void);
    ~Nub_t(void) noexcept(false);
  
    i8    iAllotHead(void);
    i8    iAllotTail(i8 nBytes);
    DESC* pDesc(void);
    void  vDesc(DESC* pDesc);
    i8    nFreeBytes(void);
    i8    iFirstTail(void);
    HEAD* aGetHead(i8 i_Head);
    TAIL* aGetTail(i8 i_Head);
    i8    nHeads(void);
    i8    iHash(void);
    void  vHashInc(i8 iDelta);
    bool  bHasSpace(i8 nBytes);
    i8    nItems(void);
    i8    iNextTail(i8 i_Tail);
    i8    i_Offset(void);
    void  vOffsetInc(i8 iDelta);
    void  vReset(void);
    i8    nTails(void);
  
    void  vSetItems(i8 nSpaceUnits, i8 nMaxItems, HEAD* axaUnits);
  
  private:
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tray (the outer tray)

  template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
  class TrayNL_t : public Object
  {
  protected:
    HEAD m_xaItem[BYTES / sizeof(HEAD)]; // array of items
  
    Nub_t<CN,HEAD,TAIL,DESC,CODA> m_oNub;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    TrayNL_t(void);
    ~TrayNL_t(void) noexcept(false);
  
    // auto conversion
    operator Nub_t<CN,HEAD,TAIL,DESC,CODA>*(void);
  
    i8    iAllotHead(void);
    DESC* pDesc(void);
    void  vDesc(DESC* pDesc);
    i8    nFreeBytes(void);
    HEAD* aGetHead(i8 i_Head);
    TAIL* aGetTail(i8 i_Tail);
    i8    iHash(void);
    bool  bHasSpace(i8 nBytes);
    i8    nHeads(void);
    i8    nItems(void);
    void  vReset(void);
    i8    nTails(void);
  
  private:
    Footer* aFooter(void);
  };
  
  template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
  TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>* _pNubNL(void* aAddr);
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
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
Nub_t<CN,HEAD,TAIL,DESC,CODA>::Nub_t(void)
: Object(CN)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  // head must be a multiple of tail
  RASSERT(0 == sizeof(HEAD) % sizeof(TAIL));
  
  m_nSpaceUnits = C_UNKNOWN;
  m_nHeadUnits  = 0;
  m_nTailUnits  = 0;

  m_nMaxItems   = C_UNKNOWN;
  m_nHeadItems  = 0;
  m_nTailItems  = 0;
  
  m_pDesc       = nullptr;

  m_axaUnits    = nullptr;

  m_i_Offset    = 8;
  m_iHash       = 0;

  i8 nBytes = sizeof(Nub_t<CN,HEAD,TAIL,DESC,CODA>);
  _checkInitialisedSize_(nBytes);
  
  _postcondition_(_MAGIC_,CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
Nub_t<CN,HEAD,TAIL,DESC,CODA>::~Nub_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot head item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,DESC,CODA>::iAllotHead(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  if (nFreeBytes() < (i8)sizeof(HEAD))
    return C_UNKNOWN;

  i8 i_Item = m_nHeadItems++;
  
  m_nHeadUnits += (sizeof(HEAD) / sizeof(TAIL));
  
  return i_Item;
}

/*----------------------------------------------------------------------------*/
/* allot tail item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,DESC,CODA>::iAllotTail(i8 nBytes)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  RASSERT(nBytes);
  
  if (nFreeBytes() < nBytes)
    return C_UNKNOWN;

  i8 nUnits = nBytes / (i8)sizeof(TAIL);
  if (nBytes % (i8)sizeof(TAIL))
    nUnits++;
  RASSERT(nUnits * (i8)sizeof(TAIL) >= nBytes);
  RASSERT(nUnits == 1);

  m_nTailUnits += nUnits;
  m_nTailItems++;

  i8 i_Tail = m_nSpaceUnits - m_nTailUnits;
  
  return i_Tail;
}

/*----------------------------------------------------------------------------*/
/* get the item describer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
DESC* Nub_t<CN,HEAD,TAIL,DESC,CODA>::pDesc(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_pDesc;
}

/*----------------------------------------------------------------------------*/
/* set the item describer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
void Nub_t<CN,HEAD,TAIL,DESC,CODA>::vDesc(DESC* pDesc)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_pDesc = pDesc;
}

/*----------------------------------------------------------------------------*/
/* get first tail                                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,DESC,CODA>::iFirstTail(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  if (! m_nTailUnits)
    return C_UNKNOWN;

  i8 i_Tail = m_nSpaceUnits - 1;

  return i_Tail;
}

/*----------------------------------------------------------------------------*/
/* get total number of free bytes                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,DESC,CODA>::nFreeBytes(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nFreeBytes = (m_nSpaceUnits - (m_nHeadUnits + m_nTailUnits)) * sizeof(TAIL);
  
  return nFreeBytes;
}

/*----------------------------------------------------------------------------*/
/* get a specific head item                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
HEAD* Nub_t<CN,HEAD,TAIL,DESC,CODA>::aGetHead(i8 i_Item)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  ASSERT(i_Item >= 0 && i_Item < m_nMaxItems);
  
  i8 i_Unit = i_Item * (sizeof(HEAD) / sizeof(TAIL));
  HEAD* aUnit = (HEAD*)&m_axaUnits[i_Unit];
  
  return aUnit;
}

/*----------------------------------------------------------------------------*/
/* get a specific tail item                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
TAIL* Nub_t<CN,HEAD,TAIL,DESC,CODA>::aGetTail(i8 i_Tail)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  ASSERT(i_Tail >= 0 && i_Tail < m_nSpaceUnits);
  
  TAIL* aTail = (TAIL*)&m_axaUnits[i_Tail];
  
  return aTail;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,DESC,CODA>::iHash(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_iHash;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
void Nub_t<CN,HEAD,TAIL,DESC,CODA>::vHashInc(i8 iDelta)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_iHash += iDelta;
}

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
bool Nub_t<CN,HEAD,TAIL,DESC,CODA>::bHasSpace(i8 nBytes)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);

  i8 nFreeBytes = this->nFreeBytes();
  
  if (nFreeBytes < nBytes)
    return false;
  else
    return true;
}

/*----------------------------------------------------------------------------*/
/* get total number of head items                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,DESC,CODA>::nHeads(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_nHeadItems;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,DESC,CODA>::nItems(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nItems = m_nHeadItems + m_nTailItems;
  
  return nItems;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,DESC,CODA>::i_Offset(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_i_Offset;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
void Nub_t<CN,HEAD,TAIL,DESC,CODA>::vOffsetInc(i8 iDelta)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_i_Offset += iDelta;
}

/*----------------------------------------------------------------------------*/
/* get next tail                                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,DESC,CODA>::iNextTail(i8 i_PrevTail)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 i_Tail = i_PrevTail - 1;
  if (i_Tail < m_nSpaceUnits - m_nTailUnits) // 10 - 3 : [0 1 2 .. 7 8 9]
    return i_Tail;
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* reset all items                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
void Nub_t<CN,HEAD,TAIL,DESC,CODA>::vReset(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_nHeadUnits = 0;
  m_nTailUnits = 0;

  m_nHeadItems = 0;
  m_nTailItems = 0;
  
  MemLib::vZero(m_axaUnits,(sizeof(TAIL) * m_nSpaceUnits),CN);
  
  m_i_Offset = 8;
  m_iHash    = 0;
}

/*----------------------------------------------------------------------------*/
/* get total number of tail items                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,DESC,CODA>::nTails(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_nTailItems;
}

/*----------------------------------------------------------------------------*/
/* set item array                                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
void Nub_t<CN,HEAD,TAIL,DESC,CODA>::vSetItems(i8 nSpaceUnits, i8 nMaxItems, HEAD *axaUnits)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_nSpaceUnits = nSpaceUnits;
  m_nMaxItems   = nMaxItems;
  
  m_axaUnits    = reinterpret_cast<TAIL*>(axaUnits);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA>
Footer* Nub_t<CN,HEAD,TAIL,DESC,CODA>::aFooter(void)
{
  _precondition_(_MAGIC_,CN);
  
  return &m_xFooter;
}




////////////////////////////////////////////////////////////////////////////////
///                                 CN                                     ///
////////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>* Structures::_pNubNL(void* aAddr)
{
  TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>* pVector =
    reinterpret_cast<TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>*>(aAddr);
  
  RASSERT_TYPE(pVector,CN);
  
  return pVector;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
void* TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>::operator new(size_t)
{
  void* pNub = HeapMem::pAllocate(CN);
  
  return pNub;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
void TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>::operator delete(void* pAddr)
{
  TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>* pNub =
    _pNubNL<CN,HEAD,TAIL,DESC,CODA,BYTES>(pAddr);
  
  HeapMem::vFree(pNub);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>::TrayNL_t(void)
: Object(CN)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nSpaceUnits = BYTES / sizeof(TAIL);
  i8 nItems = BYTES / sizeof(HEAD);
  m_oNub.vSetItems(nSpaceUnits,nItems,&m_xaItem[0]);
  
  vReset();
  
  _setFooter_();
  
  i8 nBytes = sizeof(TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>);
  _checkInitialisedSize_(nBytes);
  
  _postcondition_(_MAGIC_,CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>::~TrayNL_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get the inner tray                                                         */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>::operator Nub_t<CN,HEAD,TAIL,DESC,CODA>*(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return &m_oNub;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot head item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
i8 TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>::iAllotHead(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 i_Item = m_oNub.iAllotHead();
  
  return i_Item;
}

/*----------------------------------------------------------------------------*/
/* get the item describer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
DESC* TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>::pDesc(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_oNub.pDesc();
}

/*----------------------------------------------------------------------------*/
/* set the item describer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
void TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>::vDesc(DESC* pDesc)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_oNub.vDesc(pDesc);
}

/*----------------------------------------------------------------------------*/
/* get total number of free items                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
i8 TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>::nFreeBytes(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nFreeBytes = m_oNub.nFreeBytes();
  
  return nFreeBytes;
}

/*----------------------------------------------------------------------------*/
/* get a specific head item                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
HEAD* TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>::aGetHead(i8 i_Item)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  HEAD* aItem = m_oNub.aGetHead(i_Item);
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* get a specific tail item                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
TAIL* TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>::aGetTail(i8 i_Item)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  TAIL* aItem = m_oNub.aGetTail(i_Item);
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* get total number of head items                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
i8 TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>::iHash(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 iHash = m_oNub.iHash();
  
  return iHash;
}



/*----------------------------------------------------------------------------*/
/* get total number of head items                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
i8 TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>::nHeads(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nHeads = m_oNub.nHeads();
  
  return nHeads;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
i8 TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>::nItems(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nItems = m_oNub.nItems();
  
  return nItems;
}

/*----------------------------------------------------------------------------*/
/* reset all items                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
void TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>::vReset(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_oNub.vReset();
}

/*----------------------------------------------------------------------------*/
/* get total number of tail items                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
i8 TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>::nTails(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nTails = m_oNub.nTails();
  
  return nTails;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class DESC, class CODA, i8 BYTES>
Footer* TrayNL_t<CN,HEAD,TAIL,DESC,CODA,BYTES>::aFooter(void)
{
  _precondition_(_MAGIC_,CN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
