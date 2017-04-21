/*============================================================================*/
/* KeyVal.cpp -- key value                                                    */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
KeyVal* DBobjects::_aKeyVal(void* aAddr)
{
  KeyVal* aKeyVal = reinterpret_cast<KeyVal*>(aAddr);
  
  RASSERT( !aKeyVal || CN_KEYVAL == aKeyVal->eClass() );
 
  return aKeyVal;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* KeyVal::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  KeyVal* pKeyVal = reinterpret_cast<KeyVal*>(Heap::pAllocate(CN_KEYVAL));
  
  return pKeyVal;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void KeyVal::operator delete(void* pKeyValAddress)
{
  KeyVal* pKeyVal = reinterpret_cast<KeyVal*>(pKeyValAddress);
  
  Heap::vFree(pKeyVal);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
KeyVal::KeyVal(KeyWsp& xKeyWsp)
: Object(CN_KEYVAL),
  m_xKey(xKeyWsp)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
KeyVal::~KeyVal(void)
{
}

/*----------------------------------------------------------------------------*/
/* get next element from key                                                  */
/*----------------------------------------------------------------------------*/
char* KeyVal::aElement(Column* pColumn, i8& nBytes, char** aaCursor)
{
  char* aElement = m_xKey.aElement(pColumn,nBytes,aaCursor);
  
  return aElement;
}

/*----------------------------------------------------------------------------*/
/* if this key less than given key?                                           */
/*----------------------------------------------------------------------------*/
bool KeyVal::bLessThan(KeyVal* pKeyValThat, ColumnVec* pColVec)
{
  bool bLessThan = false;
  
  char* aCursorThis = m_xKey.aWspArea();
  char* aCursorThat = pKeyValThat->m_xKey.aWspArea();
  
  for (i8 iColumn = 0; iColumn < pColVec->nItems(); iColumn++) {
    
    Column* pColumn = pColVec->pGet(iColumn);
    
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
bool KeyVal::bLessEqual(KeyVal* pKeyValThat, ColumnVec* pColVec)
{
  bool bLessEqual = false;
  
  char* aCursorThis = m_xKey.aWspArea();
  char* aCursorThat = pKeyValThat->m_xKey.aWspArea();
  
  for (i8 iColumn = 0; iColumn < pColVec->nItems(); iColumn++) {
    
    Column* pColumn = pColVec->pGet(iColumn);
    
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
bool KeyVal::bEqualTo(KeyVal* pKeyValThat, ColumnVec* pColVec)
{
  bool bEqualTo = false;
  
  char* aCursorThis = m_xKey.aWspArea();
  char* aCursorThat = pKeyValThat->m_xKey.aWspArea();
  
  for (i8 iColumn = 0; iColumn < pColVec->nItems(); iColumn++) {
    
    Column* pColumn = pColVec->pGet(iColumn);
    
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
bool KeyVal::bNotEqual(KeyVal* pKeyValThat, ColumnVec* pColVec)
{
  bool bNotEqual = false;
  
  char* aCursorThis = m_xKey.aWspArea();
  char* aCursorThat = pKeyValThat->m_xKey.aWspArea();
  
  for (i8 iColumn = 0; iColumn < pColVec->nItems(); iColumn++) {
    
    Column* pColumn = pColVec->pGet(iColumn);
    
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
bool KeyVal::bGreaterEqual(KeyVal* pKeyValThat, ColumnVec* pColVec)
{
  bool bGreaterEqual = false;
  
  char* aCursorThis = m_xKey.aWspArea();
  char* aCursorThat = pKeyValThat->m_xKey.aWspArea();
  
  for (i8 iColumn = 0; iColumn < pColVec->nItems(); iColumn++) {
    
    Column* pColumn = pColVec->pGet(iColumn);
    
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
bool KeyVal::bGreaterThan(KeyVal* pKeyValThat, ColumnVec* pColVec)
{
  bool bGreaterThan = false;
  
  char* aCursorThis = m_xKey.aWspArea();
  char* aCursorThat = pKeyValThat->m_xKey.aWspArea();
  
  for (i8 iColumn = 0; iColumn < pColVec->nItems(); iColumn++) {
    
    Column* pColumn = pColVec->pGet(iColumn);
    
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
i8 KeyVal::iCmp(KeyVal* pKeyValThat, ColumnVec* pColVec)
{
  i8 iCmp = 0;
  
  char* aCursorThis = m_xKey.aWspArea();
  char* aCursorThat = pKeyValThat->m_xKey.aWspArea();
  
  for (i8 iColumn = 0; iColumn < pColVec->nItems(); iColumn++) {
    
    Column* pColumn = pColVec->pGet(iColumn);
    
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
bool KeyVal::bMatches(KeyVal* pKeyValThat, ColumnVec* pColVec, RelOpAry* pRelOpAry)
{
  char* aCursorThis = m_xKey.aWspArea();
  char* aCursorThat = pKeyValThat->m_xKey.aWspArea();
  
  bool bCmp = false;
  
  for (i8 iColumn = 0; iColumn < pColVec->nItems(); iColumn++) {
    
    Column* pColumn = pColVec->pGet(iColumn);
    
    i8 nBytesThis;
    i8 nBytesThat;
    
    char* aElementThis = aElement(pColumn,nBytesThis,&aCursorThis);
    char* aElementThat = aElement(pColumn,nBytesThat,&aCursorThat);
    
    eRO eRelOp = pRelOpAry->xGet(iColumn);
    
    bCmp = pColumn->bCmp(eRelOp,
                         aElementThis,nBytesThis,
                         aElementThat,nBytesThat);
    
    if (bCmp)
      break;
  }
  
  return bCmp;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void KeyVal::vShow(ColumnVec* pColVec)
{
  //m_xKey.vShow(eColumnType);
  
  char* aCursor = m_xKey.aWspArea();
  s32 sKeyString;
  
  vTrace("Key %p: size %lld "_Str, this, m_xKey.nSize());
  
  sKeyString = "( "_Str;
  
  for (i8 iColumn = 0; iColumn < pColVec->nItems(); iColumn++) {
    
    Column* pColumn = pColVec->pGet(iColumn);
    eCT eColumnType = pColumn->eColumnType();
    
    i8 nBytes;
    
    char* aElement = this->aElement(pColumn,nBytes,&aCursor);
    
    i8 iKey;
    Str* pKey;
    
    switch (eColumnType) {
      case CT_INTEGER:
        iKey = *(i8*)aElement;
        vTrace("Element %lld:(size %lld value:%lld) "_Str,
               iColumn,
               nBytes,
               iKey
               );
        StrLib::vSprintCat((char*)(sKeyString.aStr()), "%lld ", iKey);
        break;
        
      case CT_VARCHAR:
        pKey = (Str*)aElement;
        vTrace("Element %lld:(size %lld value:%s) "_Str,
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
  vTrace("%s)\n"_Str, sKeyString.aStr());
  
}

/*----------------------------------------------------------------------------*/
/* get key                                                                    */
/*----------------------------------------------------------------------------*/
Key& KeyVal::xKey(void)
{
  return m_xKey;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
