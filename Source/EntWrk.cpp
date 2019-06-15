/*============================================================================*/
/* EntWrk.cpp -- Working Index Entry                                          */
/*============================================================================*/

#include "Data.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* EntWrk::operator new(size_t)
{
  vFatal("fatal: EntWrk class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* EntWrk::operator new(size_t, EntWrk* pEntWrk)
{
  return pEntWrk;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntWrk::EntWrk(void)
: Object(CN_ENTWRK),
  m_xEnt(CN_ENT,sizeof(Ent))
{
  _precondition_(_MAGIC_,CN_ENTWRK);

  m_xEnt.vBuffer(0);
  m_xEnt.vItem(0);
  m_xEnt.vLevels(0);
  
  m_xEnt.vKeyBytes(0);

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m__KeyBytes,sizeof(m__KeyBytes),CN_ENTWRK);
#else
  m__KeyBytes[0] = '\0';
#endif
  
  _checkInitialisedSize_(sizeof(EntWrk));
  
  _postcondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntWrk::~EntWrk(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compare key1 and key2                                                      */
/*----------------------------------------------------------------------------*/
eKC EntWrk::eCompare(Ent* aEnt1, Ent* aEnt2)
{
  i8 nKeySize1 = aEnt1->nKeyBytes();
  i8 nKeySize2 = aEnt2->nKeyBytes();
  
  i8 nCmpBytes = nKeySize1 < nKeySize2 ? nKeySize1 : nKeySize2;
  
  byte* ayKey1Bytes = aEnt1->ayByteSeq();
  byte* ayKey2Bytes = aEnt2->ayByteSeq();
  
  i8 iCmp = MemLib::iCompareWords(ayKey1Bytes,ayKey2Bytes,nCmpBytes);
  
  if (iCmp == 0)
    return KC_SAME;
  
  if (iCmp < 0)
    return KC_BEFORE;
  
  return KC_AFTER;
}

/*----------------------------------------------------------------------------*/
/* get next element from key                                                  */
/*----------------------------------------------------------------------------*/
byte* EntWrk::ayEachElm(Column* pColumn, i8& nBytes, byte** aayCursor)
{
  byte* ayElm = pColumn->ayValue(nBytes,aayCursor);
  
  return ayElm;
}

/*----------------------------------------------------------------------------*/
/* if key1 is equal to key2 ?                                                 */
/*----------------------------------------------------------------------------*/
bool EntWrk::bEqualTo(Ent* aEnt1, Ent* aEnt2)
{
  eKC eCmp = eCompare(aEnt1, aEnt2);
  return eCmp == KC_SAME ? true : false;
}

/*----------------------------------------------------------------------------*/
/* if key1 greater than key2 ?                                                */
/*----------------------------------------------------------------------------*/
bool EntWrk::bGreaterThan(Ent* aEnt1, Ent* aEnt2)
{
  eKC eCmp = eCompare(aEnt1, aEnt2);
  return eCmp == KC_AFTER ? true : false;
}

/*----------------------------------------------------------------------------*/
/* if key1 greater than equal to key2 ?                                       */
/*----------------------------------------------------------------------------*/
bool EntWrk::bGreaterEqual(Ent* aEnt1, Ent* aEnt2)
{
  eKC eCmp = eCompare(aEnt1, aEnt2);
  return eCmp == KC_AFTER || eCmp == KC_SAME ? true : false;
}

/*----------------------------------------------------------------------------*/
/* if key1 less than eqiual to key2 ?                                         */
/*----------------------------------------------------------------------------*/
bool EntWrk::bLessEqual(Ent* aEnt1, Ent* aEnt2)
{
  eKC eCmp = eCompare(aEnt1, aEnt2);
  return eCmp == KC_BEFORE || eCmp == KC_SAME ? true : false;
}

/*----------------------------------------------------------------------------*/
/* if key1 less than key2 ?                                                   */
/*----------------------------------------------------------------------------*/
bool EntWrk::bLessThan(Ent* aEnt1, Ent* aEnt2)
{
  eKC eCmp = eCompare(aEnt1, aEnt2);
  return eCmp == KC_BEFORE ? true : false;
}

/*----------------------------------------------------------------------------*/
/* if key1 not equal to key2 ?                                                */
/*----------------------------------------------------------------------------*/
bool EntWrk::bNotEqual(Ent* aEnt1, Ent* aEnt2)
{
  eKC eCmp = eCompare(aEnt1, aEnt2);
  return eCmp != KC_SAME ? true : false;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add element to key                                                         */
/*----------------------------------------------------------------------------*/
void EntWrk::vAppendElement(Row* pRow, Column* pCol)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 nBytes = 0;
  
  i8 iValue = pRow->m_aiValue[pCol->iColumnNo()];
  
  if (Null::bIsNull(iValue)) {
    byte* ayDst = (byte*)ayElementCrs();
    nBytes = Null::nCopy(ayDst);
    vIncKeyBytes(nBytes);
    return;
  }
  
  
  if (DM_VCH == pCol->eDatumType()) {
    char* aDst = (char*)ayElementCrs();
    char* aSrc = (char*)iValue;
    nBytes = StrLib::iWordFlip(aDst, aSrc);
    vIncKeyBytes(nBytes);
  }
  
  byte* ayDst = ayElementCrs();
  nBytes = DtmLib::nCopy(ayDst,iValue);
  vIncKeyBytes(nBytes);
}

/*----------------------------------------------------------------------------*/
/* add min lower value to key                                                 */
/*----------------------------------------------------------------------------*/
void EntWrk::vAppendMinusInfinity(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  byte* ayDst = ayElementCrs();
  DtmLib::vCopy(ayDst,(i8)C_DATUM_MINUS_INFINITY);

  vIncKeyBytes(C_BYTES_PER_WORD);
}

/*----------------------------------------------------------------------------*/
/* add max upper value to key                                                 */
/*----------------------------------------------------------------------------*/
void EntWrk::vAppendPlusInfinity(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
   
  byte* ayDst = ayElementCrs();
  DtmLib::vCopy(ayDst,(i8)C_DATUM_PLUS_INFINITY);
  
  vIncKeyBytes(C_BYTES_PER_WORD);
}

/*----------------------------------------------------------------------------*/
/* get buffer no from ref                                                     */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iBuffer(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 iBuffer = m_xEnt.iBuffer();
  
  return iBuffer;
}

/*----------------------------------------------------------------------------*/
/* set buffer no in re                                                        */
/*----------------------------------------------------------------------------*/
void EntWrk::vBuffer(i8 iBuffer)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xEnt.vBuffer(iBuffer);
}

/*----------------------------------------------------------------------------*/
/* get byte sequence of key                                                   */
/*----------------------------------------------------------------------------*/
byte* EntWrk::ayByteSeq(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return m_xEnt.ayByteSeq();
}

/*----------------------------------------------------------------------------*/
/* set byte sequence of key                                                   */
/*----------------------------------------------------------------------------*/
void EntWrk::vByteSeq(i8 nKeyBytes, byte* ayByteSeq)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xEnt.vKeyBytes(nKeyBytes);
  
  MemLib::vCopy(this->ayByteSeq(),ayByteSeq,nKeyBytes,CN_ENTWRK);
}

/*----------------------------------------------------------------------------*/
/* get element cursor (where to add next element)                             */
/*----------------------------------------------------------------------------*/
byte* EntWrk::ayElementCrs(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 nKeyBytes = m_xEnt.nKeyBytes();

  byte* ayNextElm = aAdd<byte*>(ayByteSeq(),nKeyBytes);

  return ayNextElm;
}

/*----------------------------------------------------------------------------*/
/* get entry pointer                                                          */
/*----------------------------------------------------------------------------*/
Ent* EntWrk::aEnt(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return &m_xEnt;
}

/*----------------------------------------------------------------------------*/
/* form given entry from this working entry                                   */
/*----------------------------------------------------------------------------*/
void EntWrk::vFormEntry(Ent* pEntry)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set entry id and levels
  
  pEntry->vRef(xRef());
  pEntry->vLevels(nLevels());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key into entry
  
  byte* aSrcKey = ayByteSeq();
  byte* aDstKey = pEntry->ayByteSeq();
  i8   nKeySize = this->nKeyBytes();
  
  MemLib::vCopy(aDstKey,aSrcKey,nKeySize,CN_ENT);
}

/*----------------------------------------------------------------------------*/
/* compute hash of key byte sequence                                          */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iHashKey(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 iHash = HshLib::iHash(ayByteSeq(),nKeyBytes());
  
  return iHash;
}

/*----------------------------------------------------------------------------*/
/* inc total key bytes by the size of element                                 */
/*----------------------------------------------------------------------------*/
void EntWrk::vIncKeyBytes(i8 nElementBytes)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xEnt.aKey()->vIncKeyBytes(nElementBytes);
}

/*----------------------------------------------------------------------------*/
/* get item no from entry reference                                           */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iItem(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return m_xEnt.iItem();
}

/*----------------------------------------------------------------------------*/
/* set item no in entry reference                                             */
/*----------------------------------------------------------------------------*/
void EntWrk::vItem(i8 iItem)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xEnt.vItem(iItem);
}

/*----------------------------------------------------------------------------*/
/* get key size                                                               */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nKeyBytes(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 nSize = m_xEnt.nKeyBytes();
  
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* set key byte sequence                                                      */
/*----------------------------------------------------------------------------*/
void EntWrk::vKeyBytes(i8 nKeyBytes)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xEnt.vKeyBytes(nKeyBytes);
}

/*----------------------------------------------------------------------------*/
/* get number of levels                                                       */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nLevels(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return m_xEnt.nLevels();
}

/*----------------------------------------------------------------------------*/
/* set number of levels                                                       */
/*----------------------------------------------------------------------------*/
void EntWrk::vLevels(i8 nLevels)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xEnt.vLevels(nLevels);
}

/*----------------------------------------------------------------------------*/
/* get the size of the next entry offset(s) in bytes                          */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nNEOsize(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 nBytes = m_xEnt.nNEOsize();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* set a random number of levels                                              */
/*----------------------------------------------------------------------------*/
void EntWrk::vRandomLevels(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  Task* pTask = Task::pExecutingTask();
  
  i8 nLevels = Random::iLogLevel(3,C_FLAT_FREQ + 1,pTask->i_NextRnd()) + 1;

  m_xEnt.vLevels(nLevels);
}

/*----------------------------------------------------------------------------*/
/* get ref (sub-ref to child entBuffer or row-ref at H0 leaf buffers to row)  */
/*----------------------------------------------------------------------------*/
Id& EntWrk::xRef(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return m_xEnt.xRef();
}

/*----------------------------------------------------------------------------*/
/* set ref (sub-ref to child entBuffer or row-ref at H0 leaf buffers to row)  */
/*----------------------------------------------------------------------------*/
void EntWrk::vRef(Id& xRef)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xEnt.vRef(xRef);
}

/*----------------------------------------------------------------------------*/
/* set this entwrk using given details                                        */
/*----------------------------------------------------------------------------*/
void EntWrk::vSetEnt(i8 iBufferNo, i8 i_Item, Ent* pEntry)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set id, transaction and offsets
  
  i8 nLevels = pEntry->nLevels();
  vLevels(nLevels);
  
  Id xRef(iBufferNo,i_Item,nLevels);
  vRef(xRef);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key byte sequence into this entry
  
  byte* ayByteSeq = pEntry->ayByteSeq();
  i8 nKeyBytes = pEntry->nKeyBytes();
  RASSERT(nKeyBytes % 8 == 0);
  
  vByteSeq(nKeyBytes,ayByteSeq);
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void EntWrk::vShow(IdxColVec* pIdxColVec)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  Key* aKey = reinterpret_cast<Key*>(ayByteSeq());
  aKey->vShow(pIdxColVec);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get the size of this (Ent+Key)                                             */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nSize_EntKey(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 nKeyBytes = this->nKeyBytes();
  
  i8 nBytes = sizeof(Ent) + nKeyBytes;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get the size this (Ent+Key+Neo)                                           */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nSize_EntKeyNeo(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 nKeyBytes = this->nKeyBytes();
  
  i8 nNeoBytes = nNEOsize();
  
  i8 nBytes = sizeof(Ent) + nKeyBytes + nNeoBytes;
  
  return nBytes;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(EntWrk)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
