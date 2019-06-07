/*============================================================================*/
/* Key.cpp -- key part of an index entry                                      */
/*============================================================================*/

#include "Data.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Key* Data::_aKey(void* aAddr)
{
  Key* aKey = reinterpret_cast<Key*>(aAddr);
  
  return aKey;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Key::operator new(size_t)
{
  vFatal("fatal: Key class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Key::operator new(size_t, Key* aKey)
{
  return aKey;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Key::Key(KeyWrk& xKeyWrk)
{
  vKey(xKeyWrk);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Key::~Key(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compare key1 and key2                                                      */
/*----------------------------------------------------------------------------*/
eKC Key::eCompare(Ent* aKeyItem1, KeyWrk& xKeyItem2)
{
  i8 nKeySize1 = aKeyItem1->nSizeData();
  i8 nKeySize2 = xKeyItem2.nKeySize();
  
  Key* aKey1 = _aKey(aKeyItem1->aDataArea());
  Key* aKey2 = xKeyItem2.aKey();
  
  i8 iCmpBytes = nKeySize1 < nKeySize2 ? nKeySize1 : nKeySize2;
  
  byte* ayCursorThis = aKey1->ayByteSequence();
  byte* ayCursorThat = aKey2->ayByteSequence();
  
  i8 iCmp = MemLib::iCompareWords(ayCursorThis, ayCursorThat, iCmpBytes);
  
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
eKC Key::eCompare(KeyWrk& xKeyItem1, KeyWrk& xKeyItem2)
{
  i8 nKeySize1 = xKeyItem1.nKeySize();
  i8 nKeySize2 = xKeyItem2.nKeySize();
  
  Key* aKey1 = xKeyItem1.aKey();
  Key* aKey2 = xKeyItem2.aKey();
  
  i8 iCmpBytes = nKeySize1 < nKeySize2 ? nKeySize1 : nKeySize2;
  
  byte* ayCursorThis = aKey1->ayByteSequence();
  byte* ayCursorThat = aKey2->ayByteSequence();
  
  i8 iCmp = MemLib::iCompareWords(ayCursorThis, ayCursorThat, iCmpBytes);
  
  if (iCmp == 0)
    return KC_SAME;
  
  if (iCmp < 0)
    return KC_BEFORE;
  
  return KC_AFTER;
}

/*----------------------------------------------------------------------------*/
/* compare key1 and key2                                                      */
/*----------------------------------------------------------------------------*/
eKC Key::eCompare(KeyWrk& xKeyItem1, Ent* aKeyItem2)
{
  i8 nKeySize1 = xKeyItem1.nKeySize();
  i8 nKeySize2 = aKeyItem2->nSizeData();
  
  Key* aKey1 = xKeyItem1.aKey();
  Key* aKey2 = _aKey(aKeyItem2->aDataArea());
  
  i8 iCmpBytes = nKeySize1 < nKeySize2 ? nKeySize1 : nKeySize2;
  
  byte* ayCursorThis = aKey1->ayByteSequence();
  byte* ayCursorThat = aKey2->ayByteSequence();
  
  i8 iCmp = MemLib::iCompareWords(ayCursorThis, ayCursorThat, iCmpBytes);
  
  if (iCmp == 0)
    return KC_SAME;
  
  if (iCmp < 0)
    return KC_BEFORE;
  
  return KC_AFTER;
}

/*----------------------------------------------------------------------------*/
/* compare key1 and key2                                                      */
/*----------------------------------------------------------------------------*/
eKC Key::eCompare(Ent* aKeyItem1, Ent* aKeyItem2)
{
  i8 nKeySize1 = aKeyItem1->nSizeData();
  i8 nKeySize2 = aKeyItem2->nSizeData();
  
  Key* aKey1 = _aKey(aKeyItem1->aDataArea());
  Key* aKey2 = _aKey(aKeyItem2->aDataArea());
  
  i8 iCmpBytes = nKeySize1 < nKeySize2 ? nKeySize1 : nKeySize2;
  
  byte* ayCursorThis = aKey1->ayByteSequence();
  byte* ayCursorThat = aKey2->ayByteSequence();
  
  i8 iCmp = MemLib::iCompareWords(ayCursorThis, ayCursorThat, iCmpBytes);
  
  if (iCmp == 0)
    return KC_SAME;
  
  if (iCmp < 0)
    return KC_BEFORE;
  
  return KC_AFTER;
}

/*----------------------------------------------------------------------------*/
/* get next element from key                                                  */
/*----------------------------------------------------------------------------*/
byte* Key::ayEachElm(Column* pColumn, i8& nBytes, byte** aayCursor)
{
  byte* ayElm = pColumn->ayValue(nBytes,aayCursor);
  
  return ayElm;
}

/*----------------------------------------------------------------------------*/
/* if key1 is equal to key2 ?                                                 */
/*----------------------------------------------------------------------------*/
bool Key::bEqualTo(Ent* aKeyItem1, Ent* aKeyItem2)
{
  eKC eCmp = eCompare(aKeyItem1, aKeyItem2);
  return eCmp == KC_SAME ? true : false;
}

/*----------------------------------------------------------------------------*/
/* if key1 greater than key2 ?                                                */
/*----------------------------------------------------------------------------*/
bool Key::bGreaterThan(Ent* aKeyItem1, Ent* aKeyItem2)
{
  eKC eCmp = eCompare(aKeyItem1, aKeyItem2);
  return eCmp == KC_AFTER ? true : false;
}

/*----------------------------------------------------------------------------*/
/* if key1 greater than equal to key2 ?                                       */
/*----------------------------------------------------------------------------*/
bool Key::bGreaterEqual(Ent* aKeyItem1, KeyWrk& xKeyItem2)
{
  eKC eCmp = eCompare(aKeyItem1, xKeyItem2);
  return eCmp == KC_AFTER || eCmp == KC_SAME ? true : false;
}

/*----------------------------------------------------------------------------*/
/* if key1 greater than equal to key2 ?                                       */
/*----------------------------------------------------------------------------*/
bool Key::bGreaterEqual(Ent* aKeyItem1, Ent* aKeyItem2)
{
  eKC eCmp = eCompare(aKeyItem1, aKeyItem2);
  return eCmp == KC_AFTER || eCmp == KC_SAME ? true : false;
}

/*----------------------------------------------------------------------------*/
/* if key1 less than eqiual to key2 ?                                         */
/*----------------------------------------------------------------------------*/
bool Key::bLessEqual(KeyWrk& xKeyItem1, Ent* aKeyItem2)
{
  eKC eCmp = eCompare(xKeyItem1, aKeyItem2);
  return eCmp == KC_BEFORE || eCmp == KC_SAME ? true : false;
}

/*----------------------------------------------------------------------------*/
/* if key1 less than key2 ?                                                   */
/*----------------------------------------------------------------------------*/
bool Key::bLessThan(KeyWrk& xKeyItem1, Ent* aKeyItem2)
{
  eKC eCmp = eCompare(xKeyItem1, aKeyItem2);
  return eCmp == KC_BEFORE ? true : false;
}

/*----------------------------------------------------------------------------*/
/* if key1 not equal to key2 ?                                                */
/*----------------------------------------------------------------------------*/
bool Key::bNotEqual(Ent* aKeyItem1, Ent* aKeyItem2)
{
  eKC eCmp = eCompare(aKeyItem1, aKeyItem2);
  return eCmp != KC_SAME ? true : false;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator <                                                                 */
/*----------------------------------------------------------------------------*/
bool Key::operator < (Key* aKeyArg)
{
  i8 iDatumA = *(i8*)ayByteSequence();
  i8 iDatumB = *(i8*)aKeyArg->ayByteSequence();
  bool b = iDatumA < iDatumB;
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* operator <=                                                                */
/*----------------------------------------------------------------------------*/
bool Key::operator <= (Key* aKeyArg)
{
  i8 iDatumA = *(i8*)ayByteSequence();
  i8 iDatumB = *(i8*)aKeyArg->ayByteSequence();
  
  bool b = iDatumA <= iDatumB;
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get first datum (assuming int)                                             */
/*----------------------------------------------------------------------------*/
i8 Key::iGetInt(void)
{
  i8 iVal = *(i8*)ayByteSequence();
  
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* set key from given key workspace                                           */
/*----------------------------------------------------------------------------*/
void Key::vKey(KeyWrk& xKeyWrk)
{
  byte* ayDst = ayByteSequence();
  byte* aySrc = (byte*)xKeyWrk.aKey();
  i8 nBytes  = xKeyWrk.nKeySize();
  
  MemLib::vCopy(ayDst,aySrc,nBytes,CN_KEY);
}

/*----------------------------------------------------------------------------*/
/* show the contents of this key                                              */
/*----------------------------------------------------------------------------*/
void Key::vShow(IdxColVec* pIdxColVec)
{
  i8 nBytes;
  
  byte* ayCursor = ayByteSequence();
  
  char caLiteral[128];
  
  for (i8 i_Co = 0; i_Co < pIdxColVec->nItems(); i_Co++) {
    
    Column* pColumn = pIdxColVec->pGet(i_Co);
    
    byte* ayElm = this->ayEachElm(pColumn,nBytes,&ayCursor);
    
    eDM eDatum = pColumn->eDatumType();
    
    if (eDatum == DM_VCH) {
      nBytes = StrLib::iWordFlipBack(caLiteral, (char*)ayElm);
    }
    else
      Record::vSprint(pColumn,(Stz*)caLiteral,(Stz*)ayElm);
    vTrace("Elm %lld:(size %lld value:%s) "_Stz,i_Co,nBytes,caLiteral);
  }
  
  //vTrace("%s)\n"_Stz, sKeyString.aStz());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
