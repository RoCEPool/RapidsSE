/*============================================================================*/
/* TrayNL_t.hpp - double-headed tray of items template                        */
/*============================================================================*/
#pragma once

namespace Structures
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // nub (the tray interface)
  
  template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
  class Nub_t : public Object
  {
  private:
    i8 m_nSpaceUnits; // number of space units
    i8 m_nHeadAllot;  // size of head to allot
  
    i8 m_nHeadItems;  // number of items allocated from the head
    i8 m_nHeadWords;  // number of words allocated from the head
    i8 m_nHeadBytes;  // number of bytes allocated from the head
 
    i8 m_nTailItems;  // number of items allocated from the tail
    i8 m_nTailUnits;  // number of space units allocated from the tail
    i8 m_nTailBytes;  // number of bytes allocated from the tail

    DESC* m_pDesc;    // object describing the items
  
    UNIT* m_axaUnits; // array of space units
    Word* m_axaWords; // array of word units

    i8 m_iHash;       // encoding hash

    // must be last
    Footer m_xFooter;
  
  public:
    Nub_t(void);
    ~Nub_t(void) noexcept(false);
  
    static i8 nBytes(i8 nUnits);
    static void  vCut(void* aAddr, i8 nSize, i8 i_Offset, i8 nCutBytes);
    static void  vExpand(void* aAddr, i8 nSize, i8 i_Offset, i8 nAddBytes);
    static TAIL* aToTail(HEAD* aHead, i8 i_Distance);
    static i8 nUnits(i8 nBytes);

    i8    iAllotHead(i8 nBytes);
    i8    iAllotTail(i8 nBytes);
    i8    nAreaBytes(void);
    DESC* pDesc(void);
    void  vDesc(DESC* pDesc);
    i8    nFreeBytes(void);
    HEAD* aGetHead(void);
    HEAD* aGetHead(i8 i_Head);
    TAIL* aGetTail(i8 i_Tail);
    i8    nHeads(void);
    i8    nHeadBytes(void);
    i8    iHash(void);
    void  vHashInc(i8 iDelta);
    bool  bHasSpace(i8 nBytes);
    i8    nItems(void);
    i8    iLastTail(void);
    i8    iPrevTail(i8 i_Tail);
    void  vReset(void);
    i8    nTails(void);
    //i8    iUnitDiff(i8 i_Head, i8 i_Tail);
    i8    iUnitDiff(HEAD* aHead, TAIL* aTail);
    i8    nUsedBytes(void);
    void  vVerifyTails(i8 i_Dump);
  
    void  vSetItems(i8 nSpaceUnits, UNIT* axaUnits);
  
  private:
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tray (the outer tray)

  template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, i8 BYTES>
  class TrayNL_t : public Object
  {
  protected:
  
    i8 m__filler[6];
  
    UNIT m_xaUnits[BYTES / sizeof(UNIT)]; // array of space units
  
    Nub_t<CN,HEAD,TAIL,UNIT,DESC> m_oNub;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    TrayNL_t(void);
    ~TrayNL_t(void) noexcept(false);
  
    Nub_t<CN,HEAD,TAIL,UNIT,DESC>* pNub(void);
  
  private:
    Footer* aFooter(void);
  };
  
  template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, i8 BYTES>
  TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,BYTES>* _pNubNL(void* aAddr);
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
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
Nub_t<CN,HEAD,TAIL,UNIT,DESC>::Nub_t(void)
: Object(CN)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  // head and tail must be a multiple of unit
  RASSERT(0 == sizeof(HEAD) % sizeof(Word));
  RASSERT(0 == sizeof(TAIL) % sizeof(UNIT));

  m_nSpaceUnits = C_UNKNOWN;
  m_nHeadAllot  = C_UNKNOWN;

  m_nHeadItems  = 0;
  m_nHeadWords  = 0;
  m_nHeadBytes  = 0;

  m_nTailItems  = 0;
  m_nTailUnits  = 0;
  m_nTailBytes  = 0;

  m_pDesc       = nullptr;

  m_axaUnits    = nullptr;
  m_axaWords    = nullptr;

  m_iHash       = 0;

  i8 nBytes = sizeof(Nub_t<CN,HEAD,TAIL,UNIT,DESC>);
  _checkInitialisedSize_(nBytes);
  
  _postcondition_(_MAGIC_,CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
Nub_t<CN,HEAD,TAIL,UNIT,DESC>::~Nub_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compute number of bytes in given number of space units                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC>::nBytes(i8 nUnits)
{
  if (! nUnits)
    return 0;
  
  i8 nBytes = nUnits * (i8)sizeof(UNIT);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* compute tail from head + distance (in units)                               */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
TAIL* Nub_t<CN,HEAD,TAIL,UNIT,DESC>::aToTail(HEAD* aHead, i8 i_Distance)
{
  UNIT* aHead_boundary = (UNIT*)MemLib::aToBoundary(aHead,sizeof(UNIT));
  
  TAIL* aTail = (TAIL*)(aHead_boundary + i_Distance);
  
  return aTail;
}

/*----------------------------------------------------------------------------*/
/* compute number of space units required to hold n bytes                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC>::nUnits(i8 nBytes)
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
/* allot head item (with size as per given number of bytes)                   */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC>::iAllotHead(i8 nBytes)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  RASSERT(nBytes);
  //RASSERT(0 == nBytes % sizeof(UNIT));

  if (nFreeBytes() < nBytes)
    return C_UNKNOWN;
  
  i8 nWords = nBytes / (i8)sizeof(Word);
  if (nBytes % (i8)sizeof(Word))
    nWords++;
  RASSERT(nWords * (i8)sizeof(Word) >= nBytes);
  
  m_nHeadBytes += nBytes;
  m_nHeadWords += nWords;
  i8 i_Head = m_nHeadItems++;

  m_nHeadAllot = nBytes;
  
  HEAD* aHead = aGetHead(i_Head);
  
  aHead->v1B(0,nWords);
  
  return i_Head;
}

/*----------------------------------------------------------------------------*/
/* allot tail item (with size as per given number of bytes)                   */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC>::iAllotTail(i8 nBytes)
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

  m_nTailBytes += nBytes;
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
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC>::nAreaBytes(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nBytes = sizeof(UNIT) * m_nSpaceUnits;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* cut bytes off                                                              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
void Nub_t<CN,HEAD,TAIL,UNIT,DESC>::vCut(void* aAddr, i8 nSize, i8 i_Offset, i8 nCutBytes)
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
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
void Nub_t<CN,HEAD,TAIL,UNIT,DESC>::vExpand(void* aAddr, i8 nSize, i8 i_Offset, i8 nAddBytes)
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
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
DESC* Nub_t<CN,HEAD,TAIL,UNIT,DESC>::pDesc(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_pDesc;
}

/*----------------------------------------------------------------------------*/
/* set the item describer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
void Nub_t<CN,HEAD,TAIL,UNIT,DESC>::vDesc(DESC* pDesc)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_pDesc = pDesc;
}

/*----------------------------------------------------------------------------*/
/* get total number of free bytes                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC>::nFreeBytes(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nFreeBytes = (m_nSpaceUnits * sizeof(UNIT))
                     - m_nHeadBytes
                     - (m_nTailUnits * sizeof(UNIT));
  
  return nFreeBytes;
}

/*----------------------------------------------------------------------------*/
/* get most recent head item                                                  */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
HEAD* Nub_t<CN,HEAD,TAIL,UNIT,DESC>::aGetHead(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  ASSERT(m_nHeadItems);
  
  i8 i_Unit = (m_nHeadItems - 1) * (m_nHeadAllot / sizeof(UNIT));
  HEAD* aHead = (HEAD*)&m_axaUnits[i_Unit];
  
  return aHead;
}

/*----------------------------------------------------------------------------*/
/* get a specific head item                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
HEAD* Nub_t<CN,HEAD,TAIL,UNIT,DESC>::aGetHead(i8 i_Head)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  ASSERT(i_Head >= 0 && i_Head < m_nHeadItems);
  
  i8 i_Word = i_Head * (m_nHeadAllot / sizeof(Word));
  HEAD* aHead = (HEAD*)&m_axaWords[i_Word];
    
  return aHead;
}

/*----------------------------------------------------------------------------*/
/* get a specific tail item                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
TAIL* Nub_t<CN,HEAD,TAIL,UNIT,DESC>::aGetTail(i8 i_Tail)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  ASSERT(m_nSpaceUnits - m_nTailUnits >= 0 && i_Tail < m_nSpaceUnits);
  
  TAIL* aTail = (TAIL*)&m_axaUnits[i_Tail];
  
  return aTail;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC>::iHash(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_iHash;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
void Nub_t<CN,HEAD,TAIL,UNIT,DESC>::vHashInc(i8 iDelta)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_iHash += iDelta;
}

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
bool Nub_t<CN,HEAD,TAIL,UNIT,DESC>::bHasSpace(i8 nBytes)
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
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC>::nHeads(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_nHeadItems;
}

/*----------------------------------------------------------------------------*/
/* get total number of head bytes                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC>::nHeadBytes(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_nHeadBytes;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC>::nItems(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nItems = m_nHeadItems + m_nTailItems;
  
  return nItems;
}

/*----------------------------------------------------------------------------*/
/* get last tail                                                              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC>::iLastTail(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  if (! m_nTailUnits)
    return C_UNKNOWN;
  
  i8 i_Tail = m_nSpaceUnits - m_nTailUnits; // 10 - 3 : [0 1 2 .. 7 8 9];
  
  return i_Tail;
}

/*----------------------------------------------------------------------------*/
/* get prev tail                                                              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC>::iPrevTail(i8 i_CurrTail)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  TAIL* aPriorTail = aGetTail(i_CurrTail);
  i8 nPriorUnits = aPriorTail->w8_1B(0);
  //RASSERT(nPriorUnits <= 4);
  
  UNIT* aPrevTail_Unit = (UNIT*)aPriorTail + nPriorUnits;
  if (aPrevTail_Unit >= &m_axaUnits[m_nSpaceUnits])
    return C_UNKNOWN;

  i8 i_Tail = i_CurrTail + nPriorUnits;
  return i_Tail;
}

/*----------------------------------------------------------------------------*/
/* reset all items                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
void Nub_t<CN,HEAD,TAIL,UNIT,DESC>::vReset(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_nHeadItems = 0;
  m_nHeadWords = 0;
  m_nHeadBytes = 0;

  m_nTailItems = 0;
  m_nTailUnits = 0;
  m_nTailBytes = 0;
  
  MemLib::vZero(m_axaUnits,(sizeof(UNIT) * m_nSpaceUnits),CN);
  
  m_iHash      = 0;
}

/*----------------------------------------------------------------------------*/
/* get total number of tail items                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC>::nTails(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_nTailItems;
}

/*----------------------------------------------------------------------------*/
/* compute unit difference between a head and a tail                          */
/*----------------------------------------------------------------------------*/
//template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
//i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC>::iUnitDiff(i8 i_Head, i8 i_Tail)
//{
//  i8 i_HeadUnit = i_Head * (m_nHeadAllot / sizeof(UNIT));
//  
//  i8 i_Diff = i_Tail - i_HeadUnit;
//  
//  return i_Diff;
//}

/*----------------------------------------------------------------------------*/
/* compute unit difference between a head and a tail                          */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC>::iUnitDiff(HEAD* aHead, TAIL* aTail)
{
  UNIT* aHead_boundary = (UNIT*)MemLib::aToBoundary(aHead,sizeof(UNIT));
  UNIT* aTail_boundary = (UNIT*)MemLib::aToBoundary(aTail,sizeof(UNIT));

  // n.b. when you subtract two pointers of the same type the result is
  // the distance (in array elements) between the two. (it is not the
  // difference in bytes between the addresses)
  
  // compute the unit difference
  i8 i_DiffInUnits = aTail_boundary - aHead_boundary;
  
  return i_DiffInUnits;
}

/*----------------------------------------------------------------------------*/
/* get total number of used bytes                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
i8 Nub_t<CN,HEAD,TAIL,UNIT,DESC>::nUsedBytes(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nBytes = m_nHeadBytes + (m_nTailUnits * sizeof(UNIT));
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* set item array                                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
void Nub_t<CN,HEAD,TAIL,UNIT,DESC>::vSetItems(i8 nSpaceUnits, UNIT* axaUnits)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_nSpaceUnits = nSpaceUnits;
  
  m_axaUnits    = axaUnits;
  m_axaWords    = (Word*)axaUnits;
}

/*----------------------------------------------------------------------------*/
/* verify tail lines of given nub                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
void Nub_t<CN,HEAD,TAIL,UNIT,DESC>::vVerifyTails(i8 i_Dump)
{
  i8 nTails = 0;
  i8 nSumTailUnits = 0;
  
  for (i8 i_Tail = iLastTail();
       i_Tail != C_UNKNOWN;
       i_Tail = iPrevTail(i_Tail)) {
    
    TAIL* aTailLine = aGetTail(i_Tail);
    i8 nTailUnits = aTailLine->w8_1B(0);
    RASSERT(nTailUnits > 0 && nTailUnits <= 255);
    
    if (i_Tail == i_Dump)
      MemLib::vWordDump(aTailLine,64);
    
    nTails++;
    nSumTailUnits += nTailUnits;
  }
  
  RASSERT(nTails == m_nTailItems);
  RASSERT(nSumTailUnits == m_nTailUnits);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC>
Footer* Nub_t<CN,HEAD,TAIL,UNIT,DESC>::aFooter(void)
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
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, i8 BYTES>
TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,BYTES>* Structures::_pNubNL(void* aAddr)
{
  TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,BYTES>* pVector =
    reinterpret_cast<TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,BYTES>*>(aAddr);
  
  RASSERT_TYPE(pVector,CN);
  
  return pVector;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, i8 BYTES>
void* TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,BYTES>::operator new(size_t)
{
  void* pNub = HeapMem::pAllocate(CN);
  
  return pNub;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, i8 BYTES>
void TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,BYTES>::operator delete(void* pAddr)
{
  TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,BYTES>* pNub =
    _pNubNL<CN,HEAD,TAIL,UNIT,DESC,BYTES>(pAddr);
  
  HeapMem::vFree(pNub);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, i8 BYTES>
TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,BYTES>::TrayNL_t(void)
: Object(CN)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  MemLib::vZero(&m__filler,sizeof(m__filler),CN);

  i8 nSpaceUnits = BYTES / sizeof(UNIT);
  m_oNub.vSetItems(nSpaceUnits,&m_xaUnits[0]);
  
  m_oNub.vReset();
  
  _setFooter_();
  
  i8 nBytes = sizeof(TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,BYTES>);
  _checkInitialisedSize_(nBytes);
  
  _postcondition_(_MAGIC_,CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, i8 BYTES>
TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,BYTES>::~TrayNL_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the inner nub / interface                                              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, i8 BYTES>
Nub_t<CN,HEAD,TAIL,UNIT,DESC>*
  TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,BYTES>::pNub(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return &m_oNub;
}






/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class TAIL, class UNIT, class DESC, i8 BYTES>
Footer* TrayNL_t<CN,HEAD,TAIL,UNIT,DESC,BYTES>::aFooter(void)
{
  _precondition_(_MAGIC_,CN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
