/*============================================================================*/
/* RowInflator.hpp -- Row Inflator Mark V                                     */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  class RowInflator : private RowCoder
  {
  public:
    //! NOTE: KEEP BOTH INFLATES UP-TO-DATE
    //! Any change in any one of the functions needs to be made in the others.
    //! Kept separate for performance reasons
    //! Highly performance-critical functions

  
    static void vInflateSet(i8** aaiDstk, HeadLine* aHeadLine,
                                StreamDef* pStreamDef,
                                eSI eStrInflate);
    static void vInflateColumn(RowSingle* pRow, HeadLine* aHeadLine,
                               i8 i_ColToRetrieve);
    static void vInflateRow(RowSingle* pRow, HeadLine* aHeadLine);
    static void vInflateRow(RowSlab* pRsbSlab, HeadLine* aHeadLine);

  private:
  
    // v1 meta code
    static void vInflateSet_v1(i8** aaiDstk, HeadLine* aHeadLine,
                                StreamDef* pStreamDef);
    static void vInflateColumn_v1(RowSingle* pRow, HeadLine* aHeadLine,
                                  i8 i_ColToRetrieve);
    static void vInflateRow_v1(RowSingle* pRow, HeadLine* aHeadLine);
    static void vInflateRow_v1(RowSlab* pRsbSlab, HeadLine* aHeadLine);
  
  
    // v2 meta clue
  
//    static void vInflate_v2(byte*& rayDstByte,
//                            byte*& rayDataByte, HeadLine* aHeadLine,
//                            u1 uHigh, u1 uLow,
//                            eSI eStrInflate, char*& racString);
  
    //static void vSkip_v2(u1 uHigh, u1 uLow, byte*& rayDataByte,
    //                     HeadLine* aHeadLine);

//    static void vPrepare_v2(u1*& rauMetaClue, byte*& rayDataByte,
//                            i8 nCols, HeadLine* aHeadLine);

    static void vInflateSet_v2(i8** aaiDstk, HeadLine* aHeadLine,
                               StreamDef* pStreamDef,
                               eSI eStrInflate);
    static void vInflateColumn_v2(RowSingle* pRow, HeadLine* aHeadLine,
                                  i8 i_ColToRetrieve);
    static void vInflateRow_v2(RowSingle* pRow, HeadLine* aHeadLine);
    static void vInflateRow_v2(RowSlab* pRsbSlab, HeadLine* aHeadLine);

  
  /*----------------------------------------------------------------------------*/
  /* general v2 skip code                                                       */
  /*----------------------------------------------------------------------------*/
  inline static void vSkip_v2(u1 uHigh, u1 uLow, byte*& rayDataByte, HeadLine* aHeadLine)
  {
    _static_

    L_Overflowed:
    
    if (uHigh <=  MN_STRING_x1) // numeric & date
      rayDataByte += uLow;
    else
    if (uHigh >=  MN_NEGATIVE)  // small value
      {;}
    else
    if (uHigh == MN_STRING_x1)  // string x 1
      rayDataByte += STRx1(uLow);
    else
    if (uHigh == MN_STRING_x2)  // string x 2
      rayDataByte += STRx2(uLow);
    else
    if (uHigh == MN_STRING_x4)  // string x 4
      rayDataByte += STRx4(uLow);
    else
    if (uHigh == MN_STRING_x8)  // string x 8
      rayDataByte += STRx8(uLow);
    else
    if (uHigh == MN_OVERFLOW) { // overflow
      OverLine* aOverLine = aHeadLine->aGetOver();
      u1 uMetaClue = *((u1*)aOverLine + C_U_1B);
      METACLUE_SPLIT(uMetaClue,uHigh,uLow);
      rayDataByte = (byte*)aOverLine + C_U_1B + C_M_1B;
      goto L_Overflowed;
    }
    else {
      RASSERT(0);
    }

  }

  /*----------------------------------------------------------------------------*/
  /* general v2 inflate code                                                    */
  /*----------------------------------------------------------------------------*/
  inline static void vInflate_v2(byte*& rayDstByte,
                                byte*& rayDataByte, HeadLine* aHeadLine,
                                u1 uHigh, u1 uLow,
                                eSI eStrInflate, char*& racStrArea)
  {
    _static_
    
    i8 nStrSize;
    i8* aiCellValue;
    char** aacCellValue;
    OverLine* aOverLine;
    u2 u_Tail_rel;
    TailLine* aTailLine;

    L_HighSwitch:
    
    switch (uHigh) {
        
      case MN_NUMERIC: // numeric values 0..8B
        
        switch (uLow) {
            
          case 0x0: // MN_NUMERIC 0B
            *rayDstByte++ = 0; // 0
            *rayDstByte++ = 0; // 1
            *rayDstByte++ = 0; // 2
            *rayDstByte++ = 0; // 3
            *rayDstByte++ = 0; // 4
            *rayDstByte++ = 0; // 5
            *rayDstByte++ = 0; // 6
            *rayDstByte++ = 0; // 7
            break;
            
          case 0x1: // MN_NUMERIC 1B
            *rayDstByte++ = *rayDataByte++; // 0
            *rayDstByte++ = 0; // 1
            *rayDstByte++ = 0; // 2
            *rayDstByte++ = 0; // 3
            *rayDstByte++ = 0; // 4
            *rayDstByte++ = 0; // 5
            *rayDstByte++ = 0; // 6
            *rayDstByte++ = 0; // 7
            break;
            
          case 0x2: // MN_NUMERIC 2B
            *rayDstByte++ = *rayDataByte++; // 0
            *rayDstByte++ = *rayDataByte++; // 1
            *rayDstByte++ = 0; // 2
            *rayDstByte++ = 0; // 3
            *rayDstByte++ = 0; // 4
            *rayDstByte++ = 0; // 5
            *rayDstByte++ = 0; // 6
            *rayDstByte++ = 0; // 7
            break;
            
          case 0x3: // MN_NUMERIC 3B
            *rayDstByte++ = *rayDataByte++; // 0
            *rayDstByte++ = *rayDataByte++; // 1
            *rayDstByte++ = *rayDataByte++; // 2
            *rayDstByte++ = 0; // 3
            *rayDstByte++ = 0; // 4
            *rayDstByte++ = 0; // 5
            *rayDstByte++ = 0; // 6
            *rayDstByte++ = 0; // 7
            break;
            
          case 0x4: // MN_NUMERIC 4B
            *rayDstByte++ = *rayDataByte++; // 0
            *rayDstByte++ = *rayDataByte++; // 1
            *rayDstByte++ = *rayDataByte++; // 2
            *rayDstByte++ = *rayDataByte++; // 3
            *rayDstByte++ = 0; // 4
            *rayDstByte++ = 0; // 5
            *rayDstByte++ = 0; // 6
            *rayDstByte++ = 0; // 7
            break;
            
          case 0x5: // MN_NUMERIC 5B
            *rayDstByte++ = *rayDataByte++; // 0
            *rayDstByte++ = *rayDataByte++; // 1
            *rayDstByte++ = *rayDataByte++; // 2
            *rayDstByte++ = *rayDataByte++; // 3
            *rayDstByte++ = *rayDataByte++; // 4
            *rayDstByte++ = 0; // 5
            *rayDstByte++ = 0; // 6
            *rayDstByte++ = 0; // 7
            break;
            
          case 0x6: // MN_NUMERIC 6B
            *rayDstByte++ = *rayDataByte++; // 0
            *rayDstByte++ = *rayDataByte++; // 1
            *rayDstByte++ = *rayDataByte++; // 2
            *rayDstByte++ = *rayDataByte++; // 3
            *rayDstByte++ = *rayDataByte++; // 4
            *rayDstByte++ = *rayDataByte++; // 5
            *rayDstByte++ = 0; // 6
            *rayDstByte++ = 0; // 7
            break;
            
          case 0x7: // MN_NUMERIC 7B
            *rayDstByte++ = *rayDataByte++; // 0
            *rayDstByte++ = *rayDataByte++; // 1
            *rayDstByte++ = *rayDataByte++; // 2
            *rayDstByte++ = *rayDataByte++; // 3
            *rayDstByte++ = *rayDataByte++; // 4
            *rayDstByte++ = *rayDataByte++; // 5
            *rayDstByte++ = *rayDataByte++; // 6
            *rayDstByte++ = 0; // 7
            break;
            
          case 0x8: // MN_NUMERIC 8B
            *rayDstByte++ = *rayDataByte++; // 0
            *rayDstByte++ = *rayDataByte++; // 1
            *rayDstByte++ = *rayDataByte++; // 2
            *rayDstByte++ = *rayDataByte++; // 3
            *rayDstByte++ = *rayDataByte++; // 4
            *rayDstByte++ = *rayDataByte++; // 5
            *rayDstByte++ = *rayDataByte++; // 6
            *rayDstByte++ = *rayDataByte++; // 7
          break;
            
          case 0x9: // MN_NUMERIC 9B
            RASSERT(0);
            break;
            
          case 0xA: // MN_NUMERIC 10B
            RASSERT(0);
            break;
            
          case 0xB: // MN_NUMERIC 11B
            RASSERT(0);
            break;
            
          case 0xC: // MN_NUMERIC 12B
            RASSERT(0);
            break;
            
          case 0xD: // MN_NUMERIC 13B
            RASSERT(0);
            break;
            
          case 0xE: // MN_NUMERIC 14B
            RASSERT(0);
            break;
            
          case 0xF: // MN_NUMERIC 15B
            RASSERT(0);
            break;
          
        }
        break;
        
      case MN_DATE_E0: // date era-0 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        aiCellValue = (i8*)rayDstByte;
        
        *rayDstByte++ = *rayDataByte++; // 0
        *rayDstByte++ = *rayDataByte++; // 1
        *rayDstByte++ = 0; // 2
        *rayDstByte++ = 0; // 3
        *rayDstByte++ = 0; // 4
        *rayDstByte++ = 0; // 5
        *rayDstByte++ = 0; // 6
        *rayDstByte++ = 0; // 7
        
        vDteFromEdt(aiCellValue,C_ERA_0);
        break;
        
      case MN_DATE_E1: // date era-1 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        aiCellValue = (i8*)rayDstByte;
        
        *rayDstByte++ = *rayDataByte++; // 0
        *rayDstByte++ = *rayDataByte++; // 1
        *rayDstByte++ = 0; // 2
        *rayDstByte++ = 0; // 3
        *rayDstByte++ = 0; // 4
        *rayDstByte++ = 0; // 5
        *rayDstByte++ = 0; // 6
        *rayDstByte++ = 0; // 7
        
        vDteFromEdt(aiCellValue,C_ERA_1);
        break;
        
      case MN_DATE_E2: // date era-2 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        aiCellValue = (i8*)rayDstByte;
        
        *rayDstByte++ = *rayDataByte++; // 0
        *rayDstByte++ = *rayDataByte++; // 1
        *rayDstByte++ = 0; // 2
        *rayDstByte++ = 0; // 3
        *rayDstByte++ = 0; // 4
        *rayDstByte++ = 0; // 5
        *rayDstByte++ = 0; // 6
        *rayDstByte++ = 0; // 7
        
        vDteFromEdt(aiCellValue,C_ERA_2);
        break;
        
      case MN_STRING_x1: // short string 0..15B (no null) - -- -- -- -- -- -- --
        nStrSize = STRx1(uLow);
        
        if (SI_AREA_ADR == eStrInflate) {
          memcpy(racStrArea,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          aacCellValue = (char**)rayDstByte;
          *aacCellValue = racStrArea;
          rayDstByte += 8;
          racStrArea += nStrSize;
        }
        else
        if (SI_DEST_PST == eStrInflate) {
          memcpy(rayDstByte,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          //vTrace("SI_DEST_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
          
          rayDstByte += (((nStrSize) / 8) * 8) + 16;
          aiCellValue = (i8*)rayDstByte;
          *aiCellValue = nStrSize - 1;
          rayDstByte += 8;
        }
        else {
          RASSERT(0);
        }
        
        break;
        
      case MN_NULL: // null - -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
        *(i8*)rayDstByte = C_DATUM_NULL;
        rayDstByte += C_BYTES_PER_WORD;
        break;
        
      case MN_SPARE0: // spare
        RASSERT(0);
        break;
        
      case MN_OVERFLOW: // overflow 2B
        aOverLine = aHeadLine->aGetOver();
        METACLUE_SPLIT( (* aAdd<u1*>(aOverLine,C_U_1B) ), uHigh, uLow);
        rayDataByte = aAdd<byte*>(aOverLine,C_U_1B + C_M_1B);
        goto L_HighSwitch;
        break;
        
        
      // end where uLow is n-bytes
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // toBytes(uLow) depends on uHigh
        
      case MN_STRING_x2: // string x 2 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrSize = STRx2(uLow);
        
        if (SI_AREA_ADR == eStrInflate) {

          memcpy(racStrArea,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          aacCellValue = (char**)rayDstByte;
          *aacCellValue = racStrArea;
          rayDstByte += 8;
          racStrArea += nStrSize;
        }
        else
        if (SI_DEST_PST == eStrInflate) {
          char* azDst = (char*)rayDstByte;
          memcpy(azDst,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          //vTrace("SI_DEST_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
          
          rayDstByte += (((nStrSize) / 8) * 8) + 8;
          aiCellValue = (i8*)rayDstByte;
          *aiCellValue = nStrSize - 1;
          rayDstByte += 8;
        }
        else {
          RASSERT(0);
        }

        break;
        
      case MN_STRING_x4: // string x 4 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrSize = STRx4(uLow);
        
        if (SI_AREA_ADR == eStrInflate) {

          memcpy(racStrArea,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          aacCellValue = (char**)rayDstByte;
          *aacCellValue = racStrArea;
          rayDstByte += 8;
          racStrArea += nStrSize;
        }
        else
        if (SI_DEST_PST == eStrInflate) {
          memcpy(rayDstByte,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          //vTrace("SI_DEST_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
          
          rayDstByte += (((nStrSize) / 8) * 8) + 8;
          aiCellValue = (i8*)rayDstByte;
          *aiCellValue = nStrSize - 1;
          rayDstByte += 8;
        }
        else {
          RASSERT(0);
        }
        
        break;
        
      case MN_STRING_x8: // string x 8 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrSize = STRx8(uLow);
        if (SI_AREA_ADR == eStrInflate) {

          memcpy(racStrArea,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          aacCellValue = (char**)rayDstByte;
          *aacCellValue = racStrArea;
          rayDstByte += 8;
          racStrArea += nStrSize;
        }
        else
        if (SI_DEST_PST == eStrInflate) {
          memcpy(rayDstByte,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          //vTrace("SI_DEST_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
          
          rayDstByte += (((nStrSize) / 8) * 8) + 8;
          aiCellValue = (i8*)rayDstByte;
          *aiCellValue = nStrSize - 1;
          rayDstByte += 8;
        }
        else {
          RASSERT(0);
        }
        
        break;
        
      case MN_STRING_TAIL: // string in tail -- -- -- -- -- -- -- -- -- -- -- --
        u_Tail_rel = *(u2*)rayDataByte;
        aTailLine = aHeadLine->aGetTailRel(u_Tail_rel);
        
        nStrSize = aTailLine->w8_1B(+ C_U_1B);
        if (SI_AREA_ADR == eStrInflate) {
          
          memcpy(racStrArea,( (char*)aTailLine + C_U_1B + C_L_1B ),(size_t)nStrSize);

          aacCellValue = (char**)rayDstByte;
          *aacCellValue = racStrArea;
          rayDstByte += 8;
          rayDataByte += 2;
          racStrArea += nStrSize;
        }
        else {
          RASSERT(0);
        }
        
        break;
        
      case MN_BLOB_0: // blob type 0 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        RASSERT(0);
        break;
        
      case MN_BLOB_1: // blob type 1 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        RASSERT(0);
        break;
        
      case MN_NEGATIVE: // -ve digit -15..-0 -- -- -- -- -- -- -- -- -- -- -- --
        aiCellValue = (i8*)rayDstByte;
        
        *rayDstByte++ = (i8)uLow; // 0
        *rayDstByte++ = 0; // 1
        *rayDstByte++ = 0; // 2
        *rayDstByte++ = 0; // 3
        *rayDstByte++ = 0; // 4
        *rayDstByte++ = 0; // 5
        *rayDstByte++ = 0; // 6
        *rayDstByte++ = 0; // 7
        
        *aiCellValue = - *aiCellValue;

        break;
        
      case MN_POSITIVE: // +ve digit 0..15 - -- -- -- -- -- -- -- -- -- -- -- --
        *rayDstByte++ = (i8)uLow; // 0
        *rayDstByte++ = 0; // 1
        *rayDstByte++ = 0; // 2
        *rayDstByte++ = 0; // 3
        *rayDstByte++ = 0; // 4
        *rayDstByte++ = 0; // 5
        *rayDstByte++ = 0; // 6
        *rayDstByte++ = 0; // 7
        break;
        
      default:
        NEVER_REACH;
    }

  }


  /*----------------------------------------------------------------------------*/
  /* prepare                                                                    */
  /*----------------------------------------------------------------------------*/
  static inline void vPrepare_v2(u1*& rauMetaClue, byte*& rayDataByte,
                                i8 nCols, HeadLine* aHeadLine)
  {
    _static_
    
    rauMetaClue = aHeadLine->auMetaArea();
    
    i8 i_BaseOffsetData = C_W_1B   // (ControlDate) 1 byte for the headline size (in words)
                          + C_T_8B // (ControlDate) 8 bytes for the perspective
                          + nCols; // metaSize

    rayDataByte = aAdd<byte*>(aHeadLine,i_BaseOffsetData);
  }



  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
