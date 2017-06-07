/*============================================================================*/
/* BndLib.cpp - bound helper                                                  */
/*============================================================================*/

#include "Pool.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* is this key before that key                                                */
/*----------------------------------------------------------------------------*/
bool BndLib::bIsBefore(Key& xKeyThis, Key& xKeyThat,
                       RelOpAry& xOpAry, IdxColVec& xColVec)
{
  i8 nCols = xColVec.nItems();
  
  i8 nBytesThis;
  i8 nBytesThat;
 
  char* aCursorThis = xKeyThis.aWspArea();
  char* aCursorThat = xKeyThat.aWspArea();
  
  for (i8 iColumn = 0; iColumn < nCols; iColumn++) {
    
    Column* pColumn = xColVec.pGet(iColumn);
    
    char* aDatumThis = xKeyThis.aEachDatum(pColumn,nBytesThis,&aCursorThis);
    char* aDatumThat = xKeyThat.aEachDatum(pColumn,nBytesThat,&aCursorThat);

    eRO eOp = xOpAry.xGet(iColumn);
    
    ASSERT(eOp == RO_LESS_THAN  ||
           eOp == RO_LESS_EQUAL ||
           eOp == RO_EQUAL);
   
    if ( ! pColumn->bCmp(eOp,aDatumThis,nBytesThis,aDatumThat,nBytesThat))
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* does given key pass key filter                                             */
/*----------------------------------------------------------------------------*/
bool BndLib::bPassesFilter(Key& xKeyThis, Key& xKeyThat,
                           RelOpAry& xOpAry, IdxColVec& xColVec)
{
  i8 nCols = xColVec.nItems();
  
  i8 nBytesThis;
  i8 nBytesThat;
  
  char* aCursorThis = xKeyThis.aWspArea();
  char* aCursorThat = xKeyThat.aWspArea();
  
  for (i8 iColumn = 0; iColumn < nCols; iColumn++) {
    
    Column* pColumn = xColVec.pGet(iColumn);
    
    char* aDatumThis = xKeyThis.aEachDatum(pColumn,nBytesThis,&aCursorThis);
    char* aDatumThat = xKeyThat.aEachDatum(pColumn,nBytesThat,&aCursorThat);
    
    eRO eOp = xOpAry.xGet(iColumn);
    
    if ( ! pColumn->bCmp(eOp,aDatumThis,nBytesThis,aDatumThat,nBytesThat))
      return false;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
