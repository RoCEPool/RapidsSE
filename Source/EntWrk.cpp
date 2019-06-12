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
  _precondition_(_MAGIC_,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compare key1 and key2                                                      */
/*----------------------------------------------------------------------------*/
eKC EntWrk::eCompare(Ent* aEnt1, EntWrk& xEntWrk2)
{
  i8 nKeySize1 = aEnt1->nKeyBytes();
  i8 nKeySize2 = xEntWrk2.nKeyBytes();

  i8 nCmpBytes = nKeySize1 < nKeySize2 ? nKeySize1 : nKeySize2;

  byte* ayKey1Bytes = aEnt1->ayKeyBytes();
  byte* ayKey2Bytes = xEntWrk2.ayKeyBytes();

  i8 iCmp = MemLib::iCompareWords(ayKey1Bytes,ayKey2Bytes,nCmpBytes);

  if (iCmp == 0) {
    if (nKeySize1 == nKeySize2)
      return KC_SAME;

    if (nKeySize1 < nKeySize2)
      return KC_BEFORE;

    if (nKeySize1 > nKeySize2)
      return KC_AFTER;
  }

  if (iCmp < 0)
    return KC_BEFORE;

  return KC_AFTER;
}

/*----------------------------------------------------------------------------*/
/* compare key1 and key2                                                      */
/*----------------------------------------------------------------------------*/
eKC EntWrk::eCompare(EntWrk& xEntWrk1, EntWrk& xEntWrk2)
{
  i8 nKey1Size = xEntWrk1.nKeyBytes();
  i8 nKey2Size = xEntWrk2.nKeyBytes();
  
  i8 nCmpBytes = nKey1Size < nKey2Size ? nKey1Size : nKey2Size;
  
  byte* ayKey1Bytes = xEntWrk1.ayKeyBytes();
  byte* ayKey2Bytes = xEntWrk2.ayKeyBytes();
  
  i8 iCmp = MemLib::iCompareWords(ayKey1Bytes,ayKey2Bytes,nCmpBytes);
  
  if (iCmp == 0)
    return KC_SAME;
  
  if (iCmp < 0)
    return KC_BEFORE;
  
  return KC_AFTER;
}

/*----------------------------------------------------------------------------*/
/* compare key1 and key2                                                      */
/*----------------------------------------------------------------------------*/
eKC EntWrk::eCompare(EntWrk& xEntWrk1, Ent* aEnt2)
{
  i8 nKeySize1 = xEntWrk1.nKeyBytes();
  i8 nKeySize2 = aEnt2->nKeyBytes();
  
  i8 nCmpBytes = nKeySize1 < nKeySize2 ? nKeySize1 : nKeySize2;
  
  byte* ayKey1Bytes = xEntWrk1.ayKeyBytes();
  byte* ayKey2Bytes = aEnt2->ayKeyBytes();
  
  i8 iCmp = MemLib::iCompareWords(ayKey1Bytes,ayKey2Bytes,nCmpBytes);
  
  if (iCmp == 0)
    return KC_SAME;
  
  if (iCmp < 0)
    return KC_BEFORE;
  
  return KC_AFTER;
}

/*----------------------------------------------------------------------------*/
/* compare key1 and key2                                                      */
/*----------------------------------------------------------------------------*/
eKC EntWrk::eCompare(Ent* aEnt1, Ent* aEnt2)
{
  i8 nKeySize1 = aEnt1->nKeyBytes();
  i8 nKeySize2 = aEnt2->nKeyBytes();
  
  i8 nCmpBytes = nKeySize1 < nKeySize2 ? nKeySize1 : nKeySize2;
  
  byte* ayKey1Bytes = aEnt1->ayKeyBytes();
  byte* ayKey2Bytes = aEnt2->ayKeyBytes();
  
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
bool EntWrk::bGreaterEqual(Ent* aEnt1, EntWrk& xEntWrk2)
{
  eKC eCmp = eCompare(aEnt1, xEntWrk2);
  return eCmp == KC_AFTER || eCmp == KC_SAME ? true : false;
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
bool EntWrk::bLessEqual(EntWrk& xEntWrk1, Ent* aEnt2)
{
  eKC eCmp = eCompare(xEntWrk1, aEnt2);
  return eCmp == KC_BEFORE || eCmp == KC_SAME ? true : false;
}

/*----------------------------------------------------------------------------*/
/* if key1 less than key2 ?                                                   */
/*----------------------------------------------------------------------------*/
bool EntWrk::bLessThan(EntWrk& xEntWrk1, Ent* aEnt2)
{
  eKC eCmp = eCompare(xEntWrk1, aEnt2);
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
void EntWrk::vAddElement(Row* pRow, Column* pCol)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 nBytes = 0;
  
  char* aDst = (char*)ayElementCrs();
  char* aSrc = (char*)pRow->m_aiValue[pCol->iColumnNo()];
  
  if (pRow->m_aiValue[pCol->iColumnNo()] == C_DATUM_NULL) {
    nBytes = DtmLib::nCopy(pCol->eDatumType(),aDst,&aSrc);
    vIncKeyBytes(nBytes);
    return;
  }
  
  if (DM_VCH == pCol->eDatumType()) {
    nBytes = StrLib::iWordFlip(aDst, aSrc);
  }
  else {
    nBytes = DtmLib::nCopy(pCol->eDatumType(),aDst,&aSrc);
  }
  
  vIncKeyBytes(nBytes);
}

/*----------------------------------------------------------------------------*/
/* add min lower value to key                                                 */
/*----------------------------------------------------------------------------*/
void EntWrk::vAddMinusInfinity(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  char* aDst = (char*)ayElementCrs();
  
  i8 iInt = C_DATUM_MINUS_INFINITY;
  Int::nCopy(aDst,(void*)&iInt);
  
  vIncKeyBytes(C_BYTES_PER_WORD);
}

/*----------------------------------------------------------------------------*/
/* add max upper value to key                                                 */
/*----------------------------------------------------------------------------*/
void EntWrk::vAddPlusInfinity(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  char* aDst = (char*)ayElementCrs();
  
  i8 iInt = C_DATUM_PLUS_INFINITY;
  Int::nCopy(aDst,(void*)&iInt);
  
  vIncKeyBytes(C_BYTES_PER_WORD);
}

/*----------------------------------------------------------------------------*/
/* get buffer no from id                                                      */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iBuffer(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 iBuffer = m_xEnt.iBuffer();
  
  return iBuffer;
}

/*----------------------------------------------------------------------------*/
/* set buffer no in id                                                        */
/*----------------------------------------------------------------------------*/
void EntWrk::vBuffer(i8 iBuffer)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xEnt.vBuffer(iBuffer);
}

/*----------------------------------------------------------------------------*/
/* get element cursor (where to add next element)                             */
/*----------------------------------------------------------------------------*/
byte* EntWrk::ayElementCrs(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 nBytesUsed = m_xEnt.nKeyBytes();

  byte* ayNext = aAdd<byte*>(ayKeyBytes(),nBytesUsed);

  return ayNext;
}

/*----------------------------------------------------------------------------*/
/* form given entry from this working entry                                   */
/*----------------------------------------------------------------------------*/
void EntWrk::vFormEntry(Ent* pEntry)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set entry id and levels
  
  pEntry->vRef(xRef());
  pEntry->vLevels(nLevels());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key into entry
  
  Key* aSrcKey  = reinterpret_cast<Key*>(ayKeyBytes());
  Key* aDstKey  = _aKey(pEntry->ayKeyBytes());
  i8   nKeySize = this->nKeyBytes();
  
  MemLib::vCopy(aDstKey,aSrcKey,nKeySize,CN_ENT);
}

/*----------------------------------------------------------------------------*/
/* compute hash of key                                                        */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iHashKey(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 iHash = HshLib::iHash(ayKeyBytes(),nKeyBytes());
  
  return iHash;
}

/*----------------------------------------------------------------------------*/
/* inc total key bytes by size of element                                     */
/*----------------------------------------------------------------------------*/
void EntWrk::vIncKeyBytes(i8 nElementBytes)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xEnt.aKey()->vIncKeyBytes(nElementBytes);
}

/*----------------------------------------------------------------------------*/
/* get item no from id                                                        */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iItem(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_xEnt.iItem();
}

/*----------------------------------------------------------------------------*/
/* set item no in id                                                          */
/*----------------------------------------------------------------------------*/
void EntWrk::vItem(i8 iItem)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xEnt.vItem(iItem);
}

/*----------------------------------------------------------------------------*/
/* get key bytes                                                              */
/*----------------------------------------------------------------------------*/
byte* EntWrk::ayKeyBytes(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return m_xEnt.ayKeyBytes();
}

/*----------------------------------------------------------------------------*/
/* set key                                                                    */
/*----------------------------------------------------------------------------*/
void EntWrk::vKeyBytes(i8 nKeySize, byte* ayKeyBytes)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xEnt.vKeyBytes(nKeySize);
  
  MemLib::vCopy(this->ayKeyBytes(),ayKeyBytes,nKeySize,CN_ENTWRK);
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
/* set key size                                                               */
/*----------------------------------------------------------------------------*/
void EntWrk::vKeyBytes(i8 nKeyBytes)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xEnt.vKeyBytes(nKeyBytes);
}

/*----------------------------------------------------------------------------*/
/* get nLevel                                                                 */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nLevels(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_xEnt.nLevels();
}

/*----------------------------------------------------------------------------*/
/* set number of levels                                                       */
/*----------------------------------------------------------------------------*/
void EntWrk::vLevels(i8 nLevels)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xEnt.vLevels(nLevels);
}

/*----------------------------------------------------------------------------*/
/* get the size of the next entry offset(s) in bytes                          */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nNEOsize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 nBytes = m_xEnt.nNEOsize();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* set a random number of levels                                              */
/*----------------------------------------------------------------------------*/
void EntWrk::vRandomLevels(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  Task* pTask = Task::pExecutingTask();
  
  i8 nLevels = Random::iLogLevel(3,C_FLAT_FREQ + 1,pTask->i_NextRnd()) + 1;

  m_xEnt.vLevels(nLevels);
}

/*----------------------------------------------------------------------------*/
/* get ref (sub-ref to child entBuffer or row-ref at H0 leaf buffers to row)  */
/*----------------------------------------------------------------------------*/
Id& EntWrk::xRef(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_xEnt.xRef();
}

/*----------------------------------------------------------------------------*/
/* set reference                                                              */
/*----------------------------------------------------------------------------*/
void EntWrk::vRef(Id& xRef)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xEnt.vRef(xRef);
}

/*----------------------------------------------------------------------------*/
/* set this entwrk using given details                                        */
/*----------------------------------------------------------------------------*/
void EntWrk::vSetEnt(i8 iBufferNo, i8 i_Item, Ent* pEntry)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set id, transaction and offsets
  
  i8 nLevels = pEntry->nLevels();
  vLevels(nLevels);
  
  Id xRef(iBufferNo,i_Item,nLevels);
  vRef(xRef);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key into this entry
  
  byte* aKeyBytes = pEntry->ayKeyBytes();
  i8 nKeySize = pEntry->nKeyBytes();
  RASSERT(nKeySize % 8 == 0);
  
  vKeyBytes(nKeySize,aKeyBytes);
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void EntWrk::vShow(IdxColVec* pIdxColVec)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  Key* aKey = reinterpret_cast<Key*>(ayKeyBytes());
  aKey->vShow(pIdxColVec);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get the size {Ent,Key}                                                     */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nSize_EntKey(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 nKeyBytes = this->nKeyBytes();
  
  i8 nBytes = sizeof(Ent) + nKeyBytes;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get the size {Ent,Key,Neo}                                                 */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nSize_EntKeyNeo(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
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
