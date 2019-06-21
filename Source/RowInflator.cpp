/*============================================================================*/
/* RowInflator.cpp -- Row Inflator Mark IV                                    */
/*============================================================================*/

#include "Data.h"

/*----------------------------------------------------------------------------*/
/* inflate headline into row                                                  */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflate(Row* pRow, HeadLine* aHeadLine)
{
  eMC  eMetaCode;
  Word wWord;
  
  i8   i_Tail_rel;
  i8   nCompressed;
  i8   nLength;
  i8   nBytesPartA;
  i8   nBytesPartB;
  char caString[C_4KB];
  
  TailLine* aTail;
  
#ifdef WITH_TRACE_DECODING
  static i8 nTotalRows = 0;
  vTrace("vInflate() of row %lld\n"_Stz,nTotalRows++);
  vDumpHead("aHeadLine"_Stz,aHeadLine,pRow->m_pColVec);
#endif
  
  char* acString = (char*)pRow->m_xWsp.ayArea();
  
  i8 nCols = pRow->m_nColumns;
  
  eMC*  aeMetaCode = aAdd<eMC*>(aHeadLine,_1B_W);
  byte* ayDataByte = (byte*)aHeadLine + pRow->m_iBaseOffset;
  i8*   aiColVal   = & pRow->m_waColVal[0].m_iInt;
  
  bool  bGetAll    = pRow->m_bGetAll;
  i8    iLastCol   = pRow->m_iLastCol;
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    if (! bGetAll && i_Col >  iLastCol) {
      //exit early
      break;
    }
    
#ifdef WITH_TRACE_DECODING
    Column* pCol = pRow->m_pColVec->pGet(i_Col);
#endif
    
    eMetaCode = *aeMetaCode++;
    
    //! temp experiment
    if ( ! ( bGetAll || pRow->m_abGetCol[i_Col] ) ) {
      
      *aiColVal++ = C_UNKNOWN;
      
    L_SkipCodeSwitch:
      
      switch(eMetaCode) {
          
          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        case MC_STR_FIT_1L:
          nLength = *(u1*)ayDataByte++;
          ayDataByte += nLength;
          break;
          
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        case MC_SHOCO_FIT_1C:
          nCompressed = (i8)*(u1*)ayDataByte++;
          ayDataByte += nCompressed;
          break;
          
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        case MC_SHOCO_SPLIT_2T1C:
          nCompressed = (i8)*(u1*)ayDataByte++;
          nBytesPartA = aHeadLine->nBytes() - (ayDataByte - (byte*)aHeadLine);
          ayDataByte += nBytesPartA;
          break;
          
        case MC_OVERFLOW_2T: {
          //vDebugPoint();
          i8 i_Over_rel = aHeadLine->iOverflow2B();
          OverLine* aOverLine = RowNub::aGetOver(aHeadLine,i_Over_rel);
          //MemLib::vWordDump(aOverLine,32);
          
          eMetaCode = * aAdd<eMC*>(aOverLine,_1B_U);
          ayDataByte = aAdd<byte*>(aOverLine,_1B_U + _1B_M);
          goto L_SkipCodeSwitch;
          break;
        }
          
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        default: ayDataByte += nDataBytes(eMetaCode); break;
      }
      
      continue;
    }
    
  L_MetaCodeSwitch:
    switch (eMetaCode) {
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_NULL:
        *aiColVal++ = C_DATUM_NULL;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_1B:
        *aiColVal++ = *(u1*)(ayDataByte);
        ayDataByte += C_1B;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_2B:
        *aiColVal++ = *(u2*)(ayDataByte);
        ayDataByte += C_2B;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_3B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        *aiColVal++ = wWord.m_iI8;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_4B:
        *aiColVal++ = *(u4*)(ayDataByte);
        ayDataByte += C_4B;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_5B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        wWord.m_yaByte[3] = *ayDataByte++;
        wWord.m_yaByte[4] = *ayDataByte++;
        *aiColVal++ = wWord.m_iI8;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_6B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        wWord.m_yaByte[3] = *ayDataByte++;
        wWord.m_yaByte[4] = *ayDataByte++;
        wWord.m_yaByte[5] = *ayDataByte++;
        *aiColVal++ = wWord.m_iI8;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_7B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        wWord.m_yaByte[3] = *ayDataByte++;
        wWord.m_yaByte[4] = *ayDataByte++;
        wWord.m_yaByte[5] = *ayDataByte++;
        wWord.m_yaByte[6] = *ayDataByte++;
        *aiColVal++ = wWord.m_iI8;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_8B:
        *aiColVal++ = *(u8*)(ayDataByte);
        ayDataByte += C_8B;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_MINUS_ONE:
        *aiColVal++ = -1;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_ZERO:
        *aiColVal++ =  0;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_ONE:
        *aiColVal++ =  1;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_TWO:
        *aiColVal++ =  2;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_THREE:
        *aiColVal++ =  3;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_FOUR:
        *aiColVal++ =  4;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_FIVE:
        *aiColVal++ =  5;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_SIX:
        *aiColVal++ =  6;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_SEVEN:
        *aiColVal++ =  7;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_EIGHT:
        *aiColVal++ =  8;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_NINE:
        *aiColVal++ =  9;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_TEN:
        *aiColVal++ = 10;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_SAME:
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_ADD_1B:
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_ADD_2B:
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_SUB_1B:
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_SUB_2B:
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_FALSE:
        *aiColVal++ =  0;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_TRUE:
        *aiColVal++ =  1;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DATE_E0_2B:
        *aiColVal++ = *(u2*)ayDataByte;
        ayDataByte += C_2B;
        vDteFromEdt((Dtm*)(aiColVal-1),C_ERA_0);
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DATE_E1_2B:
        *aiColVal++ = *(u2*)ayDataByte;
        ayDataByte += C_2B;
        vDteFromEdt((Dtm*)(aiColVal-1),C_ERA_1);
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DATE_E2_2B:
        *aiColVal++ = *(u2*)ayDataByte;
        ayDataByte += C_2B;
        vDteFromEdt((Dtm*)(aiColVal-1),C_ERA_2);
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DAYS_ADD_1B:
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_DAYS_SUB_1B:
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_EMPTY:
        *(char**)(aiColVal++) = acString;
        *acString++ = '\0';
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_DUP_2H:
        break;

      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_FIT_1L:
        nLength = *(u1*)ayDataByte++;
        MemLib::vCopy((byte*)acString,
                      (byte*)ayDataByte,
                      nLength,
                      CN_ROWCOMPACTOR);
        *(char**)(aiColVal++) = acString;
        ayDataByte += nLength;
        acString   += nLength;
        *acString++ = '\0';
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_TAIL_2T:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        i_Tail_rel = wWord.m_iI8;
        aTail = aGetTailRel(aHeadLine,i_Tail_rel);
        nLength = aTail->w8_1B(+ _1B_U);
        MemLib::vCopy(acString,(byte*)aTail+_1B_U+_1B_L,nLength,CN_ROWCOMPACTOR);
        *(char**)(aiColVal++) = acString;
        acString += nLength;
        *acString++ = '\0';
         break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_STR_SPLIT_2T1L:
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BSTR_FIT_2L:
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BSTR_TAIL_2T:
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_BSTR_SPLIT_2T2L:
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_SHOCO_DUP_3D: {
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        byte* ayShoco_dup = aAdd<byte*>(aHeadLine,( +  wWord.m_iI8 ));
        nCompressed = (i8)*(u1*)ayShoco_dup++;
        nLength =
        Compress::iFromShoco(acString,                 // decompressed dest
                             sizeof(caString),          // max destination
                             ayShoco_dup,              // compressed source
                             nCompressed);             // bytes compressed
        *(char**)(aiColVal++) = acString;
        acString += nLength;
        *acString++ = '\0';
      }
       break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_SHOCO_FIT_1C:
        nCompressed = (i8)*(u1*)ayDataByte++;
        nLength =
        Compress::iFromShoco(acString,                 // decompressed dest
                             sizeof(caString),          // max destination
                             ayDataByte,               // compressed source
                             nCompressed);             // bytes compressed
        *(char**)(aiColVal++) = acString;
        acString += nLength;
        *acString++ = '\0';
         ayDataByte += nCompressed;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_SHOCO_TAIL_2T:
        //i_Tail_rel  = (i8)*(i2*)aDataByte;
        //aDataByte  += 2;
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        i_Tail_rel = wWord.m_iI8;
        aTail  = aGetTailRel(aHeadLine,i_Tail_rel);
        nCompressed = aTail->w8_1B(C_OFFSET_COMPRESSED);
        nLength =
        Compress::iFromShoco(acString,                 // decompressed dest
                             sizeof(caString),          // max destination
                             ((byte*)aTail)       // compressed source
                               + _1B_U
                               + _1B_C,
                             nCompressed);             // bytes compressed
        *(char**)(aiColVal++) = acString;
        acString += nLength;
        *acString++ = '\0';
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_SHOCO_SPLIT_2T1C:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        i_Tail_rel = wWord.m_iI8;
        aTail  = aGetTailRel(aHeadLine,i_Tail_rel);
        nCompressed = (i8)*(u1*)ayDataByte++;
        nBytesPartA = aHeadLine->nBytes() - (ayDataByte - (byte*)aHeadLine);
        nBytesPartB = nCompressed - nBytesPartA;
        MemLib::vCopy((byte*)caString,
                      (byte*)ayDataByte,
                      nBytesPartA,
                      CN_ROWCOMPACTOR);
        MemLib::vCopy((byte*)caString+nBytesPartA,
                      (byte*)aTail+1,
                      nBytesPartB,
                      CN_ROWCOMPACTOR);
        nLength =
        Compress::iFromShoco(acString,                 // decompressed dest
                             sizeof(caString),          // max destination
                             (byte*)caString,           // compressed source
                             nCompressed);             // bytes compressed
        *(char**)(aiColVal++) = acString;
        acString += nLength;
        *acString++ = '\0';
        ayDataByte += nBytesPartA;
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_TWOS_1B:
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_TWOS_2B:
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_TWOS_3B:
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_TWOS_4B:
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_TWOS_5B:
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_TWOS_6B:
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_TWOS_7B:
        break;
        
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      case MC_OVERFLOW_2T: {
        //vDebugPoint();
        i8 i_Over_rel = aHeadLine->iOverflow2B();
        OverLine* aOverLine = RowNub::aGetOver(aHeadLine,i_Over_rel);
        //MemLib::vWordDump(aOverLine,32);

        eMetaCode = * aAdd<eMC*>(aOverLine,_1B_U);
        ayDataByte = aAdd<byte*>(aOverLine,_1B_U + _1B_M);
        goto L_MetaCodeSwitch;
        break;
      }
        
      default:
        RASSERT(0);
    }
    

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef WITH_TRACE_DECODING
    if (C_DATUM_NULL == *(aiValue-1)) {
      vTrace(" [%lld %15s] null\n"_Stz,i_Col,pCol->szColNm());
      continue; // to next i_Col
    }
        
    switch (pCol->eDatumType()) {
      case DM_BOL:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_BCD:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_DTE:
        wWord.m_iDte = *(aiValue-1);
        vTrace(" [%lld %15s] %4lld : %02lld-%02lld-%02lld\n"_Stz,
               i_Col,
               pCol->szColNm(),
               wWord.m_iDte,
               (i8)wWord.xDte.m_uYearDelta + C_YEAR_BASE,
               (i8)wWord.xDte.m_uMonth,
               (i8)wWord.xDte.m_uDay);
        break;
      case DM_FLT:
        vTrace(" [%lld %15s] %lg\n"_Stz,i_Col,pCol->szColNm(),*(d8*)(aiValue-1));
        break;
      case DM_INT:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_IVL:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_TME:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_TSP:
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;
      case DM_VCH:
        vTrace(" [%lld %15s] %s\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
        break;

      default:
        RASSERT(0);
    }
#endif
  }
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


#if 1

/*----------------------------------------------------------------------------*/
/* get encoded datum from row                                                 */
/*----------------------------------------------------------------------------*/
void RowInflator::vDecode(Datum* pDatum, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_LastColumn, i8& i_LastOffset)
{
  eDM eDatum = pCol->eDatumType();
  
  switch (eDatum) {
      
    case DM_BCD:
      vDecodeBcd(pDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
      break;
      
    case DM_BOL:
      vDecodeBol(pDatum,aHeadLine,pCol);
      break;
      
    case DM_DTE:
      vDecodeDte(pDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
      break;
      
    case DM_FLT:
      vDecodeFlt(pDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
      break;
      
    case DM_INT:
      vDecodeInt(pDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
      break;
      
    case DM_IVL:
      vDecodeIvl(pDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
      break;
      
    case DM_TME:
      vDecodeTme(pDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
      break;
      
    case DM_TSP:
      vDecodeTsp(pDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
      break;
      
    case DM_VCH:
      vDecodeVch(pDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
      break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode decimal from given head line                                        */
/*----------------------------------------------------------------------------*/
void RowInflator::vDecodeBcd(Datum* pDatum, HeadLine* aHeadLine,
                              ColumnVec* pColVec, Column* pCol,
                              i8& i_LastColumn, i8& i_Offset)
{
  
  pDatum->m_eDatum = DM_BCD;
  
  eMC eEncoding = eGetEncoding(aHeadLine,pCol,pColVec,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MC_NULL: pDatum->m_wDtm.m_iInt = C_DATUM_NULL;               break;
    case MC_1B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_1B); break;
    case MC_2B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_2B); break;
    case MC_3B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_3B); break;
    case MC_4B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_4B); break;
    case MC_5B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_5B); break;
    case MC_6B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_6B); break;
    case MC_7B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_7B); break;
    case MC_8B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_8B); break;
      
    case MC_MINUS_ONE: pDatum->m_wDtm.m_iInt = (i8)-1;                break;
    case MC_ZERO:      pDatum->m_wDtm.m_iInt = (i8)0;                 break;
    case MC_ONE:       pDatum->m_wDtm.m_iInt = (i8)1;                 break;
    case MC_TWO:       pDatum->m_wDtm.m_iInt = (i8)2;                 break;
    case MC_THREE:     pDatum->m_wDtm.m_iInt = (i8)3;                 break;
    case MC_FOUR:      pDatum->m_wDtm.m_iInt = (i8)4;                 break;
    case MC_FIVE:      pDatum->m_wDtm.m_iInt = (i8)5;                 break;
    case MC_SIX:       pDatum->m_wDtm.m_iInt = (i8)6;                 break;
    case MC_SEVEN:     pDatum->m_wDtm.m_iInt = (i8)7;                 break;
    case MC_EIGHT:     pDatum->m_wDtm.m_iInt = (i8)8;                 break;
    case MC_NINE:      pDatum->m_wDtm.m_iInt = (i8)9;                 break;
    case MC_TEN:       pDatum->m_wDtm.m_iInt = (i8)10;                break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode boolean from given head line                                        */
/*----------------------------------------------------------------------------*/
void RowInflator::vDecodeBol(Datum* pDatum, HeadLine* aHeadLine,Column* pCol)
{
  pDatum->m_eDatum = DM_BOL;
  
  //eMC eEncoding = eGetEncoding(aHeadLine,pCol,pColVec,i_LastColumn,i_Offset);
  
  i8 i_Column = pCol->iColumnNo();
  eMC eEncoding = aHeadLine->eMetaCode(i_Column);
  
  if (MC_FALSE == eEncoding)
    pDatum->m_wDtm.m_iInt = 0;
  else if (MC_TRUE == eEncoding)
    pDatum->m_wDtm.m_iInt = 1;
  else
    pDatum->m_wDtm.m_iInt = C_DATUM_NULL;
  
  /*
   switch (eEncoding) {
   
   case MC_NULL:  pDatum->m_iInt = C_DATUM_NULL; break;
   case MC_FALSE: pDatum->m_iInt = (i8)0;        break;
   case MC_TRUE:  pDatum->m_iInt = (i8)1;        break;
   
   default:
   NEVER_REACH;
   }
   */
}

/*----------------------------------------------------------------------------*/
/* decode date from given head line                                           */
/*----------------------------------------------------------------------------*/
void RowInflator::vDecodeDte(Datum* pDatum, HeadLine* aHeadLine,
                              ColumnVec* pColVec, Column* pCol,
                              i8& i_LastColumn, i8& i_Offset)
{
  pDatum->m_eDatum = DM_DTE;
  i8 iEra = C_ERA_0;
  
  //!! needs more work
  eMC eThisEncoding = eGetEncoding(aHeadLine,pCol,pColVec,i_LastColumn,i_Offset);
  
  if (eThisEncoding == MC_DAYS_ADD_1B || eThisEncoding == MC_DAYS_SUB_1B) {
    // Not yet implemented in MarkIII
    RASSERT(0);
  }
  
  switch (eThisEncoding) {
      
    case MC_NULL:
      pDatum->m_wDtm.m_iInt = C_DATUM_NULL;
      return;
    case MC_DATE_E0_2B:
      pDatum->m_wDtm.m_iDte = wGetLogical(aHeadLine,i_Offset,C_2B);
      iEra = C_ERA_0;
      break;
    case MC_DATE_E1_2B:
      pDatum->m_wDtm.m_iDte = wGetLogical(aHeadLine,i_Offset,C_2B);
      iEra = C_ERA_1;
      break;
    case MC_DATE_E2_2B:
      pDatum->m_wDtm.m_iDte = wGetLogical(aHeadLine,i_Offset,C_2B);
      iEra = C_ERA_2;
      break;
    default:
      NEVER_REACH;
  }
  
  if (eThisEncoding == MC_DAYS_ADD_1B || eThisEncoding == MC_DAYS_SUB_1B) {
    RASSERT(0);
  }
  else {
    vDteFromEdt(&pDatum->m_wDtm,iEra);
  }
}

/*----------------------------------------------------------------------------*/
/* decode float from given head line                                          */
/*----------------------------------------------------------------------------*/
void RowInflator::vDecodeFlt(Datum* pDatum, HeadLine* aHeadLine,
                              ColumnVec* pColVec, Column* pCol,
                              i8& i_LastColumn, i8& i_Offset)
{
  pDatum->m_eDatum = DM_FLT;
  
  eMC eEncoding = eGetEncoding(aHeadLine,pCol,pColVec,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MC_NULL: pDatum->m_wDtm.m_iInt = C_DATUM_NULL;               break;
    case MC_1B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_1B); break;
    case MC_2B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_2B); break;
    case MC_3B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_3B); break;
    case MC_4B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_4B); break;
    case MC_5B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_5B); break;
    case MC_6B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_6B); break;
    case MC_7B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_7B); break;
    case MC_8B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_8B); break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode integer from given head line                                        */
/*----------------------------------------------------------------------------*/
void RowInflator::vDecodeInt(Datum* pDatum, HeadLine* aHeadLine,
                              ColumnVec* pColVec, Column* pCol,
                              i8& i_LastColumn, i8& i_Offset)
{
  pDatum->m_eDatum = DM_INT;
  
  eMC eEncoding = eGetEncoding(aHeadLine,pCol,pColVec,i_LastColumn,i_Offset);
  //vTrace("Decoding %lld\n"_Stz, eEncoding);
  
  switch (eEncoding) {
      
    case MC_NULL: pDatum->m_wDtm.m_iInt = C_DATUM_NULL;               break;
    case MC_1B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_1B); break;
    case MC_2B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_2B); break;
    case MC_3B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_3B); break;
    case MC_4B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_4B); break;
    case MC_5B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_5B); break;
    case MC_6B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_6B); break;
    case MC_7B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_7B); break;
    case MC_8B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_8B); break;
      
    case MC_MINUS_ONE: pDatum->m_wDtm.m_iInt = (i8)-1;                break;
    case MC_ZERO:      pDatum->m_wDtm.m_iInt = (i8)0;                 break;
    case MC_ONE:       pDatum->m_wDtm.m_iInt = (i8)1;                 break;
    case MC_TWO:       pDatum->m_wDtm.m_iInt = (i8)2;                 break;
    case MC_THREE:     pDatum->m_wDtm.m_iInt = (i8)3;                 break;
    case MC_FOUR:      pDatum->m_wDtm.m_iInt = (i8)4;                 break;
    case MC_FIVE:      pDatum->m_wDtm.m_iInt = (i8)5;                 break;
    case MC_SIX:       pDatum->m_wDtm.m_iInt = (i8)6;                 break;
    case MC_SEVEN:     pDatum->m_wDtm.m_iInt = (i8)7;                 break;
    case MC_EIGHT:     pDatum->m_wDtm.m_iInt = (i8)8;                 break;
    case MC_NINE:      pDatum->m_wDtm.m_iInt = (i8)9;                 break;
    case MC_TEN:       pDatum->m_wDtm.m_iInt = (i8)10;                break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode interval from given head line                                       */
/*----------------------------------------------------------------------------*/
void RowInflator::vDecodeIvl(Datum* pDatum, HeadLine* aHeadLine,
                              ColumnVec* pColVec, Column* pCol,
                              i8& i_LastColumn, i8& i_Offset)
{
  pDatum->m_eDatum = DM_IVL;
  
  eMC eEncoding = eGetEncoding(aHeadLine,pCol,pColVec,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MC_NULL: pDatum->m_wDtm.m_iInt = C_DATUM_NULL;               break;
    case MC_1B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_1B); break;
    case MC_2B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_2B); break;
    case MC_3B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_3B); break;
    case MC_4B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_4B); break;
    case MC_5B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_5B); break;
    case MC_6B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_6B); break;
    case MC_7B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_7B); break;
    case MC_8B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_8B); break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode time from given head line                                           */
/*----------------------------------------------------------------------------*/
void RowInflator::vDecodeTme(Datum* pDatum, HeadLine* aHeadLine,
                              ColumnVec* pColVec, Column* pCol,
                              i8& i_LastColumn, i8& i_Offset)
{
  pDatum->m_eDatum = DM_TME;
  
  eMC eEncoding = eGetEncoding(aHeadLine,pCol,pColVec,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MC_NULL: pDatum->m_wDtm.m_iInt = C_DATUM_NULL;               break;
    case MC_1B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_1B); break;
    case MC_2B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_2B); break;
    case MC_3B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_3B); break;
    case MC_4B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_4B); break;
    case MC_5B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_5B); break;
    case MC_6B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_6B); break;
    case MC_7B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_7B); break;
    case MC_8B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_8B); break;
      
    default:
      NEVER_REACH;
  }
  
  //vRestoreTme(pDatum);
  pDatum->m_wDtm.m_iInt <<= 40;
}

/*----------------------------------------------------------------------------*/
/* decode timestamp from given head line                                      */
/*----------------------------------------------------------------------------*/
void RowInflator::vDecodeTsp(Datum* pDatum, HeadLine* aHeadLine,
                              ColumnVec* pColVec, Column* pCol,
                              i8& i_LastColumn, i8& i_Offset)
{
  pDatum->m_eDatum = DM_TSP;
  
  eMC eEncoding = eGetEncoding(aHeadLine,pCol,pColVec,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MC_NULL: pDatum->m_wDtm.m_iInt = C_DATUM_NULL;                         break;
    case MC_1B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_1B); break;
    case MC_2B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_2B); break;
    case MC_3B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_3B); break;
    case MC_4B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_4B); break;
    case MC_5B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_5B); break;
    case MC_6B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_6B); break;
    case MC_7B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_7B); break;
    case MC_8B:   pDatum->m_wDtm.m_iInt = wGetLogical(aHeadLine,i_Offset,C_8B); break;
      
    default:
      NEVER_REACH;
  }
  
  if (eEncoding == MC_5B)
    //vRestoreTsp(pDatum);
    pDatum->m_wDtm.m_iInt <<= 24;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given head line                                        */
/*----------------------------------------------------------------------------*/
void RowInflator::vDecodeVch(Datum* pDatum, HeadLine* aHeadLine,
                              ColumnVec* pColVec, Column* pCol,
                              i8& i_LastColumn, i8& i_Offset)
{
  
  pDatum->m_eDatum = DM_VCH;
  
  //! needs more work
  eMC eEncoding = eGetEncoding(aHeadLine,pCol,pColVec,i_LastColumn,i_Offset);
  
  if (MC_NULL == eEncoding) {
    pDatum->m_wDtm.m_iInt = C_DATUM_NULL;
    return;
  }
  
  else if (MC_STR_EMPTY == eEncoding) {
    pDatum->m_nChars = 0;
    *pDatum->m_sVch.aChar() = '\0';
    pDatum->m_wDtm.m_iInt = 0;
  }
  
  else if (MC_STR_FIT_1L == eEncoding)
    vDecodeVchSmall(pDatum,aHeadLine,pCol,i_Offset);
  
  else if (MC_STR_TAIL_2T == eEncoding)
    vDecodeVchStringTail(pDatum,aHeadLine,pCol,i_Offset);
  
  else if (MC_SHOCO_FIT_1C == eEncoding)
    vDecodeVchShocoFit(pDatum,aHeadLine,pCol,i_Offset);
  
  else if (MC_SHOCO_DUP_3D == eEncoding)
    vDecodeVchShocoDup(pDatum,aHeadLine,pCol,i_Offset);
  
  else if (MC_SHOCO_TAIL_2T == eEncoding)
    vDecodeVchShocoTail(pDatum,aHeadLine,pCol,i_Offset);
  
  // ??
  else if (MC_SHOCO_SPLIT_2T1C == eEncoding)
    vDecodeVchShocoSplit(pDatum,aHeadLine,pCol,i_Offset);
  
  else
    NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* decode MC_SHOCO_DUP_3D shoco duplicate                                     */
/*----------------------------------------------------------------------------*/
void RowInflator::vDecodeVchShocoDup(Datum* pDatum, HeadLine* aHeadLine,
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
  
  i8 i_Dsr = wGetLogical(aHeadLine,i_Offset,_3B_DSR);
  
  byte* ayC_Shoco = aAdd<byte*>(aHeadLine,( + i_Dsr )); //! WARNING
  
  i8 nCompressed = *(u1*)ayC_Shoco;
  
  i8 nDecompressed =
  Compress::iFromShoco((char*)pDatum->m_sVch.aChar(),
                       pDatum->m_sVch.nSpace(),
                       ayC_Shoco + _1B_C,
                       nCompressed);
  ASSERT(nDecompressed > 0);
  pDatum->m_nChars = nDecompressed;
}

/*----------------------------------------------------------------------------*/
/* decode MC_SHOCO_FIT_1C tail line shoco                                     */
/*----------------------------------------------------------------------------*/
void RowInflator::vDecodeVchShocoFit(Datum* pDatum, HeadLine* aHeadLine,
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
  
  i8 nCompressed = wGetLogical(aHeadLine,i_Offset,_1B_C);
  //RASSERT(nCompressed <= 56);
  
  i8 nDecompressed =
  Compress::iFromShoco((char*)pDatum->m_sVch.aChar(),
                       pDatum->m_sVch.nSpace(),
                       ayAdrLogical(aHeadLine,i_Offset + _1B_C),
                       //((byte*)aHeadLine)+i_Offset+1,
                       nCompressed);
  ASSERT(nDecompressed > 0);
  pDatum->m_nChars = nDecompressed;
}

/*----------------------------------------------------------------------------*/
/* decode MC_SHOCO_SPLIT_2T1C tail line shoco                                 */
/*----------------------------------------------------------------------------*/
void RowInflator::vDecodeVchShocoSplit(Datum* pDatum, HeadLine* aHeadLine,
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
  
  i8 i_Tail_rel = wGetLogical(aHeadLine,i_Offset,C_2B);
  TailLine* aShocoTail = aGetTailRel(aHeadLine,i_Tail_rel);
  
  i8 nCompressed = wGetLogical(aHeadLine,(i_Offset+2),C_1B);
  
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
  Compress::iFromShoco((char*)pDatum->m_sVch.aChar(),
                       255,
                       (byte*)caShoco+2,
                       nCompressed);
  ASSERT(nDecompressed);
  pDatum->m_nChars = nDecompressed;
}

/*----------------------------------------------------------------------------*/
/* decode MC_SHOCO_TAIL_2T tail line shoco                                    */
/*----------------------------------------------------------------------------*/
void RowInflator::vDecodeVchShocoTail(Datum* pDatum, HeadLine* aHeadLine,
                                       Column* pCol, i8 i_Offset)
{
  i8 i_Tail_rel = wGetLogical(aHeadLine,i_Offset,C_2B);
  TailLine* aShocoTail = aGetTailRel(aHeadLine,i_Tail_rel);
  
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
  
  i8 nCompressed = aShocoTail->w8_1B(C_OFFSET_COMPRESSED);
  //RASSERT(nCompressed <= 4);
  
  i8 nDecompressed =
  Compress::iFromShoco(pDatum->m_sVch.aChar(),   // decompressed destination
                       255,                      // max destination
                       ((byte*)aShocoTail)       // from compressed source
                       + C_OFFSET_COMPRESSED
                       + _1B_C,
                       nCompressed);             // bytes compressed
  ASSERT(nDecompressed);
  pDatum->m_nChars = nDecompressed;
}

/*----------------------------------------------------------------------------*/
/* decode MC_STRING_TAIL_2T tail line string                                 */
/*----------------------------------------------------------------------------*/
void RowInflator::vDecodeVchStringTail(Datum* pDatum, HeadLine* aHeadLine,
                                        Column* pCol, i8 i_Offset)
{
  i8 i_Tail_rel = wGetLogical(aHeadLine,i_Offset,C_2B);
  TailLine* aTail_str = aGetTailRel(aHeadLine,i_Tail_rel);
  
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
  
  i8 nLength = aTail_str->w8_1B(+ _1B_U);
  byte* ayString = (byte*)aTail_str + _1B_U + _1B_L;
  
  MemLib::vCopy((byte*)pDatum->m_sVch.aChar(),
                (byte*)ayString,
                nLength,
                CN_ROWCOMPACTOR);
  
  * (pDatum->m_sVch.aChar() + nLength) = '\0';
  
  pDatum->m_nChars = nLength;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given head line                                        */
/*----------------------------------------------------------------------------*/
void RowInflator::vDecodeVchSmall(Datum* pDatum, HeadLine* aHeadLine,
                                   Column* pCol, i8 i_Offset)
{
  vDebugPoint();
  
  i8 nLength = wGetLogical(aHeadLine,i_Offset,C_1B);
  
  MemLib::vCopy((byte*)pDatum->m_sVch.aChar(),
                (byte*)ayAdrLogical(aHeadLine,i_Offset + 1),
                nLength,
                CN_ROWCOMPACTOR);
  
  * (pDatum->m_sVch.aChar() + nLength) = '\0';
  
  pDatum->m_nChars = nLength;
  
}

#endif


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
