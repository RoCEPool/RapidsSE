/*============================================================================*/
/* TrayNL_t.hpp - double-headed tray of items template                        */
/*============================================================================*/
#pragma once

namespace Structures
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // nub (the inner tray)
  
  template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
  class Nub_t : public Object
  {
  private:
    i8 m_nSpaceUnits; // number of space units
    i8 m_nHeadUnits;  // number of space units allocated from the head
    i8 m_nTailUnits;  // number of space units allocated from the tail

    i8 m_nHeadItems;  // number of items allocated from the head
    i8 m_nTailItems;  // number of items allocated from the tail
  
    DESC* m_pDesc;    // object describing the items

    CODA* m_pCoda;    // object decoding the items

    UNIT* m_axaUnits; // array of space units

    i8 m_i_Offset;    // encoding offset
    i8 m_iHash;       // encoding hash

    // must be last
    Footer m_xFooter;
  
  public:
    Nub_t(void);
    ~Nub_t(void) noexcept(false);
  
    static void  vCut(void* aAddr, i8 nSize, i8 i_Offset, i8 nCutBytes);
    static void  vExpand(void* aAddr, i8 nSize, i8 i_Offset, i8 nAddBytes);
    static TAIL* aToTail(HEAD* aHead, i8 i_Distance);
    static i8 iToTail(i8 i_Head, i8 i_Distance);
    static i8 iUnitDiff(i8 i_Head, i8 i_Tail);
    static i8 nUnits(i8 nBytes);

    i8    iAllotHead(void);
    i8    iAllotTail(i8 nBytes);
    i8    nAreaBytes(void);
    CODA* pCoda(void);
    void  vCoda(CODA* pCoda);
    DESC* pDesc(void);
    void  vDesc(DESC* pDesc);
    i8    nFreeBytes(void);
    i8    iFirstTail(void);
    HEAD* aGetHead(i8 i_Head);
    TAIL* aGetTail(i8 i_Tail);
    i8    nHeads(void);
    i8    iHash(void);
    void  vHashInc(i8 iDelta);
    bool  bHasSpace(i8 nBytes);
    i8    nItems(void);
    i8    iLastTail(void);
    i8    iNextTail(i8 i_Tail);
    i8    i_Offset(void);
    void  vOffsetInc(i8 iDelta);
    i8    iPrevTail(i8 i_Tail);
    void  vReset(void);
    i8    nTails(void);
    i8    nUsedBytes(void);

    void  vSetItems(i8 nSpaceUnits, UNIT* axaUnits);
  
  private:
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tray (the outer tray)

  template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
  class TrayNL_t : public Object
  {
  protected:
    UNIT m_xaUnits[BYTES / sizeof(UNIT)]; // array of space units
  
    Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA> m_oNub;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    TrayNL_t(void);
    ~TrayNL_t(void) noexcept(false);
  
    // auto conversion
    operator Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>*(void);
  
    i8    iAllotHead(void);
    CODA* pCoda(void);
    void  vCoda(CODA* pCoda);
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
  
  template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
  TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>* _pNubNL(void* aAddr);
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
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::Nub_t(void)
: Object(CN)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  // head and tail must be a multiple of unit
  RASSERT(0 == sizeof(HEAD) % sizeof(UNIT));
  RASSERT(0 == sizeof(TAIL) % sizeof(UNIT));

  m_nSpaceUnits = C_UNKNOWN;
  m_nHeadUnits  = 0;
  m_nTailUnits  = 0;

  m_nHeadItems  = 0;
  m_nTailItems  = 0;
  
  m_pCoda       = nullptr;
  m_pDesc       = nullptr;

  m_axaUnits    = nullptr;

  m_i_Offset    = 8;
  m_iHash       = 0;

  i8 nBytes = sizeof(Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>);
  _checkInitialisedSize_(nBytes);
  
  _postcondition_(_MAGIC_,CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::~Nub_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compute tail from head + diff                                              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
TAIL* Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::aToTail(HEAD* aHead, i8 i_Distance)
{
  TAIL* aTail = (TAIL*)((UNIT*)aHead + i_Distance);
  
  return aTail;
}

/*----------------------------------------------------------------------------*/
/* compute tail from head + diff                                              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::iToTail(i8 i_Head, i8 i_Diff)
{
  i8 i_HeadUnit = i_Head * (sizeof(HEAD) / sizeof(UNIT));
  
  i8 i_Tail = i_HeadUnit + i_Diff;
  
  return i_Tail;
}

/*----------------------------------------------------------------------------*/
/* compute unit difference between a head and a tail                          */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::iUnitDiff(i8 i_Head, i8 i_Tail)
{
  i8 i_HeadUnit = i_Head * (sizeof(HEAD) / sizeof(UNIT));
  
  i8 i_Diff = i_Tail - i_HeadUnit;
  
  return i_Diff;
}

/*----------------------------------------------------------------------------*/
/* allot tail item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::nUnits(i8 nBytes)
{
  if (! nBytes)
    return 0;
  
  i8 nUnits = nBytes / (i8)sizeof(UNIT);
  if (nBytes % (i8)sizeof(UNIT))
    nUnits++;
  RASSERT(nUnits * (i8)sizeof(UNIT) >= nBytes);
  
  return nUnits;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot head item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::iAllotHead(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  if (nFreeBytes() < (i8)sizeof(HEAD))
    return C_UNKNOWN;

  i8 i_Head = m_nHeadItems++;

  i8 nUnits = (sizeof(HEAD) / sizeof(UNIT));
  RASSERT(nUnits == 4 || nUnits == 1); //! temp

  m_nHeadUnits += nUnits;
  
  return i_Head;
}

/*----------------------------------------------------------------------------*/
/* allot tail item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::iAllotTail(i8 nBytes)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  RASSERT(nBytes);
  
  if (nFreeBytes() < nBytes)
    return C_UNKNOWN;

  i8 nUnits = nBytes / (i8)sizeof(UNIT);
  if (nBytes % (i8)sizeof(UNIT))
    nUnits++;
  RASSERT(nUnits * (i8)sizeof(UNIT) >= nBytes);
  //RASSERT(nUnits == 4 || nUnits == 1); //! temp

  m_nTailUnits += nUnits;
  m_nTailItems++;

  i8 i_Tail = m_nSpaceUnits - m_nTailUnits;
  
  TAIL* aTail = aGetTail(i_Tail);
  aTail->v1B(0,nUnits);
  
  return i_Tail;
}

/*----------------------------------------------------------------------------*/
/* get total space area (used + free)                                         */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::nAreaBytes(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nBytes = sizeof(UNIT) * m_nSpaceUnits;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get the item decoder                                                       */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
CODA* Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::pCoda(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_pCoda;
}

/*----------------------------------------------------------------------------*/
/* set the item decoder                                                       */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
void Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::vCoda(CODA* pCoda)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_pCoda = pCoda;
}

/*----------------------------------------------------------------------------*/
/* cut bytes off                                                              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
void Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::vCut(void* aAddr, i8 nSize, i8 i_Offset, i8 nCutBytes)
{
  i8 iStartMove = i_Offset + nCutBytes ;
  RASSERT(iStartMove <= nSize);
  
  // get the start address
  byte* aSrc = aAdd<byte*>(aAddr, iStartMove);
  
  // get the dst address
  byte* aDst = aAdd<byte*>(aAddr, i_Offset);
  
  // copy nCutBytes from src to dst
  MemLib::vCopy(aDst, aSrc, nSize - (iStartMove), CN);
  
  // calculate the end address after cut
  i8 iLen = nSize - nCutBytes;
  byte* aAddrEnd = aAdd<byte*>(aAddr, iLen);
  
  // zero the last nCutBytes
  MemLib::vZero(aAddrEnd, nCutBytes, CN);
}

/*----------------------------------------------------------------------------*/
/* extend bytes to make space for additional data                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
void Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::vExpand(void* aAddr, i8 nSize, i8 i_Offset, i8 nAddBytes)
{
  i8 iStartMove = i_Offset;
  RASSERT(iStartMove + nAddBytes <= nSize);
  
  // get the start address
  byte* aSrc = aAdd<byte*>(aAddr, iStartMove);
  
  // get the dst address
  byte* aDst = aAdd<byte*>(aAddr, i_Offset + nAddBytes);
  
  // copy nExtendBytes from src to dst
  MemLib::vCopy(aDst, aSrc, nSize - (iStartMove), CN);
}

/*----------------------------------------------------------------------------*/
/* get the item describer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
DESC* Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::pDesc(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_pDesc;
}

/*----------------------------------------------------------------------------*/
/* set the item describer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
void Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::vDesc(DESC* pDesc)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_pDesc = pDesc;
}

/*----------------------------------------------------------------------------*/
/* get first tail                                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::iFirstTail(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  if (! m_nTailUnits)
    return C_UNKNOWN;

  i8 i_Tail = m_nSpaceUnits - (sizeof(TAIL) / sizeof(UNIT));

  return i_Tail;
}

/*----------------------------------------------------------------------------*/
/* get total number of free bytes                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::nFreeBytes(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nFreeBytes = (m_nSpaceUnits - (m_nHeadUnits + m_nTailUnits)) * sizeof(UNIT);
  
  return nFreeBytes;
}

/*----------------------------------------------------------------------------*/
/* get a specific head item                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
HEAD* Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::aGetHead(i8 i_Head)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  ASSERT(i_Head >= 0 && i_Head < m_nHeadItems);
  
  i8 i_Unit = i_Head * (sizeof(HEAD) / sizeof(UNIT));
  HEAD* aHead = (HEAD*)&m_axaUnits[i_Unit];
  
  return aHead;
}

/*----------------------------------------------------------------------------*/
/* get a specific tail item                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
TAIL* Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::aGetTail(i8 i_Tail)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  ASSERT(m_nSpaceUnits - m_nTailUnits >= 0 && i_Tail < m_nSpaceUnits);
  
  TAIL* aTail = (TAIL*)&m_axaUnits[i_Tail];
  
  return aTail;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::iHash(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_iHash;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
void Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::vHashInc(i8 iDelta)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_iHash += iDelta;
}

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
bool Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::bHasSpace(i8 nBytes)
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
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::nHeads(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_nHeadItems;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::nItems(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nItems = m_nHeadItems + m_nTailItems;
  
  return nItems;
}

/*----------------------------------------------------------------------------*/
/* get last tail                                                              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::iLastTail(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  if (! m_nTailUnits)
    return C_UNKNOWN;
  
  i8 i_Tail = m_nSpaceUnits - m_nTailUnits; // 10 - 3 : [0 1 2 .. 7 8 9];
  
  return i_Tail;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::i_Offset(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_i_Offset;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
void Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::vOffsetInc(i8 iDelta)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_i_Offset += iDelta;
  RASSERT(m_i_Offset <= 64);
}

/*----------------------------------------------------------------------------*/
/* get next tail                                                              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::iNextTail(i8 i_PrevTail)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 i_Tail = i_PrevTail - (sizeof(TAIL) / sizeof(UNIT));
  if (i_Tail >= m_nSpaceUnits - m_nTailUnits) // 10 - 3 : [0 1 2 .. 7 8 9]
    return i_Tail;
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* get prev tail                                                              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::iPrevTail(i8 i_CurrTail)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  TAIL* aPriorTail = aGetTail(i_CurrTail);
  i8 nPriorUnits = aPriorTail->w8_1B(0);
  //RASSERT(nPriorUnits <= 4);
  
  UNIT* aPrevTail_Unit = (UNIT*)aPriorTail + nPriorUnits;
  if (m_pCoda && aPrevTail_Unit >= (UNIT*)m_pCoda)
    return C_UNKNOWN;
  if (aPrevTail_Unit >= &m_axaUnits[m_nSpaceUnits])
    return C_UNKNOWN;

  i8 i_Tail = i_CurrTail + nPriorUnits;
  return i_Tail;
}

/*----------------------------------------------------------------------------*/
/* reset all items                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
void Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::vReset(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_nHeadUnits = 0;
  m_nTailUnits = 0;

  m_nHeadItems = 0;
  m_nTailItems = 0;
  
  MemLib::vZero(m_axaUnits,(sizeof(UNIT) * m_nSpaceUnits),CN);
  
  m_i_Offset = 8;
  m_iHash    = 0;
}

/*----------------------------------------------------------------------------*/
/* get total number of tail items                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::nTails(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_nTailItems;
}

/*----------------------------------------------------------------------------*/
/* get total number of used bytes                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::nUsedBytes(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nBytes = (m_nHeadUnits + m_nTailUnits) * sizeof(UNIT);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* set item array                                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
void Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::vSetItems(i8 nSpaceUnits, UNIT* axaUnits)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_nSpaceUnits = nSpaceUnits;
  
  m_axaUnits    = axaUnits;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA>
Footer* Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>::aFooter(void)
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
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>* Structures::_pNubNL(void* aAddr)
{
  TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>* pVector =
    reinterpret_cast<TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>*>(aAddr);
  
  RASSERT_TYPE(pVector,CN);
  
  return pVector;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
void* TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>::operator new(size_t)
{
  void* pNub = HeapMem::pAllocate(CN);
  
  return pNub;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
void TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>::operator delete(void* pAddr)
{
  TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>* pNub =
    _pNubNL<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>(pAddr);
  
  HeapMem::vFree(pNub);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>::TrayNL_t(void)
: Object(CN)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nSpaceUnits = BYTES / sizeof(UNIT);
  m_oNub.vSetItems(nSpaceUnits,&m_xaUnits[0]);
  
  vReset();
  
  i8 i_Coda = m_oNub.iAllotTail(sizeof(CODA)+1);
  CODA* pCoda = (CODA*)( (char*)m_oNub.aGetTail(i_Coda) + 1 );
  new (pCoda) CODA;
  
  m_oNub.vCoda(pCoda);

  _setFooter_();
  
  i8 nBytes = sizeof(TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>);
  _checkInitialisedSize_(nBytes);
  
  _postcondition_(_MAGIC_,CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>::~TrayNL_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get the inner tray                                                         */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>::operator Nub_t<CN,HEAD,TAIL,UNIT,DESC,CODA>*(void)
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
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
i8 TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>::iAllotHead(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 i_Item = m_oNub.iAllotHead();
  
  return i_Item;
}

/*----------------------------------------------------------------------------*/
/* get the item coda                                                          */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
CODA* TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>::pCoda(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_oNub.pCoda();
}

/*----------------------------------------------------------------------------*/
/* set the item describer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
void TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>::vCoda(CODA* pCoda)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_oNub.vCoda(pCoda);
}

/*----------------------------------------------------------------------------*/
/* get the item describer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
DESC* TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>::pDesc(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_oNub.pDesc();
}

/*----------------------------------------------------------------------------*/
/* set the item describer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
void TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>::vDesc(DESC* pDesc)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_oNub.vDesc(pDesc);
}

/*----------------------------------------------------------------------------*/
/* get total number of free items                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
i8 TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>::nFreeBytes(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nFreeBytes = m_oNub.nFreeBytes();
  
  return nFreeBytes;
}

/*----------------------------------------------------------------------------*/
/* get a specific head item                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
HEAD* TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>::aGetHead(i8 i_Item)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  HEAD* aItem = m_oNub.aGetHead(i_Item);
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* get a specific tail item                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
TAIL* TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>::aGetTail(i8 i_Item)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  TAIL* aItem = m_oNub.aGetTail(i_Item);
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* get total number of head items                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
i8 TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>::iHash(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 iHash = m_oNub.iHash();
  
  return iHash;
}



/*----------------------------------------------------------------------------*/
/* get total number of head items                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
i8 TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>::nHeads(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nHeads = m_oNub.nHeads();
  
  return nHeads;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
i8 TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>::nItems(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nItems = m_oNub.nItems();
  
  return nItems;
}

/*----------------------------------------------------------------------------*/
/* reset all items                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
void TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>::vReset(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_oNub.vReset();
}

/*----------------------------------------------------------------------------*/
/* get total number of tail items                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
i8 TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>::nTails(void)
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
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, class CODA, i8 BYTES>
Footer* TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,CODA,BYTES>::aFooter(void)
{
  _precondition_(_MAGIC_,CN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
