/*============================================================================*/
/* RsbBuffer.hpp -- Row Slab Buffer                                           */
/*============================================================================*/

#include "DataBuffers.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(DataBuffers,RsbBuffer,CN_RSBBUFFER)
__aNewFunction(RsbBuffer,CN_RSBBUFFER)
__vDeleteFunction(RsbBuffer,CN_RSBBUFFER)
__aFooterFunction(RsbBuffer)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RsbBuffer::RsbBuffer(void)
: DataBuffer(CN_RSBBUFFER,false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RSBBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_RSBBUFFER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RSBBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RsbBuffer::~RsbBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RSBBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get row slab                                                               */
/*----------------------------------------------------------------------------*/
RowSlab* RsbBuffer::pRowSlab(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RSBBUFFER,_FOOTER_);
  
  return & m_oRowSlab;
}

/*----------------------------------------------------------------------------*/
/* convert this rsb buffer into one or more csv buffers                       */
/*----------------------------------------------------------------------------*/
CsvBuffer* RsbBuffer::pToCsvBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RSBBUFFER,_FOOTER_);
    
  SP_RowPix spRowPix(m_oRowSlab.nCols(),m_oRowSlab.axaColPix());
  
  CsvBuffer* pFirstCsvBuffer = new CsvBuffer;
  CsvBuffer* pCsvBuffer = pFirstCsvBuffer;
  
  Stz* szRecord = nullptr;
  i8 nSize = 1;
  
  i8 nRows = m_oRowSlab.nRows();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    m8* amRowCells = m_oRowSlab.amRowCells(i_Row);
    
    Stz* szRow = spRowPix.sz((Rsb*)amRowCells,true);

    nSize = szRow->nSize();
    
    if (pCsvBuffer->nFreeBytes() < nSize + 1) {
      CsvBuffer* pNewCsvBuffer = new CsvBuffer;
      pCsvBuffer->vNextBuffer(pNewCsvBuffer);
      pCsvBuffer = pNewCsvBuffer;
    }

    szRecord = pCsvBuffer->szAllotRecord(nSize - 1);

    StrLib::vCopyLen(szRecord->azChar(),szRow->azChar(),nSize - 1);
  }
  
  if (szRecord)
    ((char*)szRecord)[nSize-1] = '\0';
  
  return pFirstCsvBuffer;
}


//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
