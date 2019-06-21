/*============================================================================*/
/* RowCompactor.cpp -- Row Compactor Mark IV                                  */
/*============================================================================*/

#include "Data.h"

static Numbat oNumBat("partKey"_Stz);


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowCompactor* Data::_pRowCoder(void* aAddr)
{
  RowCompactor* pRowCompactor = reinterpret_cast<RowCompactor*>(aAddr);
  
  return pRowCompactor;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowCompactor::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ROWCOMPACTOR);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RowCompactor::operator delete(void* aAddr)
{
  RowCompactor* pRowCompactor = reinterpret_cast<RowCompactor*>(aAddr);
  
  HeapMem::vFree(pRowCompactor);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowCompactor::RowCompactor(void)
: Object(CN_ROWCOMPACTOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  m_pCrwNub       = nullptr;
  m_pColVec       = nullptr;
  m_nHeadSize     = C_UNKNOWN;
  m_nMetaSize     = C_UNKNOWN;
  m_i_BaseOffset  = C_UNKNOWN;

  m_i_Crw         = C_UNKNOWN;
  m_aCrw          = nullptr;
  m_i_DataOffset  = C_UNKNOWN;
  
  m_pRowSamples   = nullptr;
  
  m_nTotalBuffers = 0;
  m_nTotalCrws    = 0;
  
  _checkInitialised_(CN_ROWCOMPACTOR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowCompactor::~RowCompactor(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public Non-Static -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin copying rows into given row nub                                      */
/*----------------------------------------------------------------------------*/
void RowCompactor::vCompactBegin(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  m_pCrwNub      = pCrwBuffer->m_oRowTray.pNub();

  m_pColVec      = m_pCrwNub->pDesc()->pColumnVec();
  m_nHeadSize    = nHeadBytes(m_pColVec);
  m_nMetaSize    = nMetaData(m_pColVec);
  m_i_BaseOffset = iBaseOffset(m_pColVec);
  
  // + + +
  m_pRowSamples  = new IrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* end copying rows into row nub                                              */
/*----------------------------------------------------------------------------*/
void RowCompactor::vCompactEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  //m_pColVec     = nullptr;
  m_nHeadSize     = C_UNKNOWN;
  m_nMetaSize     = C_UNKNOWN;
  m_i_BaseOffset  = C_UNKNOWN;


  m_pCrwNub       = nullptr;
  m_i_Crw         = C_UNKNOWN;
  m_aCrw          = nullptr;
  m_i_DataOffset  = C_UNKNOWN;
  
  // + + +
  delete m_pRowSamples;
  m_pRowSamples  = nullptr;;
}

/*----------------------------------------------------------------------------*/
/* copy (& recode) given row into current nub                                 */
/*----------------------------------------------------------------------------*/
bool RowCompactor::bCompactRow(Row* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  //hpRow hp_Row;
  //vTrace("%s\n"_Stz,hp_Row.sz(pRow));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // encode the row
  
  RowEncoding* pErwRow = new RowEncoding(m_pColVec);

  pErwRow->vEncode(pRow);
  
  //hpRowEncoding hp_RowEnc;
  //vTrace("\n%s\n"_Stz,hp_RowEnc.sz(pRowEncoding));

  pErwRow->vSplitOver(256,m_pColVec->nItems());

  //vTrace("\n%s\n"_Stz,hp_RowEnc.sz(pRowEncoding));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check for space in target nub
  
  i8 nSpaceNeeded = pErwRow->nBytesNeeded();
  
  if (! m_pCrwNub->bHasSpace(nSpaceNeeded)) {
    m_nTotalBuffers++;
    delete pErwRow;
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // initialise copy
  
  m_i_Crw   = m_pCrwNub->iAllotHead(m_nHeadSize);
  m_aCrw    = m_pCrwNub->aGetHead(m_i_Crw);
  m_i_DataOffset = m_i_BaseOffset;
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Mark IV samples
  
  //Row* pSampleRow = m_pRowSamples->aAllotRow(m_pColVec);
  //if (! pSampleRow)
  //  vTrace("No sample space\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compact encoded row into a crw

  vCompactEncoded(pErwRow);
  m_nTotalCrws++;

//#ifdef WITH_TRACE_ENCODING
  if (m_nTotalCrws <= 10)
    HeadLine::vDumpHead("m_aCrw"_Stz,m_aCrw,m_pColVec);
//#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // analyse free space in the into head
  
#ifdef WITH_ANALYSE_FREE
  vAnalyseFree();
#endif
  
#ifdef WITH_VERIFY_TAILS
  m_pCrwNub->vVerifyTails(C_UNKNOWN);
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete pErwRow;

  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Private Non-Static ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyse the free bytes of the head                                         */
/*----------------------------------------------------------------------------*/
void RowCompactor::vAnalyseFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);

  //i8 nFreeBytes = nFreeSpaceHead(m_aCrw,m_pColVec);
  //m_oFreq.vAdd(nFreeBytes);
}

/*----------------------------------------------------------------------------*/
/* compact the encoding                                                       */
/*----------------------------------------------------------------------------*/
void RowCompactor::vCompactEncoded(RowEncoding* pRowEncoding)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  i8 nCols = m_pColVec->nItems();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    Column* pCol = m_pColVec->pGet(i_Col);
    
    eMC eMetaCode = pRowEncoding->xEncode[i_Col].m_eMetaCode;
    Dtm wDataCode = pRowEncoding->xEncode[i_Col].m_wDataCode;
    i8  nDataByte = pRowEncoding->xEncode[i_Col].m_nDataBytes;
    
    //vDumpHead("vHeadCode a"_Stz,m_aCrw,m_pColVec);
    if (8 == i_Col)
      vDebugPoint();
    vDtmCode(eMetaCode,wDataCode,nDataByte,pCol);
    //vDumpHead("vHeadCode b"_Stz,m_aCrw,m_pColVec);
  }
}

/*----------------------------------------------------------------------------*/
/* add encoded datum onto the end of crw                                      */
/*----------------------------------------------------------------------------*/
void RowCompactor::vDtmCode(eMC eMetaCode, Dtm wDataCode, i8 nDataByte, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // meta code

  vMetaCode(m_aCrw,pCol,eMetaCode);
  
  if (! nDataByte)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // data bytes
  
  // non-vch
  if (DM_VCH != pCol->eDatumType()) {
    
    vPutLogical(m_aCrw,
                m_i_DataOffset,
                nDataByte,
                (w8)wDataCode.m_iInt);
    
    m_i_DataOffset += nDataByte;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // vch
  m_aCrw->vPut(m_i_DataOffset,nDataByte,C_1B);
  m_i_DataOffset++;
  
  byte* ayDst = (byte*)m_aCrw + m_i_DataOffset;
  char* acSrc = (char*)wDataCode.m_szVch;

  MemLib::vCopy(ayDst,
                acSrc,
                nDataByte,
                CN_ROWCOMPACTOR);
  
  m_i_DataOffset += nDataByte;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RowCompactor::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
