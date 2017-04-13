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
/* compare element from another key                                           */
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
    
    bLessThan = pColumn->bLessThan(aElementThis,nBytesThis,aElementThat,nBytesThat);
    if (!bLessThan)
      break;
  }
  
  return bLessThan;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
