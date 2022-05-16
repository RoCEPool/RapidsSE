/*============================================================================*/
/* RowInflator.cpp -- Compact Row Inflator                                    */
/*============================================================================*/

#include "BASE.h"

#include "IndexDef.hpp"

#include "RowBuffer.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "TailLine.hpp"

#include "Crw.hpp"
#include "RowInflator.hpp"

/*----------------------------------------------------------------------------*/
/* inflate set onto Dtsk                                                      */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflate_ColumnSet_into_CellArray(i8**       _aaCellArray,
                                                    char*&     _racStrArea,
                                                    eSI        _eStrInflateRule,
                                                    HeadLine_T* _aHeadLine,
                                                    i8*        _iaColSet,
                                                    i8         _nCols)
{
  _static_

  #include "TccStuff.h"

  #include "vFLAT_Preliminary.h"
  #include "vFLAT_DrivingLoop.h"
  #include "vFLAT_InflateColumn.h"
  #include "vFLAT_SkipColumn.h"

  NEVER_REACH;
  
}

/*----------------------------------------------------------------------------*/
/* inflate headline into row                                                  */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflate_OneColumn_into_RowSingle(RowSingle* pRow,
                                 HeadLine* aHeadLine,
                                 i8 i_ColToRetrieve)
{
  NEVER_REACH;
  
  UNUSED(pRow);
  UNUSED(aHeadLine);
  UNUSED(i_ColToRetrieve);
}

/*----------------------------------------------------------------------------*/
/* inflate headline into row                                                  */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflate_AllColumns_into_RowSingle(RowSingle* pRowSingle,
                                                     HeadLine* aHeadLine)
{
  _static_
  
  //vTrace("vInflate_AllColumns_into_RowSingle\n"_Stz);

  u1* auMetaCode;
  byte* ayDataByte;

  pRowSingle->vResetRow();

  i8 nCols = pRowSingle->nColumns();

  vPrepare_v2(auMetaCode,ayDataByte,nCols,aHeadLine);

  byte* ayCellArray = pRowSingle->ayCellArray();
    

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
#ifdef WITH_TRACE_INFLATE
    Column* pCol = pRow->m_pColVec->pGet(i_Col);
#endif
    
    u1 uMetaCode = *auMetaCode++;
    u1 uHigh = uMetaCode >> 4;
    u1 uLow  = uMetaCode & 0x0F;

    char* azStrArea = (char*)pRowSingle->szStzAlottable();

    vInflate_CodedValue_into_Cell(ayCellArray,azStrArea,SI_STRAREA_OFS,
                                  aHeadLine,ayDataByte,uHigh,uLow);
    
    if (azStrArea != (char*)pRowSingle->szStzAlottable()) {
      i8 nBytesTaken = nDifference(azStrArea,pRowSingle->szStzAlottable());
      pRowSingle->vStzBytesTaken(nBytesTaken);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* inflate headline into row                                                  */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflate_AllColumns_into_RowBuffer(RowBuffer* pRowBuffer,
                                                     HeadLine* aHeadLine)
{
  _static_
  
  //vTrace("vInflate_AllColumns_into_RowBuffer\n"_Stz);

  i8 nCols = pRowBuffer->nColumns();

  u1* auMetaCode;
  byte* ayDataByte;
  vPrepare_v2(auMetaCode,ayDataByte,nCols,aHeadLine);

  Row* pRow = pRowBuffer->pRowCurrent();

  byte* ayCellArray = (byte*)pRow->aCellArray();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {

    u1 uMetaCode = *auMetaCode++;
    u1 uHigh = uMetaCode >> 4;
    u1 uLow  = uMetaCode & 0x0F;
    
    char* azStrArea = (char*)pRowBuffer->azStrArea();

    vInflate_CodedValue_into_Cell(ayCellArray,azStrArea,SI_STRAREA_OFS,
                                  aHeadLine,ayDataByte,uHigh,uLow);

    if (azStrArea != (char*)pRowBuffer->azStrArea()) {
      i8 nBytesTaken = nDifference(azStrArea,pRowBuffer->azStrArea());
      pRowBuffer->vTakeHead(nBytesTaken);
    }

  }
}

/*----------------------------------------------------------------------------*/
/* inflate headline into row                                                  */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflate_AllColumns_into_RowBufferAt(RowBuffer* pRowBuffer,
                                                     i8 iAtCell,
                                                     HeadLine* aHeadLine,
                                                     i8 nSrcCols)
{
  _static_

  //vTrace("vInflate_AllColumns_into_RowBuffer\n"_Stz);

  u1* auMetaCode;
  byte* ayDataByte;
  vPrepare_v2(auMetaCode,ayDataByte,nSrcCols,aHeadLine);

  Row* pRow = pRowBuffer->pRowCurrent();

  byte* ayCellArray = (byte*)pRow->aColCell(iAtCell);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (i8 i_Col = 0; i_Col < nSrcCols; i_Col++) {

    u1 uMetaCode = *auMetaCode++;
    u1 uHigh = uMetaCode >> 4;
    u1 uLow  = uMetaCode & 0x0F;

    char* azStrArea = (char*)pRowBuffer->azStrArea();

    vInflate_CodedValue_into_Cell(ayCellArray,azStrArea,SI_STRAREA_OFS,
                                  aHeadLine,ayDataByte,uHigh,uLow);

    if (azStrArea != (char*)pRowBuffer->azStrArea()) {
      i8 nBytesTaken = nDifference(azStrArea,pRowBuffer->azStrArea());
      pRowBuffer->vTakeHead(nBytesTaken);
    }

  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
