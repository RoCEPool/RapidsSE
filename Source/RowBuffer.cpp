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
  RowBuffer* pRowBuffer = _pRowBuffer(pObject);
  
  //pIrwBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a ref set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::aAllotRow(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // allot using bytes as we don't know the size of the item
  byte* ayRow = ayAllotBytes(sizeof(Row));
  
  if (! ayRow)
    return nullptr;
  
  vIncItems();
  
  Row* aRow = new(ayRow) Row(pStreamDef,C_ALL);
  
  return aRow;
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
Row* RowBuffer::aLastRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Row* aRow = aFirstItem();
  Row* aLastRow = nullptr;
  
  while (aRow) {
    aLastRow = aRow;
    aRow = aNextRow(aRow);
  };
  
  return aLastRow;
}

/*----------------------------------------------------------------------------*/
/* return next ref set in this buffer                                         */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::aNextRow(Row* aPriorRow)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Row* aRow = aAdjacentItem(aPriorRow);
  
  if (aRow)
    return aRow;

  return nullptr;
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
  
  for (i8 i_Row = 0; i_Row < nItems(); i_Row++) {
    hvRowSingle->vCopy(pRow);
    hvRec->vAppend(hvRowSingle);
    hvRec->vPrintRow();
    
    // get the next row
    pRow = aNextRow(pRow);
  }
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void RowBuffer::vShow(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IRWBUFFER,_FOOTER_);
  
  i8 nRows = nItems();
  
  vTrace("IrwBuffer: %p (buffer %lld): rows: %lld\n"_Stz,
         this,m_iBufferNo,nRows);
  
  vTrace("  prevBuffer %p nextBuffer %p\n"_Stz,
         m_pPrevBuffer,m_pNextBuffer);
  
  vTrace("  partition: %lld \n"_Stz, m_iPartitionNo);
  
  Row* pRow = aFirstRow();
  HV_Row hvRowSingle(pStreamDef, C_ALL);

  HV_Rec hvRec(pStreamDef,C_ALL);
  while (pRow) {
    hvRowSingle->vCopy(pRow);
    hvRec->vAppend(hvRowSingle);
    pRow = aNextRow(pRow);
    hvRec->vPrintRow();
  }
  
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
