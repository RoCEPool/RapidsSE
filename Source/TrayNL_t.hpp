/*============================================================================*/
/* TrayNL_t.hpp - double-headed tray of items template                        */
/*============================================================================*/
#pragma once

namespace Structures
{
  static const i8 C_SPACEUNIT_QUANTA = sizeof(SpaceUnit);
  static const i8 C_TAILLINE_QUANTA  = sizeof(DoubleWord);
  static const i8 C_HEADLINE_QUANTA  = sizeof(SingleWord);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // nub (the tray interface)
  
  template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
  class Nub_t : public Object
  {
  private:
    i8 m_iEncoding;   //
  
    i8 m_nSpaceUnits; // number of space units
    i8 m_jHeadSize;   // size of head to allot
  
    i8 m_nHeadItems;  // number of items allocated from the head
    i8 m_nHeadWords;  // number of words allocated from the head
    i8 m_nHeadBytes;  // number of bytes allocated from the head
    i8 m_nHeadBytes_used;  // number of bytes used in the large encode headline
 
    i8 m_nHeadDeleted;  // number of items deleted
    
    i8 m_nTailItems;  // number of items allocated from the tail
    i8 m_nTailUnits;  // number of space units allocated from the tail
    i8 m_nTailBytes;  // number of bytes allocated from the tail
  
    i8 m_nStrItems;
    i8 m_i_LastStr;   // last string tail

    DESC* m_pDesc;    // object describing the items
  
    SpaceUnit* m_axaUnits; // array of space units
    SingleWord*      m_axaWords; // array of space units as word units
  
    // must be last
    Footer m_xFooter;
  
  public:
    Nub_t(void);
    ~Nub_t(void) noexcept(false);
  
    static i8    nBytes(i8 nUnits);
    static OVER* aGetOver_rel(HEAD* aHead, i8 i_Over_rel);
    static TAIL* aGetTail_rel(HEAD* aHead, i8 i_Tail_rel);
    static i8    nUnits(i8 nBytes);
    static i8    iUnitDiff(HEAD* aHead, TAIL* aTail);

    i8    iAllotHead(i8 nBytes_unrounded);
    HEAD* aAllotHead(i8 nHeadSize);
    i8    iAllotOver(i8 nBytes_unrounded);
    OVER* aAllotOver(i8 nOverSize);
    i8    iAllotTail(i8 nBytes_unrounded);
    TAIL* aAllotTail(i8 nTailSize);
    i8    nAllotedBytes(void);
    i8    nAreaBytes(void);
    void  vDelete(HEAD* aHeadLine);
    DESC* pDesc(void);
    void  vDesc(DESC* pDesc);
    void  vDumpStrings(void);
    void  vDumpTails(void);
    i8    iFindTail(TAIL* aFindLine);
    TAIL* aFindTail(TAIL* aFindLine, i8& i_Tail);
    i8    nFreeAllotableBytes(void);
    HEAD* aGetHead(void);
    HEAD* aGetHead(i8 i_Head);
    OVER* aGetOver(i8 i_Over);
    TAIL* aGetTail(i8 i_Tail);
    i8    nHeads(void);
    i8    nHeadBytes(void);
    i8    nHeadBytes_used(void);
    void  vHeadBytes_used(i8 nBytes);
    i8    nHeadsDeleted(void);
    bool  bHasSpace(i8 nBytes);
    i8    nItems(void);
    TAIL* aLastStr(void);
    i8    iLastTail(void);
    TAIL* aLastTail(void);
    i8    iPrevTail(i8 i_Tail);
    TAIL* aPrevStr(TAIL* aCurrStr, i8& i_Tail);
    TAIL* aPrevTail(TAIL* aCurrTail, i8& i_Tail);
    void  vReset(void);
    void  vSetItems(i8 nSpaceUnits, SpaceUnit* axaUnits);
    void  vSetLastStr(i8 i_LastStr);
    i8    nStrs(void);
    i8    nTails(void);
    i8    iToUnit(TAIL* aTail);
    void  vVerifyTails(i8 i_Dump);
  
  private:
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tray (the outer tray)

  template <eCN CN, class HEAD, class OVER, class TAIL, class DESC, i8 BYTES>
  class TrayNL_t : public Object
  {
  protected:
  
    i8 m__filler[1]; // fill so m_xaUnits starts on a double word boundry
  
    //alignas(DoubleWord)
    SpaceUnit m_xaUnits[BYTES / C_SPACEUNIT_QUANTA]; // array of space units
  
    Nub_t<CN,HEAD,OVER,TAIL,DESC> m_oNub;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    TrayNL_t(void);
    ~TrayNL_t(void) noexcept(false);
  
    Nub_t<CN,HEAD,OVER,TAIL,DESC>* pNub(void);
    void vRestoreNub(void);
    
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
  RASSERT(0 == sizeof(HEAD) % C_HEADLINE_QUANTA);
  RASSERT(0 == sizeof(TAIL) % C_SPACEUNIT_QUANTA);

  m_iEncoding   = 1;
  m_nSpaceUnits = C_UNKNOWN;
  m_jHeadSize   = C_UNKNOWN;

  m_nHeadItems  = 0;
  m_nHeadWords  = 0;
  m_nHeadBytes  = 0;
  m_nHeadBytes_used = 0;
  m_nHeadDeleted = 0;
  
  m_nTailItems  = 0;
  m_nTailUnits  = 0;
  m_nTailBytes  = 0;

  m_nStrItems   = 0;
  m_i_LastStr   = 0;

  m_pDesc       = nullptr;

  m_axaUnits    = nullptr;
  m_axaWords    = nullptr;

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
  
  i8 nBytes = nUnits * (i8)C_SPACEUNIT_QUANTA;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* compute over from head + relative distance (in units)                      */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
OVER* Nub_t<CN,HEAD,OVER,TAIL,DESC>::aGetOver_rel(HEAD* aHead, i8 i_Over_rel)
{
  OVER* aOver = (OVER*)aGetTail_rel(aHead,i_Over_rel);
  
  return aOver;
}

/*----------------------------------------------------------------------------*/
/* compute tail from head + relative distance (in units)                      */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
TAIL* Nub_t<CN,HEAD,OVER,TAIL,DESC>::aGetTail_rel(HEAD* aHead, i8 i_Tail_rel)
{
  SpaceUnit* aHead_unit = (SpaceUnit*)MemLib::aToBoundary(aHead,C_SPACEUNIT_QUANTA);
  
  SpaceUnit* aTail_unit = (SpaceUnit*)(aHead_unit + i_Tail_rel);
  
  return (TAIL*)aTail_unit;
}

/*----------------------------------------------------------------------------*/
/* compute number of space units required to hold n bytes                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::nUnits(i8 nBytes)
{
  i8 nUnits = MemLib::nQuantaRequired(nBytes,C_SPACEUNIT_QUANTA);

  return nUnits;
}

/*----------------------------------------------------------------------------*/
/* compute unit difference between a head and a tail                          */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::iUnitDiff(HEAD* aHead, TAIL* aTail)
{
  SpaceUnit* aHead_unit = (SpaceUnit*)MemLib::aToBoundary(aHead,C_SPACEUNIT_QUANTA);
  SpaceUnit* aTail_unit = (SpaceUnit*)MemLib::aToBoundary(aTail,C_SPACEUNIT_QUANTA);
  RASSERT((TAIL*)aTail_unit == aTail);
  
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
/* allot head item (with <W> size computed from given number of bytes)        */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::iAllotHead(i8 nBytes_unrounded)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nBytes_revised = nBytes_unrounded;
    
  if (nFreeAllotableBytes() < nBytes_revised)
    return C_UNKNOWN;
  
  i8 nWords = MemLib::nQuantaRequired(nBytes_revised,C_HEADLINE_QUANTA);
  
  m_nHeadBytes += nBytes_revised;
  m_nHeadWords += nWords;
  i8 i_Head = m_nHeadItems++;

  m_jHeadSize = nBytes_revised;
  
  HEAD* aHead = aGetHead(i_Head);
  
  // set <W>
  aHead->vW(nWords);
  
  return i_Head;
}

/*----------------------------------------------------------------------------*/
/* allot a head line                                                          */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
HEAD* Nub_t<CN,HEAD,OVER,TAIL,DESC>::aAllotHead(i8 nHeadSize)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);

  // allot a head line in nub (the <W> is init'd by iAllotHead())
  i8 i_Head = iAllotHead(nHeadSize);
  
  if (C_UNKNOWN == i_Head)
    return nullptr;
  
  HEAD* aHeadLine = aGetHead(i_Head);
  
  return aHeadLine;
}

/*----------------------------------------------------------------------------*/
/* allot overflow item (with <U> size computed from given number of bytes)    */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::iAllotOver(i8 nBytes_unrounded)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 i_Over = iAllotTail(nBytes_unrounded);
  
  return i_Over;
}

/*----------------------------------------------------------------------------*/
/* allot a over line                                                          */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
OVER* Nub_t<CN,HEAD,OVER,TAIL,DESC>::aAllotOver(i8 nOverSize)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);

  // allot the over line (the <U> is init'd by iAllotOver())
  i8 i_Over = iAllotOver(nOverSize);
  OVER* aOverLine = aGetOver(i_Over);
  
  return aOverLine;
}

/*----------------------------------------------------------------------------*/
/* allot tail item (with <U> size computed from given number of bytes)        */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::iAllotTail(i8 nBytes_unrounded)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  RASSERT(nBytes_unrounded);
  
  if (nFreeAllotableBytes() < nBytes_unrounded)
    return C_UNKNOWN;

  i8 nUnits = MemLib::nQuantaRequired(nBytes_unrounded,C_SPACEUNIT_QUANTA);

  m_nTailBytes += nBytes_unrounded;
  m_nTailUnits += nUnits;
  m_nTailItems++;

  i8 i_Tail = m_nSpaceUnits - m_nTailUnits;
  
  TAIL* aTail = aGetTail(i_Tail);
  
  // set <U>
  aTail->vU(nUnits);
  
  return i_Tail;
}

/*----------------------------------------------------------------------------*/
/* allot a tail line                                                          */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
TAIL* Nub_t<CN,HEAD,OVER,TAIL,DESC>::aAllotTail(i8 nTailSize)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 i_Tail = iAllotTail(nTailSize);
  TAIL* aTailLine = aGetTail(i_Tail);
  
  return aTailLine;
}

/*----------------------------------------------------------------------------*/
/* get current total number of alloted bytes                                  */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::nAllotedBytes(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nBytes = m_nHeadBytes + (m_nTailUnits * C_SPACEUNIT_QUANTA);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get total space area (used + free)                                         */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::nAreaBytes(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nBytes = C_SPACEUNIT_QUANTA * m_nSpaceUnits;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* delete an item                                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
void Nub_t<CN,HEAD,OVER,TAIL,DESC>::vDelete(HEAD* aHeadLine)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  aHeadLine->vDelete();
  m_nHeadDeleted++;
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
/* dump tail strings                                                          */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
void Nub_t<CN,HEAD,OVER,TAIL,DESC>::vDumpStrings(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);

  for (i8 i_Tail = m_i_LastStr;
       i_Tail != C_UNKNOWN;
       i_Tail = iPrevTail(i_Tail)) {
    
    TAIL* aTailLine = aGetTail(i_Tail);
    
    char* azStr = aAdd<char*>(aTailLine,(C_U_1B + C_L_1B));
    
    vTrace("Repeating Str @%lld : <U %lld> <L %lld> %s\n"_Stz,
           i_Tail,
           aTailLine->iU(),
           aTailLine->iL(),
           azStr);
    
//    if (i_Tail == m_i_LastStr)
//      break;
  }
}

/*----------------------------------------------------------------------------*/
/* dump tail lines                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
void Nub_t<CN,HEAD,OVER,TAIL,DESC>::vDumpTails(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);

  for (i8 i_Tail = iLastTail();
       i_Tail != C_UNKNOWN;
       i_Tail = iPrevTail(i_Tail)) {
    
    TAIL* aTailLine = aGetTail(i_Tail);
    
    char* azStr = aAdd<char*>(aTailLine,(C_U_1B + C_L_1B));
    
    vTrace("Tail %lld : <U %lld> <L %lld> %s\n"_Stz,
           i_Tail,
           aTailLine->iU(),
           aTailLine->iL(),
           azStr);
  }
}


/*----------------------------------------------------------------------------*/
/* search for duplicate tail line                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::iFindTail(TAIL* aFindLine)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);

  for (i8 i_Tail = iLastTail();
       i_Tail != C_UNKNOWN;
       i_Tail = iPrevTail(i_Tail)) {
    
    TAIL* aTailLine = aGetTail(i_Tail);
    i8 nTailBytes = aTailLine->nBytes();
    
    //aFindLine->vDump();
    //aTailLine->vDump();
    
    if (0 == MemLib::iCompareBytes(aTailLine,aFindLine,nTailBytes))
      return i_Tail;
  }
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* search for duplicate tail line                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
TAIL* Nub_t<CN,HEAD,OVER,TAIL,DESC>::aFindTail(TAIL* aFindLine, i8& i_Tail)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);

  for (TAIL* aStrLine = aLastStr();
       aStrLine;
       aStrLine = aPrevStr(aStrLine,i_Tail)) {
    
    i8 nTailBytes = aStrLine->nBytes();
    
    //aFindLine->vDump();
    //aTailLine->vDump();
    
    if (0 == MemLib::iCompareBytes(aStrLine,aFindLine,nTailBytes)) {
      i_Tail = iToUnit(aStrLine);
      return aStrLine;
    }
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get total number of allotable free bytes remaining                         */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::nFreeAllotableBytes(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nFreeBytes = (m_nSpaceUnits * C_SPACEUNIT_QUANTA)
                     - m_nHeadBytes
                     - (m_nTailUnits * C_SPACEUNIT_QUANTA);
  
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
  
  i8 i_Unit = (m_nHeadItems - 1) * (m_jHeadSize / C_SPACEUNIT_QUANTA);
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
  
  i8 i_Word = i_Head * (m_jHeadSize / C_HEADLINE_QUANTA);
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
  
  RASSERT(aTail == (TAIL*)MemLib::aToBoundary(aTail,C_SPACEUNIT_QUANTA));
  
  return aTail;
}

/*----------------------------------------------------------------------------*/
/* does buffer have space to allot nBytes ?                                   */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
bool Nub_t<CN,HEAD,OVER,TAIL,DESC>::bHasSpace(i8 nBytes)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);

  i8 nFreeBytes = this->nFreeAllotableBytes();
  
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
/* get current total of alloted head bytes                                    */
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
/* get current number of head items                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::nHeadsDeleted(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_nHeadDeleted;
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
/* get last string line                                                       */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
TAIL* Nub_t<CN,HEAD,OVER,TAIL,DESC>::aLastStr(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  if (! m_i_LastStr)
    return nullptr;
  
  TAIL* aLastStr = (TAIL*) & m_axaUnits[m_i_LastStr];
  
  return aLastStr;
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
  
  i8 i_LastTail = m_nSpaceUnits - m_nTailUnits; // 10 - 3 : [0 1 2 .. 7 8 9];
  
  return i_LastTail;
}

/*----------------------------------------------------------------------------*/
/* get last tail                                                              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
TAIL* Nub_t<CN,HEAD,OVER,TAIL,DESC>::aLastTail(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  if (! m_nTailUnits)
    return nullptr;
  
  i8 i_LastTail = m_nSpaceUnits - m_nTailUnits; // 10 - 3 : [0 1 2 .. 7 8 9];
  
  TAIL* aLastTail = (TAIL*) & m_axaUnits[i_LastTail];

  return aLastTail;
}

/*----------------------------------------------------------------------------*/
/* get prev string line (goes left-to-right / ascending memory address)       */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
TAIL* Nub_t<CN,HEAD,OVER,TAIL,DESC>::aPrevStr(TAIL* aCurrStr, i8& i_Tail)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nCurrUnits = aCurrStr->w8_1B(0);
  
  SpaceUnit* aPrevStr_unit = (SpaceUnit*)aCurrStr + nCurrUnits;
  if (aPrevStr_unit >= &m_axaUnits[m_nSpaceUnits])
    return nullptr;
  
  TAIL* aPrevStr = (TAIL*) aPrevStr_unit;
  
  i_Tail = iToUnit(aPrevStr);
  
  return aPrevStr;
}

/*----------------------------------------------------------------------------*/
/* get prev tail (goes left-to-right / ascending memory address)              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::iPrevTail(i8 i_CurrTail)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  TAIL* aCurrTail = aGetTail(i_CurrTail);
  i8 nCurrUnits = aCurrTail->w8_1B(0);
  
  SpaceUnit* aCurrTail_unit = (SpaceUnit*)aCurrTail + nCurrUnits;
  if (aCurrTail_unit >= &m_axaUnits[m_nSpaceUnits])
    return C_UNKNOWN;

  i8 i_PrevTail = i_CurrTail + nCurrUnits;
  return i_PrevTail;
}

/*----------------------------------------------------------------------------*/
/* get prev tail (goes left-to-right / ascending memory address)              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
TAIL* Nub_t<CN,HEAD,OVER,TAIL,DESC>::aPrevTail(TAIL* aCurrTail, i8& i_Tail)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nCurrUnits = aCurrTail->w8_1B(0);
  
  SpaceUnit* aPrevTail_unit = (SpaceUnit*)aCurrTail + nCurrUnits;
  if (aPrevTail_unit >= &m_axaUnits[m_nSpaceUnits])
    return nullptr;
  
  TAIL* aPrevTail = (TAIL*) aPrevTail_unit;
  
  i_Tail = iToUnit(aPrevTail);
  
  return aPrevTail;
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
  m_nHeadBytes_used = 0;

  m_nTailItems = 0;
  m_nTailUnits = 0;
  m_nTailBytes = 0;
  
  MemLib::vZero(m_axaUnits,(C_SPACEUNIT_QUANTA * m_nSpaceUnits),CN);
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
  m_axaWords    = (SingleWord*)axaUnits;
}

/*----------------------------------------------------------------------------*/
/* set last string subscript                                                  */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
void Nub_t<CN,HEAD,OVER,TAIL,DESC>::vSetLastStr(i8 i_LastStr)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  m_i_LastStr = i_LastStr;
  m_nStrItems++;
}

/*----------------------------------------------------------------------------*/
/* get current number of str items                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::nStrs(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  return m_nStrItems;
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
/* compute the i_Unit subscript of given tail                                 */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
i8 Nub_t<CN,HEAD,OVER,TAIL,DESC>::iToUnit(TAIL* aTail)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  // how far (in bytes) into m_axaUnits
  i8 nBytes = nDifference(aTail,m_axaUnits);
  
  // how far (in units) into m_axaUnits
  i8 i_Unit = nBytes / (i8)sizeof(SpaceUnit);
  
  return i_Unit;
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

  //vTrace("this : %p ; &m_xaUnits[0] : %p\n"_Stz,this,&m_xaUnits[0]);
  ASSERT(DoubleWord::bOnDoubleWordBoundary(m_xaUnits));
  
  MemLib::vZero(&m__filler,sizeof(m__filler),CN);

  i8 nSpaceUnits = BYTES / C_SPACEUNIT_QUANTA;
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
/* get the inner nub / interface                                              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class HEAD, class OVER, class TAIL, class DESC, i8 BYTES>
void TrayNL_t<CN,HEAD,OVER,TAIL,DESC,BYTES>::vRestoreNub(void)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  i8 nSpaceUnits = BYTES / C_SPACEUNIT_QUANTA;
  m_oNub.vSetItems(nSpaceUnits,&m_xaUnits[0]);
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
