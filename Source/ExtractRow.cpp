/*============================================================================*/
/* ExtractRow.cpp -- Extract Row                                              */
/*============================================================================*/

#include "Jobs.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
  
/*----------------------------------------------------------------------------*/
/* allocate a new extractRow from heap                                        */
/*----------------------------------------------------------------------------*/
void* ExtractRow::operator new(size_t)
{
  ExtractRow* pExtractRow =
  static_cast<ExtractRow*>(HeapMem::pAllocate(CN_EXTRACTROW));
  
  return pExtractRow;
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* free a extractRow back to the heap                                         */
/*----------------------------------------------------------------------------*/
void ExtractRow::operator delete(void* aAddress)
{
  ExtractRow* pExtractRow = static_cast<ExtractRow*>(aAddress);
  
  HeapMem::vFree(pExtractRow);
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ExtractRow::ExtractRow(void)
: Object(CN_EXTRACTROW)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXTRACTROW,_FOOTER_);
  
  m_pRow   = new Row(m_pTable->pStreamDef(), C_ALL);
  m_pTable = nullptr;
  
  _checkInitialised_(CN_WRITEBEHIND);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_EXTRACTROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ExtractRow::~ExtractRow(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXTRACTROW,_FOOTER_);
  
  // delete the row
  delete m_pRow;
}

/*----------------------------------------------------------------------------*/
/*-- Public Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Extract row (with hash) from given record string                           */
/*----------------------------------------------------------------------------*/
bool ExtractRow::bExtractRow(char** aazRecStr, bool &bBlankLine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXTRACTROW,_FOOTER_);
  
  StreamDef* pStreamDef = m_pTable->pStreamDef();
  
  m_pRow->vResetValues();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  bool bResult = CsvCursor::bExtractNext(m_pRow,pStreamDef,
                                         aazRecStr,&m_oFormatters,
                                         bBlankLine);
  return bResult;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
