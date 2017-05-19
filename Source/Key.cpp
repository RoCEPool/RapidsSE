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
Key::Key(KeyWsp& xKeyWsp)
{
  vKey(xKeyWsp);
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
void Key::vKey(KeyWsp& xKeyWsp)
{
  Str* aDst = aWspStr();
  Str* aSrc = xKeyWsp.aWspStr();
  i8 nBytes  = xKeyWsp.nWspUsed();
  
  MemLib::vCopy(aDst,aSrc,nBytes);
}

/*----------------------------------------------------------------------------*/
/* get next element from key                                                  */
/*----------------------------------------------------------------------------*/
char* Key::aElement(Column* pColumn, i8& nBytes, char** aaCursor)
{
  char* aElement = pColumn->aValue(nBytes,aaCursor);
  
  return aElement;
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
    
    char* aElementThis = aElement(pColumn,nBytesThis,&aCursorThis);
    char* aElementThat = aElement(pColumn,nBytesThat,&aCursorThat);
    
    bLessThan = pColumn->bLessThan(aElementThis,nBytesThis,
                                   aElementThat,nBytesThat);
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
    
    char* aElementThis = aElement(pColumn,nBytesThis,&aCursorThis);
    char* aElementThat = aElement(pColumn,nBytesThat,&aCursorThat);
    
    bLessEqual = pColumn->bLessEqual(aElementThis,nBytesThis,
                                     aElementThat,nBytesThat);
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
    
    char* aElementThis = aElement(pColumn,nBytesThis,&aCursorThis);
    char* aElementThat = aElement(pColumn,nBytesThat,&aCursorThat);
    
    bEqualTo = pColumn->bEqualTo(aElementThis,nBytesThis,
                                 aElementThat,nBytesThat);
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
    
    char* aElementThis = aElement(pColumn,nBytesThis,&aCursorThis);
    char* aElementThat = aElement(pColumn,nBytesThat,&aCursorThat);
    
    bNotEqual = pColumn->bNotEqual(aElementThis,nBytesThis,
                                   aElementThat,nBytesThat);
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
    
    char* aElementThis = aElement(pColumn,nBytesThis,&aCursorThis);
    char* aElementThat = aElement(pColumn,nBytesThat,&aCursorThat);
    
    bGreaterEqual = pColumn->bGreaterEqual(aElementThis,nBytesThis,
                                           aElementThat,nBytesThat);
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
    
    char* aElementThis = aElement(pColumn,nBytesThis,&aCursorThis);
    char* aElementThat = aElement(pColumn,nBytesThat,&aCursorThat);
    
    bGreaterThan = pColumn->bGreaterThan(aElementThis,nBytesThis,
                                         aElementThat,nBytesThat);
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
    
    char* aElementThis = aElement(pColumn,nBytesThis,&aCursorThis);
    char* aElementThat = aElement(pColumn,nBytesThat,&aCursorThat);
    
    iCmp = pColumn->iCmp(aElementThis,nBytesThis,
                         aElementThat,nBytesThat);
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
    
    char* aElementThis = aElement(pColumn,nBytesThis,&aCursorThis);
    char* aElementThat = aElement(pColumn,nBytesThat,&aCursorThat);
    
    eRO eRelOp = pRelOpAry->xGet(iColumn);
    
    bCmp = pColumn->bCmp(eRelOp,
                         aElementThis,nBytesThis,
                         aElementThat,nBytesThat);
    
    if ( ! bCmp )
      break;
  }
  
  return bCmp;
}

/*----------------------------------------------------------------------------*/
/* compare this key with given key                                            */
/*----------------------------------------------------------------------------*/
bool Key::bCompare(ePN ePosition, Key* pKeyThat, IdxColVec* pIdxColVec)
{
  bool bCompare = false;
  
  switch (ePosition) {
    case PN_BEFORE: bCompare = bBefore(pKeyThat,pIdxColVec); break;
    case PN_SAME:   bCompare = bSame(pKeyThat,pIdxColVec);   break;
    case PN_AFTER:  bCompare = bAfter(pKeyThat,pIdxColVec);  break;
      
    default:
      ASSERT(0);
  }
  
  return bCompare;
}

/*----------------------------------------------------------------------------*/
/* is this key before given key                                               */
/*----------------------------------------------------------------------------*/
bool Key::bBefore(Key* pKeyThat, IdxColVec* pIdxColVec)
{
  /*
   * a multi-part key is before some other key if:
   *
   *   i) the first element is less than the first element of the other key
   *        i.e. thisElement[0] < thatElement[0]
   *
   *  ii) the leading elements are equal to the corresponding elements of the
   *      the other key and a next element is less than the corresponding 
   *      element of the other key
   *
   *        e.g. thisElement[0] == thatElement[0] &&
   *             thisElement[1] <  thatElement[1]
   *
   *        e.g. thisElement[0] == thatElement[0] &&
   *             thisElement[1] == thatElement[1] &&
   *             thisElement[2] <  thatElement[2]
   *
   *      the above example hold even if there are more elements
   */
  
  char* aCursorThis = aWspArea();
  char* aCursorThat = pKeyThat->aWspArea();
  
  i8 iCmp = 0;
  
  for (i8 iColumn = 0; iColumn < pIdxColVec->nItems(); iColumn++) {
    
    Column* pColumn = pIdxColVec->pGet(iColumn);
    
    i8 nBytesThis;
    i8 nBytesThat;
    
    char* aElementThis = aElement(pColumn,nBytesThis,&aCursorThis);
    char* aElementThat = aElement(pColumn,nBytesThat,&aCursorThat);
    
    iCmp = pColumn->iCmp(aElementThis,nBytesThis,
                         aElementThat,nBytesThat);
    
    // return true (is before) if this element is less than that element
    if (iCmp < 0)
      return true;
    
    // return false (not before) if this element is greater than that element
    if (iCmp > 0)
      return false;
  }
  
  // all elements are the same
  ASSERT(0 == iCmp);
  
  // return false (not before)
  return false;
}

/*----------------------------------------------------------------------------*/
/* is this key same as given key                                              */
/*----------------------------------------------------------------------------*/
bool Key::bSame(Key* pKeyThat, IdxColVec* pIdxColVec)
{
  /*
   * a multi-part key is same as some other key if:
   *
   *  i) each element is equal to the corresponding element of the
   *      the other key
   *        e.g. thisElement[0] == thatElement[0] &&
   *             thisElement[1] == thatElement[1] &&
   *             thisElement[2] == thatElement[2]
   */
  
  char* aCursorThis = aWspArea();
  char* aCursorThat = pKeyThat->aWspArea();
  
  i8 iCmp = 0;
  
  for (i8 iColumn = 0; iColumn < pIdxColVec->nItems(); iColumn++) {
    
    Column* pColumn = pIdxColVec->pGet(iColumn);
    
    i8 nBytesThis;
    i8 nBytesThat;
    
    char* aElementThis = aElement(pColumn,nBytesThis,&aCursorThis);
    char* aElementThat = aElement(pColumn,nBytesThat,&aCursorThat);
    
    iCmp = pColumn->iCmp(aElementThis,nBytesThis,
                         aElementThat,nBytesThat);
    
    // return false (not same) if this element is not equal to that element
    if (iCmp)
      return false;
  }
  
  // all elements are the same
  ASSERT(0 == iCmp);
  
  // return true (is same)
  return true;
}

/*----------------------------------------------------------------------------*/
/* is this key after given key                                                */
/*----------------------------------------------------------------------------*/
bool Key::bAfter(Key* pKeyThat, IdxColVec* pIdxColVec)
{
  /*
   * a multi-part key is after some other key if:
   *
   *   i) the first element is greater than the first element of the other key
   *        i.e. thisElement[0] > thatElement[0]
   *
   *  ii) the leading elements are equal to the corresponding elements of the
   *      the other key and a next element is greater than the corresponding
   *      element of the other key
   *
   *        e.g. thisElement[0] == thatElement[0] &&
   *             thisElement[1] >  thatElement[1]
   *
   *        e.g. thisElement[0] == thatElement[0] &&
   *             thisElement[1] == thatElement[1] &&
   *             thisElement[2] >  thatElement[2]
   *
   *      the above example hold even if there are more elements
   */
  
  char* aCursorThis = aWspArea();
  char* aCursorThat = pKeyThat->aWspArea();
  
  i8 iCmp = 0;
  
  for (i8 iColumn = 0; iColumn < pIdxColVec->nItems(); iColumn++) {
    
    Column* pColumn = pIdxColVec->pGet(iColumn);
    
    i8 nBytesThis;
    i8 nBytesThat;
    
    char* aElementThis = aElement(pColumn,nBytesThis,&aCursorThis);
    char* aElementThat = aElement(pColumn,nBytesThat,&aCursorThat);
    
    iCmp = pColumn->iCmp(aElementThis,nBytesThis,
                         aElementThat,nBytesThat);

    // return true (is after) if this element is greater than that element
    if (iCmp > 0)
      return true;
    
    // return false (not after) if this element is less than that element
    if (iCmp < 0)
      return false;
    
    // so far the elements are equal
  }
  
  // all elements are the same
  ASSERT(0 == iCmp);
  
  // return true (is after)
  return true; // had to do this for the assert in indexBuffer::pSplitBuffer()
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
    
    char* aElement = this->aElement(pColumn,nBytes,&aCursor);
    
    i8 iKey;
    Str* pKey;
    
    switch (eColumnType) {
        
      case CT_DATE: {
        Dte xDte;
        xDte = *(Dte*)aElement;
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
        xFlt = *(Flt*)aElement;
        vTrace("Element %lld:(size %lld value:%llf) "_Str,
               iColumn,
               nBytes,
               xFlt.dAsD8()
               );
        StrLib::vSprintCat((char*)(sKeyString.aStr()), "%lld ", iKey);
        break;
      }
        
      case CT_INTEGER:
        iKey = *(i8*)aElement;
        vTrace("E%lld:(size %lld value:%lld) "_Str,
               iColumn,
               nBytes,
               iKey
               );
        StrLib::vSprintCat((char*)(sKeyString.aStr()), "%lld ", iKey);
        break;
        
      case CT_INTERVAL: {
        Ivl xIvl;
        xIvl = *(Ivl*)aElement;
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
        xTme = *(Tme*)aElement;
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
        xTsp = *(Tsp*)aElement;
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
        pKey = (Str*)aElement;
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
