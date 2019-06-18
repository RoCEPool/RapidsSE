/*============================================================================*/
/* JobEncodeRow.cpp -- Encode Row Job                                         */
/*============================================================================*/

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* JobEncodeRow::operator new(size_t)
{
  vFatal("fatal: JobEncodeRow class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobEncodeRow::JobEncodeRow(void)
{
  for (i8 i_Ptn = 0; i_Ptn < C_MAX_PARTITIONS; i_Ptn++) {
    m_paRowCoder_Copy[i_Ptn] = nullptr;
  }
  for (i8 i_Ptn = 0; i_Ptn < gpAttachMap->m_pGrid->m_nPartitions; i_Ptn++) {
    m_paRowCoder_Copy[i_Ptn] = new RowCoder;
  }
  
  m_pRow = nullptr;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobEncodeRow::~JobEncodeRow(void) noexcept(false)
{
  for (i8 i_Ptn = 0; i_Ptn < gpAttachMap->m_pGrid->m_nPartitions; i_Ptn++) {
    
#ifdef WITH_ANALYSE_FREE
    //m_paRowCoder_Copy[i_Ptn]->vReport(m_pRowCoder_Encode);
#endif
    
    delete m_paRowCoder_Copy[i_Ptn];
  }
  
  delete m_pRow;
}

/*----------------------------------------------------------------------------*/
/*-- Public Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Encode row (with hash) from given record string                            */
/*----------------------------------------------------------------------------*/
bool JobEncodeRow::bEncodeRow(Stz* szRecStr, bool &bBlankLine)
{
  ColumnVec* pColVec = pTable()->pColumnVec();
  
  if (m_pRow)
    m_pRow->vResetValues();
  else
    m_pRow = new Row(pColVec);

  char* acRecCrs = szRecStr->aChar();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  bool bResult = Extractor::bExtractNext(m_pRow,&acRecCrs,pColVec,
                                         &m_oFormatters,bBlankLine);
  if (! bResult)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //hpRow hp_Row;
  //vTrace("%s\n"_Stz,hp_Row.sz(m_pRow));

  return bResult;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* JobEncodeRow::pTable(void)
{
  Table* pTable = m_oRowParts.pNub()->pDesc();
  
  return pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void JobEncodeRow::vTable(Table* pTable)
{
  m_oRowParts.pNub()->vDesc(pTable);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
