/*============================================================================*/
/* RowInflator.cpp -- Compact Row Inflator                                    */
/*============================================================================*/

#include "CompactBuffers.h"

/*----------------------------------------------------------------------------*/
/* inflate set onto Dtsk                                                      */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflate_ColumnSet_into_CellArray(i8** aaCellArray,
                                                    char*& racStrArea,
                                                    eSI eStrInflateRule,
                                                    HeadLine* aHeadLine,
                                                    i8* iaColSet,
                                                    StreamDef* pStreamDef)
{
  _static_

  //vTrace("vInflate_ColumnSet_into_CellArray\n"_Stz);
  
  u1* auMetaCode;
  byte* ayDataByte;
  
  i8 nCols = pStreamDef->nTotalColumns();

  //vPrepare_v2(auMetaCode,ayDataByte,nCols,aHeadLine);
  auMetaCode = aHeadLine->auMetaArea();
  
  i8 i_BaseOffsetData = C_W_1B   // (ControlDate) 1 byte for the headline size (in words)
  + C_T_8B // (ControlDate) 8 bytes for the perspective
  + nCols; // metaSize
  
  ayDataByte = aAdd<byte*>(aHeadLine,i_BaseOffsetData);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //
  
  char* acStrArea = nullptr;
  
  i8* aiDstk_lcl = *aaCellArray;
  
  byte* ayDstByte = (byte*)++aiDstk_lcl;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each column

  i8 i_WhichCol = 0;
  i8 i_ColToRetrieve = iaColSet[i_WhichCol];
  
  i8 nSizeUnits;
  i8 nHeadBytes;
  i8 i_Over_rel;
  i8 nStrSize;
  
  i8* aiCellValue;
  u1 uMetaCode;
  u1 uHigh;
  u1 uLow;
  
  u2 u_Tail_rel;
  
  char* azDst;
  char** aacCellValue;
  
  Edt* aEdt;
  Dte* aDte;
  
  OverLine* aOverLine;
  TailLine* aTailLine;
  
  for (i8 i_Col = 0; /*none*/; i_Col++) {
        
    uMetaCode = *auMetaCode++;
    uHigh = uMetaCode >> 4;
    uLow  = uMetaCode & 0x0F;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ignore column
    
    if (i_Col != i_ColToRetrieve) {
      
    L_Overflowed:
      
      if (uHigh <=  MC_STRING_x1) // numeric & date
        ayDataByte += uLow;
      else
        if (uHigh >=  MC_NEGATIVE)  // small value
        {;}
      else
      if (uHigh == MC_STRING_x1)  // string x 1
        ayDataByte += STRx1(uLow);
      else
      if (uHigh == MC_STRING_x2)  // string x 2
        ayDataByte += STRx2(uLow);
      else
      if (uHigh == MC_STRING_x4)  // string x 4
        ayDataByte += STRx4(uLow);
      else
      if (uHigh == MC_STRING_x8)  // string x 8
        ayDataByte += STRx8(uLow);
      else
      if (uHigh == MC_STRING_TAIL)  // string tail
        ayDataByte += 2;
      else
      if (uHigh == MC_OVERFLOW) { // overflow
        
        //OverLine* aOverLine = aHeadLine->aGetOver();
        
        // calculation to get overflow
        nSizeUnits = aHeadLine->w8_1B(C_HEADLINE_OFS_WORDS);
        if (nSizeUnits >= 128) nSizeUnits -= 128;
        nHeadBytes = nSizeUnits * C_BYTES_PER_WORD;
        i_Over_rel = aHeadLine->w8_2B(nHeadBytes - C_2B);
        
        aOverLine = RowNub::aGetOver_rel(aHeadLine,i_Over_rel);
        //OverLine* aOverLine = (OverLine*)aGetTail_rel(aHeadLine,i_Over_rel);
        
        uMetaCode = *((u1*)aOverLine + C_U_1B);
        METACODE_SPLIT(uMetaCode,uHigh,uLow);
        ayDataByte = (byte*)aOverLine + C_U_1B + C_M_1B;
        goto L_Overflowed;
      }
      else {
        RASSERT(0);
      }
      
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inflate column

    //vTrace("Inflate %lld\n"_Stz,i_Col);
    
    //RASSERT(SI_CELLARRAY_ADR == eStrInflateRule || SI_CELLARRAY_PST == eStrInflateRule);
    
    
    /////////////////////////////////////////////////////////////////////////////
    
  L_HighSwitch:
    
    aiCellValue = (i8*)ayDstByte;
    
    switch (uHigh) {
        
      case MC_NUMERIC: // numeric values 0..8B
        
        switch (uLow) {
            
          case 0x0: // MC_NUMERIC 0B
              *aiCellValue = 0;
              ayDstByte += C_8B;
//            *ayDstByte++ = 0; // 0
//            *ayDstByte++ = 0; // 1
//            *ayDstByte++ = 0; // 2
//            *ayDstByte++ = 0; // 3
//            *ayDstByte++ = 0; // 4
//            *ayDstByte++ = 0; // 5
//            *ayDstByte++ = 0; // 6
//            *ayDstByte++ = 0; // 7
            break;
            
          case 0x1: // MC_NUMERIC 1B
            *aiCellValue = 0;
            *ayDstByte++ = *ayDataByte++; // 0
            ayDstByte += C_7B;
//            *ayDstByte++ = 0; // 1
//            *ayDstByte++ = 0; // 2
//            *ayDstByte++ = 0; // 3
//            *ayDstByte++ = 0; // 4
//            *ayDstByte++ = 0; // 5
//            *ayDstByte++ = 0; // 6
//            *ayDstByte++ = 0; // 7
            break;
            
          case 0x2: // MC_NUMERIC 2B
            *aiCellValue = *(u2*)(ayDataByte);
            ayDataByte += C_2B;
            ayDstByte += C_8B;
//            *ayDstByte++ = *ayDataByte++; // 0
//            *ayDstByte++ = *ayDataByte++; // 1
//            *ayDstByte++ = 0; // 2
//            *ayDstByte++ = 0; // 3
//            *ayDstByte++ = 0; // 4
//            *ayDstByte++ = 0; // 5
//            *ayDstByte++ = 0; // 6
//            *ayDstByte++ = 0; // 7
            break;
            
          case 0x3: // MC_NUMERIC 3B
            *aiCellValue = 0;
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            ayDstByte += C_5B;
//            *ayDstByte++ = 0; // 3
//            *ayDstByte++ = 0; // 4
//            *ayDstByte++ = 0; // 5
//            *ayDstByte++ = 0; // 6
//            *ayDstByte++ = 0; // 7
            break;
            
          case 0x4: // MC_NUMERIC 4B
            *aiCellValue = *(u4*)(ayDataByte);
            ayDataByte += C_4B;
            ayDstByte += C_8B;
//            *ayDstByte++ = *ayDataByte++; // 0
//            *ayDstByte++ = *ayDataByte++; // 1
//            *ayDstByte++ = *ayDataByte++; // 2
//            *ayDstByte++ = *ayDataByte++; // 3
//            *ayDstByte++ = 0; // 4
//            *ayDstByte++ = 0; // 5
//            *ayDstByte++ = 0; // 6
//            *ayDstByte++ = 0; // 7
            break;
            
          case 0x5: // MC_NUMERIC 5B
            *aiCellValue = 0;
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            *ayDstByte++ = *ayDataByte++; // 3
            *ayDstByte++ = *ayDataByte++; // 4
            ayDstByte += C_3B;
//            *ayDstByte++ = 0; // 5
//            *ayDstByte++ = 0; // 6
//            *ayDstByte++ = 0; // 7
            break;
            
          case 0x6: // MC_NUMERIC 6B
            *aiCellValue = 0;
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            *ayDstByte++ = *ayDataByte++; // 3
            *ayDstByte++ = *ayDataByte++; // 4
            *ayDstByte++ = *ayDataByte++; // 5
            ayDstByte += C_2B;
//            *ayDstByte++ = 0; // 6
//            *ayDstByte++ = 0; // 7
            break;
            
          case 0x7: // MC_NUMERIC 7B
            //*aiCellValue = 0;
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            *ayDstByte++ = *ayDataByte++; // 3
            *ayDstByte++ = *ayDataByte++; // 4
            *ayDstByte++ = *ayDataByte++; // 5
            *ayDstByte++ = *ayDataByte++; // 6
            *ayDstByte++ = 0; // 7
            break;
            
          case 0x8: // MC_NUMERIC 8B
            *aiCellValue = *(u8*)(ayDataByte);
//            *ayDstByte++ = *ayDataByte++; // 0
//            *ayDstByte++ = *ayDataByte++; // 1
//            *ayDstByte++ = *ayDataByte++; // 2
//            *ayDstByte++ = *ayDataByte++; // 3
//            *ayDstByte++ = *ayDataByte++; // 4
//            *ayDstByte++ = *ayDataByte++; // 5
//            *ayDstByte++ = *ayDataByte++; // 6
//            *ayDstByte++ = *ayDataByte++; // 7
            ayDstByte += C_8B;
            ayDataByte += C_8B;
            break;
            
          case 0x9: // MC_NUMERIC 9B
            RASSERT(0);
            break;
            
          case 0xA: // MC_NUMERIC 10B
            RASSERT(0);
            break;
            
          case 0xB: // MC_NUMERIC 11B
            RASSERT(0);
            break;
            
          case 0xC: // MC_NUMERIC 12B
            RASSERT(0);
            break;
            
          case 0xD: // MC_NUMERIC 13B
            RASSERT(0);
            break;
            
          case 0xE: // MC_NUMERIC 14B
            RASSERT(0);
            break;
            
          case 0xF: // MC_NUMERIC 15B
            RASSERT(0);
            break;
            
        }
        break;
        
      case MC_DATE_E0: // date era-0 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        *aiCellValue = 0;
        *ayDstByte++ = *ayDataByte++; // 0
        *ayDstByte++ = *ayDataByte++; // 1
        ayDstByte += 6;
//        *ayDstByte++ = 0; // 2
//        *ayDstByte++ = 0; // 3
//        *ayDstByte++ = 0; // 4
//        *ayDstByte++ = 0; // 5
//        *ayDstByte++ = 0; // 6
//        *ayDstByte++ = 0; // 7
        
        //vDteFromEdt(aiCellValue,C_ERA_0);
        aEdt = (Edt*)aiCellValue;
        aDte = (Dte*)aiCellValue;
        
        aDte->m_uYearDelta = aEdt->m_uYearInEra + C_ERA_0;
        aDte->m_uMonth     = aEdt->m_uMonth;
        aDte->m_uDay       = aEdt->m_uDay;
        break;
        
      case MC_DATE_E1: // date era-1 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        *aiCellValue = 0;
        *ayDstByte++ = *ayDataByte++; // 0
        *ayDstByte++ = *ayDataByte++; // 1
        ayDstByte += 6;
//        *ayDstByte++ = 0; // 2
//        *ayDstByte++ = 0; // 3
//        *ayDstByte++ = 0; // 4
//        *ayDstByte++ = 0; // 5
//        *ayDstByte++ = 0; // 6
//        *ayDstByte++ = 0; // 7
        
        //vDteFromEdt(aiCellValue,C_ERA_1);
        aEdt = (Edt*)aiCellValue;
        aDte = (Dte*)aiCellValue;
        
        aDte->m_uYearDelta = aEdt->m_uYearInEra + C_ERA_1;
        aDte->m_uMonth     = aEdt->m_uMonth;
        aDte->m_uDay       = aEdt->m_uDay;
        break;
        
      case MC_DATE_E2: // date era-2 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        *aiCellValue = 0;
        *ayDstByte++ = *ayDataByte++; // 0
        *ayDstByte++ = *ayDataByte++; // 1
        ayDstByte += 6;
//        *ayDstByte++ = 0; // 2
//        *ayDstByte++ = 0; // 3
//        *ayDstByte++ = 0; // 4
//        *ayDstByte++ = 0; // 5
//        *ayDstByte++ = 0; // 6
//        *ayDstByte++ = 0; // 7
        
        //vDteFromEdt(aiCellValue,C_ERA_2);
        aEdt = (Edt*)aiCellValue;
        aDte = (Dte*)aiCellValue;
        
        aDte->m_uYearDelta = aEdt->m_uYearInEra + C_ERA_2;
        aDte->m_uMonth     = aEdt->m_uMonth;
        aDte->m_uDay       = aEdt->m_uDay;
        break;
        
      case MC_STRING_x1: // short string 0..15B (no null) - -- -- -- -- -- -- --
        nStrSize = STRx1(uLow);
        
        if (SI_CELLARRAY_ADR == eStrInflateRule) {
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = (char*)ayDataByte;
          ayDstByte += 8;
          
          ayDataByte += nStrSize;
        }
        else
        if (SI_STRAREA_OFS == eStrInflateRule) {
          // copy into str area, advance data point ptr
          memcpy(racStrArea,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;

          i8 i_Ofs = - nDifference(racStrArea,ayDstByte);
          aiCellValue = (i8*)ayDstByte;
          *aiCellValue = i_Ofs;
          ayDstByte += 8;

          racStrArea += nStrSize;
        }
        else
        if (SI_STRAREA_ADR == eStrInflateRule) {
          memcpy(acStrArea,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;
          
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = acStrArea;
          ayDstByte += 8;
          acStrArea += nStrSize;
        }
        else
        if (SI_CELLARRAY_PST == eStrInflateRule) {
          memcpy(ayDstByte,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;
          
          //vTrace("SI_CELLARRAY_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
          
          ayDstByte += (((nStrSize) / 8) * 8) + 16;
          aiCellValue = (i8*)ayDstByte;
          *aiCellValue = nStrSize - 1;
          ayDstByte += 8;
        }
        else {
          RASSERT(0);
        }
        
        break;
        
      case MC_NULL: // null - -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
        *(i8*)ayDstByte = C_DATUM_NULL;
        ayDstByte += C_BYTES_PER_WORD;
        break;
        
      case MC_SPARE0: // spare
        RASSERT(0);
        break;
        
      case MC_OVERFLOW: // overflow 2B
        
        // calculation to get overflow
        //aOverLine = aHeadLine->aGetOver();
        nSizeUnits = aHeadLine->w8_1B(C_HEADLINE_OFS_WORDS);
        if (nSizeUnits >= 128) nSizeUnits -= 128;
        nHeadBytes = nSizeUnits * C_BYTES_PER_WORD;
        i_Over_rel = aHeadLine->w8_2B(nHeadBytes - C_2B);
        
        aOverLine = RowNub::aGetOver_rel(aHeadLine,i_Over_rel);
        
        METACODE_SPLIT( (* aAdd<u1*>(aOverLine,C_U_1B) ), uHigh, uLow);
        ayDataByte = aAdd<byte*>(aOverLine,C_U_1B + C_M_1B);
        goto L_HighSwitch;

        break;
        
        
        // end where uLow is n-bytes
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // toBytes(uLow) depends on uHigh
        
      case MC_STRING_x2: // string x 2 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrSize = STRx2(uLow);
        
        if (SI_CELLARRAY_ADR == eStrInflateRule) {
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = (char*)ayDataByte;
          ayDstByte += 8;
          
          ayDataByte += nStrSize;
        }
        else
        if (SI_STRAREA_OFS == eStrInflateRule) {
          // copy into str area, advance data point ptr
          memcpy(racStrArea,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;

          i8 i_Ofs = - nDifference(racStrArea,ayDstByte);
          aiCellValue = (i8*)ayDstByte;
          *aiCellValue = i_Ofs;
          ayDstByte += 8;

          racStrArea += nStrSize;
        }
        else
        if (SI_STRAREA_ADR == eStrInflateRule) {
          
          memcpy(acStrArea,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;
          
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = acStrArea;
          ayDstByte += 8;
          acStrArea += nStrSize;
        }
        else
        if (SI_CELLARRAY_PST == eStrInflateRule) {
          azDst = (char*)ayDstByte;
          memcpy(azDst,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;
          
          //vTrace("SI_CELLARRAY_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
          
          ayDstByte += (((nStrSize) / 8) * 8) + 8;
          aiCellValue = (i8*)ayDstByte;
          *aiCellValue = nStrSize - 1;
          ayDstByte += 8;
        }
        else {
          RASSERT(0);
        }
        
        break;
        
      case MC_STRING_x4: // string x 4 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrSize = STRx4(uLow);
        
        if (SI_CELLARRAY_ADR == eStrInflateRule) {
          aacCellValue = (char**)ayDataByte;
          *aacCellValue = (char*)ayDataByte;
          ayDstByte += 8;
          
          ayDataByte += nStrSize;
        }
        else
        if (SI_STRAREA_OFS == eStrInflateRule) {
          // copy into str area, advance data point ptr
          memcpy(racStrArea,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;

          i8 i_Ofs = - nDifference(racStrArea,ayDstByte);
          aiCellValue = (i8*)ayDstByte;
          *aiCellValue = i_Ofs;
          ayDstByte += 8;

          racStrArea += nStrSize;
        }
        else
        if (SI_STRAREA_ADR == eStrInflateRule) {
          
          memcpy(acStrArea,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;
          
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = acStrArea;
          ayDstByte += 8;
          acStrArea += nStrSize;
        }
        else
        if (SI_CELLARRAY_PST == eStrInflateRule) {
          memcpy(ayDstByte,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;
          
          //vTrace("SI_CELLARRAY_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
          
          ayDstByte += (((nStrSize) / 8) * 8) + 8;
          aiCellValue = (i8*)ayDstByte;
          *aiCellValue = nStrSize - 1;
          ayDstByte += 8;
        }
        else {
          RASSERT(0);
        }
        
        break;
        
      case MC_STRING_x8: // string x 8 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrSize = STRx8(uLow);
        
        if (SI_CELLARRAY_ADR == eStrInflateRule) {
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = (char*)ayDataByte;
          ayDstByte += 8;
          
          ayDataByte += nStrSize;
        }
        else
        if (SI_STRAREA_OFS == eStrInflateRule) {
          // copy into str area, advance data point ptr
          memcpy(racStrArea,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;

          i8 i_Ofs = - nDifference(racStrArea,ayDstByte);
          aiCellValue = (i8*)ayDstByte;
          *aiCellValue = i_Ofs;
          ayDstByte += 8;

          racStrArea += nStrSize;
        }
        else
        if (SI_STRAREA_ADR == eStrInflateRule) {
          
          memcpy(acStrArea,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;
          
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = acStrArea;
          ayDstByte += 8;
          acStrArea += nStrSize;
        }
        else
        if (SI_CELLARRAY_PST == eStrInflateRule) {
          memcpy(ayDstByte,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;
          
          //vTrace("SI_CELLARRAY_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
          
          ayDstByte += (((nStrSize) / 8) * 8) + 8;
          aiCellValue = (i8*)ayDstByte;
          *aiCellValue = nStrSize - 1;
          ayDstByte += 8;
        }
        else {
          RASSERT(0);
        }
        
        break;
        
      case MC_STRING_TAIL: // string in tail -- -- -- -- -- -- -- -- -- -- -- --
        u_Tail_rel = *(u2*)ayDataByte;
        aTailLine = aHeadLine->aGetTailRel(u_Tail_rel);
        
        nStrSize = aTailLine->w8_1B(+ C_U_1B);
        if (SI_STRAREA_ADR == eStrInflateRule) {
          
          memcpy(acStrArea,( (char*)aTailLine + C_U_1B + C_L_1B ),(size_t)nStrSize);
          
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = acStrArea;
          ayDstByte += 8;
          ayDataByte += 2;
          acStrArea += nStrSize;
        }
        else {
          RASSERT(0);
        }
        
        break;
        
      case MC_BLOB_0: // blob type 0 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        RASSERT(0);
        break;
        
      case MC_BLOB_1: // blob type 1 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        RASSERT(0);
        break;
        
      case MC_NEGATIVE: // -ve digit -15..-0 -- -- -- -- -- -- -- -- -- -- -- --
        aiCellValue = (i8*)ayDstByte;
        
        *ayDstByte++ = (i8)uLow; // 0
        *ayDstByte++ = 0; // 1
        *ayDstByte++ = 0; // 2
        *ayDstByte++ = 0; // 3
        *ayDstByte++ = 0; // 4
        *ayDstByte++ = 0; // 5
        *ayDstByte++ = 0; // 6
        *ayDstByte++ = 0; // 7
        
        *aiCellValue = - *aiCellValue;
        
        break;
        
      case MC_POSITIVE: // +ve digit 0..15 - -- -- -- -- -- -- -- -- -- -- -- --
        *ayDstByte++ = (i8)uLow; // 0
        *ayDstByte++ = 0; // 1
        *ayDstByte++ = 0; // 2
        *ayDstByte++ = 0; // 3
        *ayDstByte++ = 0; // 4
        *ayDstByte++ = 0; // 5
        *ayDstByte++ = 0; // 6
        *ayDstByte++ = 0; // 7
        break;
        
      default:
        NEVER_REACH;
    }
    ///////////////////////////////////////////////////////////////////////////////

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move to next column to retrieve
    
//    if (++i_WhichCol >= nWhichCols)
//      break;
    
    i_ColToRetrieve = iaColSet[++i_WhichCol];
    if (-1 == i_ColToRetrieve)
      break;
  }
  
  // end for each column
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // update callers aiDstk

  aiDstk_lcl = (i8*)ayDstByte;
  
  *aaCellArray = --aiDstk_lcl;

}

/*----------------------------------------------------------------------------*/
/* inflate headline into row                                                  */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflate_OneColumn_into_RowSingle(RowSingle* pRow,
                                 HeadLine* aHeadLine,
                                 i8 i_ColToRetrieve)
{
  NEVER_REACH;
}


/*----------------------------------------------------------------------------*/
/* inflate headline into row                                                  */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflate_AllColumns_into_RowSingle(RowSingle* pRowSingle,
                                                     HeadLine* aHeadLine)
{
  _static_
  
  //vTrace("vInflate_AllColumns_into_RowSingle\n"_Stz);
  
  u1* auMetaCode;
  byte* ayDataByte;

  i8 nCols = pRowSingle->m_nColumns;

  vPrepare_v2(auMetaCode,ayDataByte,nCols,aHeadLine);

  byte* ayCellArray = pRowSingle->ayCellArray();
    

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
#ifdef WITH_INFLATE_TRACE
    Column* pCol = pRow->m_pColVec->pGet(i_Col);
#endif
    
    u1 uMetaCode = *auMetaCode++;
    u1 uHigh = uMetaCode >> 4;
    u1 uLow  = uMetaCode & 0x0F;

    char* azStrArea = (char*)pRowSingle->szStzAlottable();

    vInflate_CodedValue_into_Cell(ayCellArray,azStrArea,SI_STRAREA_OFS,
                                  aHeadLine,ayDataByte,uHigh,uLow);
    
    if (azStrArea != (char*)pRowSingle->szStzAlottable()) {
      i8 nBytesTaken = nDifference(azStrArea,pRowSingle->szStzAlottable());
      pRowSingle->vStzBytesTaken(nBytesTaken);
    }
    
  }
  // for each column
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
}

/*----------------------------------------------------------------------------*/
/* inflate headline into row                                                  */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflate_AllColumns_into_RowSlab(RowSlab* pRowSlab,
                                  HeadLine* aHeadLine)
{
  NEVER_REACH;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
