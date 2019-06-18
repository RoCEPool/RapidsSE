/*============================================================================*/
/* RowInflator.cpp -- Row Inflator Mark IV                                    */
/*============================================================================*/

#include "Data.h"

/*----------------------------------------------------------------------------*/
/* inflate headline into row                                                  */
/*----------------------------------------------------------------------------*/
#ifdef WITH_QUICKER_INFLATE

void RowInflator::vInflate(Row* pRow, HeadLine* aHeadLine)
{
  eMC  eMetaCode;
  
  i8   i_Tail_rel;
  i8   nCompressed;
  i8   nDecompressed;
  i8   nLength;
  i8   nBytesPartA;
  i8   nBytesPartB;
  char caString[C_4KB];
  
  TailLine* aShocoTail;
  
#ifdef WITH_TRACE_DECODING
  static i8 nTotalRows = 0;
  vTrace("vInflate() of row %lld\n"_Stz,nTotalRows++);
  vDumpHead("aHeadLine"_Stz,aHeadLine,pRow->m_pColVec);
#endif

  char* aString = pRow->m_xWsp.aArea();
  
  i8 nCols = pRow->m_nColumns;
  
  byte* aMetaCode = (byte*)aHeadLine + _1B_W;
  byte* aDataByte = (byte*)aHeadLine + pRow->m_iBaseOffset;
  byte* aValue    = (byte*)&pRow->m_aiValue[0];
  
  bool  bGetAll   = pRow->m_bGetAll;
  i8    iLastCol  = pRow->m_iLastCol;
  
  MemLib::vZero(aValue,nCols*sizeof(i8),CN_ROW);
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    if (! bGetAll && i_Col >  iLastCol) {
      //exit early
      break;
    }
    
#ifdef WITH_TRACE_DECODING
    Column* pCol = pRow->m_pColVec->pGet(i_Col);
#endif
    
    eMetaCode = (eMC)*aMetaCode++;
    
    //! temp experiment
    if ( ! ( bGetAll || pRow->m_abGetCol[i_Col] ) ) {
      
      *(u8*)aValue = C_UNKNOWN;
      aValue += 8;
      
      switch(eMetaCode) {
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        case MC_STR_FIT_1L:
          nLength = *(u1*)aDataByte++;
          aDataByte += nLength;
          break;
          
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        case MC_SHOCO_FIT_1C:
          nCompressed = (i8)*(u1*)aDataByte++;
          aDataByte += nCompressed;
          break;
          
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        case MC_SHOCO_SPLIT_2T1C:
          nCompressed = (i8)*(u1*)aDataByte++;
          nBytesPartA = aHeadLine->nBytes() - (aDataByte - (byte*)aHeadLine);
          aDataByte += nBytesPartA;
          break;
          
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        default: aDataByte += RowCompactor::nDataBytes(eMetaCode); break;
      }
    
      continue;
    }
    
    switch (eMetaCode) {
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_NULL:
        *(u8*)aValue = C_DATUM_NULL;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] null\n"_Stz,i_Col,pCol->szColNm());
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_1B:
        *(u1*)aValue = *(u1*)(aDataByte);
        aValue += 8;
        aDataByte += C_1B;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_2B:
        *(u2*)aValue = *(u2*)(aDataByte);
        aValue += 8;
        aDataByte += C_2B;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_3B:
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        aValue += 5;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
 
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_4B:
        *(u4*)aValue++ = *(u4*)(aDataByte);
        aValue += 4;
        aDataByte += C_4B;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_5B:
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++= *aDataByte++;
        aValue += 3;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_6B:
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++= *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        aValue += 2;;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_7B:
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        *aValue++ = *aDataByte++;
        aValue++;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_8B:
        *(u8*)aValue = *(u8*)(aDataByte);
        aValue += 8;
        aDataByte += C_8B;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_MINUS_ONE:
        *(u8*)aValue = -1;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_ZERO:
        *(u8*)aValue = 0;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_ONE:
        *(u8*)aValue = 1;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWO:
        *(u8*)aValue = 2;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_THREE:
        *(u8*)aValue = 3;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_FOUR:
        *(u8*)aValue = 4;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_FIVE:
        *(u8*)aValue = 5;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SIX:
        *(u8*)aValue = 6;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SEVEN:
        *(u8*)aValue = 7;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_EIGHT:
        *(u8*)aValue = 8;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_NINE:
        *(u8*)aValue = 9;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TEN:
        *(u8*)aValue = 10;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SAME:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_ADD_1B:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_ADD_2B:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SUB_1B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SUB_2B:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_FALSE:
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TRUE:
        *aValue =  1;
        aValue += 8;
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;

/*
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DATE_E0_2B:
        *aiValue++ = *(u2*)aDataByte;
        aDataByte += C_2B;
        vDteFromEdt((Word*)(aiValue-1),C_ERA_0);
#ifdef WITH_TRACE_DECODING
        wWord.m_iDte = *(aiValue-1);
        vTrace(" [%lld %15s] %4lld : %02lld-%02lld-%02lld\n"_Stz,
               i_Col,
               pCol->szColNm(),
               wWord.m_iDte,
               (i8)wWord.xEdt.m_uYearInEra + C_ERA_0 + C_YEAR_BASE,
               (i8)wWord.xEdt.m_uMonth,
               (i8)wWord.xEdt.m_uDay);
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DATE_E1_2B:
        *aiValue++ = *(u2*)aDataByte;
        aDataByte += C_2B;
        vDteFromEdt((Word*)(aiValue-1),C_ERA_1);
#ifdef WITH_TRACE_DECODING
        wWord.m_iDte = *(aiValue-1);
        vTrace(" [%lld %15s] %4lld : %02lld-%02lld-%02lld\n"_Stz,
               i_Col,
               pCol->szColNm(),
               wWord.m_iDte,
               (i8)wWord.xEdt.m_uYearInEra + C_ERA_1 + C_YEAR_BASE,
               (i8)wWord.xEdt.m_uMonth,
               (i8)wWord.xEdt.m_uDay);
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DATE_E2_2B:
        *aiValue++ = *(u2*)aDataByte;
        aDataByte += C_2B;
        vDteFromEdt((Word*)(aiValue-1),C_ERA_2);
#ifdef WITH_TRACE_DECODING
        wWord.m_iDte = *(aiValue-1);
        vTrace(" [%lld %15s] %4lld : %02lld-%02lld-%02lld\n"_Stz,
               i_Col,
               pCol->szColNm(),
               wWord.m_iDte,
               (i8)wWord.xEdt.m_uYearInEra + C_ERA_2 + C_YEAR_BASE,
               (i8)wWord.xEdt.m_uMonth,
               (i8)wWord.xEdt.m_uDay);
#endif
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DAYS_ADD_1B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DAYS_SUB_1B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_STR_EMPTY:        
        *(char**)(aiValue++) = aString;
        *aString++ = '\0';
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %lld\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_STR_FIT_1L:
        nLength = *(u1*)aDataByte++;
        MemLib::vCopy((byte*)aString,
                      (byte*)aDataByte,
                      nLength,
                      CN_ROWCOMPACTOR);
        *(char**)(aiValue++) = aString;
        aDataByte += nLength;
        aString   += nLength;
        *aString++ = '\0';
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %s\n"_Stz,i_Col,pCol->szColNm(),*(aiValue-1));
#endif
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_STR_TAIL_2T:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_STR_SPLIT_2T1L:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_BSTR_FIT_2L:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_BSTR_TAIL_2T:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_BSTR_SPLIT_2T2L:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SHOCO_FIT_1C:
        nCompressed = (i8)*(u1*)aDataByte++;
        nDecompressed =
          Compress::iFromShoco(aString,                  // decompressed dest
                               sizeof(caShoco),          // max destination
                               aDataByte,                // compressed source
                               nCompressed);             // bytes compressed
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %s\n"_Stz,i_Col,pCol->szColNm(),aString);
#endif
        *(char**)(aiValue++) = aString;
        aString += nDecompressed;
        *aString++ = '\0';
        pRow->m_i_Wsp += nDecompressed;
        aDataByte += nCompressed;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SHOCO_TAIL_2T:
        //i_Tail_rel  = (i8)*(i2*)aDataByte;
        //aDataByte  += 2;
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *aDataByte++;
        wWord.m_yaByte[1] = *aDataByte++;
        i_Tail_rel = wWord.m_iI8;
        aShocoTail  = aGetTailRel(aHeadLine,i_Tail_rel);
        nCompressed = aShocoTail->w8_1B(C_OFFSET_COMPRESSED);
        nDecompressed =
          Compress::iFromShoco(aString,                  // decompressed dest
                               sizeof(caShoco),          // max destination
                               ((byte*)aShocoTail)       // compressed source
                                 + C_OFFSET_COMPRESSED
                                 + _1B_C,
                               nCompressed);             // bytes compressed
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %s\n"_Stz,i_Col,pCol->szColNm(),aString);
#endif
        *(char**)(aiValue++) = aString;
        aString += nDecompressed;
        *aString++ = '\0';
        pRow->m_i_Wsp += nDecompressed;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SHOCO_SPLIT_2T1C:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *aDataByte++;
        wWord.m_yaByte[1] = *aDataByte++;
        i_Tail_rel = wWord.m_iI8;
        aShocoTail  = aGetTailRel(aHeadLine,i_Tail_rel);
        nCompressed = (i8)*(u1*)aDataByte++;
        nBytesPartA = aHeadLine->nBytes() - (aDataByte - (byte*)aHeadLine);
        nBytesPartB = nCompressed - nBytesPartA;
        MemLib::vCopy((byte*)caShoco,
                      (byte*)aDataByte,
                      nBytesPartA,
                      CN_ROWCOMPACTOR);
        MemLib::vCopy((byte*)caShoco+nBytesPartA,
                      (byte*)aShocoTail+1,
                      nBytesPartB,
                      CN_ROWCOMPACTOR);
        nDecompressed =
          Compress::iFromShoco(aString,                  // decompressed dest
                               sizeof(caShoco),          // max destination
                               (byte*)caShoco,           // compressed source
                               nCompressed);             // bytes compressed
#ifdef WITH_TRACE_DECODING
        vTrace(" [%lld %15s] %s\n"_Stz,i_Col,pCol->szColNm(),aString);
#endif
        *(char**)(aiValue++) = aString;
        aString += nDecompressed;
        *aString++ = '\0';
        pRow->m_i_Wsp += nDecompressed;
        aDataByte += nBytesPartA;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_1B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_2B:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_3B:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_4B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_5B:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_6B:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_7B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_OVERFLOW_2T:
        break;
 */
      default:
        RASSERT(0);
    }
    
  }
}

#else
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
  i8*   aiValue    = & pRow->m_aiValue[0];
  
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
      
      *aiValue++ = C_UNKNOWN;
      
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
        default: ayDataByte += RowCompactor::nDataBytes(eMetaCode); break;
      }
      
      continue;
    }
    
  L_MetaCodeSwitch:
    switch (eMetaCode) {
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_NULL:
        *aiValue++ = C_DATUM_NULL;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_1B:
        *aiValue++ = *(u1*)(ayDataByte);
        ayDataByte += C_1B;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_2B:
        *aiValue++ = *(u2*)(ayDataByte);
        ayDataByte += C_2B;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_3B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        *aiValue++ = wWord.m_iI8;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_4B:
        *aiValue++ = *(u4*)(ayDataByte);
        ayDataByte += C_4B;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_5B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        wWord.m_yaByte[3] = *ayDataByte++;
        wWord.m_yaByte[4] = *ayDataByte++;
        *aiValue++ = wWord.m_iI8;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_6B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        wWord.m_yaByte[3] = *ayDataByte++;
        wWord.m_yaByte[4] = *ayDataByte++;
        wWord.m_yaByte[5] = *ayDataByte++;
        *aiValue++ = wWord.m_iI8;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_7B:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        wWord.m_yaByte[2] = *ayDataByte++;
        wWord.m_yaByte[3] = *ayDataByte++;
        wWord.m_yaByte[4] = *ayDataByte++;
        wWord.m_yaByte[5] = *ayDataByte++;
        wWord.m_yaByte[6] = *ayDataByte++;
        *aiValue++ = wWord.m_iI8;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_8B:
        *aiValue++ = *(u8*)(ayDataByte);
        ayDataByte += C_8B;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_MINUS_ONE:
        *aiValue++ = -1;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_ZERO:
        *aiValue++ =  0;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_ONE:
        *aiValue++ =  1;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWO:
        *aiValue++ =  2;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_THREE:
        *aiValue++ =  3;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_FOUR:
        *aiValue++ =  4;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_FIVE:
        *aiValue++ =  5;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SIX:
        *aiValue++ =  6;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SEVEN:
        *aiValue++ =  7;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_EIGHT:
        *aiValue++ =  8;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_NINE:
        *aiValue++ =  9;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TEN:
        *aiValue++ = 10;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SAME:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_ADD_1B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_ADD_2B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SUB_1B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SUB_2B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_FALSE:
        *aiValue++ =  0;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TRUE:
        *aiValue++ =  1;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DATE_E0_2B:
        *aiValue++ = *(u2*)ayDataByte;
        ayDataByte += C_2B;
        RowCompactor::vDteFromEdt((Word*)(aiValue-1),C_ERA_0);
        break;
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DATE_E1_2B:
        *aiValue++ = *(u2*)ayDataByte;
        ayDataByte += C_2B;
        RowCompactor::vDteFromEdt((Word*)(aiValue-1),C_ERA_1);
        break;
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DATE_E2_2B:
        *aiValue++ = *(u2*)ayDataByte;
        ayDataByte += C_2B;
        RowCompactor::vDteFromEdt((Word*)(aiValue-1),C_ERA_2);
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DAYS_ADD_1B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_DAYS_SUB_1B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_STR_EMPTY:
        *(char**)(aiValue++) = acString;
        *acString++ = '\0';
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_STR_DUP_2H:
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_STR_FIT_1L:
        nLength = *(u1*)ayDataByte++;
        MemLib::vCopy((byte*)acString,
                      (byte*)ayDataByte,
                      nLength,
                      CN_ROWCOMPACTOR);
        *(char**)(aiValue++) = acString;
        ayDataByte += nLength;
        acString   += nLength;
        *acString++ = '\0';
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_STR_TAIL_2T:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        i_Tail_rel = wWord.m_iI8;
        aTail = RowCompactor::aGetTailRel(aHeadLine,i_Tail_rel);
        nLength = aTail->w8_1B(+ _1B_U);
        MemLib::vCopy(acString,(byte*)aTail+_1B_U+_1B_L,nLength,CN_ROWCOMPACTOR);
        *(char**)(aiValue++) = acString;
        acString += nLength;
        *acString++ = '\0';
        pRow->m_i_Wsp += nLength;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_STR_SPLIT_2T1L:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_BSTR_FIT_2L:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_BSTR_TAIL_2T:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_BSTR_SPLIT_2T2L:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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
        *(char**)(aiValue++) = acString;
        acString += nLength;
        *acString++ = '\0';
        pRow->m_i_Wsp += nLength;
      }
       break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SHOCO_FIT_1C:
        nCompressed = (i8)*(u1*)ayDataByte++;
        nLength =
        Compress::iFromShoco(acString,                 // decompressed dest
                             sizeof(caString),          // max destination
                             ayDataByte,               // compressed source
                             nCompressed);             // bytes compressed
        *(char**)(aiValue++) = acString;
        acString += nLength;
        *acString++ = '\0';
        pRow->m_i_Wsp += nLength;
        ayDataByte += nCompressed;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SHOCO_TAIL_2T:
        //i_Tail_rel  = (i8)*(i2*)aDataByte;
        //aDataByte  += 2;
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        i_Tail_rel = wWord.m_iI8;
        aTail  = RowCompactor::aGetTailRel(aHeadLine,i_Tail_rel);
        nCompressed = aTail->w8_1B(C_OFFSET_COMPRESSED);
        nLength =
        Compress::iFromShoco(acString,                 // decompressed dest
                             sizeof(caString),          // max destination
                             ((byte*)aTail)       // compressed source
                               + _1B_U
                               + _1B_C,
                             nCompressed);             // bytes compressed
        *(char**)(aiValue++) = acString;
        acString += nLength;
        *acString++ = '\0';
        pRow->m_i_Wsp += nLength;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_SHOCO_SPLIT_2T1C:
        wWord.m_iI8 = 0;
        wWord.m_yaByte[0] = *ayDataByte++;
        wWord.m_yaByte[1] = *ayDataByte++;
        i_Tail_rel = wWord.m_iI8;
        aTail  = RowCompactor::aGetTailRel(aHeadLine,i_Tail_rel);
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
        *(char**)(aiValue++) = acString;
        acString += nLength;
        *acString++ = '\0';
        pRow->m_i_Wsp += nLength;
        ayDataByte += nBytesPartA;
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_1B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_2B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_3B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_4B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_5B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_6B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case MC_TWOS_7B:
        break;
        
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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
#endif


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
