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

  m_nHeadFreeBytes = 0;
  m_nHeadFreeBytes_0_7   = 0;
  m_nHeadFreeBytes_8_15  = 0;
  m_nHeadFreeBytes_16_23 = 0;
  m_nHeadFreeBytes_24_31 = 0;
  m_nHeadFreeBytes_32_39 = 0;
  m_nHeadFreeBytes_40_47 = 0;
  m_nHeadFreeBytes_48_55 = 0;
  m_nHeadFreeBytes_56_63 = 0;
  m_nHeadFreeBytes_64_   = 0;
  
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
  m_nHeadBytes   = nHeadBytes_(m_pColVec);
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
  
  RowEncoding* pRowEncoding = new RowEncoding(m_pColVec);

  pRowEncoding->vEncode(pRow);
  
  //hpRowEncoding hp_RowEnc;
  //vTrace("\n%s\n"_Stz,hp_RowEnc.sz(pRowEncoding));

  pRowEncoding->vSplitOver(256,m_pColVec->nItems());

  //vTrace("\n%s\n"_Stz,hp_RowEnc.sz(pRowEncoding));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check for space in target nub
  
  i8 nSpaceNeeded = pRowEncoding->nBytesNeeded();
  
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
  
  Row* pSampleRow = m_pRowSamples->aAllotRow(m_pColVec);
  //if (! pSampleRow)
  //  vTrace("No sample space\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compact encoded row into a crw

  vCompact(pRowEncoding);
  m_nTotalHeads++;

//#ifdef WITH_TRACE_ENCODING
  if (m_nTotalHeads <= 10)
    vDumpHead("m_aIntoHead"_Stz,m_aIntoHead,m_pColVec);
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
  delete pRowEncoding;

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

  i8 nFreeBytes = nFreeSpaceHead(m_aIntoHead,m_pColVec);
  m_nHeadFreeBytes  += nFreeBytes;
  
       if (nFreeBytes <= 7 ) m_nHeadFreeBytes_0_7++;
  else if (nFreeBytes <= 15) m_nHeadFreeBytes_8_15++;
  else if (nFreeBytes <= 23) m_nHeadFreeBytes_16_23++;
  else if (nFreeBytes <= 31) m_nHeadFreeBytes_24_31++;
  else if (nFreeBytes <= 39) m_nHeadFreeBytes_32_39++;
  else if (nFreeBytes <= 47) m_nHeadFreeBytes_40_47++;
  else if (nFreeBytes <= 55) m_nHeadFreeBytes_48_55++;
  else if (nFreeBytes <= 63) m_nHeadFreeBytes_56_63++;
  else                       m_nHeadFreeBytes_64_++;
}

/*----------------------------------------------------------------------------*/
/* compact the encoding                                                       */
/*----------------------------------------------------------------------------*/
void RowCompactor::vCompact(RowEncoding* pRowEncoding)
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
    vHeadCode(eMetaCode,wDataCode,nDataByte,pCol);
    //vDumpHead("vHeadCode b"_Stz,m_aIntoHead,m_pColVec);
  }
}

/*----------------------------------------------------------------------------*/
/* add encoded datum onto the end of into-head                                */
/*----------------------------------------------------------------------------*/
void RowCompactor::vHeadCode(eMC eMetaCode, Dtm wBytes, i8 nBytes, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOMPACTOR,_FOOTER_);
  
  vMetaCode(m_aIntoHead,pCol,eMetaCode);
  
  if (! nBytes)
    return;
  
  if (DM_VCH != pCol->eDatumType()) {
    
    vPutLogical(m_aIntoHead,
                m_i_IntoOffset,
                nBytes,
                (w8)wBytes.m_iInt);
    
    m_i_IntoOffset += nBytes;
    return;
  }
  
  m_aIntoHead->vPut(m_i_IntoOffset,nBytes,C_1B);
  m_i_IntoOffset++;
  
  char* acSrc = (char*)wBytes.m_szVch;
  byte* ayDst = (byte*)m_aIntoHead + m_i_IntoOffset;
  MemLib::vCopy(ayDst,
                acSrc,
                nBytes,
                CN_ROWCOMPACTOR);
  
  m_i_IntoOffset += nBytes;
}

/*----------------------------------------------------------------------------*/
/* get address of given shoco column from head line                           */
/*----------------------------------------------------------------------------*/
byte* RowCompactor::ayShoco(HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol)
{
  i8 i_LastOffset = m_i_BaseOffset;
  i8 i_LastColumn = 0;
  
  eMC eEncoding = eGetEncoding(aHeadLine,pCol,pColVec,
                               i_LastColumn,i_LastOffset);
  
  if (MC_SHOCO_FIT_1C != eEncoding)
    return nullptr;
  
  // get address of <C><shoco...>
  byte* ayAddress = ayAdrLogical(aHeadLine,i_LastOffset);
  
  return ayAddress;
}


////////////////////////////////////////////////////////////////////////////////
//                                Static Functions                            //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/*-- Public Static -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* dump head line                                                             */
/*----------------------------------------------------------------------------*/
void RowCompactor::vDumpHead(Stz* szTitle,
                             HeadLine* aHeadLine,
                             ColumnVec* pColVec)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // word dump of headLine (& overLine)
  
  i8 nHeadBytes = aHeadLine->nBytes();
  i8 nHeadWords = aHeadLine->nWords();

  OverLine* aOverLine = nullptr;
  i8 i_Over_rel = aHeadLine->iOverflow2B();
  i8 nOverBytes = C_UNKNOWN;
  i8 nOverUnits = C_UNKNOWN;

  if (aHeadLine->bHasOverflow()) {
    vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);
    
    MemLib::vWordDump(aHeadLine,nHeadBytes);

    aOverLine  = aGetOverRel(aHeadLine,i_Over_rel);
    nOverBytes = aOverLine->nBytes();
    nOverUnits = aOverLine->nUnits();

    vTrace("\n%s OverLine %p (diff %lldB %lldU):\n"_Stz,
           szTitle,aOverLine,
           nSub(aOverLine,aHeadLine),
           nSub(aOverLine,aHeadLine) / sizeof(SpaceUnit));
    
    MemLib::vWordDump(aOverLine,nOverBytes);
  }
  else if (aHeadLine->bIsLargeHead()) {
    vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);
    
    w8* awLastWord  = MemLib::awFindNonZeroEnd(aHeadLine,nHeadWords);
    i8 nWords = (awLastWord - (w8*)aHeadLine) + 1;
    i8 nBytes_clipped = nWords * C_BYTES_PER_WORD;

    MemLib::vWordDump(aHeadLine,nBytes_clipped);
  }
  else {
    vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);
    
    MemLib::vWordDump(aHeadLine,nHeadBytes);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  i8 nFreeBytes = nFreeSpaceHead(aHeadLine,pColVec);
  if (nFreeBytes < 0)
    nFreeBytes = 0;

  if (aHeadLine->bHasOverflow()) {
    i8 i_Over_rel = aHeadLine->iOverflow2B();
    OverLine* aOverLine = RowNub::aGetOver(aHeadLine,i_Over_rel);
    eMC eOverMeta = (eMC)aOverLine->w8_1B(1);
    i8 i_Col_over;
    for (i_Col_over = 0; i_Col_over < nColumns; i_Col_over++)
      if (MC_OVERFLOW_2T == aHeadLine->eMetaCode(i_Col_over))
        break;


    vTrace("HL: 0x%012llx <W>=%lld%s {M}=%lld [OT]=%04x @ col %lld %s FreeBytes %lld\n"
           "OT: 0x%012llx <U>=%lld <M>=%ll02x %s\n"_Stz,
           aHeadLine,
           aHeadLine->nWords(),
           aHeadLine->bIsLargeHead() ? " LARGEHEAD"_Stz : ""_Stz,
           nColumns,
           aHeadLine->iOverflow2B(),
           i_Col_over,
           pColVec->pGet(i_Col_over)->szColNm(),
           nFreeBytes,

           aOverLine,
           nOverUnits,
           eOverMeta,
           aMC[eOverMeta]
          );
  }
  else {
    vTrace("HL: 0x%012llx <W>=%lld%s FreeBytes %lld; "
           "OT: none\n"_Stz,
           aHeadLine,
           aHeadLine->nWords(),
           aHeadLine->bIsLargeHead() ? " LARGEHEAD"_Stz : ""_Stz,
           nFreeBytes);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  HV_Rec hvRec(pColVec);
  Datum oDatum;
  
  i8 i_Offset = iBaseOffset(pColVec);
  
  vTrace("Col ColNm         Datum MetaCode"
         "           Offset Address      DataBytes           Value\n"_Stz);
  
  
  eDM   eDatum;
  Stz*  szDatum;
  eMC   eMetaCode;
  Stz*  szMetaCode;
  byte* ayAddr;
  i8    nBytes;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
 // for each column
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    
    // get the column at index
    Column* pCol = pColVec->pGet(i_Col);
    
    // get the encoded information
    eDatum     = pCol->eDatumType();
    szDatum    = aDM[eDatum];
    eMetaCode  = aHeadLine->eMetaCode(i_Col);
    szMetaCode = aMC[eMetaCode];
    
    if (MC_OVERFLOW_2T == eMetaCode) {
      i8 i_Over_rel = aHeadLine->iOverflow2B();
      OverLine* aOverLine = RowNub::aGetOver(aHeadLine,i_Over_rel);
      eMetaCode  = (eMC)aOverLine->w8_1B(1);
      szMetaCode = aMC[eMetaCode];
      i_Offset   = nHeadBytes + 2;
      ayAddr     = ayAdrLogical(aHeadLine,i_Offset);
      nBytes     = nDataBytes(eMetaCode);
      //vTrace("--- --------------- --- -- ------------------- "
      //       "-- ------------ -- ---------------- -----------\n"_Stz);
      vTrace("*** *************** *** ** ******************* "
             "** ************ ** **************** **********************\n"_Stz);
    }
    else {
      ayAddr = ayAdrLogical(aHeadLine,i_Offset);
      nBytes = nDataBytes(eMetaCode);
    }
    if (MC_STR_FIT_1L == eMetaCode) {
      RASSERT(1 == nBytes);
      nBytes += wGetLogical(aHeadLine,i_Offset,C_1B);
    }
    if (MC_SHOCO_FIT_1C == eMetaCode) {
      RASSERT(1 == nBytes);
      nBytes += wGetLogical(aHeadLine,i_Offset,C_1B);
    }

    //      colNo colNm Dtm  MC   aMC   Ofset Addr   Bytes
    vTrace("%3lld %-15s %-3s %02x %-19s %2lld %012llx %2lld "_Stz,
           i_Col,
           pCol->szColNm(),
           szDatum,
           eMetaCode,
           szMetaCode,
           i_Offset,
           ayAddr,
           nBytes);
    
    if (nBytes) {
      if (MC_OVERFLOW_2T == eMetaCode) {
        for (i8 i_Byte = 0; i_Byte < nBytes; i_Byte++)
          vTrace("%02x"_Stz,*(ayAddr + i_Byte));
        //vTrace("\n"_Stz); //! temp
        //break;
      }
      else
        for (i8 i_Byte = 0; i_Byte < nBytes; i_Byte++)
          vTrace("%02x"_Stz,wGetLogical(aHeadLine,(i_Offset+i_Byte),C_1B));
    }
    
    for (i8 i_Byte = nBytes; i_Byte < 8; i_Byte++)
      vTrace("__"_Stz);
    vTrace(" "_Stz);
    
    hvRec->vReset();
    
    // first get the encoded value into datum
    RowInflator::vDecode(&oDatum,aHeadLine,pColVec,pCol,i_Col,i_Offset);
    hvRec->vAppend(pCol, &oDatum);
    
    vTrace("%s"_Stz,hvRec->szRecStr());
    
    if (MC_SHOCO_SPLIT_2T1C == eMetaCode) {
      i8 i2T = wGetLogical(aHeadLine,i_Offset,C_2B);
      i8 i1C = wGetLogical(aHeadLine,(i_Offset+2),C_1B);
      vTrace(" [ =%lld.%lld ]"_Stz,i2T,i1C);
    }
    else if (MC_SHOCO_FIT_1C == eMetaCode) {
      i8 i1C = wGetLogical(aHeadLine,i_Offset,C_1B);
      vTrace(" [ =%lld ]"_Stz,i1C);
    }
    else if (MC_STR_FIT_1L == eMetaCode) {
      i8 i1C = wGetLogical(aHeadLine,i_Offset,C_1B);
      vTrace(" [ =%lld ]"_Stz,i1C);
    }
    else {
      i8 iDataValue = wGetLogical(aHeadLine,i_Offset,nBytes);
      if (nBytes)
        vTrace(" [ =%lld ]"_Stz,iDataValue);
      else
        vTrace(" [ =nil ]"_Stz);
    }
    
    vTrace("\n"_Stz);
    
    i_Offset += nBytes;
    
    if (eMetaCode == MC_STR_SPLIT_2T1L)
      vDebugPoint();
    if (eMetaCode == MC_STR_DUP_2H)
      vDebugPoint();
    if (eMetaCode == MC_SHOCO_DUP_3D)
      vDebugPoint();
  }
  
  
  //vDumpHead_IV("DumpHeap_IV"_Stz,aHeadLine,pColVec);
}

/*----------------------------------------------------------------------------*/
/* dump head line                                                             */
/*----------------------------------------------------------------------------*/
void RowCompactor::vDumpHead_IV(Stz* szTitle,
                                HeadLine* aHeadLine,
                                ColumnVec* pColVec)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // word dump of headLine (& overLine)
  
  i8 nHeadBytes = aHeadLine->nBytes();
  i8 nHeadWords = aHeadLine->nWords();
  
  OverLine* aOverLine = nullptr;
  i8 i_Over_rel = aHeadLine->iOverflow2B();
  i8 nOverBytes = C_UNKNOWN;
  i8 nOverUnits = C_UNKNOWN;
  
  if (aHeadLine->bHasOverflow()) {
    vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);
    
    MemLib::vWordDump(aHeadLine,nHeadBytes);
    
    aOverLine  = aGetOverRel(aHeadLine,i_Over_rel);
    nOverBytes = aOverLine->nBytes();
    nOverUnits = aOverLine->nUnits();
    
    vTrace("\n%s OverLine %p (diff %lldB %lldU):\n"_Stz,
           szTitle,aOverLine,
           nSub(aOverLine,aHeadLine),
           nSub(aOverLine,aHeadLine) / sizeof(SpaceUnit));
    
    MemLib::vWordDump(aOverLine,nOverBytes);
  }
  else if (aHeadLine->bIsLargeHead()) {
    vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);
    
    w8* awLastWord  = MemLib::awFindNonZeroEnd(aHeadLine,nHeadWords);
    i8 nWords = (awLastWord - (w8*)aHeadLine) + 1;
    i8 nBytes_clipped = nWords * C_BYTES_PER_WORD;
    
    MemLib::vWordDump(aHeadLine,nBytes_clipped);
  }
  else {
    vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);
    
    MemLib::vWordDump(aHeadLine,nHeadBytes);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  i8 nFreeBytes = nFreeSpaceHead(aHeadLine,pColVec);
  if (nFreeBytes < 0)
    nFreeBytes = 0;
  
  if (aHeadLine->bHasOverflow()) {
    i8 i_Over_rel = aHeadLine->iOverflow2B();
    OverLine* aOverLine = RowNub::aGetOver(aHeadLine,i_Over_rel);
    eMC eOverMeta = (eMC)aOverLine->w8_1B(1);
    i8 i_Col_over;
    for (i_Col_over = 0; i_Col_over < nColumns; i_Col_over++)
      if (MC_OVERFLOW_2T == aHeadLine->eMetaCode(i_Col_over))
        break;
    
    
    vTrace("HL: 0x%012llx <W>=%lld%s {M}=%lld [OT]=%04x @ col %lld %s FreeBytes %lld\n"
           "OT: 0x%012llx <U>=%lld <M>=%ll02x %s\n"_Stz,
           aHeadLine,
           aHeadLine->nWords(),
           aHeadLine->bIsLargeHead() ? " LARGEHEAD"_Stz : ""_Stz,
           nColumns,
           aHeadLine->iOverflow2B(),
           i_Col_over,
           pColVec->pGet(i_Col_over)->szColNm(),
           nFreeBytes,
           
           aOverLine,
           nOverUnits,
           eOverMeta,
           aMC[eOverMeta]
           );
  }
  else {
    vTrace("HL: 0x%012llx <W>=%lld%s FreeBytes %lld; "
           "OT: none\n"_Stz,
           aHeadLine,
           aHeadLine->nWords(),
           aHeadLine->bIsLargeHead() ? " LARGEHEAD"_Stz : ""_Stz,
           nFreeBytes);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  HV_Rec hvRec(pColVec);
  Datum oDatum;
  
  i8 i_Offset = iBaseOffset(pColVec);
  
  vTrace("Col ColNm         Datum MetaCode"
         "           Offset Address      DataBytes           Value\n"_Stz);
  
  
  eDM   eDatum;
  Stz*  szDatum;
  eMC   eMetaCode;
  Stz*  szMetaCode;
  byte* ayAddr;
  i8    nBytes;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Row* pRow = new Row(pColVec);
  
  RowInflator::vInflate(pRow,aHeadLine);
  
  // for each column
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    
    // get the column
    Column* pCol = pColVec->pGet(i_Col);
    
    // get the encoded information
    eDatum     = pCol->eDatumType();
    szDatum    = aDM[eDatum];
    eMetaCode  = aHeadLine->eMetaCode(i_Col);
    szMetaCode = aMC[eMetaCode];
    
    vTrace("%3lld %-15s %-3s %02x\n"_Stz,
           i_Col,
           pCol->szColNm(),
           szDatum,
           eMetaCode,
           szMetaCode
           );
    
  } // for (i_Col)
  
  delete pRow;
}

/*----------------------------------------------------------------------------*/
/*-- Private Static ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* calc size of the head line for column vector                               */
/*----------------------------------------------------------------------------*/
i8 RowCompactor::nHeadBytes_(ColumnVec* pColVec)
{
  // experiment 1
  //i8 nBytes = sizeof(CacheLine) + (1 * C_BYTES_PER_WORD);
  //i8 nHeadWords = MemLib::nQuantaRequired(nBytes, sizeof(Word));
  
  // experiment 2
  //i8 nBytes = pColVec->nItems() * 4;
  //i8 nHeadBytes = MemLib::nQuantaRequired(nBytes, sizeof(Word));
  
  // experiment 3
  //i8 nBytes = pColVec->nItems() * 5;
  //i8 nHeadBytes = MemLib::nQuantaRequired(nBytes, sizeof(Word));
  
  // experiment 4
  //i8 nBytes = pColVec->nItems() * 6;
  //i8 nHeadBytes = MemLib::nQuantaRequired(nBytes, sizeof(Word));
  
  // seth
  //i8 nBytes = pColVec->nItems() * 10;
  //i8 nHeadBytes = MemLib::nQuantaRequired(nBytes, sizeof(Word));
  
  //nBytes     = 24;
  //nHeadWords = MemLib::nQuantaRequired(nBytes, sizeof(Word));
  
  //i8 nBytesHead = nHeadWords * (i8)C_BYTES_PER_WORD;
  i8 nBytesHead = 256;
  RASSERT(nBytesHead >= C_HEAD_MIN_BYTES);
  
  return nBytesHead;
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
