/*============================================================================*/
/* RowBufferr.hpp -- Row Buffer                                               */
/*============================================================================*/

#include "DataBuffers.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowBuffer* DataBuffers::_pRowBuffer(void* aAddress)
{
  RowBuffer* pRowBuffer = reinterpret_cast<RowBuffer*>(aAddress);
  
  return pRowBuffer;
}

__aNewFunction(RowBuffer,CN_ROWBUFFER)
__vDeleteFunction(RowBuffer,CN_ROWBUFFER)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowBuffer::RowBuffer(void)
: DataBuffer(CN_ROWBUFFER, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER);
  
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowBuffer::RowBuffer(eCN eClassNo)
: DataBuffer(eClassNo, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowBuffer::~RowBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump reference buffer                                                      */
/*----------------------------------------------------------------------------*/
void RowBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  //RowBuffer* pRowBuffer = _pRowBuffer(pObject);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a ref set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::aAllotRow(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // allot using bytes as we don't know the size of the item
  Row* ayRow = ayAllotHead(nBytes);
  
  if (! ayRow)
    return nullptr;
  
  vIncItems();
  
  Row* aRow = new(ayRow) Row();
  
  return aRow;
}

/*----------------------------------------------------------------------------*/
/* allot a ref set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::aAllotRow(RowSingle* pRowSingle)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  StreamDef* pStreamDef = pRowSingle->m_pStreamDef;
  
  i8 nHeadSize = Row::nSizeBytes(pStreamDef);
  i8 nTailSize = pRowSingle->nSizeVch();
  
  if (nFreeBytes() < nHeadSize + nTailSize)
    return nullptr;
  
  Row* pRow = RowBuffer::aAllotRow(nHeadSize);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy single row into this buffer
  
  for (i8 i_Col = 0; i_Col < pStreamDef->nColumns(C_ALL); i_Col++) {
    
    Column* pCol = pStreamDef->pGetColumn(i_Col);
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // vch
    
    if (DM_VCH == pCol->eDatum()) {
      
      // check for null
      if (pRowSingle->bIsColNull(i_Col)) {
        pRow->vSetColNull(i_Col);
        continue;
      }
      
      // get src string
      char* acSrcString = (char*)pRowSingle->szColVal(i_Col);
      i8 nSrcLength = StrLib::nSize(acSrcString);
      
      // STRING_DEPOSIT
      // allocate a string from the tail
      Stz* acDstString = ayAllotTail(nSrcLength);
      i8 nCopyBytes = StrLib::nCopy((char*)acDstString, acSrcString);
      ASSERT(nCopyBytes + 1 == nSrcLength);
      // set offset of tail string
      pRow->vColVal_ofs(i_Col,iToOffset(acDstString));
      // END STRING_DEPOSIT

      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // all others
    
    pRow->vColVal_int(i_Col,pRowSingle->iColVal(i_Col));
  }
  
  //pRow->vDump(pStreamDef);
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* return first ref set in this buffer                                        */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::aFirstRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Row* aRow = aFirstItem();
  
  return aRow;
}

/*----------------------------------------------------------------------------*/
/* return last ref set in this buffer                                         */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::aLastRow(i8 nRowBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Row* aRow = aFirstItem();
  Row* aLastRow = nullptr;
  
  while (aRow) {
    aLastRow = aRow;
    aRow = aNextRow(aRow, nRowBytes);
  };
  
  return aLastRow;
}

/*----------------------------------------------------------------------------*/
/* return next ref set in this buffer                                         */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::aNextRow(Row* aPriorRow, i8 nRowBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Row* aRow = aAdjacentItem(aPriorRow, nRowBytes);
  
  return aRow;
}

/*----------------------------------------------------------------------------*/
/* print rows                                                                 */
/*----------------------------------------------------------------------------*/
void RowBuffer::vPrintRows(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  HV_Rec hvRec(pStreamDef,C_ALL);
  
  // NOTE: We need to print only nItem no of rows
  // as the limitOp may have truncated the rows via setting the nItems()
  
  // get the first row
  Row* pRow = aFirstRow();
  HV_Row hvRowSingle(pStreamDef, C_ALL);
  i8 nRowHeadSize = Row::nSizeBytes(pStreamDef);
  
  for (i8 i_Row = 0; i_Row < nItems(); i_Row++) {
    hvRowSingle->vCopy(pRow);
    hvRec->vAppend(hvRowSingle);
    hvRec->vPrintRow();
    
    // get the next row
    pRow = aNextRow(pRow, nRowHeadSize);
  }
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void RowBuffer::vShow(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  i8 nRows = nItems();
  
  vTrace("RowBuffer: %p (buffer %lld): rows: %lld\n"_Stz,
         this,m_iBufferNo,nRows);
  
  vTrace("  prevBuffer %p nextBuffer %p\n"_Stz,
         m_pPrevBuffer,m_pNextBuffer);
  
  vTrace("  partition: %lld \n"_Stz, m_tPtnNo);
  
  Row* pRow = aFirstRow();
  HV_Row hvRowSingle(pStreamDef, C_ALL);
  i8 nRowHeadSize = pStreamDef->nColumns(C_ALL)* sizeof(i8);
  
  HV_Rec hvRec(pStreamDef,C_ALL);
  while (pRow) {
    hvRowSingle->vCopy(pRow);
    hvRec->vAppend(hvRowSingle);
    pRow = aNextRow(pRow, nRowHeadSize);
    hvRec->vPrintRow();
  }
  
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* calculate the absolute offset within buffer for given item                 */
/*----------------------------------------------------------------------------*/
i8 RowBuffer::iToOffset(Stz* aItem)
{
  i4 iOffset = static_cast<i4>(nDifference(aItem,this));
  
  return iOffset;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
