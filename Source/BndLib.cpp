/*============================================================================*/
/* BndLib.cpp - bound helper                                                  */
/*============================================================================*/

#include "Pool.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* is this key before that key                                                */
/*----------------------------------------------------------------------------*/
bool BndLib::bIsBefore(Key& xKeyThis, Key& xKeyThat,
                       RelOpAry& xOpAry, ColumnVec& xColVec)
{
  i8 nCols = xColVec.nItems();
  
  i8 nBytesThis;
  i8 nBytesThat;
 
  char* aCursorThis = xKeyThis.aWspArea();
  char* aCursorThat = xKeyThat.aWspArea();
  
  for (i8 iColumn = 0; iColumn < nCols; iColumn++) {
    
    Column* pColumn = xColVec.pGet(iColumn);
    
    char* aElementThis = xKeyThis.aElement(pColumn,nBytesThis,&aCursorThis);
    char* aElementThat = xKeyThat.aElement(pColumn,nBytesThat,&aCursorThat);

    eRO eOp = xOpAry.xGet(iColumn);
   
    if ( ! pColumn->bCmp(eOp,aElementThis,nBytesThis,aElementThat,nBytesThat))
      return false;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
