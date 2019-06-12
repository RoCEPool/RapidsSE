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
: Object(CN_ENTWRK)
{
  _precondition_(_MAGIC_,CN_ENTWRK);
  
  // check initialisation of rowid
  ASSERT(0 == m_xId.iBuffer() &&
         0 == m_xId.iItem()   &&
         0 == m_xId.nLevels());
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_ENTWRK);
#endif
  
  m_xKey.vSizeKey(C_UNKNOWN);
  
  m_xWsp.vInit();
  
  m_ayElementCrs = m_xWsp.ayArea();

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
/* compare left key with right key                                            */
/*----------------------------------------------------------------------------*/
//eKC EntWrk::eCompare(Key* aLeft, Key* aRight)
//{
//}

/*----------------------------------------------------------------------------*/
/* compare key1 and key2                                                      */
/*----------------------------------------------------------------------------*/
eKC EntWrk::eCompare(Ent* aEnt1, EntWrk& xEntWrk2)
{
  i8 nKeySize1 = aEnt1->nSizeKey();
  i8 nKeySize2 = xEntWrk2.nKeySize();

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
  i8 nKey1Size = xEntWrk1.nKeySize();
  i8 nKey2Size = xEntWrk2.nKeySize();
  
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
  i8 nKeySize1 = xEntWrk1.nKeySize();
  i8 nKeySize2 = aEnt2->nSizeKey();
  
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
  i8 nKeySize1 = aEnt1->nSizeKey();
  i8 nKeySize2 = aEnt2->nSizeKey();
  
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
  
  char* aDst = (char*)m_xWsp.ayLatter();
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
  
  char* aDst = (char*)m_xWsp.ayLatter();
  
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
  
  char* aDst = (char*)m_xWsp.ayLatter();
  
  i8 iInt = C_DATUM_PLUS_INFINITY;
  Int::nCopy(aDst,(void*)&iInt);
  
  vIncKeyBytes(C_BYTES_PER_WORD);
}

/*----------------------------------------------------------------------------*/
/* get buffer no from id                                                      */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iBufferNo(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 iBuffer = m_xId.iBuffer();
  
  return iBuffer;
}

/*----------------------------------------------------------------------------*/
/* set buffer no in id                                                        */
/*----------------------------------------------------------------------------*/
void EntWrk::vBufferNo(i8 iBufferNo)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xId.vBuffer(iBufferNo);
}

/*----------------------------------------------------------------------------*/
/* get element cursor (where to add next element)                             */
/*----------------------------------------------------------------------------*/
byte* EntWrk::ayElementCrs(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return m_xWsp.ayLatter();
}

/*----------------------------------------------------------------------------*/
/* get the size {Ent,keyBytes}                                                */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nEntKeySize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 nKeyBytes = nKeySize();
  
  i8 nBytes = sizeof(Ent) + nKeyBytes;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get the size {Ent,keyBytes,Neo}                                            */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nEntKeyNeoSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 nKeyBytes = nKeySize();
  i8 nNeoBytes = nOffsetsSize();

  i8 nBytes = sizeof(Ent) + nKeyBytes + nNeoBytes;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* form given entry from this working entry                                   */
/*----------------------------------------------------------------------------*/
void EntWrk::vFormEntry(Ent* pEntry)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set entry id and levels
  
  pEntry->vId(xId());
  pEntry->vLevels(nLevels());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key into entry
  
  Key* aSrcKey  = reinterpret_cast<Key*>(m_xWsp.ayArea());
  Key* aDstKey  = _aKey(pEntry->ayKeyBytes());
  i8   nKeySize = this->nKeySize();
  
  MemLib::vCopy(aDstKey,aSrcKey,nKeySize,CN_ENT);
}

/*----------------------------------------------------------------------------*/
/* compute hash of key                                                        */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iHashKey(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 iHash = HshLib::iHash(m_xWsp.ayArea(),nKeySize());
  
  return iHash;
}

/*----------------------------------------------------------------------------*/
/* get row id                                                                 */
/*----------------------------------------------------------------------------*/
Id& EntWrk::xId(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_xId;
}

/*----------------------------------------------------------------------------*/
/* set row id                                                                 */
/*----------------------------------------------------------------------------*/
void EntWrk::vId(Id& xId)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xId = xId;
}

/*----------------------------------------------------------------------------*/
/* inc total key bytes by size of element                                     */
/*----------------------------------------------------------------------------*/
void EntWrk::vIncKeyBytes(i8 nElementBytes)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xWsp.vTake(nElementBytes);
}

/*----------------------------------------------------------------------------*/
/* get item no from id                                                        */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iItemNo(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_xId.iItem();
}

/*----------------------------------------------------------------------------*/
/* set item no in id                                                          */
/*----------------------------------------------------------------------------*/
void EntWrk::vItemNo(i8 i_Item)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xId.vItem(i_Item);
}

/*----------------------------------------------------------------------------*/
/* get key bytes                                                              */
/*----------------------------------------------------------------------------*/
byte* EntWrk::ayKeyBytes(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return m_xWsp.ayArea();
}

/*----------------------------------------------------------------------------*/
/* set key                                                                    */
/*----------------------------------------------------------------------------*/
void EntWrk::vKeyBytes(i8 nKeySize, byte* ayKeyBytes)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
   m_xWsp.vUsed(nKeySize);
  
  MemLib::vCopy(m_xWsp.ayArea(),ayKeyBytes,nKeySize,CN_ENTWRK);
}

/*----------------------------------------------------------------------------*/
/* get key size                                                               */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nKeySize(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 nSize = m_xWsp.nUsed();
  
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* set key size                                                               */
/*----------------------------------------------------------------------------*/
void EntWrk::vKeySize(i8 nKeySize)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xWsp.vUsed(nKeySize);
}

/*----------------------------------------------------------------------------*/
/* get nLevel                                                                 */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nLevels(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_xId.nLevels();
}

/*----------------------------------------------------------------------------*/
/* get the size of the next entry offset(s) in bytes                          */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nOffsetsSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 nLevels = m_xId.nLevels();
  i8 nBytes  = C_NEO_SIZE * nLevels;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* set number of levels                                                       */
/*----------------------------------------------------------------------------*/
void EntWrk::vLevels(i8 nLevels)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xId.vLevels(nLevels);
}

/*----------------------------------------------------------------------------*/
/* set a random number of levels                                              */
/*----------------------------------------------------------------------------*/
void EntWrk::vRandomLevels(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  Task* pTask = Task::pExecutingTask();
  
  i8 nLevels = Random::iLogLevel(3,C_FLAT_FREQ + 1,pTask->i_NextRnd()) + 1;

  m_xId.vLevels(nLevels);
}

/*----------------------------------------------------------------------------*/
/* set this entwrk using given details                                        */
/*----------------------------------------------------------------------------*/
void EntWrk::vSetWrk(i8 iBufferNo, i8 i_Item, Ent* pEntry)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set id, transaction and offsets
  
  i8 nLevels = pEntry->nLevels();
  vLevels(nLevels);
  
  Id xId(iBufferNo,i_Item,nLevels);
  vId(xId);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key into this entry
  
  byte* aKeyBytes = pEntry->ayKeyBytes();
  i8 nKeySize = pEntry->nSizeKey();
  RASSERT(nKeySize % 8 == 0);
  
  vKeyBytes(nKeySize,aKeyBytes);
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void EntWrk::vShow(IdxColVec* pIdxColVec)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  Key* aKey = reinterpret_cast<Key*>(m_xWsp.ayArea());
  aKey->vShow(pIdxColVec);
  vTrace("\n"_Stz);
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
