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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator <                                                                 */
/*----------------------------------------------------------------------------*/
bool Key::operator < (Key* aKeyArg)
{
  i8 iDatumA = *(i8*)aByteSequence();
  i8 iDatumB = *(i8*)aKeyArg->aByteSequence();
  bool b = iDatumA < iDatumB;
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* operator <=                                                                */
/*----------------------------------------------------------------------------*/
bool Key::operator <= (Key* aKeyArg)
{
  i8 iDatumA = *(i8*)aByteSequence();
  i8 iDatumB = *(i8*)aKeyArg->aByteSequence();
  
  bool b = iDatumA <= iDatumB;
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* set key from given key workspace                                           */
/*----------------------------------------------------------------------------*/
void Key::vKey(KeyWrk& xKeyWrk)
{
  char* aDst = aByteSequence();
  char* aSrc = (char*)xKeyWrk.aKey();
  i8 nBytes  = xKeyWrk.nKeySize();
  
  MemLib::vCopy(aDst,aSrc,nBytes,CN_KEY);
}

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
  
  char* aCursorThis = aKey1->aByteSequence();
  char* aCursorThat = aKey2->aByteSequence();
  
  i8 iCmp = MemLib::iCompareWords(aCursorThis, aCursorThat, iCmpBytes);
  
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
  
  char* aCursorThis = aKey1->aByteSequence();
  char* aCursorThat = aKey2->aByteSequence();
  
  i8 iCmp = MemLib::iCompareWords(aCursorThis, aCursorThat, iCmpBytes);
  
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
  
  char* aCursorThis = aKey1->aByteSequence();
  char* aCursorThat = aKey2->aByteSequence();
  
  i8 iCmp = MemLib::iCompareWords(aCursorThis, aCursorThat, iCmpBytes);
  
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
  
  char* aCursorThis = aKey1->aByteSequence();
  char* aCursorThat = aKey2->aByteSequence();
  
  i8 iCmp = MemLib::iCompareWords(aCursorThis, aCursorThat, iCmpBytes);
  
  if (iCmp == 0)
    return KC_SAME;
  
  if (iCmp < 0)
    return KC_BEFORE;
  
  return KC_AFTER;
}

/*----------------------------------------------------------------------------*/
/* get next element from key                                                  */
/*----------------------------------------------------------------------------*/
char* Key::aEachDatum(Column* pColumn, i8& nBytes, char** aaCursor)
{
  char* aEachDatum = pColumn->aValue(nBytes,aaCursor);
  return aEachDatum;
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
/* show the contents of this key                                              */
/*----------------------------------------------------------------------------*/
void Key::vShow(IdxColVec* pIdxColVec)
{
  i8 nBytes;
  
  char* aCursor = aByteSequence();
  
  char caLiteral[128];
  
  for (i8 i_Column = 0; i_Column < pIdxColVec->nItems(); i_Column++) {
    
    Column* pColumn = pIdxColVec->pGet(i_Column);
    
    char* aDatum = this->aEachDatum(pColumn,nBytes,&aCursor);
    
    eDM eDatum = pColumn->eDatumType();
    
    if (eDatum == DM_VCH) {
      nBytes = StrLib::iWordFlipBack(caLiteral, aDatum);
    }
    else
      Record::vSprint(pColumn,(Stz*)caLiteral,(Stz*)aDatum);
    vTrace("Elm %lld:(size %lld value:%s) "_Stz,i_Column,nBytes,caLiteral);
  }
  
  //vTrace("%s)\n"_Stz, sKeyString.aStz());
}

/*----------------------------------------------------------------------------*/
/* get first datum (assuming int)                                             */
/*----------------------------------------------------------------------------*/
i8 Key::iGetInt(void)
{
  i8 iVal = *(i8*)aByteSequence();
  
  return iVal;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
