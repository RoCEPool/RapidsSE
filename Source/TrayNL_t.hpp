/*============================================================================*/
/* TrayNL_t.hpp - double-headed tray of items template                        */
/*============================================================================*/
#pragma once

namespace Structures
{
  static const i8 C_U_codehead     = 0;
  static const i8 C_BYTES_CODEHEAD = 0;
  static const i8 C_UNITS_CODEHEAD = C_MAX_RECORD / sizeof(Word);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // nub (the tray interface)
  
  static const i8 C_SPACEUNIT = sizeof(SpaceUnit);
  static const i8 C_TAILUNIT  = sizeof(DoubleWord);
  static const i8 C_HEADUNIT  = sizeof(Word);

  
  template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
  class Nub_t : public Object
  {
  private:
    i8 m_nSpaceUnits; // number of space units
    i8 m_nHeadAllot;  // size of head to allot
  
    i8 m_nHeadItems;  // number of items allocated from the head
    i8 m_nHeadWords;  // number of words allocated from the head
    i8 m_nHeadBytes;  // number of bytes allocated from the head
    i8 m_nHeadBytes_used;  // number of bytes used in the large encode headline
 
    i8 m_nTailItems;  // number of items allocated from the tail
    i8 m_nTailUnits;  // number of space units allocated from the tail
    i8 m_nTailBytes;  // number of bytes allocated from the tail

    DESC* m_pDesc;    // object describing the items
  
    i8 m_iHash;       // encoding hash

    SpaceUnit* m_axaUnits; // array of space units
    Word*      m_axaWords; // array of space units as word units
  
    // must be last
    Footer m_xFooter;
  
  public:
    Nub_t(void);
    ~Nub_t(void) noexcept(false);
  
    static i8    nBytes(i8 nUnits);
    static OVER* aGetOver(HEAD* aHead, i8 i_Over_rel);
    static TAIL* aGetTail(HEAD* aHead, i8 i_Tail_rel);
    static i8    nUnits(i8 nBytes);
    static i8    iUnitDiff(HEAD* aHead, TAIL* aTail);

    i8    iAllotHead(i8 nBytes);
    i8    iAllotOver(i8 nBytes);
    i8    iAllotTail(i8 nBytes);
    i8    nAreaBytes(void);
    DESC* pDesc(void);
    void  vDesc(DESC* pDesc);
    i8    nFreeBytes(void);
    HEAD* aGetHead(void);
    HEAD* aGetHead(i8 i_Head);
    OVER* aGetOver(i8 i_Over);
    TAIL* aGetTail(i8 i_Tail);
    i8    nHeads(void);
    i8    nHeadBytes(void);
    i8    nHeadBytes_used(void);
    void  vHeadBytes_used(i8 nBytes);
    i8    iHash(void);
    void  vHashInc(i8 iDelta);
    bool  bHasSpace(i8 nBytes);
    i8    nItems(void);
    i8    iLastTail(void);
    i8    iPrevTail(i8 i_Tail);
    void  vReset(void);
    i8    nTails(void);
    i8    nUsedBytes(void);
    void  vVerifyTails(i8 i_Dump);
  
    void  vSetItems(i8 nSpaceUnits, SpaceUnit* axaUnits);
  
  private:
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tray (the outer tray)

  template <eCN CN, class HEAD, class OVER, class TAIL, class DESC, i8 BYTES>
  class TrayNL_t : public Object
  {
  protected:
  
    i8 m__filler[6]; // fill so m_xaUnits starts on a double word boundry
  
    SpaceUnit m_xaUnits[BYTES / C_SPACEUNIT]; // array of space units
  
    Nub_t<CN,HEAD,OVER,TAIL,DESC> m_oNub;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    TrayNL_t(void);
    ~TrayNL_t(void) noexcept(false);
  
    Nub_t<CN,HEAD,OVER,TAIL,DESC>* pNub(void);
  
  private:
    Footer* aFooter(void);
  };
  
  template <eCN CN, class HEAD, class OVER, class TAIL, class DESC, i8 BYTES>
  TrayNL_t<CN,HEAD,OVER,TAIL,DESC,BYTES>* _pNubNL(void* aAddr);
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
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
Nub_t<CN,HEAD,OVER,TAIL,DESC>::Nub_t(void)
: Object(CN)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  // head and tail must be a multiples of their units
  RASSERT(0 == sizeof(HEAD) % C_HEADUNIT);
  RASSERT(0 == sizeof(TAIL) % C_SPACEUNIT);

  m_nSpaceUnits = C_UNKNOWN;
  m_nHeadAllot  = C_UNKNOWN;

  m_nHeadItems  = 0;
  m_nHeadWords  = 0;
  m_nHeadBytes  = 0;
  m_nHeadBytes_used = 0;

  m_nTailItems  = 0;
  m_nTailUnits  = 0;
  m_nTailBytes  = 0;

  m_pDesc       = nullptr;

  m_axaUnits    = nullptr;
  m_axaWords    = nullptr;

  m_iHash       = 0;

  i8 nBytes = sizeof(Nub_t<CN,HEAD,OVER,TAIL,DESC>);
  _checkInitialisedSize_(nBytes);
  
  _postcondition_(_MAGIC_,CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
Nub_t<CN,HEAD,OVER,TAIL,DESC>::~Nub_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compute number of bytes in given number of space units                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::nBytes(i8 nUnits)
{
  if (! nUnits)
    return 0;
  
  i8 nBytes = nUnits * (i8)C_SPACEUNIT;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* compute over from head + relative distance (in units)                      */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
OVER* Nub_t<CN,HEAD,OVER,TAIL,DESC>::aGetOver(HEAD* aHead, i8 i_Over_rel)
{
  OVER* aOver = (OVER*)aGetTail(aHead,i_Over_rel);
  
  return aOver;
}

/*----------------------------------------------------------------------------*/
/* compute tail from head + relative distance (in units)                      */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
TAIL* Nub_t<CN,HEAD,OVER,TAIL,DESC>::aGetTail(HEAD* aHead, i8 i_Tail_rel)
{
  SpaceUnit* aHead_unit = (SpaceUnit*)MemLib::aToBoundary(aHead,C_SPACEUNIT);
  
  SpaceUnit* aTail_unit = (SpaceUnit*)(aHead_unit + i_Tail_rel);
  
  return (TAIL*)aTail_unit;
}

/*----------------------------------------------------------------------------*/
/* compute number of space units required to hold n bytes                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::nUnits(i8 nBytes)
{
  i8 nUnits = MemLib::nQuantaRequired(nBytes,C_SPACEUNIT);

  return nUnits;
}

/*----------------------------------------------------------------------------*/
/* compute unit difference between a head and a tail                          */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::iUnitDiff(HEAD* aHead, TAIL* aTail)
{
  SpaceUnit* aHead_unit = (SpaceUnit*)MemLib::aToBoundary(aHead,C_SPACEUNIT);
  SpaceUnit* aTail_unit = (SpaceUnit*)MemLib::aToBoundary(aTail,C_SPACEUNIT);
  
  // n.b. when you subtract two pointers of the same type the result is
  // the distance (in array elements) between the two. (it is not the
  // difference in bytes between the addresses)
  
  // compute the unit difference
  i8 i_Diff_units = aTail_unit - aHead_unit;
  
  return i_Diff_units;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot head item (with size as per given number of bytes)                   */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::iAllotHead(i8 nBytes)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nBytes_revised = (C_BYTES_CODEHEAD == nBytes)
                        ? C_UNITS_CODEHEAD * sizeof(Word)
                        : nBytes;
  
  if (nFreeBytes() < nBytes)
    return C_UNKNOWN;
  
  i8 nWords = MemLib::nQuantaRequired(nBytes_revised,C_HEADUNIT);
  RASSERT(nWords * (i8)C_HEADUNIT >= nBytes_revised);
  
  m_nHeadBytes += nBytes_revised;
  m_nHeadWords += nWords;
  i8 i_Head = m_nHeadItems++;

  m_nHeadAllot = nBytes;
  
  HEAD* aHead = aGetHead(i_Head);
  
  aHead->v1B(0,(C_BYTES_CODEHEAD == nBytes) ? C_U_codehead : nWords);
  
  return i_Head;
}

/*----------------------------------------------------------------------------*/
/* allot overflow item (with size as per given number of bytes)               */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::iAllotOver(i8 nBytes)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 i_Over = iAllotTail(nBytes);
  
  return i_Over;
}

/*----------------------------------------------------------------------------*/
/* allot tail item (with size as per given number of bytes)                   */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::iAllotTail(i8 nBytes)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  RASSERT(nBytes);
  
  if (nFreeBytes() < nBytes)
    return C_UNKNOWN;

  i8 nUnits = MemLib::nQuantaRequired(nBytes,C_SPACEUNIT);
  RASSERT(nUnits * (i8)C_SPACEUNIT >= nBytes);

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
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::nAreaBytes(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nBytes = C_SPACEUNIT * m_nSpaceUnits;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get the item describer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
DESC* Nub_t<CN,HEAD,OVER,TAIL,DESC>::pDesc(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_pDesc;
}

/*----------------------------------------------------------------------------*/
/* set the item describer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
void Nub_t<CN,HEAD,OVER,TAIL,DESC>::vDesc(DESC* pDesc)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_pDesc = pDesc;
}

/*----------------------------------------------------------------------------*/
/* get total number of free bytes                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::nFreeBytes(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nFreeBytes = (m_nSpaceUnits * C_SPACEUNIT)
                     - m_nHeadBytes
                     - (m_nTailUnits * C_SPACEUNIT);
  
  return nFreeBytes;
}

/*----------------------------------------------------------------------------*/
/* get most recent head item                                                  */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
HEAD* Nub_t<CN,HEAD,OVER,TAIL,DESC>::aGetHead(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  ASSERT(m_nHeadItems);
  
  i8 i_Unit = (m_nHeadItems - 1) * (m_nHeadAllot / C_SPACEUNIT);
  HEAD* aHead = (HEAD*)&m_axaUnits[i_Unit];
  
  return aHead;
}

/*----------------------------------------------------------------------------*/
/* get a specific head item                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
HEAD* Nub_t<CN,HEAD,OVER,TAIL,DESC>::aGetHead(i8 i_Head)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  ASSERT(i_Head >= 0 && i_Head < m_nHeadItems);
  
  i8 i_Word = i_Head * (m_nHeadAllot / C_HEADUNIT);
  HEAD* aHead = (HEAD*)&m_axaWords[i_Word];
    
  return aHead;
}

/*----------------------------------------------------------------------------*/
/* get a specific overflow item                                               */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
OVER* Nub_t<CN,HEAD,OVER,TAIL,DESC>::aGetOver(i8 i_Over)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  OVER* aOver = (OVER*)aGetTail(i_Over);
  
  return aOver;
}

/*----------------------------------------------------------------------------*/
/* get a specific tail item                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
TAIL* Nub_t<CN,HEAD,OVER,TAIL,DESC>::aGetTail(i8 i_Tail)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  ASSERT(m_nSpaceUnits - m_nTailUnits >= 0 && i_Tail < m_nSpaceUnits);
  
  TAIL* aTail = (TAIL*)&m_axaUnits[i_Tail];
  
  return aTail;
}

/*----------------------------------------------------------------------------*/
/* get encoding hash                                                          */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::iHash(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_iHash;
}

/*----------------------------------------------------------------------------*/
/* inc encoding hash                                                          */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
void Nub_t<CN,HEAD,OVER,TAIL,DESC>::vHashInc(i8 iDelta)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_iHash += iDelta;
}

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
bool Nub_t<CN,HEAD,OVER,TAIL,DESC>::bHasSpace(i8 nBytes)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);

  i8 nFreeBytes = this->nFreeBytes();
  
  if (nFreeBytes < nBytes)
    return false;
  else
    return true;
}

/*----------------------------------------------------------------------------*/
/* get current number of head items                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::nHeads(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_nHeadItems;
}

/*----------------------------------------------------------------------------*/
/* get current total of head bytes                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::nHeadBytes(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_nHeadBytes;
}

/*----------------------------------------------------------------------------*/
/* get head bytes actually used                                               */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::nHeadBytes_used(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_nHeadBytes_used;
}

/*----------------------------------------------------------------------------*/
/* set head bytes actually used                                               */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
void Nub_t<CN,HEAD,OVER,TAIL,DESC>::vHeadBytes_used(i8 nBytes)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_nHeadBytes_used = nBytes;
}

/*----------------------------------------------------------------------------*/
/* get total number of actual items                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::nItems(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nItems = m_nHeadItems + m_nTailItems;
  
  return nItems;
}

/*----------------------------------------------------------------------------*/
/* get last tail                                                              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::iLastTail(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  if (! m_nTailUnits)
    return C_UNKNOWN;
  
  i8 i_Tail = m_nSpaceUnits - m_nTailUnits; // 10 - 3 : [0 1 2 .. 7 8 9];
  
  return i_Tail;
}

/*----------------------------------------------------------------------------*/
/* get prev tail (goes left-to-right / ascending memory address)              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::iPrevTail(i8 i_CurrTail)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  TAIL* aPriorTail = aGetTail(i_CurrTail);
  i8 nPriorUnits = aPriorTail->w8_1B(0);
  //RASSERT(nPriorUnits <= 4);
  
  SpaceUnit* aPrevTail_unit = (SpaceUnit*)aPriorTail + nPriorUnits;
  if (aPrevTail_unit >= &m_axaUnits[m_nSpaceUnits])
    return C_UNKNOWN;

  i8 i_Tail = i_CurrTail + nPriorUnits;
  return i_Tail;
}

/*----------------------------------------------------------------------------*/
/* reset the nub                                                              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
void Nub_t<CN,HEAD,OVER,TAIL,DESC>::vReset(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_nHeadItems = 0;
  m_nHeadWords = 0;
  m_nHeadBytes = 0;

  m_nTailItems = 0;
  m_nTailUnits = 0;
  m_nTailBytes = 0;
  
  MemLib::vZero(m_axaUnits,(C_SPACEUNIT * m_nSpaceUnits),CN);
  
  m_iHash      = 0;
}

/*----------------------------------------------------------------------------*/
/* get current number of tail items                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::nTails(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_nTailItems;
}

/*----------------------------------------------------------------------------*/
/* get current total number of used bytes                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::nUsedBytes(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nBytes = m_nHeadBytes + (m_nTailUnits * C_SPACEUNIT);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* set item array                                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
void Nub_t<CN,HEAD,OVER,TAIL,DESC>::vSetItems(i8 nSpaceUnits, SpaceUnit* axaUnits)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_nSpaceUnits = nSpaceUnits;
  
  m_axaUnits    = axaUnits;
  m_axaWords    = (Word*)axaUnits;
}

/*----------------------------------------------------------------------------*/
/* verify tail lines of given nub                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
void Nub_t<CN,HEAD,OVER,TAIL,DESC>::vVerifyTails(i8 i_Dump)
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
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
Footer* Nub_t<CN,HEAD,OVER,TAIL,DESC>::aFooter(void)
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
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC, i8 BYTES>
TrayNL_t<CN,HEAD,OVER,TAIL,DESC,BYTES>* Structures::_pNubNL(void* aAddr)
{
  TrayNL_t<CN,HEAD,OVER,TAIL,DESC,BYTES>* pVector =
    reinterpret_cast<TrayNL_t<CN,HEAD,OVER,TAIL,DESC,BYTES>*>(aAddr);
  
  RASSERT_TYPE(pVector,CN);
  
  return pVector;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC, i8 BYTES>
void* TrayNL_t<CN,HEAD,OVER,TAIL,DESC,BYTES>::operator new(size_t)
{
  void* pNub = HeapMem::pAllocate(CN);
  
  return pNub;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC, i8 BYTES>
void TrayNL_t<CN,HEAD,OVER,TAIL,DESC,BYTES>::operator delete(void* pAddr)
{
  TrayNL_t<CN,HEAD,OVER,TAIL,DESC,BYTES>* pNub =
    _pNubNL<CN,HEAD,OVER,TAIL,DESC,BYTES>(pAddr);
  
  HeapMem::vFree(pNub);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC, i8 BYTES>
TrayNL_t<CN,HEAD,OVER,TAIL,DESC,BYTES>::TrayNL_t(void)
: Object(CN)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  MemLib::vZero(&m__filler,sizeof(m__filler),CN);

  i8 nSpaceUnits = BYTES / C_SPACEUNIT;
  m_oNub.vSetItems(nSpaceUnits,&m_xaUnits[0]);
  
  m_oNub.vReset();
  
  _setFooter_();
  
  i8 nBytes = sizeof(TrayNL_t<CN,HEAD,OVER,TAIL,DESC,BYTES>);
  _checkInitialisedSize_(nBytes);
  
  _postcondition_(_MAGIC_,CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC, i8 BYTES>
TrayNL_t<CN,HEAD,OVER,TAIL,DESC,BYTES>::~TrayNL_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the inner nub / interface                                              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC, i8 BYTES>
Nub_t<CN,HEAD,OVER,TAIL,DESC>*
  TrayNL_t<CN,HEAD,OVER,TAIL,DESC,BYTES>::pNub(void)
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
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC, i8 BYTES>
Footer* TrayNL_t<CN,HEAD,OVER,TAIL,DESC,BYTES>::aFooter(void)
{
  _precondition_(_MAGIC_,CN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
