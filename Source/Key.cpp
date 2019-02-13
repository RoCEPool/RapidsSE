/*============================================================================*/
/* KeyData.cpp -- key part of an index entry                                      */
/*============================================================================*/

#include "Data.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
KeyData* Data::_aKeyData(void* aAddr)
{
  KeyData* aKey = reinterpret_cast<KeyData*>(aAddr);
  
  return aKey;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* KeyData::operator new(size_t)
{
  vFatal("fatal: KeyData class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* KeyData::operator new(size_t, KeyData* aKey)
{
  return aKey;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
KeyData::KeyData(KeyWrk& xKeyWrk)
{
  vKey(xKeyWrk);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
KeyData::~KeyData(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator <                                                                 */
/*----------------------------------------------------------------------------*/
bool KeyData::operator <(KeyData* aKeyArg)
{
  i8 iDatumA = *(i8*)aByteSequence();
  i8 iDatumB = *(i8*)aKeyArg->aByteSequence();
  bool b = iDatumA < iDatumB;
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* operator <=                                                                */
/*----------------------------------------------------------------------------*/
bool KeyData::operator <=(KeyData* aKeyArg)
{
  i8 iDatumA = *(i8*)aByteSequence();
  i8 iDatumB = *(i8*)aKeyArg->aByteSequence();
  
  bool b = iDatumA <= iDatumB;
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* set key from given key workspace                                           */
/*----------------------------------------------------------------------------*/
void KeyData::vKey(KeyWrk& xKeyWrk)
{
  char* aDst = aByteSequence();
  char* aSrc = (char*)xKeyWrk.aKey();
  i8 nBytes  = xKeyWrk.nKeySize();
  
  MemLib::vCopy(aDst,aSrc,nBytes,CN_KEY);
}

/*----------------------------------------------------------------------------*/
/* get key size                                                               */
/*----------------------------------------------------------------------------*/
i8 KeyData::nKeySize(IdxColVec* pIdxColVec)
{
  i8 nCols = pIdxColVec->nItems();
  i8 nKeySize = 0;
  i8 nBytes;
  char* aCursor = aByteSequence();
  
  for (i8 i_Column = 0; i_Column < nCols; i_Column++) {
    Column* pColumn = pIdxColVec->pGet(i_Column);
    
    char* aElement = aEachDatum(pColumn, nBytes, &aCursor);
    nKeySize += nBytes;
  }
  
  return nKeySize;
}

/*----------------------------------------------------------------------------*/
/* compare key1 and key2                                                      */
/*----------------------------------------------------------------------------*/
eKC KeyData::eCompare(Item* aKeyItem1, KeyWrk& xKeyItem2)
{  
  i8 nKeySize1 = aKeyItem1->nSizeData();
  i8 nKeySize2 = xKeyItem2.nKeySize();
  
  KeyData* aKey1 = _aKeyData(aKeyItem1->aDataArea());
  KeyData* aKey2 = xKeyItem2.aKey();
  
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
eKC KeyData::eCompare(KeyWrk& xKeyItem1, Item* aKeyItem2)
{
  i8 nKeySize1 = xKeyItem1.nKeySize();
  i8 nKeySize2 = aKeyItem2->nSizeData();
  
  KeyData* aKey1 = xKeyItem1.aKey();
  KeyData* aKey2 = _aKeyData(aKeyItem2->aDataArea());
  
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
char* KeyData::aEachDatum(Column* pColumn, i8& nBytes, char** aaCursor)
{
  char* aEachDatum = pColumn->aValue(nBytes,aaCursor);
  
  return aEachDatum;
}

/*----------------------------------------------------------------------------*/
/* if key1 is equal to key2 ?                                                 */
/*----------------------------------------------------------------------------*/
bool KeyData::bEqualTo(Item* aKeyItem1, Item* aKeyItem2)
{
  i8 nKeySize1 = aKeyItem1->nSizeData();
  i8 nKeySize2 = aKeyItem2->nSizeData();
  
  KeyData* aKey1 = _aKeyData(aKeyItem1->aDataArea());
  KeyData* aKey2 = _aKeyData(aKeyItem2->aDataArea());
  
  if (nKeySize1 != nKeySize2)
    return false;
  
  char* aCursorThis = aKey1->aByteSequence();
  char* aCursorThat = aKey2->aByteSequence();
  
  i8 iCmp = MemLib::iCompareWords(aCursorThis, aCursorThat, nKeySize1);
  
  if (iCmp == 0)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* if key1 greater than key2 ?                                                */
/*----------------------------------------------------------------------------*/
bool KeyData::bGreaterThan(Item* aKeyItem1, Item* aKeyItem2)
{
  i8 nKeySize1 = aKeyItem1->nSizeData();
  i8 nKeySize2 = aKeyItem2->nSizeData();
  
  KeyData* aKey1 = _aKeyData(aKeyItem1->aDataArea());
  KeyData* aKey2 = _aKeyData(aKeyItem2->aDataArea());
  
  i8 iCmpBytes = nKeySize1 < nKeySize2 ? nKeySize1 : nKeySize2;
  
  char* aCursorThis = aKey1->aByteSequence();
  char* aCursorThat = aKey2->aByteSequence();
  
  i8 iCmp = MemLib::iCompareWords(aCursorThis, aCursorThat, iCmpBytes);
  
  if (iCmp > 0)
    return true;
  
  if (iCmp == 0) {
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* if key1 greater than equal to key2 ?                                       */
/*----------------------------------------------------------------------------*/
bool KeyData::bGreaterEqual(Item* aKeyItem1, KeyWrk& xKeyItem2)
{
  i8 nKeySize1 = aKeyItem1->nSizeData();
  i8 nKeySize2 = xKeyItem2.nKeySize();
  
  KeyData* aKey1 = _aKeyData(aKeyItem1->aDataArea());
  KeyData* aKey2 = xKeyItem2.aKey();
  
  i8 iCmpBytes = nKeySize1 < nKeySize2 ? nKeySize1 : nKeySize2;
  
  char* aCursorThis = aKey1->aByteSequence();
  char* aCursorThat = aKey2->aByteSequence();
  
  i8 iCmp = MemLib::iCompareWords(aCursorThis, aCursorThat, iCmpBytes);
  
  if (iCmp > 0)
    return true;
  
  if (iCmp == 0)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* if key1 greater than equal to key2 ?                                       */
/*----------------------------------------------------------------------------*/
bool KeyData::bGreaterEqual(Item* aKeyItem1, Item* aKeyItem2)
{
  i8 nKeySize1 = aKeyItem1->nSizeData();
  i8 nKeySize2 = aKeyItem2->nSizeData();
  
  KeyData* aKey1 = _aKeyData(aKeyItem1->aDataArea());
  KeyData* aKey2 = _aKeyData(aKeyItem2->aDataArea());
  
  i8 iCmpBytes = nKeySize1 < nKeySize2 ? nKeySize1 : nKeySize2;
  
  char* aCursorThis = aKey1->aByteSequence();
  char* aCursorThat = aKey2->aByteSequence();
  
  i8 iCmp = MemLib::iCompareWords(aCursorThis, aCursorThat, iCmpBytes);
  
  if (iCmp > 0)
    return true;
  
  if (iCmp == 0)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* if key1 less than key2 ?                                                   */
/*----------------------------------------------------------------------------*/
bool KeyData::bLessThan(KeyWrk& xKeyItem1, Item* aKeyItem2)
{
  i8 nKeySize1 = xKeyItem1.nKeySize();
  i8 nKeySize2 = aKeyItem2->nSizeData();
  
  KeyData* aKey1 = xKeyItem1.aKey();
  KeyData* aKey2 = _aKeyData(aKeyItem2->aDataArea());
  
  i8 iCmpBytes = nKeySize1 < nKeySize2 ? nKeySize1 : nKeySize2;
  
  char* aCursorThis = aKey1->aByteSequence();
  char* aCursorThat = aKey2->aByteSequence();
    
  i8 iCmp = MemLib::iCompareWords(aCursorThis, aCursorThat, iCmpBytes);
    
  if (iCmp < 0)
    return true;
 
  if (iCmp == 0)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* if key1 not equal to key2 ?                                                */
/*----------------------------------------------------------------------------*/
bool KeyData::bNotEqual(Item* aKeyItem1, Item* aKeyItem2)
{
  i8 nKeySize1 = aKeyItem1->nSizeData();
  i8 nKeySize2 = aKeyItem2->nSizeData();
  
  KeyData* aKey1 = _aKeyData(aKeyItem1->aDataArea());
  KeyData* aKey2 = _aKeyData(aKeyItem2->aDataArea());
  
  if (nKeySize1 != nKeySize2)
    return true;
  
  char* aCursorThis = aKey1->aByteSequence();
  char* aCursorThat = aKey2->aByteSequence();
  
  i8 iCmp = MemLib::iCompareWords(aCursorThis, aCursorThat, nKeySize1);
  
  if (iCmp != 0)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* if this key less than given key?                                           */
/*----------------------------------------------------------------------------*/
//bool KeyData::bLessThan(KeyData* aKeyThat, IdxColVec* pIdxColVec)
//{
//  bool bLessThan = false;
//
//  char* aCursorThis = aByteSequence();
//  char* aCursorThat = aKeyThat->aByteSequence();
//
//  for (i8 i_Column = 0; i_Column < pIdxColVec->nItems(); i_Column++) {
//
//    Column* pColumn = pIdxColVec->pGet(i_Column);
//
//    i8 nBytesThis;
//    i8 nBytesThat;
//
//    char* aDatumThis = pColumn->aValue(nBytesThis,&aCursorThis);
//    char* aDatumThat = pColumn->aValue(nBytesThat,&aCursorThat);
//
//    bLessThan = pColumn->bLessThan(aDatumThis,aDatumThat);
//
//    if (!bLessThan)
//      break;
//  }
//
//  return bLessThan;
//}

/*----------------------------------------------------------------------------*/
/* if this key less than or equal to given key?                               */
/*----------------------------------------------------------------------------*/
//bool KeyData::bLessEqual(KeyData* aKeyThat, IdxColVec* pIdxColVec)
//{
//  bool bLessEqual = false;
//
//  char* aCursorThis = aByteSequence();
//  char* aCursorThat = aKeyThat->aByteSequence();
//
//  for (i8 i_Column = 0; i_Column < pIdxColVec->nItems(); i_Column++) {
//
//    Column* pColumn = pIdxColVec->pGet(i_Column);
//
//    i8 nBytesThis;
//    i8 nBytesThat;
//
//    char* aDatumThis = pColumn->aValue(nBytesThis,&aCursorThis);
//    char* aDatumThat = pColumn->aValue(nBytesThat,&aCursorThat);
//
//    bLessEqual = pColumn->bLessEqual(aDatumThis,aDatumThat);
//
//    if (!bLessEqual)
//      break;
//  }
//
//  return bLessEqual;
//}

/*----------------------------------------------------------------------------*/
/* if this key equal to given key?                                            */
/*----------------------------------------------------------------------------*/
//bool KeyData::bEqualTo(KeyData* aKeyThat, IdxColVec* pIdxColVec)
//{
//  bool bEqualTo = false;
//
//  char* aCursorThis = aByteSequence();
//  char* aCursorThat = aKeyThat->aByteSequence();
//
//  for (i8 i_Column = 0; i_Column < pIdxColVec->nItems(); i_Column++) {
//
//    Column* pColumn = pIdxColVec->pGet(i_Column);
//
//    i8 nBytesThis;
//    i8 nBytesThat;
//
//    char* aDatumThis = pColumn->aValue(nBytesThis,&aCursorThis);
//    char* aDatumThat = pColumn->aValue(nBytesThat,&aCursorThat);
//
//    bEqualTo = pColumn->bEqualTo(aDatumThis,aDatumThat);
//
//    if (!bEqualTo)
//      break;
//  }
//
//  return bEqualTo;
//}

/*----------------------------------------------------------------------------*/
/* if this key not equal to given key?                                        */
/*----------------------------------------------------------------------------*/
//bool KeyData::bNotEqual(KeyData* aKeyThat, IdxColVec* pIdxColVec)
//{
//  bool bNotEqual = false;
//
//  char* aCursorThis = aByteSequence();
//  char* aCursorThat = aKeyThat->aByteSequence();
//
//  for (i8 i_Column = 0; i_Column < pIdxColVec->nItems(); i_Column++) {
//
//    Column* pColumn = pIdxColVec->pGet(i_Column);
//
//    i8 nBytesThis;
//    i8 nBytesThat;
//
//    char* aDatumThis = pColumn->aValue(nBytesThis,&aCursorThis);
//    char* aDatumThat = pColumn->aValue(nBytesThat,&aCursorThat);
//
//    bNotEqual = pColumn->bNotEqual(aDatumThis,aDatumThat);
//    if (!bNotEqual)
//      break;
//  }
//
//  return bNotEqual;
//}

/*----------------------------------------------------------------------------*/
/* if this key greater than or equal to given key?                            */
/*----------------------------------------------------------------------------*/
//bool KeyData::bGreaterEqual(KeyData* aKeyThat, IdxColVec* pIdxColVec)
//{
//  bool bGreaterEqual = false;
//
//  char* aCursorThis = aByteSequence();
//  char* aCursorThat = aKeyThat->aByteSequence();
//
//  i8 nBytesThis;
//  i8 nBytesThat;
//
//  for (i8 i_Column = 0; i_Column < pIdxColVec->nItems(); i_Column++) {
//
//    Column* pColumn = pIdxColVec->pGet(i_Column);
//
//    char* aDatumThis = pColumn->aValue(nBytesThis,&aCursorThis);
//    char* aDatumThat = pColumn->aValue(nBytesThat,&aCursorThat);
//
//    bGreaterEqual = pColumn->bGreaterEqual(aDatumThis,aDatumThat);
//    if (!bGreaterEqual)
//      break;
//  }
//
//  return bGreaterEqual;
//}

/*----------------------------------------------------------------------------*/
/* is this key greater than or equal to given key?                            */
/*----------------------------------------------------------------------------*/
//bool KeyData::bGreaterEqualSingle(KeyData* aKeyThat, Index* pIndex)
//{
//  char* aCursorThis = aByteSequence();
//  char* aCursorThat = aKeyThat->aByteSequence();
//
//  bool bGreaterEqual = pIndex->bGreaterEqual(aCursorThis,aCursorThat);
//  
//  return bGreaterEqual;
//}

/*----------------------------------------------------------------------------*/
/* is this key greater than given key?                                        */
/*----------------------------------------------------------------------------*/
//bool KeyData::bGreaterThanSingle(KeyData* aKeyThat, Index* pIndex)
//{
//  char* aCursorThis = aByteSequence();
//  char* aCursorThat = aKeyThat->aByteSequence();
//  
//  bool bGreaterThan = pIndex->bGreaterThan(aCursorThis,aCursorThat);
//  
//  return bGreaterThan;
//}

/*----------------------------------------------------------------------------*/
/* if this key greater than given key?                                        */
/*----------------------------------------------------------------------------*/
//bool KeyData::bGreaterThan(KeyData* aKeyThat, IdxColVec* pIdxColVec)
//{
//  bool bGreaterThan = false;
//
//  char* aCursorThis = aByteSequence();
//  char* aCursorThat = aKeyThat->aByteSequence();
//
//  for (i8 i_Column = 0; i_Column < pIdxColVec->nItems(); i_Column++) {
//
//    Column* pColumn = pIdxColVec->pGet(i_Column);
//
//    i8 nBytesThis;
//    i8 nBytesThat;
//
//    char* aDatumThis = pColumn->aValue(nBytesThis,&aCursorThis);
//    char* aDatumThat = pColumn->aValue(nBytesThat,&aCursorThat);
//
//    bGreaterThan = pColumn->bGreaterThan(aDatumThis,aDatumThat);
//
//    if (!bGreaterThan)
//      break;
//  }
//
//  return bGreaterThan;
//}

/*----------------------------------------------------------------------------*/
/* juxta-compare this key with given key                                      */
/*----------------------------------------------------------------------------*/
bool KeyData::bJuxtaCompare(eJC eJuxtaCompare, KeyData* aKeyThat,
                        IdxColVec* pIdxColVec, i8 nSearchKeyCols,
                        bool& bIsEqual, eRO eRelnOp)
{
  bool bJuxtaResult = false;
  
  switch (eJuxtaCompare) {
      
    case JC_BEFORE:
      bJuxtaResult = bJuxtaBefore(aKeyThat,pIdxColVec,nSearchKeyCols,bIsEqual,eRelnOp);
      break;
      
    case JC_SAME:
      bJuxtaResult = bJuxtaSame(aKeyThat,pIdxColVec,nSearchKeyCols,bIsEqual);
      break;
      
    case JC_AFTER:
      bJuxtaResult = bJuxtaAfter(aKeyThat,pIdxColVec,nSearchKeyCols,bIsEqual,eRelnOp);
      break;
      
    default:
      NEVER_REACH;
  }
  
  return bJuxtaResult;
}

/*----------------------------------------------------------------------------*/
/* is this key juxta-before given search key?                                 */
/*----------------------------------------------------------------------------*/
bool KeyData::bJuxtaBefore(KeyData* aSearchKey, IdxColVec* pIdxColVec, i8 nSearchKeyCols,
                       bool& bIsEqual,  eRO eRelnOp)
{
  /*
   * a multi-part key is said to be juxta-before some other key if:
   *
   *  i) each element is equal to the corresponding element of the
   *      the other key
   *
   *        e.g. (1) thisElement[0] == thatElement[0] &&
   *                 thisElement[1] == thatElement[1] &&
   *                 thisElement[2] == thatElement[2]
   *
   *  ii) the leading elements are equal to the corresponding elements of the
   *      the other key and a next element is less than the corresponding
   *      element of the other key
   *
   *        e.g. (2) thisElement[0] <  thatElement[0]
   *
   *        e.g. (3) thisElement[0] == thatElement[0] &&
   *                 thisElement[1] <  thatElement[1]
   *
   *        e.g. (4) thisElement[0] == thatElement[0] &&
   *                 thisElement[1] == thatElement[1] &&
   *                 thisElement[2] <  thatElement[2]
   *
   *      the above three examples hold even if there are more elements
   */
  
  bIsEqual = false;
  
  char* aCursorThis = aByteSequence();
  char* aCursorThat = aSearchKey->aByteSequence();
  
  i8 iCmp = 0;
  
  for (i8 i_Column = 0; i_Column < pIdxColVec->nItems(); i_Column++) {
    
    if (i_Column >= nSearchKeyCols)
      break;
    
    Column* pColumn = pIdxColVec->pGet(i_Column);
    
    i8 nBytesThis;
    i8 nBytesThat;
    
    char* aDatumThis = aEachDatum(pColumn,nBytesThis,&aCursorThis);
    char* aDatumThat = aEachDatum(pColumn,nBytesThat,&aCursorThat);
    
    iCmp = pColumn->iCmp(aDatumThis,aDatumThat);
    
    // return true (is before) if this element is less than that element
    // examples (2), (3), (4)
    if (iCmp < 0)
      return true;
    
    // return false (not before) if this element is greater than that element
    if (iCmp > 0)
      return false;
    
    // so far the elements are equal
    
    if (RO_LESS_THAN == eRelnOp)
      return false;
  }
  
  // all elements are the same
  ASSERT(0 == iCmp);
  bIsEqual = true;
  
  // return true (is before)
  // example (1)
  return true;
}

/*----------------------------------------------------------------------------*/
/* is this key juxta-same as given search key?                                */
/*----------------------------------------------------------------------------*/
bool KeyData::bJuxtaSame(KeyData* aSearchKey, IdxColVec* pIdxColVec, i8 nSearchKeyCols,
                     bool& bIsEqual)
{
  /*
   * a multi-part key is juxta-same as some other key if:
   *
   *  each element is equal to the corresponding element of the the other key
   *    e.g. thisElement[0] == thatElement[0] &&
   *         thisElement[1] == thatElement[1] &&
   *         thisElement[2] == thatElement[2]
   */
  
  bIsEqual = false;
  
  char* aCursorThis = aByteSequence();
  char* aCursorThat = aSearchKey->aByteSequence();
  
  i8 iCmp = 0;
  
  for (i8 i_Column = 0; i_Column < pIdxColVec->nItems(); i_Column++) {
    
    if (i_Column >= nSearchKeyCols)
      break;
    
    Column* pColumn = pIdxColVec->pGet(i_Column);
    
    i8 nBytesThis;
    i8 nBytesThat;
    
    char* aDatumThis = aEachDatum(pColumn,nBytesThis,&aCursorThis);
    char* aDatumThat = aEachDatum(pColumn,nBytesThat,&aCursorThat);
    
    iCmp = pColumn->iCmp(aDatumThis,aDatumThat);
    
    // return false (not same) if this element is not equal to that element
    if (iCmp)
      return false;
    
    // so far the elements are equal
  }
  
  // all elements are the same
  ASSERT(0 == iCmp);
  bIsEqual = true;
  
  // return true (is same)
  return true;
}

/*----------------------------------------------------------------------------*/
/* is this key juxta-after given search key?                                  */
/*----------------------------------------------------------------------------*/
bool KeyData::bJuxtaAfter(KeyData* aSearchKey, IdxColVec* pIdxColVec, i8 nSearchKeyCols,
                      bool& bIsEqual, eRO eRelnOp)
{
  /*
   * a multi-part key is said to be juxta-after some other key if:
   *
   *  i) each element is equal to the corresponding element of the
   *      the other key
   *
   *        e.g. (1) thisElement[0] == thatElement[0] &&
   *                 thisElement[1] == thatElement[1] &&
   *                 thisElement[2] == thatElement[2]
   *
   *  ii) the leading elements are equal to the corresponding elements of the
   *      the other key and a next element is greater than the corresponding
   *      element of the other key
   *
   *        e.g. (2) thisElement[0] >  thatElement[0]
   *
   *        e.g. (3) thisElement[0] == thatElement[0] &&
   *                 thisElement[1] >  thatElement[1]
   *
   *        e.g. (4) thisElement[0] == thatElement[0] &&
   *                 thisElement[1] == thatElement[1] &&
   *                 thisElement[2] >  thatElement[2]
   *
   *      the above three examples hold even if there are more elements
   */
  
  bIsEqual = false;
  
  char* aCursorThis = aByteSequence();
  char* aCursorThat = aSearchKey->aByteSequence();
  
  i8 iCmp = 0;
  
  for (i8 i_Column = 0; i_Column < pIdxColVec->nItems(); i_Column++) {
    
    if (i_Column >= nSearchKeyCols)
      break;
    
    Column* pColumn = pIdxColVec->pGet(i_Column);
    
    i8 nBytesThis;
    i8 nBytesThat;
    
    char* aDatumThis = aEachDatum(pColumn,nBytesThis,&aCursorThis);
    char* aDatumThat = aEachDatum(pColumn,nBytesThat,&aCursorThat);
    
    iCmp = pColumn->iCmp(aDatumThis,aDatumThat);

    // return true (is after) if this element is greater than that element
    // examples (2), (3), (4)
    if (iCmp > 0)
      return true;
    
    // return false (not after) if this element is less than that element
    if (iCmp < 0)
      return false;
    
    // so far the elements are equal
    
    if (RO_GREATER_THAN == eRelnOp)
      return false;
  }
  
  // all elements are the same
  ASSERT(0 == iCmp);
  bIsEqual = true;
  
  // return true (is after)
  // example (1)
  return true;
}

/*----------------------------------------------------------------------------*/
/* show the contents of this key                                              */
/*----------------------------------------------------------------------------*/
#ifdef USE_VLQ_DATUMS
void KeyData::vShow(IdxColVec* pIdxColVec, bool bCompressed)
{
  i8 nBytes;
  i8 iKey;
  Stz* pKey;
  char* aCursor = aByteSequence();
  
  char caLiteral[64];
  
  for (i8 i_Column = 0; i_Column < pIdxColVec->nItems(); i_Column++) {
    
    Column* pColumn = pIdxColVec->pGet(i_Column);
    
    char* aDatum = this->aEachDatum(pColumn,nBytes,&aCursor);
    
    eDM eDatum = pColumn->eDatumType();
    if (bCompressed)
      DtmLib::vSprintVLQ(eDatum,(Stz*)caLiteral,(Stz*)aDatum,pColumn);
    else
      DtmLib::vSprint(eDatum,(Stz*)caLiteral,(Stz*)aDatum,pColumn);
    vTrace("Elm %lld:(size %lld value:%s) "_Stz,i_Column,nBytes,caLiteral);
  }
  
  //vTrace("%s)\n"_Stz, sKeyString.aStz());
}
#else
void KeyData::vShow(IdxColVec* pIdxColVec, bool bCompressed)
{
  i8 nBytes;
  i8 iKey;
  Stz* pKey;
  char* aCursor = aByteSequence();
  
  char caLiteral[64];
  
  for (i8 i_Column = 0; i_Column < pIdxColVec->nItems(); i_Column++) {
    
    Column* pColumn = pIdxColVec->pGet(i_Column);
    
    char* aDatum = this->aEachDatum(pColumn,nBytes,&aCursor);
  
    eDM eDatum = pColumn->eDatumType();
    DtmLib::vSprint(eDatum,(Stz*)caLiteral,(Stz*)aDatum,pColumn);
    vTrace("Elm %lld:(size %lld value:%s) "_Stz,i_Column,nBytes,caLiteral);
  }

  //vTrace("%s)\n"_Stz, sKeyString.aStz());
}
#endif

/*----------------------------------------------------------------------------*/
/* sprint sort key into given string variable                                 */
/*----------------------------------------------------------------------------*/
#ifdef USE_VLQ_DATUMS
void KeyData::vSprintSortKey(s48K& sVar, IdxColVec* pIdxColVec)
{
  i8 nBytes;
  char* aWspCursor = aByteSequence();

  sVar = ""_Stz;
  
  char caLiteral[64];
  
  for (i8 i_Column = 0; i_Column < pIdxColVec->nItems(); i_Column++) {
    
    if (i_Column)
      sVar.vAppend(',');
    
    Column* pColumn = pIdxColVec->pGet(i_Column);
    eDM eDatum = pColumn->eDatumType();
    
    char* aDatum = aWspCursor;
    aWspCursor = aAdd<char*>(aWspCursor,sizeof(w8));
    
    if (DM_VCH == eDatum)
      aDatum = *((char**)aDatum);
    
    DtmLib::vSprintVLQ(eDatum,(Stz*)caLiteral,(Stz*)aDatum,pColumn);
    sVar.vConcat((Stz*)caLiteral);
  }
  
}
#else
void KeyData::vSprintSortKey(s48K& sVar, IdxColVec* pIdxColVec)
{
  i8 nBytes;
  char* aWspCursor = aByteSequence();
  
  sVar = ""_Stz;
  
  char caLiteral[64];
  
  for (i8 i_Column = 0; i_Column < pIdxColVec->nItems(); i_Column++) {
    
    if (i_Column)
      sVar.vAppend(',');
    
    Column* pColumn = pIdxColVec->pGet(i_Column);
    eDM eDatum = pColumn->eDatumType();
    
    char* aDatum = aWspCursor;
    aWspCursor = pAdd<char*>(aWspCursor,sizeof(w8));
    
    if (DM_VCH == eDatum)
      aDatum = *((char**)aDatum);
    
    DtmLib::vSprint(eDatum,(Stz*)caLiteral,(Stz*)aDatum,pColumn);
    sVar.vConcat((Stz*)caLiteral);
  }
  
}
#endif
/*----------------------------------------------------------------------------*/
/* get first datum (assuming int)                                             */
/*----------------------------------------------------------------------------*/
i8 KeyData::iGetInt(void)
{
  i8 iVal = *(i8*)aByteSequence();
  
  return iVal;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
