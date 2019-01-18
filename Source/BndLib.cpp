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
 
  char* aCursorThis = xKeyThis.aByteSequence();
  char* aCursorThat = xKeyThat.aByteSequence();
  
  for (i8 i_Column = 0; i_Column < nCols; i_Column++) {
    
    Column* pColumn = xColVec.pGet(i_Column);
    
    char* aDatumThis = xKeyThis.aEachDatum(pColumn,nBytesThis,&aCursorThis);
    char* aDatumThat = xKeyThat.aEachDatum(pColumn,nBytesThat,&aCursorThat);

    eRO eOp = xOpAry.xGet(i_Column);
    
    ASSERT(eOp == RO_LESS_THAN  ||
           eOp == RO_LESS_EQUAL ||
           eOp == RO_EQUAL_TO);
   
    if ( ! pColumn->bCmp(eOp,aDatumThis,aDatumThat))
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
  
  char* aCursorThis = xKeyThis.aByteSequence();
  char* aCursorThat = xKeyThat.aByteSequence();
  
  for (i8 iColumn = 0; iColumn < nCols; iColumn++) {
    
    Column* pColumn = xColVec.pGet(iColumn);
    
    char* aDatumThis = xKeyThis.aEachDatum(pColumn,nBytesThis,&aCursorThis);
    char* aDatumThat = xKeyThat.aEachDatum(pColumn,nBytesThat,&aCursorThat);
    
    eRO eOp = xOpAry.xGet(iColumn);
    
    if ( ! pColumn->bCmp(eOp,aDatumThis,aDatumThat))
      return false;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
