/*============================================================================*/
/* JobExtractRow.cpp -- Extract Row Job                                       */
/*============================================================================*/

#include "Jobs.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* JobExtractRow::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: JobExtractRow class cannot be new'd\n"_Stz);
  
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
    m_paRowCompactor[i_Ptn] = nullptr;
  
  for (i8 i_Ptn = 0; i_Ptn < AM::nPartitions(); i_Ptn++)
    m_paRowCompactor[i_Ptn] = new RowCompactor;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobExtractRow::~JobExtractRow(void) noexcept(false)
{
  delete m_pRow;
  
  for (i8 i_Ptn = 0; i_Ptn < AM::nPartitions(); i_Ptn++) {
    delete m_paRowCompactor[i_Ptn];
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
  StreamDef* pStreamDef = m_pTable->pStreamDef();
  
  if (m_pRow)
    m_pRow->vResetValues();
  else
    m_pRow = new Row(pStreamDef,C_ALL);

  char* acRecCrs = szRecStr->azChar();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  bool bResult = RowExtractor::bExtractNext(m_pRow,&acRecCrs,pStreamDef,
                                         &m_oFormatters,bBlankLine);
  if (! bResult)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //SP_Row spRow;
  //vTrace("%s\n"_Stz,spRow.sz(m_pRow));

  return bResult;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
