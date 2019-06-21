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
  
  m_bEncoding      = false;
  m_pColVec        = nullptr;
  m_nMetaData      = C_UNKNOWN;
  m_i_BaseOffset   = C_UNKNOWN;

  m_pIntoNub       = nullptr;
  m_i_IntoHead     = C_UNKNOWN;
  m_aIntoHead      = nullptr;
  m_nHeadBytes     = C_UNKNOWN;
  //---
  m_i_IntoOver     = C_UNKNOWN;
  m_aIntoOver      = nullptr;
  m_nOverBytes     = C_UNKNOWN;
  m_bCopyOver      = false;
  //---
  m_i_IntoOffset   = C_UNKNOWN;
  
  // + + + + + + + + + + + + + +
  
  m_pRowSamples    = nullptr;
  
  m_pRowEncoder    = nullptr;
  
  //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  m_nTotalHeads = 0;
  
  m_nTails     = 0;
  m_nTailUnits = 0;
  
  m_nTotalBuffers = 0;

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
  
  m_pIntoNub     = pCrwBuffer->m_oRowTray.pNub();

  m_bEncoding    = false;
  m_pColVec      = m_pIntoNub->pDesc()->pColumnVec();
  m_nHeadBytes   = nHeadBytes(m_pColVec);
  m_nMetaData    = nMetaData(m_pColVec);
  m_i_BaseOffset = iBaseOffset(m_pColVec);
  m_bCopyOver    = false;
  
  // + + +
  m_pRowSamples  = new IrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* end copying rows into row nub                                              */
/*----------------------------------------------------------------------------*/
void RowCompactor::vCompactEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  m_bEncoding     = false;
  //m_pColVec     = nullptr;
  m_nHeadBytes    = C_UNKNOWN;
  m_nMetaData     = C_UNKNOWN;
  m_i_BaseOffset  = C_UNKNOWN;
  //---
  m_i_IntoOver    = C_UNKNOWN;
  m_aIntoOver     = nullptr;
  m_nOverBytes    = C_UNKNOWN;
  m_bCopyOver     = false;

  m_pIntoNub      = nullptr;
  m_i_IntoHead    = C_UNKNOWN;
  m_aIntoHead     = nullptr;
  m_i_IntoOffset  = C_UNKNOWN;
  
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
  
  RASSERT(! m_bEncoding);
  
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
  
  if (! m_pIntoNub->bHasSpace(nSpaceNeeded)) {
    m_nTotalBuffers++;
    delete m_pRowEncoder;
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // initialise copy
  
  m_i_IntoHead   = m_pIntoNub->iAllotHead(m_nHeadBytes);
  m_aIntoHead    = m_pIntoNub->aGetHead(m_i_IntoHead);
  m_i_IntoOffset = m_i_BaseOffset;
  
  //---
  m_i_IntoOver   = C_UNKNOWN;
  m_aIntoOver    = nullptr;
  m_nOverBytes   = C_UNKNOWN;
  m_bCopyOver    = false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Mark IV samples
  
  //Row* pSampleRow = m_pRowSamples->aAllotRow(m_pColVec);
  //if (! pSampleRow)
  //  vTrace("No sample space\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compact encoded row into a crw

  vCompactEncoded(pErwRow);
  m_nTotalHeads++;

//#ifdef WITH_TRACE_ENCODING
  if (m_nTotalHeads <= 10)
    HeadLine::vDumpHead("m_aIntoHead"_Stz,m_aIntoHead,m_pColVec);
//#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // analyse free space in the into head
  
#ifdef WITH_ANALYSE_FREE
  vAnalyseFree();
#endif
  
#ifdef WITH_VERIFY_TAILS
  m_pIntoNub->vVerifyTails(C_UNKNOWN);
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete m_pRowEncoder;
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

  //i8 nFreeBytes = nFreeSpaceHead(m_aIntoHead,m_pColVec);
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
    
    //vDumpHead("vHeadCode a"_Stz,m_aIntoHead,m_pColVec);
    if (8 == i_Col)
      vDebugPoint();
    vDtmCode(eMetaCode,wDataCode,nDataByte,pCol);
    //vDumpHead("vHeadCode b"_Stz,m_aIntoHead,m_pColVec);
  }
}

/*----------------------------------------------------------------------------*/
/* add encoded datum onto the end of into-head                                */
/*----------------------------------------------------------------------------*/
void RowCompactor::vDtmCode(eMC eMetaCode, Dtm wDataCode, i8 nDataByte, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  vMetaCode(m_aIntoHead,pCol,eMetaCode);
  
  if (! nDataByte)
    return;
  
  if (DM_VCH != pCol->eDatumType()) {
    
    vPutLogical(m_aIntoHead,
                m_i_IntoOffset,
                nDataByte,
                (w8)wDataCode.m_iInt);
    
    m_i_IntoOffset += nDataByte;
    return;
  }
  
  m_aIntoHead->vPut(m_i_IntoOffset,nDataByte,C_1B);
  m_i_IntoOffset++;
  
  char* acSrc = (char*)wDataCode.m_szVch;
  byte* ayDst = (byte*)m_aIntoHead + m_i_IntoOffset;
  MemLib::vCopy(ayDst,
                acSrc,
                nDataByte,
                CN_ROWCOMPACTOR);
  
  m_i_IntoOffset += nDataByte;
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
