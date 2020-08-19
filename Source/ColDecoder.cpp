/*============================================================================*/
/* ColDecoder.cpp -- Col Decoder Mark V                                       */
/*============================================================================*/

#include "CompactBuffers.h"

/*----------------------------------------------------------------------------*/
/* get encoded datum from row                                                 */
/*----------------------------------------------------------------------------*/
void ColDecoder::vDecode(Datum* pDatum,
                         HeadLine* aHeadLine, StreamDef* pStreamDef,
                         Column* pCol, i8& i_ColCrs_, i8& i_OfsCrs_)
{
  eDM eDatum = pCol->eDatum();
  
  switch (eDatum) {
      
    case DM_BCD:
      vDecodeBcd(pDatum,aHeadLine,pStreamDef,pCol,i_ColCrs_,i_OfsCrs_);
      break;
      
    case DM_BOL:
      vDecodeBol(pDatum,aHeadLine,pStreamDef,pCol);
      break;
      
    case DM_DTE:
      vDecodeDte(pDatum,aHeadLine,pStreamDef,pCol,i_ColCrs_,i_OfsCrs_);
      break;
      
    case DM_FLT:
      vDecodeFlt(pDatum,aHeadLine,pStreamDef,pCol,i_ColCrs_,i_OfsCrs_);
      break;
      
    case DM_INT:
      vDecodeInt(pDatum,aHeadLine,pStreamDef,pCol,i_ColCrs_,i_OfsCrs_);
      break;
      
    case DM_IVL:
      vDecodeIvl(pDatum,aHeadLine,pStreamDef,pCol,i_ColCrs_,i_OfsCrs_);
      break;
      
    case DM_TME:
      vDecodeTme(pDatum,aHeadLine,pStreamDef,pCol,i_ColCrs_,i_OfsCrs_);
      break;
      
    case DM_TSP:
      vDecodeTsp(pDatum,aHeadLine,pStreamDef,pCol,i_ColCrs_,i_OfsCrs_);
      break;
      
    case DM_VCH:
      vDecodeVch(pDatum,aHeadLine,pStreamDef,pCol,i_ColCrs_,i_OfsCrs_);
      break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode decimal from given head line                                        */
/*----------------------------------------------------------------------------*/
void ColDecoder::vDecodeBcd(Datum* pDatum,
                            HeadLine* aHeadLine, StreamDef* pStreamDef,
                            Column* pCol, i8& i_ColCrs_, i8& i_OfsCrs_)
{
  eMC eEncoding = aHeadLine->eGetEncMetaCode(pCol,i_ColCrs_,i_OfsCrs_,pStreamDef);
  
  switch (eEncoding) {
      
    case MC_NULL: (* pDatum) = C_DATUM_NULL; break;
      
    case MC_BCD_1B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_1B); break;
    case MC_BCD_2B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_2B); break;
    case MC_BCD_3B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_3B); break;
    case MC_BCD_4B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_4B); break;
    case MC_BCD_5B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_5B); break;
    case MC_BCD_6B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_6B); break;
    case MC_BCD_7B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_7B); break;
    case MC_BCD_8B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_8B); break;
      
    case MC_BCD_M_ONE: (* pDatum) = (i8)-1;  break;
    case MC_BCD_ZERO:  (* pDatum) = (i8)0;   break;
    case MC_BCD_ONE:   (* pDatum) = (i8)1;   break;
    case MC_BCD_TWO:   (* pDatum) = (i8)2;   break;
    case MC_BCD_THREE: (* pDatum) = (i8)3;   break;
    case MC_BCD_FOUR:  (* pDatum) = (i8)4;   break;
    case MC_BCD_FIVE:  (* pDatum) = (i8)5;   break;
    case MC_BCD_SIX:   (* pDatum) = (i8)6;   break;
    case MC_BCD_SEVEN: (* pDatum) = (i8)7;   break;
    case MC_BCD_EIGHT: (* pDatum) = (i8)8;   break;
    case MC_BCD_NINE:  (* pDatum) = (i8)9;   break;
    case MC_BCD_TEN:   (* pDatum) = (i8)10;  break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode boolean from given head line                                        */
/*----------------------------------------------------------------------------*/
void ColDecoder::vDecodeBol(Datum* pDatum,
                            HeadLine* aHeadLine, StreamDef* pStreamDef,
                            Column* pCol)
{
  i8 i_Col = pCol->iColumnNo();
  eMC eEncoding = aHeadLine->eMetaCode(i_Col);
  
  if (MC_BOL_FALSE == eEncoding)
    (* pDatum) = (i8)0;
  else if (MC_BOL_TRUE == eEncoding)
    (* pDatum) = 1;
  else
    (* pDatum) = C_DATUM_NULL;
  
  /*
   switch (eEncoding) {
   
   case MC_NULL:  pDatum->m_iInt = C_DATUM_NULL; break;
   case MC_BOL_FALSE: pDatum->m_iInt = (i8)0;        break;
   case MC_BOL_TRUE:  pDatum->m_iInt = (i8)1;        break;
   
   default:
   NEVER_REACH;
   }
   */
}

/*----------------------------------------------------------------------------*/
/* decode date from given head line                                           */
/*----------------------------------------------------------------------------*/
void ColDecoder::vDecodeDte(Datum* pDatum,
                            HeadLine* aHeadLine, StreamDef* pStreamDef,
                            Column* pCol, i8& i_ColCrs_, i8& i_OfsCrs_)
{
  i8 iEra = C_ERA_0;
  
  //!! needs more work
  eMC eEncoding = aHeadLine->eGetEncMetaCode(pCol,i_ColCrs_,i_OfsCrs_,pStreamDef);
  
  switch (eEncoding) {
      
    case MC_NULL:
      (* pDatum) = C_DATUM_NULL;
      return;
    case MC_DTE_E0_2B:
      pDatum->vDte(aHeadLine->wGetLogical(i_OfsCrs_,C_2B));
      iEra = C_ERA_0;
      break;
    case MC_DTE_E1_2B:
      pDatum->vDte(aHeadLine->wGetLogical(i_OfsCrs_,C_2B));
      iEra = C_ERA_1;
      break;
    case MC_DTE_E2_2B:
      pDatum->vDte(aHeadLine->wGetLogical(i_OfsCrs_,C_2B));
      iEra = C_ERA_2;
      break;
    default:
      NEVER_REACH;
  }
  
  i8 iDte;
  
  vDteFromEdt(&iDte,pDatum->iEdt(),iEra);
  
  pDatum->vDte(iDte);
}

/*----------------------------------------------------------------------------*/
/* decode float from given head line                                          */
/*----------------------------------------------------------------------------*/
void ColDecoder::vDecodeFlt(Datum* pDatum,
                            HeadLine* aHeadLine, StreamDef* pStreamDef,
                            Column* pCol, i8& i_ColCrs_, i8& i_OfsCrs_)
{
  eMC eEncoding = aHeadLine->eGetEncMetaCode(pCol,i_ColCrs_,i_OfsCrs_,pStreamDef);
  
  switch (eEncoding) {
      
    case MC_NULL: (* pDatum) = C_DATUM_NULL; break;
      
    case MC_FLT_1B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_1B); break;
    case MC_FLT_2B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_2B); break;
    case MC_FLT_3B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_3B); break;
    case MC_FLT_4B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_4B); break;
    case MC_FLT_5B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_5B); break;
    case MC_FLT_6B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_6B); break;
    case MC_FLT_7B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_7B); break;
    case MC_FLT_8B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_8B); break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode integer from given head line                                        */
/*----------------------------------------------------------------------------*/
void ColDecoder::vDecodeInt(Datum* pDatum,
                            HeadLine* aHeadLine, StreamDef* pStreamDef,
                            Column* pCol, i8& i_ColCrs_, i8& i_OfsCrs_)
{
  eMC eEncoding = aHeadLine->eGetEncMetaCode(pCol,i_ColCrs_,i_OfsCrs_,pStreamDef);
  //vTrace("Decoding %lld\n"_Stz, eEncoding);
  
  switch (eEncoding) {
      
    case MC_NULL: (* pDatum) = C_DATUM_NULL; break;
      
    case MC_INT_1B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_1B); break;
    case MC_INT_2B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_2B); break;
    case MC_INT_3B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_3B); break;
    case MC_INT_4B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_4B); break;
    case MC_INT_5B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_5B); break;
    case MC_INT_6B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_6B); break;
    case MC_INT_7B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_7B); break;
    case MC_INT_8B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_8B); break;
      
    case MC_INT_M_ONE:  (* pDatum) = (i8)-1; break;
    case MC_INT_ZERO:   (* pDatum) = (i8)0;  break;
    case MC_INT_ONE:    (* pDatum) = (i8)1;  break;
    case MC_INT_TWO:    (* pDatum) = (i8)2;  break;
    case MC_INT_THREE:  (* pDatum) = (i8)3;  break;
    case MC_INT_FOUR:   (* pDatum) = (i8)4;  break;
    case MC_INT_FIVE:   (* pDatum) = (i8)5;  break;
    case MC_INT_SIX:    (* pDatum) = (i8)6;  break;
    case MC_INT_SEVEN:  (* pDatum) = (i8)7;  break;
    case MC_INT_EIGHT:  (* pDatum) = (i8)8;  break;
    case MC_INT_NINE:   (* pDatum) = (i8)9;  break;
    case MC_INT_TEN:    (* pDatum) = (i8)10; break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode interval from given head line                                       */
/*----------------------------------------------------------------------------*/
void ColDecoder::vDecodeIvl(Datum* pDatum,
                            HeadLine* aHeadLine, StreamDef* pStreamDef,
                            Column* pCol, i8& i_ColCrs_, i8& i_OfsCrs_)
{
  eMC eEncoding = aHeadLine->eGetEncMetaCode(pCol,i_ColCrs_,i_OfsCrs_,pStreamDef);
  
  switch (eEncoding) {
      
    case MC_NULL: (* pDatum) = C_DATUM_NULL; break;
      
    case MC_IVL_1B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_1B); break;
    case MC_IVL_2B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_2B); break;
    case MC_IVL_3B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_3B); break;
    case MC_IVL_4B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_4B); break;
    case MC_IVL_5B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_5B); break;
    case MC_IVL_6B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_6B); break;
    case MC_IVL_7B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_7B); break;
    case MC_IVL_8B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_8B); break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode time from given head line                                           */
/*----------------------------------------------------------------------------*/
void ColDecoder::vDecodeTme(Datum* pDatum,
                            HeadLine* aHeadLine, StreamDef* pStreamDef,
                            Column* pCol, i8& i_ColCrs_, i8& i_OfsCrs_)
{
  eMC eEncoding = aHeadLine->eGetEncMetaCode(pCol,i_ColCrs_,i_OfsCrs_,pStreamDef);
  
  switch (eEncoding) {
      
    case MC_NULL: (* pDatum) = C_DATUM_NULL; break;
      
    case MC_TME_1B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_1B); break;
    case MC_TME_2B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_2B); break;
    case MC_TME_3B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_3B); break;
    case MC_TME_4B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_4B); break;
    case MC_TME_5B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_5B); break;
    case MC_TME_6B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_6B); break;
    case MC_TME_7B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_7B); break;
    case MC_TME_8B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs_,C_8B); break;
      
    default:
      NEVER_REACH;
  }
  
  //vRestoreTme(pDatum);
  (* pDatum) = pDatum->iInt() << 40;
}

/*----------------------------------------------------------------------------*/
/* decode timestamp from given head line                                      */
/*----------------------------------------------------------------------------*/
void ColDecoder::vDecodeTsp(Datum* pDatum,
                            HeadLine* aHeadLine, StreamDef* pStreamDef,
                            Column* pCol, i8& i_ColCrs_, i8& i_OfsCrs)
{
  eMC eEncoding = aHeadLine->eGetEncMetaCode(pCol,i_ColCrs_,i_OfsCrs,pStreamDef);
  
  switch (eEncoding) {
      
    case MC_NULL: (* pDatum) = C_DATUM_NULL; break;
      
    case MC_TSP_8B: (* pDatum) = aHeadLine->wGetLogical(i_OfsCrs,C_8B); break;
      
    default:
      NEVER_REACH;
  }
  
//  if (eEncoding == MC_5B)
//    //vRestoreTsp(pDatum);
//    pDatum->u.m_xAnyVal.m_iInt <<= 24;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given head line                                        */
/*----------------------------------------------------------------------------*/
void ColDecoder::vDecodeVch(Datum* pDatum,
                            HeadLine* aHeadLine, StreamDef* pStreamDef,
                            Column* pCol, i8& i_ColCrs_, i8& i_OfsCrs_)
{
  //! needs more work
  eMC eEncoding = aHeadLine->eGetEncMetaCode(pCol,i_ColCrs_,i_OfsCrs_,pStreamDef);
  
  if (MC_NULL == eEncoding) {
    (* pDatum) = C_DATUM_NULL;
    return;
  }
  
  else if (MC_STR_EMPTY == eEncoding) {
    
    pDatum->vStz(""_Stz);
    //pDatum->vStzDump();
  }
  
  else if (MC_STR_FIT_1L == eEncoding)
    vDecodeVchSmall(pDatum,aHeadLine,pStreamDef,pCol,i_OfsCrs_);
  
  else if (MC_STR_FIT_2L == eEncoding)
    vDecodeVchLong(pDatum,aHeadLine,pStreamDef,pCol,i_OfsCrs_);
  
  else if (MC_STR_TAIL_2T == eEncoding)
    vDecodeVchStringTail(pDatum,aHeadLine,pStreamDef,pCol,i_OfsCrs_);
    
  else
    NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given head line                                        */
/*----------------------------------------------------------------------------*/
void ColDecoder::vDecodeVchLong(Datum* pDatum,
                                HeadLine* aHeadLine, StreamDef* pStreamDef,
                                Column* pCol, i8 i_Offset)
{
  //vDebugPoint();
  
  i8 nLength = aHeadLine->wGetLogical(i_Offset,C_2B);
  
  pDatum->vStz((char*)aHeadLine->ayAdrLogical(i_Offset + C_2B),nLength);
  //pDatum->vStzDump();
}

/*----------------------------------------------------------------------------*/
/* decode MC_SHOCO_FIT_1C tail line shoco                                     */
/*----------------------------------------------------------------------------*/
void ColDecoder::vDecodeVchShocoFit(Datum* pDatum,
                                    HeadLine* aHeadLine, StreamDef* pStreamDef,
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
    Compress::iFromShoco(pDatum->azChar(),
                         pDatum->nStzSpace(),
                         aHeadLine->ayAdrLogical(i_Offset + C_C_1B),
                         nCompressed);

  pDatum->vStzSize(nDecompressed + 1);
  
  //pDatum->vStzDump();
}

/*----------------------------------------------------------------------------*/
/* decode MC_SHOCO_SPLIT_2T1C tail line shoco                                 */
/*----------------------------------------------------------------------------*/
void ColDecoder::vDecodeVchShocoSplit(Datum* pDatum,
                                      HeadLine* aHeadLine, StreamDef* pStreamDef,
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
  
  i8 nChars =
      Compress::iFromShoco(pDatum->azChar(),
                           pDatum->nStzSpace(),
                           (byte*)caShoco+2,
                           nCompressed);
  ASSERT(nChars);
  pDatum->vStzSize(nChars + 1);
  
  //pDatum->vStzDump();
}

/*----------------------------------------------------------------------------*/
/* decode MC_SHOCO_TAIL_2T tail line shoco                                    */
/*----------------------------------------------------------------------------*/
void ColDecoder::vDecodeVchShocoTail(Datum* pDatum,
                                     HeadLine* aHeadLine, StreamDef* pStreamDef,
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
  
  i8 nCompressed = aShocoTail->w8_1B(+ C_U_1B);
  //RASSERT(nCompressed <= 4);
  
  i8 nDecompressed =
    Compress::iFromShoco(pDatum->azChar(),         // decompressed destination
                         pDatum->nStzSpace(),      // max destination
                         ((byte*)aShocoTail)       // from compressed source
                           + C_U_1B
                           + C_C_1B,
                         nCompressed);             // bytes compressed

  pDatum->vStzSize(nDecompressed + 1);
  
  //pDatum->vStzDump();
}

/*----------------------------------------------------------------------------*/
/* decode MC_STRING_TAIL_2T tail line string                                 */
/*----------------------------------------------------------------------------*/
void ColDecoder::vDecodeVchStringTail(Datum* pDatum,
                                      HeadLine* aHeadLine, StreamDef* pStreamDef,
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
  

  pDatum->vStz((char*)ayString,nLength);
 
  //pDatum->vStzDump();
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given head line                                        */
/*----------------------------------------------------------------------------*/
void ColDecoder::vDecodeVchSmall(Datum* pDatum,
                                 HeadLine* aHeadLine, StreamDef* pStreamDef,
                                 Column* pCol, i8 i_Offset)
{
  //vDebugPoint();
  
  i8 nLength = aHeadLine->wGetLogical(i_Offset,C_1B);
  
  pDatum->vStz((char*)aHeadLine->ayAdrLogical(i_Offset + 1),nLength);

  //pDatum->vStzDump();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
