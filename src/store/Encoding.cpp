/*============================================================================*/
/* Encoding.cpp -- Row Encoding (intermediate representation of compact row)  */
/*============================================================================*/

#include "BASE.h"

#include "Row.hpp"

#include "RowCoder.hpp"
#include "Encoding.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Encoding,CN_ENCODING)
__aNewFunction(Encoding,CN_ENCODING)
__vDeleteFunction(Encoding,CN_ENCODING)
__aFooterFunction(Encoding)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Encoding::Encoding(StreamDef* pStreamDef)
: Object(CN_ENCODING)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  m_pStreamDef = pStreamDef;
  m_nColumns   = pStreamDef->nColumns();
  
  m_jHeadSize  = nHeadSize(m_pStreamDef);
  m_jMetaSize  = nMetaSize(m_pStreamDef);
  m_i_BaseOffsetData = iBaseOffsetData(m_pStreamDef);
 
  m_jOverSize  = 0;
  
  m_jBytesNeeded = C_UNKNOWN;
  m_nShiftOver   = C_UNKNOWN;

  m_i_Offset_next = C_UNKNOWN;

  m_nExtraTails   = 0;

  // - - - - - - - - - - - - - - - - - -

  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    
    xaColEnc[i_Col].m_pCol       = nullptr;
    
    xaColEnc[i_Col].m_uMetaCode  = 0;
    xaColEnc[i_Col].m_wDataCode  = C_UNKNOWN;
    xaColEnc[i_Col].m_jDataSize  = C_UNKNOWN;
    xaColEnc[i_Col].m_i_Offset   = 0;
    xaColEnc[i_Col].m_i_Till     = 0;
    xaColEnc[i_Col].m_bSplit     = false;
    
    xaColEnc[i_Col].m_i_Tail     = C_UNKNOWN;
    xaColEnc[i_Col].m_aTailLine  = nullptr;
  }
  
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    xaColEnc[i_Col].m_pCol   = m_pStreamDef->pGetColumn(i_Col);
  }
  
  //MemLib::vZero(m__PadToCacheLine,sizeof(m__PadToCacheLine),CN_ENCODING);

#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_ENCODING);
#else
  // m_xWsp
#endif
  
  _checkInitialised_(CN_ENCODING);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Encoding::~Encoding(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* how many bytes need in compact form                                        */
/*----------------------------------------------------------------------------*/
i8 Encoding::jBytesNeeded(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  return m_jBytesNeeded;
}

/*----------------------------------------------------------------------------*/
/* dump encoding                                                              */
/*----------------------------------------------------------------------------*/
void Encoding::vDumpEncoding(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);

  vTrace("Encoding\n"_Stz);
  vTrace("--------\n"_Stz);
  
  
  vTrace("m_nColumns         : %lld\n"_Stz, m_nColumns);
  vTrace("m_jHeadSize        : %lld\n"_Stz, m_jHeadSize);
  vTrace("m_jOverSize        : %lld\n"_Stz, m_jOverSize);
  vTrace("m_jBytesNeeded     : %lld\n"_Stz, m_jBytesNeeded);
  vTrace("m_jMetaSize        : %lld\n"_Stz, m_jMetaSize);
  vTrace("m_i_BaseOffsetData : %lld\n"_Stz, m_i_BaseOffsetData);
  vTrace("m_nShiftOver       : %lld\n"_Stz, m_nShiftOver);
  vTrace("m_i_Offset_next    : %lld\n"_Stz, m_i_Offset_next);
  vTrace("m_nExtraTails      : %lld\n"_Stz, m_nExtraTails);

  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    
    _ColEnc& rxColEnc = xaColEnc[i_Col];

    vTrace("[%02lld]\n"_Stz,i_Col);
    vTrace("  m_sColumnNm      : %s\n"_Stz,   rxColEnc.m_pCol->szColNm());
    vTrace("  m_eColumnType    : %s\n"_Stz,   aCT[rxColEnc.m_pCol->eColumnType()]);
    vTrace("  m_uHigh          : %s\n"_Stz,   aMC[rxColEnc.m_uHigh]);
    vTrace("  m_uLow           : %lld\n"_Stz, rxColEnc.m_uLow);
    vTrace("  m_wDataCode      : ???\n"_Stz);
    vTrace("  m_jDataSize      : %lld\n"_Stz, rxColEnc.m_jDataSize);
    vTrace("  m_i_Offset       : %lld\n"_Stz, rxColEnc.m_i_Offset);
    vTrace("  m_i_Till         : %lld\n"_Stz, rxColEnc.m_i_Till);
    vTrace("  m_bSplit         : %lld\n"_Stz, (i8)rxColEnc.m_bSplit);
    vTrace("  m_i_Tail         : %lld\n"_Stz, rxColEnc.m_i_Tail);
    vTrace("  m_aTailLine      : %p\n"_Stz,   rxColEnc.m_aTailLine);

  }
  
  vTrace("End Encoding\n"_Stz);
  vTrace("============\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* encode inflated row                                                        */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeRow(Row* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  vReset();
  
  vInitialEncodingRow(pRow);
}

/*----------------------------------------------------------------------------*/
/* morph the given column encoding (to MC_STRING_TAIL)                        */
/*----------------------------------------------------------------------------*/
void Encoding::vMorphEncoding(_ColEnc* aColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  //ASSERT(MC_STR_FIT_1L == aColEnc->m_eMetaCode);
  ASSERT(METACODE_HIGH(aColEnc->m_uMetaCode) >= MC_STRING_x1 ||
           (METACODE_HIGH(aColEnc->m_uMetaCode) >= MC_STRING_x2 &&
            METACODE_HIGH(aColEnc->m_uMetaCode) <= MC_STRING_x8));
  ASSERT(C_UNKNOWN != aColEnc->m_i_Tail);

  // calc the adjustment required because MC_STR_FIT_1L becoming MC_STR_TAIL_2T
  ASSERT(aColEnc->m_jDataSize > C_2B);
  i8 nOffsetAdj = - (aColEnc->m_jDataSize - C_2B);
  ASSERT(nOffsetAdj < 0);
  
  // morph the given column into a MC_STR_TAIL_2T
  aColEnc->m_uMetaCode = METACODE_FORM(MC_STRING_TAIL,2);
  aColEnc->m_wDataCode = aColEnc->m_i_Tail;
  aColEnc->m_jDataSize = C_2B;
  aColEnc->m_i_Till   += nOffsetAdj;

  // adjust remaining columns based on MC_STR_FIT_1L becoming MC_STR_TAIL_2T
  for (i8 i_RmgCol = (1 + aColEnc->m_pCol->iColumnNo());
       i_RmgCol < m_nColumns;
       i_RmgCol++) {
    xaColEnc[i_RmgCol].m_i_Offset += nOffsetAdj;
    xaColEnc[i_RmgCol].m_i_Till   += nOffsetAdj;
  }
  
  m_jBytesNeeded += nOffsetAdj;
  m_jOverSize = m_jBytesNeeded - m_jHeadSize;
}

/*----------------------------------------------------------------------------*/
/* does this encoding overflow the head?                                      */
/*----------------------------------------------------------------------------*/
bool Encoding::bOverFlowed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  if (m_nShiftOver > 0)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* calc the size of the over line                                             */
/*----------------------------------------------------------------------------*/
i8 Encoding::jOverSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  i8 nBytesNeed = this->jBytesNeeded();
  i8 nOverSize  = nBytesNeed - m_jHeadSize;
  
  RASSERT(nOverSize == m_jOverSize);
  return m_jOverSize;
}

/*----------------------------------------------------------------------------*/
/* split at headSize for overflow (if necessary)                              */
/*----------------------------------------------------------------------------*/
void Encoding::vSplitOver(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // test if encoding fits in head
  if (m_jBytesNeeded <= m_jHeadSize) {
    m_nShiftOver = 0;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // split into head and oveflow
  
  Column* pSplitCol = pNonFitting();
  
  // ray : 30 may 2020 : i added this if to test for pSplitCol.  was crashing
  if (pSplitCol) {
    i8 i_SplitCol = pSplitCol->iColumnNo();
  
    // split at that column
    vSplitAt(i_SplitCol);
  }
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* encode the columns of given row and compute their initial offsets          */
/*----------------------------------------------------------------------------*/
void Encoding::vInitialEncodingRow(Row* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  m_i_Offset_next = m_i_BaseOffsetData;
  
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    
    _ColEnc& xrColEnc = xaColEnc[i_Col];
    
    Dtm wColVal = pRow->wColDtm(i_Col);
    
    // - - - - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // encode the column value
    
    switch (xrColEnc.m_pCol->eDatum()) {
        
      case DM_BCD: vEncodeBcd(xrColEnc,wColVal);  break;
      case DM_BOL: vEncodeBol(xrColEnc,wColVal);  break;
      case DM_DTE: vEncodeDte(xrColEnc,wColVal);  break;
      case DM_FLT: vEncodeFlt(xrColEnc,wColVal);  break;
      case DM_INT: vEncodeInt(xrColEnc,wColVal);  break;
      case DM_IVL: vEncodeIvl(xrColEnc,wColVal);  break;
      case DM_IVY: NEVER_REACH;                   break;
      case DM_TME: vEncodeTme(xrColEnc,wColVal);  break;
      case DM_TSP: vEncodeTsp(xrColEnc,wColVal);  break;
      case DM_VCH:
        // get the stz from the row
        if ( ! wColVal.bIsNull() )
          wColVal.m_szVch = pRow->szColVal_ofs(i_Col);
        vEncodeVch(xrColEnc,wColVal);
        break;
      case DM_UNK: NEVER_REACH;                   break;

      default:
        NEVER_REACH;
        
    } // switch eDatum
    
    // - - - - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // compute initial offset and till for this column
    
    vOffsetTill(xrColEnc);
    
  } // for i_Col
  
  m_jBytesNeeded = m_i_Offset_next;
  
  m_jOverSize = m_jBytesNeeded - m_jHeadSize;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* find first non-fitting column                                              */
/*----------------------------------------------------------------------------*/
Column* Encoding::pNonFitting(void)
{
  i8 i_LastCol = m_nColumns - 1;
    
  // last column fits? (e.g. 0..31 < 32)
  if (xaColEnc[i_LastCol].m_i_Till < m_jHeadSize)
    return nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (i8 i_Col = i_LastCol - 1; i_Col >= 0; i_Col--) {

    if (xaColEnc[i_Col].m_i_Till < m_jHeadSize - C_OT_2B)
      return xaColEnc[ (i_Col + 1) ].m_pCol;
  }

  return xaColEnc[0].m_pCol;
}

/*----------------------------------------------------------------------------*/
/* reset all encodings                                                        */
/*----------------------------------------------------------------------------*/
void Encoding::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  m_jBytesNeeded = C_UNKNOWN;
  m_nShiftOver   = C_UNKNOWN;
  
  m_i_Offset_next = C_UNKNOWN;

  //for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {

    // leave m_pCol
    // leave m_eDatum
    
    xaColEnc[i_Col].m_wDataCode  = C_UNKNOWN;
    xaColEnc[i_Col].m_jDataSize  = C_UNKNOWN;
    xaColEnc[i_Col].m_i_Offset   = 0;
    xaColEnc[i_Col].m_i_Till     = 0;
    xaColEnc[i_Col].m_bSplit     = false;
    
    xaColEnc[i_Col].m_i_Tail     = C_UNKNOWN;
    xaColEnc[i_Col].m_aTailLine  = nullptr;
  }
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_ENCODING);
#else
  m_xWsp.vInit();
#endif
}

/*----------------------------------------------------------------------------*/
/* split columns at head size for overflow                                    */
/*----------------------------------------------------------------------------*/
void Encoding::vSplitAt(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  // mark the column where split occurred
  xaColEnc[i_Col].m_bSplit = true;
  
  // calc the number of bytes needed to shift columns into overflow
  m_nShiftOver = m_jHeadSize - xaColEnc[i_Col].m_i_Offset + C_U_1B + C_M_1B;
  
  m_jBytesNeeded += m_nShiftOver;
  m_jOverSize = m_jBytesNeeded - m_jHeadSize;

  // shift columns into overflow
  for ( ; i_Col < m_nColumns; i_Col++) {
    xaColEnc[i_Col].m_i_Offset += m_nShiftOver;
    xaColEnc[i_Col].m_i_Till   += m_nShiftOver;
  }
}

/*----------------------------------------------------------------------------*/
/* test if column fits within head                                            */
/*----------------------------------------------------------------------------*/
bool Encoding::bWithinHead(i8 i_Col)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // a) last column
  
  if (i_Col == m_nColumns - 1) {

    // last column fits? (e.g. 0..31 < 32)
    if (xaColEnc[i_Col].m_i_Till < m_jHeadSize)
      return true;

    // last column doesnt fit
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // b) not-last column
  
  // this and next column fit? (e.g. 0..31 < 32)
  i8 i_Col_next = i_Col + 1;
  if (xaColEnc[i_Col_next].m_i_Till < m_jHeadSize)
    return true;
 
  // this column plus overflow fit? (e.g. 0..29 + 2 < 32)
  if (xaColEnc[i_Col].m_i_Till + C_OT_2B < m_jHeadSize)
    return true;

  // this column doesnt fit
  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
