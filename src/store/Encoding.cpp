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
  
  m_jHeadSize  = jHeadSizeStream(m_pStreamDef);
  m_jMetaSize  = nMetaSize(m_pStreamDef);
  m_o_DataOffsetBase = oDataFromBase(m_pStreamDef);
 
  m_jOverSize  = 0;
  
  m_jBytesNeeded = C_UNKNOWN;
  m_nShiftOver   = C_UNKNOWN;

  m_o_DataOffsetNext = C_UNKNOWN;

  m_nExtraTails   = 0;
  m_jExtraSize    = 0;
  
  // - - - - - - - - - - - - - - - - - -

  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    _ColEnc& rxColEnc = xaColEnc[i_Col];
    
    rxColEnc.m_pCol   = nullptr;
    rxColEnc.m_wDatum = C_UNKNOWN;

    rxColEnc.m_uMetaCode  = 0;
    rxColEnc.m_wDataCode  = C_UNKNOWN;
    rxColEnc.m_jDataBytes = C_UNKNOWN;
    rxColEnc.m_jVchSize   = C_UNKNOWN;

    rxColEnc.m_o_DataFrom = 0;
    rxColEnc.m_o_DataBreak= 0;
    rxColEnc.m_bSplit     = false;
    rxColEnc.m_bInOver    = false;

    rxColEnc.m_i_Tail     = C_UNKNOWN;
    rxColEnc.m_aTailLine  = nullptr;
    rxColEnc.m_bDuplicate = false;
    rxColEnc.m_jTailBytes = 0;
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
  vTrace("m_i_DataOffsetBase : %lld\n"_Stz, m_o_DataOffsetBase);
  vTrace("m_nShiftOver       : %lld\n"_Stz, m_nShiftOver);
  vTrace("m_i_Offset_next    : %lld\n"_Stz, m_o_DataOffsetNext);
  vTrace("m_nExtraTails      : %lld\n"_Stz, m_nExtraTails);
  vTrace("m_jExtraSize       : %lld\n"_Stz, m_jExtraSize);

  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    
    _ColEnc& rxColEnc = xaColEnc[i_Col];

    vTrace("[%02lld]\n"_Stz,i_Col);
    vTrace("  m_sColumnNm      : %s\n"_Stz,   rxColEnc.m_pCol->szColNm());
    vTrace("  m_eColumnType    : %s\n"_Stz,   aCT[rxColEnc.m_pCol->eColumnType()]);
    vTrace("  m_bSplit         : %lld\n"_Stz, (i8)rxColEnc.m_bSplit);
    vTrace("  m_uHigh          : %s\n"_Stz,   aMC[rxColEnc.m_uHigh]);
    vTrace("  m_uLow           : %lld\n"_Stz, rxColEnc.m_uLow);
    
    if (CT_VARCHAR == rxColEnc.m_pCol->eColumnType() &&
        MC_NULL != rxColEnc.m_uHigh &&
        MC_STRING_TAIL != rxColEnc.m_uHigh)
      vTrace("  m_wDataCode      : %s\n"_Stz,   rxColEnc.m_wDataCode.m_azStr);
    else
      vTrace("  m_wDataCode      : %lld\n"_Stz,   rxColEnc.m_wDataCode.m_iWrd);
    
    vTrace("  m_jDataBytes     : %lld\n"_Stz, rxColEnc.m_jDataBytes);
    vTrace("  m_jVchSize       : %lld\n"_Stz, rxColEnc.m_jVchSize);
    vTrace("  m_bInOver        : %lld\n"_Stz, (i8)rxColEnc.m_bInOver);
    vTrace("  m_o_DataFrom     : %lld\n"_Stz, rxColEnc.m_o_DataFrom);
    vTrace("  m_o_DataBreak    : %lld\n"_Stz, rxColEnc.m_o_DataBreak);
    vTrace("  m_i_Tail         : %lld\n"_Stz, rxColEnc.m_i_Tail);
    vTrace("  m_aTailLine      : %p\n"_Stz,   rxColEnc.m_aTailLine);
    vTrace("  m_bDuplicate     : %lld\n"_Stz, (i8)rxColEnc.m_bDuplicate);
    vTrace("  m_jTailBytes     : %lld\n"_Stz, rxColEnc.m_jTailBytes);

  }
  
  vTrace("End Encoding\n"_Stz);
  vTrace("============\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* morph the given column encoding (to MC_STRING_TAIL)                        */
/*----------------------------------------------------------------------------*/
void Encoding::vMorphEncoding(_ColEnc& rxColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  //ASSERT(MC_STR_FIT_1L == aColEnc->m_eMetaCode);
  ASSERT(  (METACODE_HIGH(rxColEnc.m_uMetaCode) >= MC_STRING_x1)
         ||
           (METACODE_HIGH(rxColEnc.m_uMetaCode) >= MC_STRING_x2 &&
            METACODE_HIGH(rxColEnc.m_uMetaCode) <= MC_STRING_x8));
  ASSERT(C_UNKNOWN != rxColEnc.m_i_Tail);

  // calc the adjustment required because MC_STR_FIT_1L becoming MC_STR_TAIL_2T
  ASSERT(rxColEnc.m_jDataBytes > C_2B);
  i8 nOffsetAdj = - (rxColEnc.m_jDataBytes - C_2B);
  ASSERT(nOffsetAdj < 0);
  
  // morph the given column into a MC_STR_TAIL_2T
  rxColEnc.m_uMetaCode = METACODE_FORM(MC_STRING_TAIL,2);
  rxColEnc.m_wDataCode = rxColEnc.m_i_Tail;
  rxColEnc.m_jDataBytes = C_2B;
  rxColEnc.m_o_DataBreak += nOffsetAdj;

  // adjust remaining columns based on MC_STR_FIT_1L becoming MC_STR_TAIL_2T
  for (i8 i_RmgCol = (1 + rxColEnc.m_pCol->iColumnNo());
       i_RmgCol < m_nColumns;
       i_RmgCol++) {
    xaColEnc[i_RmgCol].m_o_DataFrom  += nOffsetAdj;
    xaColEnc[i_RmgCol].m_o_DataBreak += nOffsetAdj;
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
  
  m_nShiftOver = 0;
  
  // test if encoding fits in head
  if (m_jBytesNeeded <= m_jHeadSize)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // split into head and oveflow
  
  Column* pSplitCol = pNonFitting();
  if (! pSplitCol)
    return;
  
  i8 i_SplitCol = pSplitCol->iColumnNo();
  xaColEnc[i_SplitCol].m_bSplit = true;
  
  // calc the number of bytes needed to shift columns into overflow
  m_nShiftOver = m_jHeadSize - xaColEnc[i_SplitCol].m_o_DataFrom + C_U_1B + C_M_1B;
  
  m_jBytesNeeded += m_nShiftOver;

  m_jOverSize = m_jBytesNeeded - m_jHeadSize;

  // shift columns into overflow
  for ( ; i_SplitCol < m_nColumns; i_SplitCol++) {
    xaColEnc[i_SplitCol].m_o_DataFrom  += m_nShiftOver;
    xaColEnc[i_SplitCol].m_o_DataBreak += m_nShiftOver;
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
  
  vReset();
  
  m_o_DataOffsetNext = m_o_DataOffsetBase;
  
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    
    _ColEnc& xrColEnc = xaColEnc[i_Col];
    
    // - - - - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // encode the column value
    
    eDM eDatum  = xrColEnc.m_pCol->eDatum();
    Dtm wColVal = pRow->wColDtm(i_Col);
    switch (eDatum) {
        
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
    // compute initial from and break offsets for this column
    
    // set the from offset of this column
    xrColEnc.m_o_DataFrom = m_o_DataOffsetNext;
    
    // if 0B case then break offset is same as from offset
    if (! xrColEnc.m_jDataBytes) {
      xrColEnc.m_o_DataBreak = m_o_DataOffsetNext;
      continue;
    }
          
    // set where the next data offset will be
    m_o_DataOffsetNext += xrColEnc.m_jDataBytes;
    
    // set the break offset byte of this column
    xrColEnc.m_o_DataBreak = m_o_DataOffsetNext;
  } // for i_Col
  
  m_jBytesNeeded = m_o_DataOffsetNext;
  
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
  if (xaColEnc[i_LastCol].m_o_DataBreak < m_jHeadSize)
    return nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (i8 i_Col = i_LastCol - 1; i_Col >= 0; i_Col--) {

    if (xaColEnc[i_Col].m_o_DataBreak < m_jHeadSize - C_OT_2B)
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
  
  m_o_DataOffsetNext = C_UNKNOWN;

  //for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    _ColEnc& rxColEnc = xaColEnc[i_Col];
    
    // leave m_pCol
    rxColEnc.m_wDatum     = C_UNKNOWN;

    rxColEnc.m_wDataCode  = C_UNKNOWN;
    rxColEnc.m_jDataBytes = C_UNKNOWN;
    rxColEnc.m_jVchSize   = C_UNKNOWN;
    rxColEnc.m_o_DataFrom = 0;
    rxColEnc.m_o_DataBreak= 0;
    rxColEnc.m_bSplit     = false;
    rxColEnc.m_bInOver    = false;

    rxColEnc.m_i_Tail     = C_UNKNOWN;
    rxColEnc.m_aTailLine  = nullptr;
    rxColEnc.m_bDuplicate = false;
    rxColEnc.m_jTailBytes = 0;
  }
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_ENCODING);
#else
  m_xWsp.vInit();
#endif
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
    if (xaColEnc[i_Col].m_o_DataBreak < m_jHeadSize)
      return true;

    // last column doesnt fit
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // b) not-last column
  
  // this and next column fit? (e.g. 0..31 < 32)
  i8 i_Col_next = i_Col + 1;
  if (xaColEnc[i_Col_next].m_o_DataBreak < m_jHeadSize)
    return true;
 
  // this column plus overflow fit? (e.g. 0..29 + 2 < 32)
  if (xaColEnc[i_Col].m_o_DataBreak + C_OT_2B < m_jHeadSize)
    return true;

  // this column doesnt fit
  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
