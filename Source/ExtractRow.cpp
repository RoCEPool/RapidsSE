/*============================================================================*/
/* ExtractRow.cpp -- Extract Row                                              */
/*============================================================================*/

#include "CompactBuffers.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ExtractRow* CompactBuffers::_pExtractRow(void* aAddress)
{
  ExtractRow* pExtractRow = reinterpret_cast<ExtractRow*>(aAddress);
  
  RASSERT_TYPE(pExtractRow,CN_EXTRACTROW);
  
  return pExtractRow;
}

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
ExtractRow::ExtractRow(Table* pTable)
: Object(CN_EXTRACTROW)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXTRACTROW,_FOOTER_);
  
  m_pTable = pTable;
  m_pRow   = new RowSingle(m_pTable->pStreamDef(), C_ALL);
  
  m_oFormatters.vSet(AM::pFmt());
  m_oFormatters.m_cEnclosedBy_Leading = '\'';
  m_oFormatters.m_cEnclosedBy_Trailing = '\'';
  //m_oFormatters.m_cRecordTerminator = '\n';
  
  _checkInitialised_(CN_EXTRACTROW);
  
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
bool ExtractRow::bExtractRow(RowSingle** ppRow, char** aazRecStr, bool &bBlankLine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXTRACTROW,_FOOTER_);
  
  StreamDef* pStreamDef = m_pTable->pStreamDef();
  
  m_pRow->vResetValues();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  bool bResult = CsvCursor::bExtractNext(m_pRow,pStreamDef,
                                         aazRecStr,&m_oFormatters,
                                         bBlankLine);
  if (bResult)
    *ppRow = m_pRow;
  
  return bResult;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(ExtractRow)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
