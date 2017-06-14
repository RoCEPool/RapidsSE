/*============================================================================*/
/* Key.cpp -- key part of an index entry                                      */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Key* DBobjects::_aKey(void* aAddr)
{
  Key* aKey = reinterpret_cast<Key*>(aAddr);
  
  return aKey;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Key::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: Key class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Key::operator new(size_t iSize, Key* pKey)
{
  RASSERT(iSize);
  return pKey;
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
Key::~Key(void)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator <                                                                 */
/*----------------------------------------------------------------------------*/
bool Key::operator <(Key* pKeyArg)
{
  i8 iDatumA = *(i8*)aWspStr();
  i8 iDatumB = *(i8*)pKeyArg->aWspStr();
  
  bool b = iDatumA < iDatumB;
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* operator <=                                                                */
/*----------------------------------------------------------------------------*/
bool Key::operator <=(Key* pKeyArg)
{
  i8 iDatumA = *(i8*)aWspStr();
  i8 iDatumB = *(i8*)pKeyArg->aWspStr();
  
  bool b = iDatumA <= iDatumB;
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* set key from given key workspace                                           */
/*----------------------------------------------------------------------------*/
void Key::vKey(KeyWrk& xKeyWrk)
{
  Str* aDst = aWspStr();
  Str* aSrc = xKeyWrk.aWspStr();
  i8 nBytes  = xKeyWrk.nWspUsed();
  
  MemLib::vCopy(aDst,aSrc,nBytes);
}

/*----------------------------------------------------------------------------*/
/* get key size                                                               */
/*----------------------------------------------------------------------------*/
i8 Key::nKeySize(IdxColVec* pIdxColVec)
{
  i8 nCols = pIdxColVec->nItems();
  i8 nKeySize = 0;
  i8 nBytes;
  char* aCursor = aWspArea();
  
  for (i8 iColumn = 0; iColumn < nCols; iColumn++) {
    Column* pColumn = pIdxColVec->pGet(iColumn);
    
    char* aElement = aEachDatum(pColumn, nBytes, &aCursor);
    nKeySize += nBytes;
  }
  
  return nKeySize;
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
/* if this key less than given key?                                           */
/*----------------------------------------------------------------------------*/
bool Key::bLessThan(Key* pKeyThat, IdxColVec* pIdxColVec)
{
  bool bLessThan = false;
  
  char* aCursorThis = aWspArea();
  char* aCursorThat = pKeyThat->aWspArea();
  
  for (i8 iColumn = 0; iColumn < pIdxColVec->nItems(); iColumn++) {
    
    Column* pColumn = pIdxColVec->pGet(iColumn);
    
    i8 nBytesThis;
    i8 nBytesThat;
    
    char* aDatumThis = aEachDatum(pColumn,nBytesThis,&aCursorThis);
    char* aDatumThat = aEachDatum(pColumn,nBytesThat,&aCursorThat);
    
    bLessThan = pColumn->bLessThan(aDatumThis,nBytesThis,
                                   aDatumThat,nBytesThat);
    if (!bLessThan)
      break;
  }
  
  return bLessThan;
}

/*----------------------------------------------------------------------------*/
/* if this key less than or equal to given key?                               */
/*----------------------------------------------------------------------------*/
bool Key::bLessEqual(Key* pKeyThat, IdxColVec* pIdxColVec)
{
  bool bLessEqual = false;
  
  char* aCursorThis = aWspArea();
  char* aCursorThat = pKeyThat->aWspArea();
  
  for (i8 iColumn = 0; iColumn < pIdxColVec->nItems(); iColumn++) {
    
    Column* pColumn = pIdxColVec->pGet(iColumn);
    
    i8 nBytesThis;
    i8 nBytesThat;
    
    char* aDatumThis = aEachDatum(pColumn,nBytesThis,&aCursorThis);
    char* aDatumThat = aEachDatum(pColumn,nBytesThat,&aCursorThat);
    
    bLessEqual = pColumn->bLessEqual(aDatumThis,nBytesThis,
                                     aDatumThat,nBytesThat);
    if (!bLessEqual)
      break;
  }
  
  return bLessEqual;
}

/*----------------------------------------------------------------------------*/
/* if this key equal to given key?                                            */
/*----------------------------------------------------------------------------*/
bool Key::bEqualTo(Key* pKeyThat, IdxColVec* pIdxColVec)
{
  bool bEqualTo = false;
  
  char* aCursorThis = aWspArea();
  char* aCursorThat = pKeyThat->aWspArea();
  
  for (i8 iColumn = 0; iColumn < pIdxColVec->nItems(); iColumn++) {
    
    Column* pColumn = pIdxColVec->pGet(iColumn);
    
    i8 nBytesThis;
    i8 nBytesThat;
    
    char* aDatumThis = aEachDatum(pColumn,nBytesThis,&aCursorThis);
    char* aDatumThat = aEachDatum(pColumn,nBytesThat,&aCursorThat);
    
    bEqualTo = pColumn->bEqualTo(aDatumThis,nBytesThis,
                                 aDatumThat,nBytesThat);
    if (!bEqualTo)
      break;
  }
  
  return bEqualTo;
}

/*----------------------------------------------------------------------------*/
/* if this key not equal to given key?                                        */
/*----------------------------------------------------------------------------*/
bool Key::bNotEqual(Key* pKeyThat, IdxColVec* pIdxColVec)
{
  bool bNotEqual = false;
  
  char* aCursorThis = aWspArea();
  char* aCursorThat = pKeyThat->aWspArea();
  
  for (i8 iColumn = 0; iColumn < pIdxColVec->nItems(); iColumn++) {
    
    Column* pColumn = pIdxColVec->pGet(iColumn);
    
    i8 nBytesThis;
    i8 nBytesThat;
    
    char* aDatumThis = aEachDatum(pColumn,nBytesThis,&aCursorThis);
    char* aDatumThat = aEachDatum(pColumn,nBytesThat,&aCursorThat);
    
    bNotEqual = pColumn->bNotEqual(aDatumThis,nBytesThis,
                                   aDatumThat,nBytesThat);
    if (!bNotEqual)
      break;
  }
  
  return bNotEqual;
}

/*----------------------------------------------------------------------------*/
/* if this key greater than or equal to given key?                            */
/*----------------------------------------------------------------------------*/
bool Key::bGreaterEqual(Key* pKeyThat, IdxColVec* pIdxColVec)
{
  bool bGreaterEqual = false;
  
  char* aCursorThis = aWspArea();
  char* aCursorThat = pKeyThat->aWspArea();
  
  for (i8 iColumn = 0; iColumn < pIdxColVec->nItems(); iColumn++) {
    
    Column* pColumn = pIdxColVec->pGet(iColumn);
    
    i8 nBytesThis;
    i8 nBytesThat;
    
    char* aDatumThis = aEachDatum(pColumn,nBytesThis,&aCursorThis);
    char* aDatumThat = aEachDatum(pColumn,nBytesThat,&aCursorThat);
    
    bGreaterEqual = pColumn->bGreaterEqual(aDatumThis,nBytesThis,
                                           aDatumThat,nBytesThat);
    if (!bGreaterEqual)
      break;
  }
  
  return bGreaterEqual;
}

/*----------------------------------------------------------------------------*/
/* if this key greater than given key?                                        */
/*----------------------------------------------------------------------------*/
bool Key::bGreaterThan(Key* pKeyThat, IdxColVec* pIdxColVec)
{
  bool bGreaterThan = false;
  
  char* aCursorThis = aWspArea();
  char* aCursorThat = pKeyThat->aWspArea();
  
  for (i8 iColumn = 0; iColumn < pIdxColVec->nItems(); iColumn++) {
    
    Column* pColumn = pIdxColVec->pGet(iColumn);
    
    i8 nBytesThis;
    i8 nBytesThat;
    
    char* aDatumThis = aEachDatum(pColumn,nBytesThis,&aCursorThis);
    char* aDatumThat = aEachDatum(pColumn,nBytesThat,&aCursorThat);
    
    bGreaterThan = pColumn->bGreaterThan(aDatumThis,nBytesThis,
                                         aDatumThat,nBytesThat);
    if (!bGreaterThan)
      break;
  }
  
  return bGreaterThan;
}

/*----------------------------------------------------------------------------*/
/* if this key greater than given key?                                        */
/*----------------------------------------------------------------------------*/
i8 Key::iCmp(Key* pKeyThat, IdxColVec* pIdxColVec)
{
  i8 iCmp = 0;
  
  char* aCursorThis = aWspArea();
  char* aCursorThat = pKeyThat->aWspArea();
  
  for (i8 iColumn = 0; iColumn < pIdxColVec->nItems(); iColumn++) {
    
    Column* pColumn = pIdxColVec->pGet(iColumn);
    
    i8 nBytesThis;
    i8 nBytesThat;
    
    char* aDatumThis = aEachDatum(pColumn,nBytesThis,&aCursorThis);
    char* aDatumThat = aEachDatum(pColumn,nBytesThat,&aCursorThat);
    
    iCmp = pColumn->iCmp(aDatumThis,nBytesThis,
                         aDatumThat,nBytesThat);
    if (iCmp)
      break;
  }
  
  return iCmp;
}

/*----------------------------------------------------------------------------*/
/* does this keyval match the given arguments                                 */
/*----------------------------------------------------------------------------*/
bool Key::bMatches(Key* pKeyThat, IdxColVec* pIdxColVec, RelOpAry* pRelOpAry)
{
  char* aCursorThis = aWspArea();
  char* aCursorThat = pKeyThat->aWspArea();
  
  bool bCmp = false;
  
  for (i8 iColumn = 0; iColumn < pIdxColVec->nItems(); iColumn++) {
    
    Column* pColumn = pIdxColVec->pGet(iColumn);
    
    i8 nBytesThis;
    i8 nBytesThat;
    
    char* aDatumThis = aEachDatum(pColumn,nBytesThis,&aCursorThis);
    char* aDatumThat = aEachDatum(pColumn,nBytesThat,&aCursorThat);
    
    eRO eRelOp = pRelOpAry->xGet(iColumn);
    
    bCmp = pColumn->bCmp(eRelOp,
                         aDatumThis,nBytesThis,
                         aDatumThat,nBytesThat);
    
    if ( ! bCmp )
      break;
  }
  
  return bCmp;
}

/*----------------------------------------------------------------------------*/
/* juxta-compare this key with given key                                      */
/*----------------------------------------------------------------------------*/
bool Key::bJuxtaCompare(eJC eJuxtaCompare, Key* pKeyThat, IdxColVec* pIdxColVec, bool& bIsEqual)
{
  bool bJuxtaCompare = false;
  
  switch (eJuxtaCompare) {
    case JC_BEFORE: bJuxtaCompare = bJuxtaBefore(pKeyThat,pIdxColVec, bIsEqual); break;
    case JC_SAME:   bJuxtaCompare = bJuxtaSame(pKeyThat,pIdxColVec, bIsEqual);   break;
    case JC_AFTER:  bJuxtaCompare = bJuxtaAfter(pKeyThat,pIdxColVec, bIsEqual);  break;
      
    default:
      ASSERT(0);
  }
  
  return bJuxtaCompare;
}

/*----------------------------------------------------------------------------*/
/* is this key juxta-before given key?                                        */
/*----------------------------------------------------------------------------*/
bool Key::bJuxtaBefore(Key* pKeyThat, IdxColVec* pIdxColVec, bool& bIsEqual)
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
  
  char* aCursorThis = aWspArea();
  char* aCursorThat = pKeyThat->aWspArea();
  
  i8 iCmp = 0;
  
  for (i8 iColumn = 0; iColumn < pIdxColVec->nItems(); iColumn++) {
    
    Column* pColumn = pIdxColVec->pGet(iColumn);
    
    i8 nBytesThis;
    i8 nBytesThat;
    
    char* aDatumThis = aEachDatum(pColumn,nBytesThis,&aCursorThis);
    char* aDatumThat = aEachDatum(pColumn,nBytesThat,&aCursorThat);
    
    iCmp = pColumn->iCmp(aDatumThis,nBytesThis,
                         aDatumThat,nBytesThat);
    
    // return true (is before) if this element is less than that element
    // examples (2), (3), (4)
    if (iCmp < 0)
      return true;
    
    // return false (not before) if this element is greater than that element
    if (iCmp > 0)
      return false;
    
    // so far the elements are equal
  }
  
  // all elements are the same
  ASSERT(0 == iCmp);
  bIsEqual = true;
  
  // return true (is before)
  // example (1)
  return true;
}

/*----------------------------------------------------------------------------*/
/* is this key juxta-same as given key?                                       */
/*----------------------------------------------------------------------------*/
bool Key::bJuxtaSame(Key* pKeyThat, IdxColVec* pIdxColVec, bool& bIsEqual)
{
  /*
   * a multi-part key is juxta-same as some other key if:
   *
   *  each element is equal to the corresponding element of the the other key
   *    e.g. thisElement[0] == thatElement[0] &&
   *         thisElement[1] == thatElement[1] &&
   *         thisElement[2] == thatElement[2]
   */
  
  char* aCursorThis = aWspArea();
  char* aCursorThat = pKeyThat->aWspArea();
  
  i8 iCmp = 0;
  
  for (i8 iColumn = 0; iColumn < pIdxColVec->nItems(); iColumn++) {
    
    Column* pColumn = pIdxColVec->pGet(iColumn);
    
    i8 nBytesThis;
    i8 nBytesThat;
    
    char* aDatumThis = aEachDatum(pColumn,nBytesThis,&aCursorThis);
    char* aDatumThat = aEachDatum(pColumn,nBytesThat,&aCursorThat);
    
    iCmp = pColumn->iCmp(aDatumThis,nBytesThis,
                         aDatumThat,nBytesThat);
    
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
/* is this key juxta-after given key?                                         */
/*----------------------------------------------------------------------------*/
bool Key::bJuxtaAfter(Key* pKeyThat, IdxColVec* pIdxColVec, bool& bIsEqual)
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
  
  char* aCursorThis = aWspArea();
  char* aCursorThat = pKeyThat->aWspArea();
  
  i8 iCmp = 0;
  
  for (i8 iColumn = 0; iColumn < pIdxColVec->nItems(); iColumn++) {
    
    Column* pColumn = pIdxColVec->pGet(iColumn);
    
    i8 nBytesThis;
    i8 nBytesThat;
    
    char* aDatumThis = aEachDatum(pColumn,nBytesThis,&aCursorThis);
    char* aDatumThat = aEachDatum(pColumn,nBytesThat,&aCursorThat);
    
    iCmp = pColumn->iCmp(aDatumThis,nBytesThis,
                         aDatumThat,nBytesThat);

    // return true (is after) if this element is greater than that element
    // examples (2), (3), (4)
    if (iCmp > 0)
      return true;
    
    // return false (not after) if this element is less than that element
    if (iCmp < 0)
      return false;
    
    // so far the elements are equal
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
void Key::vShow(IdxColVec* pIdxColVec)
{
  char* aCursor = aWspArea();
  s256 sKeyString;
  
  //vTrace("Key %p "_Str, this);
  
  sKeyString = "( "_Str;
  
  for (i8 iColumn = 0; iColumn < pIdxColVec->nItems(); iColumn++) {
    
    Column* pColumn = pIdxColVec      ->pGet(iColumn);
    eCT eColumnType = pColumn->eColumnType();
    
    i8 nBytes;
    
    char* aEachDatum = this->aEachDatum(pColumn,nBytes,&aCursor);
    
    i8 iKey;
    Str* pKey;
    
    switch (eColumnType) {
        
      case CT_DATE: {
        Dte xDte;
        xDte = *(Dte*)aEachDatum;
        vTrace("Element %lld:(size %lld value:%04lld-%02lld-%02lld) "_Str,
               iColumn,
               nBytes,
               xDte.iYear(),
               xDte.iMonth(),
               xDte.iDay()
               );
        StrLib::vSprintCat((char*)(sKeyString.aStr()), "%lld ", iKey);
        break;
      }
        
      case CT_FLOAT: {
        Flt xFlt;
        xFlt = *(Flt*)aEachDatum;
        vTrace("Element %lld:(size %lld value:%llf) "_Str,
               iColumn,
               nBytes,
               xFlt.dAsD8()
               );
        StrLib::vSprintCat((char*)(sKeyString.aStr()), "%lld ", iKey);
        break;
      }
        
      case CT_INTEGER:
        iKey = *(i8*)aEachDatum;
        vTrace("E%lld:(size %lld value:%lld) "_Str,
               iColumn,
               nBytes,
               iKey
               );
        StrLib::vSprintCat((char*)(sKeyString.aStr()), "%lld ", iKey);
        break;
        
      case CT_INTERVAL: {
        Ivl xIvl;
        xIvl = *(Ivl*)aEachDatum;
        vTrace("Element %lld:(size %lld value:%lld.%03lld) "_Str,
               iColumn,
               nBytes,
               xIvl.iSecond(),
               xIvl.iMilli()
               );
        StrLib::vSprintCat((char*)(sKeyString.aStr()), "%lld ", iKey);
        break;
      }
        
      case CT_TIME: {
        Tme xTme;
        xTme = *(Tme*)aEachDatum;
        vTrace("Element %lld:(size %lld value:%02lld:%02lld:%02lld.%lld) "_Str,
               iColumn,
               nBytes,
               xTme.iHour(),
               xTme.iMinute(),
               xTme.iSecond(),
               xTme.iMilli()
               );
        StrLib::vSprintCat((char*)(sKeyString.aStr()), "%lld ", iKey);
        break;
      }
        
      case CT_TIMESTAMP: {
        Tsp xTsp;
        xTsp = *(Tsp*)aEachDatum;
        vTrace("Element %lld:(size %lld value:%04lld-%02lld-%02lld %02lld:%02lld:%02lld.%lld) "_Str,
               iColumn,
               nBytes,
               xTsp.iYear(),
               xTsp.iMonth(),
               xTsp.iDay(),
               xTsp.iHour(),
               xTsp.iMinute(),
               xTsp.iSecond(),
               xTsp.iMilli()
               );
        StrLib::vSprintCat((char*)(sKeyString.aStr()), "%lld ", iKey);
        break;
      }
        
      case CT_VARCHAR:
        pKey = (Str*)aEachDatum;
        vTrace("E%lld:(size %lld value:%s) "_Str,
               iColumn,
               nBytes,
               pKey
               );
        StrLib::vSprintCat((char*)sKeyString.aStr(), "%s ", pKey);
        break;
        
      default:
        ASSERT(0);
    }
    
  }

  //vTrace("%s)\n"_Str, sKeyString.aStr());
}

/*----------------------------------------------------------------------------*/
/* get wsp area                                                               */
/*----------------------------------------------------------------------------*/
char* Key::aWspArea(void)
{
  return m_xWsp.aArea();
}

/*----------------------------------------------------------------------------*/
/* get wsp area                                                               */
/*----------------------------------------------------------------------------*/
Str* Key::aWspStr(void)
{
  return m_xWsp.aStr();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
