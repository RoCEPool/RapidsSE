/*============================================================================*/
/* JobExtractRow.cpp -- Extract Row Job                                       */
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
void* JobExtractRow::operator new(size_t)
{
  vFatal("fatal: JobExtractRow class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobExtractRow::JobExtractRow(void)
{
  m_pRow   = nullptr;
  
  m_pTable = nullptr;
  
  for (i8 i_Ptn = 0; i_Ptn < C_MAX_PARTITIONS; i_Ptn++)
    m_paRowCoder_Copy[i_Ptn] = nullptr;
  
  for (i8 i_Ptn = 0; i_Ptn < gpAttachMap->m_pGrid->m_nPartitions; i_Ptn++)
    m_paRowCoder_Copy[i_Ptn] = new RowCompactor;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobExtractRow::~JobExtractRow(void) noexcept(false)
{
  delete m_pRow;
  
  for (i8 i_Ptn = 0; i_Ptn < gpAttachMap->m_pGrid->m_nPartitions; i_Ptn++) {
    
#ifdef WITH_ANALYSE_FREE
    //m_paRowCoder_Copy[i_Ptn]->vReport(m_pRowCoder_Encode);
#endif
    
    delete m_paRowCoder_Copy[i_Ptn];
  }

}

/*----------------------------------------------------------------------------*/
/*-- Public Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Extract row (with hash) from given record string                           */
/*----------------------------------------------------------------------------*/
bool JobExtractRow::bExtractRow(Stz* szRecStr, bool &bBlankLine)
{
  ColumnVec* pColVec = m_pTable->pColumnVec();
  
  if (m_pRow)
    m_pRow->vResetValues();
  else
    m_pRow = new Row(pColVec);

  char* acRecCrs = szRecStr->aChar();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  bool bResult = RowExtractor::bExtractNext(m_pRow,&acRecCrs,pColVec,
                                         &m_oFormatters,bBlankLine);
  if (! bResult)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //hpRow hp_Row;
  //vTrace("%s\n"_Stz,hp_Row.sz(m_pRow));

  return bResult;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
