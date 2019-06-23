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
  
  m_pRowSamples   = nullptr;
  
  m_nTotalBuffers = 0;
  m_nTotalCrws    = 0;
  
  //m_oFreqDist
#ifdef WITH_ANALYSE_FREE
  m_oFreqDist.vInit(7,15,23,31,39,47,55,63,-1);
#endif
  
  _checkInitialised_(CN_ROWCOMPACTOR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowCompactor::~RowCompactor(void) noexcept(false)
{
  
#ifdef WITH_ANALYSE_FREE
  /*
   vTrace(": HeadFree %lldB : "
   "0..7B:%lld 8..15B:%lld 16..23B:%lld 24..31B:%lld "
   "32..39B:%lld 40..47B:%lld 48..55B:%lld 56..63B:%lld 64..B:%lld\n"_Stz,
   
   m_oFreqDist.nSum(),
   
   m_oFreqDist.iDist(7),
   m_oFreqDist.iDist(15),
   m_oFreqDist.iDist(23),
   m_oFreqDist.iDist(31),
   m_oFreqDist.iDist(39),
   m_oFreqDist.iDist(47),
   m_oFreqDist.iDist(55),
   m_oFreqDist.iDist(63),
   m_oFreqDist.iDist(INT_MAX));
   */
  vTrace(": HeadFree %lldB : "_Stz, m_oFreqDist.nSum());
  i8 iLastBucket = 0;
  i8 iBucket;
  for (i8 i_Index = 0; i_Index < m_oFreqDist.nBuckets() - 1; i_Index++) {
    iBucket = m_oFreqDist.iBucket(i_Index);
    vTrace("%lld..%lldB:%lld "_Stz, iLastBucket, iBucket, m_oFreqDist.iDist(i_Index));
    iLastBucket = iBucket;
  }
  i8 i_Index = m_oFreqDist.nBuckets() - 1;
  iBucket = m_oFreqDist.iBucket(i_Index);
  vTrace("%lld..%lldB:%lld "_Stz, iLastBucket, iBucket, m_oFreqDist.iDist(i_Index));
  vTrace("\n"_Stz);
#endif
  
}


/*----------------------------------------------------------------------------*/
/*-- Public Non-Static -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin compactine rows into given crw buffer                                */
/*----------------------------------------------------------------------------*/
void RowCompactor::vCompactBegin(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  m_pCrwNub       = pCrwBuffer->m_oRowTray.pNub();
  m_pColVec       = m_pCrwNub->pDesc()->pColumnVec();
  m_nHeadSize     = nHeadBytes(m_pColVec);
  m_nMetaSize     = nMetaData(m_pColVec);
  m_i_BaseOffset  = iBaseOffset(m_pColVec);
  
  m_pRowSamples   = new IrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* end copying rows into given crw buffer                                     */
/*----------------------------------------------------------------------------*/
void RowCompactor::vCompactEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  m_pCrwNub       = nullptr;
  m_pColVec       = nullptr;
  m_nHeadSize     = C_UNKNOWN;
  m_nMetaSize     = C_UNKNOWN;
  m_i_BaseOffset  = C_UNKNOWN;
  
  delete m_pRowSamples;
  m_pRowSamples   = nullptr;;
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
  // Mark IV samples
  
  //Row* pSampleRow = m_pRowSamples->aAllotRow(m_pColVec);
  //if (! pSampleRow)
  //  vTrace("No sample space\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compact encoded row into a crw
  
  vCompactEncoded(pErwRow);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // analyse free space in the into head
  
  
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
  
}

/*----------------------------------------------------------------------------*/
/* add encoded datum onto the end of crw                                      */
/*----------------------------------------------------------------------------*/
void RowCompactor::vAddCode(HeadLine* aHeadLine, Column* pCol,
                            eMC eMetaCode, Dtm wDataCode, i8 nDataSize,
                            i8& i_DataOffset_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // put meta code into head
  
  vMetaCode(aHeadLine,pCol,eMetaCode);
  
  if (! nDataSize)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append data bytes onto head
  
  // non-vch
  
  if (DM_VCH != pCol->eDatumType()) {
    
    vPutLogical(aHeadLine,
                i_DataOffset_,
                nDataSize,
                (w8)wDataCode.m_iInt);
    
    i_DataOffset_ += nDataSize;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // vch
  
  // append <L>
  aHeadLine->vPut(i_DataOffset_,nDataSize,C_1B);
  i_DataOffset_++;
  
  // append <short>
  byte* ayDst = (byte*)aHeadLine + i_DataOffset_;
  char* acSrc = (char*)wDataCode.m_szVch;
  
  MemLib::vCopy(ayDst,
                acSrc,
                nDataSize,
                CN_ROWCOMPACTOR);
  
  i_DataOffset_ += nDataSize;
}

/*----------------------------------------------------------------------------*/
/* compact the encoding                                                       */
/*----------------------------------------------------------------------------*/
void RowCompactor::vCompactEncoded(RowEncoding* pRowEncoding)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  i8 i_Crw = m_pCrwNub->iAllotHead(m_nHeadSize);
  HeadLine* aHeadLine = m_pCrwNub->aGetHead(i_Crw);
  i8 i_DataOffset_ = m_i_BaseOffset;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  i8 nCols = m_pColVec->nItems();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    Column* pCol  = m_pColVec->pGet(i_Col);
    
    eMC eMetaCode = pRowEncoding->xEncode[i_Col].m_eMetaCode;
    Dtm wDataCode = pRowEncoding->xEncode[i_Col].m_wDataCode;
    i8  nDataSize = pRowEncoding->xEncode[i_Col].m_nDataSize;
    
    //vDumpHead("vHeadCode a"_Stz,m_aCrw,m_pColVec);
    if (8 == i_Col)
      vDebugPoint();
    vAddCode(aHeadLine,pCol,eMetaCode,wDataCode,nDataSize,i_DataOffset_);
    //vDumpHead("vHeadCode b"_Stz,m_aCrw,m_pColVec);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_nTotalCrws++;
  
  //#ifdef WITH_TRACE_ENCODING
  if (m_nTotalCrws <= 10)
    HeadLine::vDumpHead("aHeadLine"_Stz,aHeadLine,m_pColVec);
  //#endif

#ifdef WITH_ANALYSE_FREE
  i8 nFreeBytes = nFreeSpaceHead(aHeadLine,m_pColVec);
  m_oFreqDist.vAdd(nFreeBytes);
#endif

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
