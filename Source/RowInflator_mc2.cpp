/*============================================================================*/
/* RowInflator_mc2.cpp -- Row Inflator (for Meta Code Version 2)              */
/*============================================================================*/

#include "CompactBuffers.h"

#define STRx1(__qty) (__qty)            //   0B ..  15B
#define STRx2(__qty) ( 16 + 2 * __qty)  //  16B ..  47B
#define STRx4(__qty) ( 48 + 4 * __qty)  //  48B .. 111B
#define STRx8(__qty) (112 + 8 * __qty)  // 112B .. 239B

/*----------------------------------------------------------------------------*/
/* inflate set onto Dtsk                                                      */
/*----------------------------------------------------------------------------*/
void RowInflator::vInflateSet_v2(i8** aaiDstk, HeadLine* aHeadLine,
                                   StreamDef* pStreamDef)
{
  eMC  eMetaCode;
  Nibbler  xMetaCode;
  SingleWord wWord;
  
  i8   i_Tail_rel;
  i8   nLength;

  TailLine* aTail;
    
  char* acString; // = nullptr;
  eMC* aeMetaCode = aHeadLine->aeMetaArea();
  
  i8 nCols = pStreamDef->nTotalColumns();
  
  i8 i_BaseOffsetData = C_W_1B   // (ControlDate) 1 byte for the headline size (in words)
                        + C_T_8B // (ControlDate) 8 bytes for the perspective
                        + nCols; // metaSize

  byte* ayDataByte = aAdd<byte*>(aHeadLine,i_BaseOffsetData);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the set of columns to be retrieved
  
  i8 nWhichCols = 0;
  i8 iWhichCol[C_MAX_COLUMNS];
  
  i8 i_ColToRetrieve;
  for (;;) {
    
    i_ColToRetrieve = *((*aaiDstk)--);
    
    if (-1 == i_ColToRetrieve)
      break;
    
    // must be in ascending order
    ASSERT(0 == nWhichCols ||
           i_ColToRetrieve > iWhichCol[nWhichCols-1]);
    
    iWhichCol[nWhichCols++] = i_ColToRetrieve;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  u1 uHigh;
  u1 uLow;
  i8 nStrLen;
  i8* aiCellValue;

  i8* aiDstk_lcl = *aaiDstk;
  
  byte* ayDstByte = (byte*)aiDstk_lcl;

  i8 i_WhichCol = 0;
  i_ColToRetrieve = iWhichCol[i_WhichCol];
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each column
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
        
    eMetaCode = *aeMetaCode++;
    
    uHigh = eMetaCode & 0xF0;
    uLow  = eMetaCode & 0x0F;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ignore column
    
    if (i_Col != i_ColToRetrieve) {

      if (uHigh <=  MN_NUMERIC)  // numeric
        ayDataByte += uLow;
      else
      if (uHigh >=  MN_NEGATIVE) // small value
        {;}
      else
      if (uHigh == MN_STRING_x2) // string x 2
        ayDataByte += STRx2(uLow);
      else
      if (uHigh == MN_STRING_x4) // string x 4
        ayDataByte += STRx4(uLow);
      else
      if (uHigh == MN_STRING_x8) // string x 8
        ayDataByte += STRx8(uLow);
      else {
        RASSERT(0);
      }
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inflate column
    
    switch (uHigh) {
        
      case MN_NUMERIC: // numeric values 0..8B
        
        switch (uLow) {
            
          case 0x0: // numeric 0B
            *ayDstByte++ = 0; // 0
            *ayDstByte++ = 0; // 1
            *ayDstByte++ = 0; // 2
            *ayDstByte++ = 0; // 3
            *ayDstByte++ = 0; // 4
            *ayDstByte++ = 0; // 5
            *ayDstByte++ = 0; // 6
            *ayDstByte++ = 0; // 7
            break;
            
          case 0x1: // numeric 1B
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = 0; // 1
            *ayDstByte++ = 0; // 2
            *ayDstByte++ = 0; // 3
            *ayDstByte++ = 0; // 4
            *ayDstByte++ = 0; // 5
            *ayDstByte++ = 0; // 6
            *ayDstByte++ = 0; // 7
            break;
            
          case 0x2: // numeric 2B
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = 0; // 2
            *ayDstByte++ = 0; // 3
            *ayDstByte++ = 0; // 4
            *ayDstByte++ = 0; // 5
            *ayDstByte++ = 0; // 6
            *ayDstByte++ = 0; // 7
            break;
            
          case 0x3: // numeric 3B
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            *ayDstByte++ = 0; // 3
            *ayDstByte++ = 0; // 4
            *ayDstByte++ = 0; // 5
            *ayDstByte++ = 0; // 6
            *ayDstByte++ = 0; // 7
            break;
            
          case 0x4: // numeric 4B
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            *ayDstByte++ = *ayDataByte++; // 3
            *ayDstByte++ = 0; // 4
            *ayDstByte++ = 0; // 5
            *ayDstByte++ = 0; // 6
            *ayDstByte++ = 0; // 7
            break;
            
          case 0x5: // numeric 5B
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            *ayDstByte++ = *ayDataByte++; // 3
            *ayDstByte++ = *ayDataByte++; // 4
            *ayDstByte++ = 0; // 5
            *ayDstByte++ = 0; // 6
            *ayDstByte++ = 0; // 7
            break;
            
          case 0x6: // numeric 6B
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            *ayDstByte++ = *ayDataByte++; // 3
            *ayDstByte++ = *ayDataByte++; // 4
            *ayDstByte++ = *ayDataByte++; // 5
            *ayDstByte++ = 0; // 6
            *ayDstByte++ = 0; // 7
            break;
            
          case 0x7: // numeric 7B
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            *ayDstByte++ = *ayDataByte++; // 3
            *ayDstByte++ = *ayDataByte++; // 4
            *ayDstByte++ = *ayDataByte++; // 5
            *ayDstByte++ = *ayDataByte++; // 6
            *ayDstByte++ = 0; // 7
            break;
            
          case 0x8: // numeric 8B
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            *ayDstByte++ = *ayDataByte++; // 3
            *ayDstByte++ = *ayDataByte++; // 4
            *ayDstByte++ = *ayDataByte++; // 5
            *ayDstByte++ = *ayDataByte++; // 6
            *ayDstByte++ = *ayDataByte++; // 7
          break;
            
          case 0x9: // numeric 9B
            RASSERT(0);
            break;
            
          case 0xA: // numeric 10B
            RASSERT(0);
            break;
            
          case 0xB: // numeric 11B
            RASSERT(0);
            break;
            
          case 0xC: // numeric 12B
            RASSERT(0);
            break;
            
          case 0xD: // numeric 13B
            RASSERT(0);
            break;
            
          case 0xE: // numeric 14B
            RASSERT(0);
            break;
            
          case 0xF: // numeric 15B
            RASSERT(0);
            break;
          
        }
        break;
        
      case MN_DATE_E0: // date era-0 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        aiCellValue = (i8*)ayDstByte;
        
        *ayDstByte++ = *ayDataByte++; // 0
        *ayDstByte++ = *ayDataByte++; // 1
        *ayDstByte++ = 0; // 2
        *ayDstByte++ = 0; // 3
        *ayDstByte++ = 0; // 4
        *ayDstByte++ = 0; // 5
        *ayDstByte++ = 0; // 6
        *ayDstByte++ = 0; // 7
        
        vDteFromEdt(aiCellValue,C_ERA_0);
        break;
        
      case MN_DATE_E1: // date era-1 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        aiCellValue = (i8*)ayDstByte;
        
        *ayDstByte++ = *ayDataByte++; // 0
        *ayDstByte++ = *ayDataByte++; // 1
        *ayDstByte++ = 0; // 2
        *ayDstByte++ = 0; // 3
        *ayDstByte++ = 0; // 4
        *ayDstByte++ = 0; // 5
        *ayDstByte++ = 0; // 6
        *ayDstByte++ = 0; // 7
        
        vDteFromEdt(aiCellValue,C_ERA_1);
        break;
        
      case MN_DATE_E2: // date era-2 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        aiCellValue = (i8*)ayDstByte;
        
        *ayDstByte++ = *ayDataByte++; // 0
        *ayDstByte++ = *ayDataByte++; // 1
        *ayDstByte++ = 0; // 2
        *ayDstByte++ = 0; // 3
        *ayDstByte++ = 0; // 4
        *ayDstByte++ = 0; // 5
        *ayDstByte++ = 0; // 6
        *ayDstByte++ = 0; // 7
        
        vDteFromEdt(aiCellValue,C_ERA_2);
        break;
        
      case MN_STRING_x1: // short string 0..15B (no null) - -- -- -- -- -- -- --
        nStrLen = STRx1(uLow);
        memcpy(ayDstByte,ayDataByte,(size_t)nStrLen);
        ayDstByte += nStrLen;
        *ayDstByte++ = '\0';
        break;
        
      case MN_NULL: // null - -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
        *(i8*)ayDstByte = C_DATUM_NULL;
        ayDstByte += C_BYTES_PER_WORD;
        break;
        
      case MN_SPARE0: // spare
        RASSERT(0);
        break;
        
      case MN_OVERFLOW: // overflow 2B
        break;
        
        
      // end where uLow is n-bytes
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // toBytes(uLow) depends on uHigh
        
      case MN_STRING_x2: // string x 2 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrLen = STRx2(uLow);
         memcpy(ayDstByte,ayDataByte,(size_t)nStrLen);
        ayDstByte += nStrLen;
        *ayDstByte++ = '\0';
        break;
        
      case MN_STRING_x4: // string x 4 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrLen = STRx4(uLow);
        memcpy(ayDstByte,ayDataByte,(size_t)nStrLen);
        ayDstByte += nStrLen;
        *ayDstByte++ = '\0';
        break;
        
      case MN_STRING_x8: // string x 8 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrLen = STRx8(uLow);
        memcpy(ayDstByte,ayDataByte,(size_t)nStrLen);
        ayDstByte += nStrLen;
        *ayDstByte++ = '\0';
        break;
        
      case MN_STRING_LEN: // length encoded string -- -- -- -- -- -- -- -- -- --
        RASSERT(0);
        break;
        
      case MN_BLOB_0: // blob type 0 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        RASSERT(0);
        break;
        
      case MN_BLOB_1: // blob type 1 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        RASSERT(0);
        break;
        
      case MN_NEGATIVE: // -ve digit -16..-1 -- -- -- -- -- -- -- -- -- -- -- --
        aiCellValue = (i8*)ayDstByte;
        
        *ayDstByte++ = (i1)uLow + 1; // 0
        *ayDstByte++ = 0; // 1
        *ayDstByte++ = 0; // 2
        *ayDstByte++ = 0; // 3
        *ayDstByte++ = 0; // 4
        *ayDstByte++ = 0; // 5
        *ayDstByte++ = 0; // 6
        *ayDstByte++ = 0; // 7
        
        *aiCellValue = - *aiCellValue;

        break;
        
      case MN_POSITIVE: // +ve digit 1..16 - -- -- -- -- -- -- -- -- -- -- -- --
        *ayDstByte++ = uLow + 1; // 0
        *ayDstByte++ = 0; // 1
        *ayDstByte++ = 0; // 2
        *ayDstByte++ = 0; // 3
        *ayDstByte++ = 0; // 4
        *ayDstByte++ = 0; // 5
        *ayDstByte++ = 0; // 6
        *ayDstByte++ = 0; // 7
        break;
    }
    
    // end inflate column
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    aiDstk_lcl++;
    
    // move to next column to retrieve
    if (++i_WhichCol >= nWhichCols) {
      *aaiDstk = aiDstk_lcl;
      return;
    }
    
    i_ColToRetrieve = iWhichCol[i_WhichCol];
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // end for each column
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
