/*============================================================================*/
/* RowDecoder.cpp -- Row Inflator Mark IV                                    */
/*============================================================================*/

#include "Rows.h"

/*----------------------------------------------------------------------------*/
/* get encoded datum from row                                                 */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecode(DtmVal* pDtmVal, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_)
{
  eDM eDatum = pCol->eDatumType();
  
  switch (eDatum) {
      
    case DM_BCD:
      vDecodeBcd(pDtmVal,aHeadLine,pColVec,pCol,i_ColCrs_,i_OfsCrs_);
      break;
      
    case DM_BOL:
      vDecodeBol(pDtmVal,aHeadLine,pCol);
      break;
      
    case DM_DTE:
      vDecodeDte(pDtmVal,aHeadLine,pColVec,pCol,i_ColCrs_,i_OfsCrs_);
      break;
      
    case DM_FLT:
      vDecodeFlt(pDtmVal,aHeadLine,pColVec,pCol,i_ColCrs_,i_OfsCrs_);
      break;
      
    case DM_INT:
      vDecodeInt(pDtmVal,aHeadLine,pColVec,pCol,i_ColCrs_,i_OfsCrs_);
      break;
      
    case DM_IVL:
      vDecodeIvl(pDtmVal,aHeadLine,pColVec,pCol,i_ColCrs_,i_OfsCrs_);
      break;
      
    case DM_TME:
      vDecodeTme(pDtmVal,aHeadLine,pColVec,pCol,i_ColCrs_,i_OfsCrs_);
      break;
      
    case DM_TSP:
      vDecodeTsp(pDtmVal,aHeadLine,pColVec,pCol,i_ColCrs_,i_OfsCrs_);
      break;
      
    case DM_VCH:
      vDecodeVch(pDtmVal,aHeadLine,pColVec,pCol,i_ColCrs_,i_OfsCrs_);
      break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode decimal from given head line                                        */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeBcd(DtmVal* pDtmVal, HeadLine* aHeadLine,
                              ColumnVec* pColVec, Column* pCol,
                              i8& i_ColCrs_, i8& i_OfsCrs_)
{
  i8 i_BaseOffset = RowCoder::iBaseOffset(pColVec);

  pDtmVal->m_eDatum = DM_BCD;

  eMC eEncoding = aHeadLine->eGetEncoding(pCol,i_BaseOffset,
                                          i_ColCrs_,i_OfsCrs_);
  switch (eEncoding) {
      
    case MC_NULL: pDtmVal->m_wDtm.m_iInt = C_DATUM_NULL; break;
      
    case MC_BCD_1B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_1B); break;
    case MC_BCD_2B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_2B); break;
    case MC_BCD_3B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_3B); break;
    case MC_BCD_4B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_4B); break;
    case MC_BCD_5B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_5B); break;
    case MC_BCD_6B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_6B); break;
    case MC_BCD_7B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_7B); break;
    case MC_BCD_8B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_8B); break;
      
    case MC_BCD_M_ONE: pDtmVal->m_wDtm.m_iInt = (i8)-1;  break;
    case MC_BCD_ZERO:  pDtmVal->m_wDtm.m_iInt = (i8)0;   break;
    case MC_BCD_ONE:   pDtmVal->m_wDtm.m_iInt = (i8)1;   break;
    case MC_BCD_TWO:   pDtmVal->m_wDtm.m_iInt = (i8)2;   break;
    case MC_BCD_THREE: pDtmVal->m_wDtm.m_iInt = (i8)3;   break;
    case MC_BCD_FOUR:  pDtmVal->m_wDtm.m_iInt = (i8)4;   break;
    case MC_BCD_FIVE:  pDtmVal->m_wDtm.m_iInt = (i8)5;   break;
    case MC_BCD_SIX:   pDtmVal->m_wDtm.m_iInt = (i8)6;   break;
    case MC_BCD_SEVEN: pDtmVal->m_wDtm.m_iInt = (i8)7;   break;
    case MC_BCD_EIGHT: pDtmVal->m_wDtm.m_iInt = (i8)8;   break;
    case MC_BCD_NINE:  pDtmVal->m_wDtm.m_iInt = (i8)9;   break;
    case MC_BCD_TEN:   pDtmVal->m_wDtm.m_iInt = (i8)10;  break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode boolean from given head line                                        */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeBol(DtmVal* pDtmVal, HeadLine* aHeadLine,Column* pCol)
{
  pDtmVal->m_eDatum = DM_BOL;
  
  //eMC eEncoding = aHeadLine->eGetEncoding(pCol,pColVec,i_ColCrs_,i_Offset);
  
  i8 i_Column = pCol->iColumnNo();
  eMC eEncoding = aHeadLine->eMetaCode(i_Column);
  
  if (MC_BOL_FALSE == eEncoding)
    pDtmVal->m_wDtm.m_iInt = 0;
  else if (MC_BOL_TRUE == eEncoding)
    pDtmVal->m_wDtm.m_iInt = 1;
  else
    pDtmVal->m_wDtm.m_iInt = C_DATUM_NULL;
  
  /*
   switch (eEncoding) {
   
   case MC_NULL:  pDtmVal->m_iInt = C_DATUM_NULL; break;
   case MC_BOL_FALSE: pDtmVal->m_iInt = (i8)0;        break;
   case MC_BOL_TRUE:  pDtmVal->m_iInt = (i8)1;        break;
   
   default:
   NEVER_REACH;
   }
   */
}

/*----------------------------------------------------------------------------*/
/* decode date from given head line                                           */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeDte(DtmVal* pDtmVal, HeadLine* aHeadLine,
                              ColumnVec* pColVec, Column* pCol,
                              i8& i_ColCrs_, i8& i_OfsCrs_)
{
  i8 i_BaseOffset = RowCoder::iBaseOffset(pColVec);
  
  pDtmVal->m_eDatum = DM_DTE;
  i8 iEra = C_ERA_0;
  
  //!! needs more work
  eMC eThisEncoding = aHeadLine->eGetEncoding(pCol,i_BaseOffset,
                                              i_ColCrs_,i_OfsCrs_);
  
  
  switch (eThisEncoding) {
      
    case MC_NULL:
      pDtmVal->m_wDtm.m_iInt = C_DATUM_NULL;
      return;
    case MC_DTE_E0_2B:
      pDtmVal->m_wDtm.m_iDte = aHeadLine->wGetLogical(i_OfsCrs_,C_2B);
      iEra = C_ERA_0;
      break;
    case MC_DTE_E1_2B:
      pDtmVal->m_wDtm.m_iDte = aHeadLine->wGetLogical(i_OfsCrs_,C_2B);
      iEra = C_ERA_1;
      break;
    case MC_DTE_E2_2B:
      pDtmVal->m_wDtm.m_iDte = aHeadLine->wGetLogical(i_OfsCrs_,C_2B);
      iEra = C_ERA_2;
      break;
    default:
      NEVER_REACH;
  }
  
  vDteFromEdt(&pDtmVal->m_wDtm,iEra);
}

/*----------------------------------------------------------------------------*/
/* decode float from given head line                                          */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeFlt(DtmVal* pDtmVal, HeadLine* aHeadLine,
                              ColumnVec* pColVec, Column* pCol,
                              i8& i_ColCrs_, i8& i_OfsCrs_)
{
  i8 i_BaseOffset = RowCoder::iBaseOffset(pColVec);
  
  pDtmVal->m_eDatum = DM_FLT;
  
  eMC eEncoding = aHeadLine->eGetEncoding(pCol,i_BaseOffset,
                                          i_ColCrs_,i_OfsCrs_);
  switch (eEncoding) {
      
    case MC_NULL: pDtmVal->m_wDtm.m_iInt = C_DATUM_NULL; break;
      
    case MC_FLT_1B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_1B); break;
    case MC_FLT_2B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_2B); break;
    case MC_FLT_3B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_3B); break;
    case MC_FLT_4B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_4B); break;
    case MC_FLT_5B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_5B); break;
    case MC_FLT_6B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_6B); break;
    case MC_FLT_7B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_7B); break;
    case MC_FLT_8B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_8B); break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode integer from given head line                                        */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeInt(DtmVal* pDtmVal, HeadLine* aHeadLine,
                            ColumnVec* pColVec, Column* pCol,
                            i8& i_ColCrs_, i8& i_OfsCrs_)
{
  i8 i_BaseOffset = RowCoder::iBaseOffset(pColVec);
  
  pDtmVal->m_eDatum = DM_INT;
  
  eMC eEncoding = aHeadLine->eGetEncoding(pCol,i_BaseOffset,
                                          i_ColCrs_,i_OfsCrs_);
  //vTrace("Decoding %lld\n"_Stz, eEncoding);
  
  switch (eEncoding) {
      
    case MC_NULL: pDtmVal->m_wDtm.m_iInt = C_DATUM_NULL; break;
      
    case MC_INT_1B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_1B); break;
    case MC_INT_2B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_2B); break;
    case MC_INT_3B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_3B); break;
    case MC_INT_4B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_4B); break;
    case MC_INT_5B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_5B); break;
    case MC_INT_6B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_6B); break;
    case MC_INT_7B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_7B); break;
    case MC_INT_8B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_8B); break;
      
    case MC_INT_M_ONE:  pDtmVal->m_wDtm.m_iInt = (i8)-1; break;
    case MC_INT_ZERO:   pDtmVal->m_wDtm.m_iInt = (i8)0;  break;
    case MC_INT_ONE:    pDtmVal->m_wDtm.m_iInt = (i8)1;  break;
    case MC_INT_TWO:    pDtmVal->m_wDtm.m_iInt = (i8)2;  break;
    case MC_INT_THREE:  pDtmVal->m_wDtm.m_iInt = (i8)3;  break;
    case MC_INT_FOUR:   pDtmVal->m_wDtm.m_iInt = (i8)4;  break;
    case MC_INT_FIVE:   pDtmVal->m_wDtm.m_iInt = (i8)5;  break;
    case MC_INT_SIX:    pDtmVal->m_wDtm.m_iInt = (i8)6;  break;
    case MC_INT_SEVEN:  pDtmVal->m_wDtm.m_iInt = (i8)7;  break;
    case MC_INT_EIGHT:  pDtmVal->m_wDtm.m_iInt = (i8)8;  break;
    case MC_INT_NINE:   pDtmVal->m_wDtm.m_iInt = (i8)9;  break;
    case MC_INT_TEN:    pDtmVal->m_wDtm.m_iInt = (i8)10; break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode interval from given head line                                       */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeIvl(DtmVal* pDtmVal, HeadLine* aHeadLine,
                              ColumnVec* pColVec, Column* pCol,
                              i8& i_ColCrs_, i8& i_OfsCrs_)
{
  i8 i_BaseOffset = RowCoder::iBaseOffset(pColVec);
  
  pDtmVal->m_eDatum = DM_IVL;
  
  eMC eEncoding = aHeadLine->eGetEncoding(pCol,i_BaseOffset,
                                          i_ColCrs_,i_OfsCrs_);
  switch (eEncoding) {
      
    case MC_NULL: pDtmVal->m_wDtm.m_iInt = C_DATUM_NULL; break;
      
    case MC_IVL_1B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_1B); break;
    case MC_IVL_2B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_2B); break;
    case MC_IVL_3B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_3B); break;
    case MC_IVL_4B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_4B); break;
    case MC_IVL_5B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_5B); break;
    case MC_IVL_6B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_6B); break;
    case MC_IVL_7B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_7B); break;
    case MC_IVL_8B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_8B); break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode time from given head line                                           */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeTme(DtmVal* pDtmVal, HeadLine* aHeadLine,
                              ColumnVec* pColVec, Column* pCol,
                              i8& i_ColCrs_, i8& i_OfsCrs_)
{
  i8 i_BaseOffset = RowCoder::iBaseOffset(pColVec);
  
  pDtmVal->m_eDatum = DM_TME;
  
  eMC eEncoding = aHeadLine->eGetEncoding(pCol,i_BaseOffset,
                                          i_ColCrs_,i_OfsCrs_);
  switch (eEncoding) {
      
    case MC_NULL: pDtmVal->m_wDtm.m_iInt = C_DATUM_NULL; break;
      
    case MC_TME_1B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_1B); break;
    case MC_TME_2B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_2B); break;
    case MC_TME_3B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_3B); break;
    case MC_TME_4B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_4B); break;
    case MC_TME_5B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_5B); break;
    case MC_TME_6B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_6B); break;
    case MC_TME_7B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_7B); break;
    case MC_TME_8B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs_,C_8B); break;
      
    default:
      NEVER_REACH;
  }
  
  //vRestoreTme(pDtmVal);
  pDtmVal->m_wDtm.m_iInt <<= 40;
}

/*----------------------------------------------------------------------------*/
/* decode timestamp from given head line                                      */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeTsp(DtmVal* pDtmVal, HeadLine* aHeadLine,
                              ColumnVec* pColVec, Column* pCol,
                              i8& i_ColCrs_, i8& i_OfsCrs)
{
  i8 i_BaseOffset = RowCoder::iBaseOffset(pColVec);
  
  pDtmVal->m_eDatum = DM_TSP;
  
  eMC eEncoding = aHeadLine->eGetEncoding(pCol,i_BaseOffset,
                                          i_ColCrs_,i_OfsCrs);
  switch (eEncoding) {
      
    case MC_NULL: pDtmVal->m_wDtm.m_iInt = C_DATUM_NULL; break;
      
    case MC_TSP_8B: pDtmVal->m_wDtm.m_iInt = aHeadLine->wGetLogical(i_OfsCrs,C_8B); break;
      
    default:
      NEVER_REACH;
  }
  
//  if (eEncoding == MC_5B)
//    //vRestoreTsp(pDtmVal);
//    pDtmVal->m_wDtm.m_iInt <<= 24;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given head line                                        */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeVch(DtmVal* pDtmVal, HeadLine* aHeadLine,
                              ColumnVec* pColVec, Column* pCol,
                              i8& i_ColCrs_, i8& i_OfsCrs_)
{
  i8 i_BaseOffset = RowCoder::iBaseOffset(pColVec);

  pDtmVal->m_eDatum = DM_VCH;
  
  //! needs more work
  eMC eEncoding = aHeadLine->eGetEncoding(pCol,i_BaseOffset,
                                          i_ColCrs_,i_OfsCrs_);
  
  if (MC_NULL == eEncoding) {
    pDtmVal->m_wDtm.m_iInt = C_DATUM_NULL;
    return;
  }
  
  else if (MC_STR_EMPTY == eEncoding) {
    pDtmVal->m_nChars = 0;
    *pDtmVal->m_sVch.aChar() = '\0';
    pDtmVal->m_wDtm.m_iInt = 0;
  }
  
  else if (MC_STR_FIT_1L == eEncoding)
    vDecodeVchSmall(pDtmVal,aHeadLine,pCol,i_OfsCrs_);
  
  else if (MC_STR_FIT_2L == eEncoding)
    vDecodeVchLong(pDtmVal,aHeadLine,pCol,i_OfsCrs_);
  
  else if (MC_STR_TAIL_2T == eEncoding)
    vDecodeVchStringTail(pDtmVal,aHeadLine,pCol,i_OfsCrs_);
    
  else
    NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given head line                                        */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeVchLong(DtmVal* pDtmVal, HeadLine* aHeadLine,
                                 Column* pCol, i8 i_Offset)
{
  vDebugPoint();
  
  i8 nLength = aHeadLine->wGetLogical(i_Offset,C_2B);
  
  MemLib::vCopy((byte*)pDtmVal->m_sVch.aChar(),
                (byte*)aHeadLine->ayAdrLogical(i_Offset + C_2B),
                nLength,
                CN_ROWCOMPACTOR);
  
  * (pDtmVal->m_sVch.aChar() + nLength) = '\0';
  
  pDtmVal->m_nChars = nLength;
}

/*----------------------------------------------------------------------------*/
/* decode MC_SHOCO_DUP_3D shoco duplicate                                     */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeVchShocoDup(DtmVal* pDtmVal, HeadLine* aHeadLine,
                                      Column* pCol, i8 i_Offset)
{
  /*
   -- Head-Line (with shoco) --
   +================================+
   | <W>{M}     ,MC_SHOCO_FIT_1C,   |
   +================================+
   |                                |
   +--------------------------------+
   |    <C><shoco...>               |
   +--------------------------------+
   
   -- Head-Line (with dsr) --
   +================================+
   | <W>{M}     ,MC_SHOCO_DUP_3D,   |
   +================================+
   |                                |
   +--------------------------------+
   |    [DSR]                       |
   +--------------------------------+
   
   where: MC_SHOCO_FIT_1C are the meta-bits for the column which has the shoco
   MC_SHOCO_DUP_3D are the meta-bits for the column which ref's the shoco
   <W> is one byte being the word length of head line
   {M} is the set of metacodes
   <C> is one byte being the number of shoco compressed bytes
   <shoco...> being the shoco compressed bytes
   [DSR] is the 3B relative backwards address of <C><shoco>
   
   */
  
  i8 i_Dsr = aHeadLine->wGetLogical(i_Offset,C_DSR_3B);
  
  byte* ayC_Shoco = aAdd<byte*>(aHeadLine,( + i_Dsr )); //! WARNING
  
  i8 nCompressed = *(u1*)ayC_Shoco;
  
  i8 nDecompressed =
  Compress::iFromShoco((char*)pDtmVal->m_sVch.aChar(),
                       pDtmVal->m_sVch.nSpace(),
                       ayC_Shoco + C_C_1B,
                       nCompressed);
  ASSERT(nDecompressed > 0);
  pDtmVal->m_nChars = nDecompressed;
}

/*----------------------------------------------------------------------------*/
/* decode MC_SHOCO_FIT_1C tail line shoco                                     */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeVchShocoFit(DtmVal* pDtmVal, HeadLine* aHeadLine,
                                      Column* pCol, i8 i_Offset)
{
  /*
   -- Head-Line --
   +================================+
   | <W>{M}        ,MC_SHOCO_FIT_1C |
   +================================+
   |                                |
   +--------------------------------+
   |    <C><shoco...> ///////////// |
   +--------------------------------+
   
   where: MC_SHOCO_FIT_1C are the meta-bits for the column
   <W> is one byte being the word length of head line
   {M} is the set of metacodes
   <C> is one byte being the number of shoco compressed bytes
   <shoco...> being the shoco compressed bytes
   */
  
  i8 nCompressed = aHeadLine->wGetLogical(i_Offset,C_C_1B);
  //RASSERT(nCompressed <= 56);
  
  i8 nDecompressed =
  Compress::iFromShoco((char*)pDtmVal->m_sVch.aChar(),
                       pDtmVal->m_sVch.nSpace(),
                       aHeadLine->ayAdrLogical(i_Offset + C_C_1B),
                       //((byte*)aHeadLine)+i_Offset+1,
                       nCompressed);
  ASSERT(nDecompressed > 0);
  pDtmVal->m_nChars = nDecompressed;
}

/*----------------------------------------------------------------------------*/
/* decode MC_SHOCO_SPLIT_2T1C tail line shoco                                 */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeVchShocoSplit(DtmVal* pDtmVal, HeadLine* aHeadLine,
                                        Column* pCol, i8 i_Offset)
{
  /*
   -- Head-Line --                       -- Shoco-Tail --
   +================================+    +=====================================+
   | <W>{M}    ,MC_SHOCO_SPLIT_2T1C |    | <U><partB.........................> |
   +================================+    +=====================================+
   |                                |
   +--------------------------------+
   |            [IT]<C><partA.....> |
   +--------------------------------+
   
   where: MC_SHOCO_TAIL_2T is the meta-bits for the column
   [IT] is 2B relative index of the Into-Tail line containing shoco
   <W> is one byte being the word length of head line
   {M} is the set of metacodes
   <U> is one byte being the unit length of a tail line
   <C> is one byte being the number of shoco compressed bytes
   <partA> is the left-half of the split shoco
   <partB> is the right-half of the split shoco
   */
  
  byte caShoco[C_4KB];
  
  i8 i_Tail_rel = aHeadLine->wGetLogical(i_Offset,C_2B);
  TailLine* aShocoTail = aHeadLine->aGetTailRel(i_Tail_rel);
  
  i8 nCompressed = aHeadLine->wGetLogical((i_Offset+2),C_1B);
  
  i8 nBytesPartA = aHeadLine->nBytes() - i_Offset - 3;                           //! <W>
  i8 nBytesPartB = nCompressed - nBytesPartA;
  
  MemLib::vCopy((byte*)caShoco+2,
                (byte*)aHeadLine + i_Offset + 3,
                nBytesPartA,
                CN_ROWCOMPACTOR);
  MemLib::vCopy((byte*)caShoco+2+nBytesPartA,
                (byte*)aShocoTail+1,
                nBytesPartB,
                CN_ROWCOMPACTOR);
  
  //MemLib::vWordDump(caShoco+2,3 * sizeof(SpaceUnit));
  
  i8 nDecompressed =
  Compress::iFromShoco((char*)pDtmVal->m_sVch.aChar(),
                       255,
                       (byte*)caShoco+2,
                       nCompressed);
  ASSERT(nDecompressed);
  pDtmVal->m_nChars = nDecompressed;
}

/*----------------------------------------------------------------------------*/
/* decode MC_SHOCO_TAIL_2T tail line shoco                                    */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeVchShocoTail(DtmVal* pDtmVal, HeadLine* aHeadLine,
                                       Column* pCol, i8 i_Offset)
{
  i8 i_Tail_rel = aHeadLine->wGetLogical(i_Offset,C_2B);
  TailLine* aShocoTail = aHeadLine->aGetTailRel(i_Tail_rel);
  
  //MemLib::vWordDump(aShocoTail,aShocoTail->nBytes());
  
  /*
   -- Shoco-Tail --
   +=====================================+
   | <U><C><shoco...> ////////////////// |
   +=====================================+
   
   where: <U> is one byte being the unit length of the tail line
   <C> is one byte being the number of shoco compressed bytes
   <shoco...> being the shoco compressed bytes
   */
  
  i8 nCompressed = aShocoTail->w8_1B(C_COMPRESSED_OFFSET);
  //RASSERT(nCompressed <= 4);
  
  i8 nDecompressed =
  Compress::iFromShoco(pDtmVal->m_sVch.aChar(),   // decompressed destination
                       255,                      // max destination
                       ((byte*)aShocoTail)       // from compressed source
                       + C_COMPRESSED_OFFSET
                       + C_C_1B,
                       nCompressed);             // bytes compressed
  ASSERT(nDecompressed);
  pDtmVal->m_nChars = nDecompressed;
}

/*----------------------------------------------------------------------------*/
/* decode MC_STRING_TAIL_2T tail line string                                 */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeVchStringTail(DtmVal* pDtmVal, HeadLine* aHeadLine,
                                        Column* pCol, i8 i_Offset)
{
  i8 i_Tail_rel = aHeadLine->wGetLogical(i_Offset,C_2B);
  TailLine* aTail_str = aHeadLine->aGetTailRel(i_Tail_rel);
  
  //MemLib::vWordDump(aShocoTail,aShocoTail->nBytes());
  
  /*
   -- String-Tail --
   +=====================================+
   | <U><L><string...> ///////////////// |
   +=====================================+
   
   where: <U> is one byte being the unit length of the tail line
   <L> is one byte being the length of the string
   <string...> being the string
   */
  
  i8 nLength = aTail_str->w8_1B(+ C_U_1B);
  byte* ayString = (byte*)aTail_str + C_U_1B + C_L_1B;
  
  MemLib::vCopy((byte*)pDtmVal->m_sVch.aChar(),
                (byte*)ayString,
                nLength,
                CN_ROWCOMPACTOR);
  
  * (pDtmVal->m_sVch.aChar() + nLength) = '\0';
  
  pDtmVal->m_nChars = nLength;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given head line                                        */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeVchSmall(DtmVal* pDtmVal, HeadLine* aHeadLine,
                                   Column* pCol, i8 i_Offset)
{
  vDebugPoint();
  
  i8 nLength = aHeadLine->wGetLogical(i_Offset,C_1B);
  
  MemLib::vCopy((byte*)pDtmVal->m_sVch.aChar(),
                (byte*)aHeadLine->ayAdrLogical(i_Offset + 1),
                nLength,
                CN_ROWCOMPACTOR);
  
  * (pDtmVal->m_sVch.aChar() + nLength) = '\0';
  
  pDtmVal->m_nChars = nLength;
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
