/*============================================================================*/
/* RowInflator.hpp -- Compact Row Inflator                                    */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  class RowInflator : private RowCoder
  {
  public:
  
    static void vInflate_AllColumns_into_RowSingle(RowSingle* pRowSingle,
                                                   HeadLine* aHeadLine);

    static void vInflate_AllColumns_into_RowBuffer(RowBuffer* pRowBuffer,
                                                   HeadLine* aHeadLine);

    static void vInflate_OneColumn_into_RowSingle(RowSingle* pRowSingle,
                                                 HeadLine* aHeadLine,
                                                 i8 i_ColToRetrieve);

    static void vInflate_ColumnSet_into_CellArray(i8** aaCellArray,
                                                 char*& racStrArea,
                                                 eSI eStrInflateRule,
                                                 HeadLine* aHeadLine,
                                                 i8* iaColSet,
                                                 StreamDef* pStreamDef);
  
  
  /*----------------------------------------------------------------------------*/
  /* general v2 inflate code                                                    */
  /*----------------------------------------------------------------------------*/
  inline static void vInflate_CodedValue_into_Cell(byte*& rayDstByte,
                                                   char*& racStrArea,
                                                   eSI eStrInflateRule,
                                                   HeadLine* aHeadLine,
                                                   byte*& rayDataByte,
                                                   u1 uHigh,
                                                   u1 uLow)
  {
    _static_
    
    //vTrace("vInflate_CodedValue_into_Cell\n"_Stz);
  
    i8 nStrSize;
    i8* aiCellValue;
    char** aacCellValue;
    OverLine* aOverLine;
    u2 u_Tail_rel;
    TailLine* aTailLine;

    L_HighSwitch:
    
    switch (uHigh) {
        
      case MC_NUMERIC: // numeric values 0..8B
        
        switch (uLow) {
            
          case 0x0: // MC_NUMERIC 0B
            *rayDstByte++ = 0; // 0
            *rayDstByte++ = 0; // 1
            *rayDstByte++ = 0; // 2
            *rayDstByte++ = 0; // 3
            *rayDstByte++ = 0; // 4
            *rayDstByte++ = 0; // 5
            *rayDstByte++ = 0; // 6
            *rayDstByte++ = 0; // 7
            break;
            
          case 0x1: // MC_NUMERIC 1B
            *rayDstByte++ = *rayDataByte++; // 0
            *rayDstByte++ = 0; // 1
            *rayDstByte++ = 0; // 2
            *rayDstByte++ = 0; // 3
            *rayDstByte++ = 0; // 4
            *rayDstByte++ = 0; // 5
            *rayDstByte++ = 0; // 6
            *rayDstByte++ = 0; // 7
            break;
            
          case 0x2: // MC_NUMERIC 2B
            *rayDstByte++ = *rayDataByte++; // 0
            *rayDstByte++ = *rayDataByte++; // 1
            *rayDstByte++ = 0; // 2
            *rayDstByte++ = 0; // 3
            *rayDstByte++ = 0; // 4
            *rayDstByte++ = 0; // 5
            *rayDstByte++ = 0; // 6
            *rayDstByte++ = 0; // 7
            break;
            
          case 0x3: // MC_NUMERIC 3B
            *rayDstByte++ = *rayDataByte++; // 0
            *rayDstByte++ = *rayDataByte++; // 1
            *rayDstByte++ = *rayDataByte++; // 2
            *rayDstByte++ = 0; // 3
            *rayDstByte++ = 0; // 4
            *rayDstByte++ = 0; // 5
            *rayDstByte++ = 0; // 6
            *rayDstByte++ = 0; // 7
            break;
            
          case 0x4: // MC_NUMERIC 4B
            *rayDstByte++ = *rayDataByte++; // 0
            *rayDstByte++ = *rayDataByte++; // 1
            *rayDstByte++ = *rayDataByte++; // 2
            *rayDstByte++ = *rayDataByte++; // 3
            *rayDstByte++ = 0; // 4
            *rayDstByte++ = 0; // 5
            *rayDstByte++ = 0; // 6
            *rayDstByte++ = 0; // 7
            break;
            
          case 0x5: // MC_NUMERIC 5B
            *rayDstByte++ = *rayDataByte++; // 0
            *rayDstByte++ = *rayDataByte++; // 1
            *rayDstByte++ = *rayDataByte++; // 2
            *rayDstByte++ = *rayDataByte++; // 3
            *rayDstByte++ = *rayDataByte++; // 4
            *rayDstByte++ = 0; // 5
            *rayDstByte++ = 0; // 6
            *rayDstByte++ = 0; // 7
            break;
            
          case 0x6: // MC_NUMERIC 6B
            *rayDstByte++ = *rayDataByte++; // 0
            *rayDstByte++ = *rayDataByte++; // 1
            *rayDstByte++ = *rayDataByte++; // 2
            *rayDstByte++ = *rayDataByte++; // 3
            *rayDstByte++ = *rayDataByte++; // 4
            *rayDstByte++ = *rayDataByte++; // 5
            *rayDstByte++ = 0; // 6
            *rayDstByte++ = 0; // 7
            break;
            
          case 0x7: // MC_NUMERIC 7B
            *rayDstByte++ = *rayDataByte++; // 0
            *rayDstByte++ = *rayDataByte++; // 1
            *rayDstByte++ = *rayDataByte++; // 2
            *rayDstByte++ = *rayDataByte++; // 3
            *rayDstByte++ = *rayDataByte++; // 4
            *rayDstByte++ = *rayDataByte++; // 5
            *rayDstByte++ = *rayDataByte++; // 6
            *rayDstByte++ = 0; // 7
            break;
            
          case 0x8: // MC_NUMERIC 8B
            *rayDstByte++ = *rayDataByte++; // 0
            *rayDstByte++ = *rayDataByte++; // 1
            *rayDstByte++ = *rayDataByte++; // 2
            *rayDstByte++ = *rayDataByte++; // 3
            *rayDstByte++ = *rayDataByte++; // 4
            *rayDstByte++ = *rayDataByte++; // 5
            *rayDstByte++ = *rayDataByte++; // 6
            *rayDstByte++ = *rayDataByte++; // 7
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
        
      case MC_DATE_E1: // date era-1 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
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
        
      case MC_DATE_E2: // date era-2 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
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
        
      case MC_STRING_x1: // short string 0..15B (no null) - -- -- -- -- -- -- --
        nStrSize = STRx1(uLow);
        
        if (SI_CELLARRAY_ADR == eStrInflateRule) {
          aacCellValue = (char**)rayDstByte;
          *aacCellValue = (char*)rayDataByte;
          rayDstByte += 8;

          rayDataByte += nStrSize;
        }
        else
        if (SI_STRAREA_OFS == eStrInflateRule) {
          // copy into str area, advance data point ptr
          memcpy(racStrArea,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          i8 i_Ofs = - nDifference(racStrArea,rayDstByte);
          aiCellValue = (i8*)rayDstByte;
          *aiCellValue = i_Ofs;
          rayDstByte += 8;

          racStrArea += nStrSize;
        }
        else
        if (SI_STRAREA_ADR == eStrInflateRule) {
          // copy into str area, advance data point ptr
          memcpy(racStrArea,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          aacCellValue = (char**)rayDstByte;
          *aacCellValue = racStrArea;
          rayDstByte += 8;
          
          racStrArea += nStrSize;
        }
        else
        if (SI_CELLARRAY_PST == eStrInflateRule) {
          memcpy(rayDstByte,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          //vTrace("SI_CELLARRAY_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
          
          rayDstByte += (((nStrSize) / 8) * 8) + 16;
          aiCellValue = (i8*)rayDstByte;
          *aiCellValue = nStrSize - 1;
          rayDstByte += 8;
        }
        else {
          RASSERT(0);
        }
        
        break;
        
      case MC_NULL: // null - -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
        *(i8*)rayDstByte = C_DATUM_NULL;
        rayDstByte += C_BYTES_PER_WORD;
        break;
        
      case MC_SPARE0: // spare
        RASSERT(0);
        break;
        
      case MC_OVERFLOW: // overflow 2B
        aOverLine = aHeadLine->aGetOver();
        METACODE_SPLIT( (* aAdd<u1*>(aOverLine,C_U_1B) ), uHigh, uLow);
        rayDataByte = aAdd<byte*>(aOverLine,C_U_1B + C_M_1B);
        goto L_HighSwitch;
        break;
        
        
      // end where uLow is n-bytes
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // toBytes(uLow) depends on uHigh
        
      case MC_STRING_x2: // string x 2 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrSize = STRx2(uLow);
        
        if (SI_STRAREA_OFS == eStrInflateRule) {
          // copy into str area, advance data point ptr
          memcpy(racStrArea,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          i8 i_Ofs = - nDifference(racStrArea,rayDstByte);
          aiCellValue = (i8*)rayDstByte;
          *aiCellValue = i_Ofs;
          rayDstByte += 8;

          racStrArea += nStrSize;
        }
        else
        if (SI_CELLARRAY_ADR == eStrInflateRule) {
          aacCellValue = (char**)rayDstByte;
          *aacCellValue = (char*)rayDataByte;
          rayDstByte += 8;

          rayDataByte += nStrSize;
        }
        else
        if (SI_STRAREA_ADR == eStrInflateRule) {

          memcpy(racStrArea,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          aacCellValue = (char**)rayDstByte;
          *aacCellValue = racStrArea;
          rayDstByte += 8;
          racStrArea += nStrSize;
        }
        else
        if (SI_CELLARRAY_PST == eStrInflateRule) {
          char* azDst = (char*)rayDstByte;
          memcpy(azDst,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          //vTrace("SI_CELLARRAY_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
          
          rayDstByte += (((nStrSize) / 8) * 8) + 16;
          aiCellValue = (i8*)rayDstByte;
          *aiCellValue = nStrSize - 1;
          rayDstByte += 8;
        }
        else {
          RASSERT(0);
        }

        break;
        
      case MC_STRING_x4: // string x 4 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrSize = STRx4(uLow);
        
        if (SI_STRAREA_OFS == eStrInflateRule) {
          // copy into str area, advance data point ptr
          memcpy(racStrArea,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          i8 i_Ofs = - nDifference(racStrArea,rayDstByte);
          aiCellValue = (i8*)rayDstByte;
          *aiCellValue = i_Ofs;
          rayDstByte += 8;

          racStrArea += nStrSize;
        }
        else
        if (SI_CELLARRAY_ADR == eStrInflateRule) {
          aacCellValue = (char**)rayDataByte;
          *aacCellValue = (char*)rayDataByte;
          rayDstByte += 8;

          rayDataByte += nStrSize;
        }
        else
        if (SI_STRAREA_ADR == eStrInflateRule) {

          memcpy(racStrArea,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          aacCellValue = (char**)rayDstByte;
          *aacCellValue = racStrArea;
          rayDstByte += 8;
          racStrArea += nStrSize;
        }
        else
        if (SI_CELLARRAY_PST == eStrInflateRule) {
          memcpy(rayDstByte,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          //vTrace("SI_CELLARRAY_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
          
          rayDstByte += (((nStrSize) / 8) * 8) + 16;
          aiCellValue = (i8*)rayDstByte;
          *aiCellValue = nStrSize - 1;
          rayDstByte += 8;
        }
        else {
          RASSERT(0);
        }
        
        break;
        
      case MC_STRING_x8: // string x 8 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrSize = STRx8(uLow);
        
        if (SI_STRAREA_OFS == eStrInflateRule) {
          // copy into str area, advance data point ptr
          memcpy(racStrArea,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          i8 i_Ofs = - nDifference(racStrArea,rayDstByte);
          aiCellValue = (i8*)rayDstByte;
          *aiCellValue = i_Ofs;
          rayDstByte += 8;

          racStrArea += nStrSize;
        }
        else
        if (SI_CELLARRAY_ADR == eStrInflateRule) {
          aacCellValue = (char**)rayDstByte;
          *aacCellValue = (char*)rayDataByte;
          rayDstByte += 8;

          rayDataByte += nStrSize;
        }
        else
        if (SI_STRAREA_ADR == eStrInflateRule) {

          memcpy(racStrArea,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          aacCellValue = (char**)rayDstByte;
          *aacCellValue = racStrArea;
          rayDstByte += 8;
          racStrArea += nStrSize;
        }
        else
        if (SI_CELLARRAY_PST == eStrInflateRule) {
          memcpy(rayDstByte,rayDataByte,(size_t)nStrSize);
          rayDataByte += nStrSize;

          //vTrace("SI_CELLARRAY_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
          
          rayDstByte += (((nStrSize) / 8) * 8) + 16;
          aiCellValue = (i8*)rayDstByte;
          *aiCellValue = nStrSize - 1;
          rayDstByte += 8;
        }
        else {
          RASSERT(0);
        }
        
        break;
        
      case MC_STRING_TAIL: // string in tail -- -- -- -- -- -- -- -- -- -- -- --
        u_Tail_rel = *(u2*)rayDataByte;
        aTailLine = aHeadLine->aGetTailRel(u_Tail_rel);
        
        nStrSize = (i8)aTailLine->w8_1B(+ C_U_1B);
        
        if (SI_STRAREA_OFS == eStrInflateRule) {
          memcpy(racStrArea,( (char*)aTailLine + C_U_1B + C_L_1B ),(size_t)nStrSize);

          i8 i_Ofs = - nDifference(racStrArea,rayDstByte);
          aiCellValue = (i8*)rayDstByte;
          *aiCellValue = i_Ofs;

          rayDstByte += 8;
          rayDataByte += 2;
          racStrArea += nStrSize;
        }
        else
        if (SI_STRAREA_ADR == eStrInflateRule) {
          
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
        
      case MC_BLOB_0: // blob type 0 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        RASSERT(0);
        break;
        
      case MC_BLOB_1: // blob type 1 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        RASSERT(0);
        break;
        
      case MC_NEGATIVE: // -ve digit -15..-0 -- -- -- -- -- -- -- -- -- -- -- --
        aiCellValue = (i8*)rayDstByte;
        
        *rayDstByte++ = uLow; // 0
        *rayDstByte++ = 0; // 1
        *rayDstByte++ = 0; // 2
        *rayDstByte++ = 0; // 3
        *rayDstByte++ = 0; // 4
        *rayDstByte++ = 0; // 5
        *rayDstByte++ = 0; // 6
        *rayDstByte++ = 0; // 7
        
        *aiCellValue = - *aiCellValue;

        break;
        
      case MC_POSITIVE: // +ve digit 0..15 - -- -- -- -- -- -- -- -- -- -- -- --
        *rayDstByte++ = uLow; // 0
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
  static inline void vPrepare_v2(u1*& rauMetaCode, byte*& rayDataByte,
                                i8 nCols, HeadLine* aHeadLine)
  {
    _static_
    
    rauMetaCode = aHeadLine->auMetaArea();
    
    i8 i_BaseOffsetData = C_W_1B   // (ControlDate) 1 byte for the headline size (in words)
                          + C_T_8B // (ControlDate) 8 bytes for the perspective
                          + nCols; // metaSize

    rayDataByte = aAdd<byte*>(aHeadLine,i_BaseOffsetData);
  }



  
  
  /*----------------------------------------------------------------------------*/
  /* inflate column set                                                         */
  /*----------------------------------------------------------------------------*/
  public:
  static inline void vInflate_Key_into_ElementArray(i8** aaElementArray,
                                                    HeadLine* aHeadLine,
                                                    i8* iaColSet,
                                                    StreamDef* pStreamDef,
                                                    IndexDef* pIndexDef,
                                                    i8 ieStrInflateRule)
  {
    _static_
    
    //vTrace("vInflate_Key_into_ElementArray\n"_Stz);
  

  i8 nCols = pStreamDef->nTotalColumns();
  
  u1* auMetaCode = aHeadLine->auMetaArea();
  
  i8 i_BaseOffsetData = C_W_1B   // (ControlDate) 1 byte for the headline size (in words)
                        + C_T_8B // (ControlDate) 8 bytes for the perspective
                        + nCols; // metaSize
  
  byte* ayDataByte = aAdd<byte*>(aHeadLine,i_BaseOffsetData);
  
  char* acStrArea = nullptr;
  
  i8* aiDstk_lcl = *aaElementArray;
  
  byte* ayDstByte = (byte*)++aiDstk_lcl;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
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
  
  Column* pIdxColumn;
  eDM     eDatum;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each column
  
  i8 i_ColSet = 0;
  i8 i_ColToRetrieve = iaColSet[i_ColSet];
  //RASSERT(-1 != i_ColToRetrieve);
  
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
      if (uHigh ==  MC_NULL)      // null value
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
        
        // calculation to get overflow
        nSizeUnits = (i8)aHeadLine->w8_1B(C_HEADLINE_OFS_WORDS);
        if (nSizeUnits >= 128) nSizeUnits -= 128;
        nHeadBytes = nSizeUnits * (i8)C_BYTES_PER_WORD;
        i_Over_rel = (i8)aHeadLine->w8_2B(nHeadBytes - C_2B);
        
        aOverLine = RowNub::aGetOver_rel(aHeadLine,i_Over_rel);
        
        uMetaCode = *((u1*)aOverLine + C_U_1B);
        METACODE_SPLIT(uMetaCode,uHigh,uLow);
        ayDataByte = (byte*)aOverLine + C_U_1B + C_M_1B;
        goto L_Overflowed;
      }
      else {
        vTrace("vInflate_ColumnSet_into_CellArray: Invalid uHigh=%lld\n"_Stz,(i8)uHigh);
        RASSERT(0);
      }
      
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inflate column
    
    // ////////////////////////////////////////////////////////////////////// //

  L_HighSwitch:
    
    aiCellValue = (i8*)ayDstByte;
    
    switch (uHigh) {
        
      case MC_NUMERIC: // numeric values 0..8B
        
        switch (uLow) {
            
          case 0x0: // MC_NUMERIC 0B
            *aiCellValue = 0;
            ayDstByte += C_8B;
            break;
            
          case 0x1: // MC_NUMERIC 1B
            *aiCellValue = 0;
            *ayDstByte++ = *ayDataByte++; // 0
            ayDstByte += C_7B;
            break;
            
          case 0x2: // MC_NUMERIC 2B
            *aiCellValue = *(u2*)(ayDataByte);
            ayDataByte += C_2B;
            ayDstByte += C_8B;
            break;
            
          case 0x3: // MC_NUMERIC 3B
            *aiCellValue = 0;
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            ayDstByte += C_5B;
            break;
            
          case 0x4: // MC_NUMERIC 4B
            *aiCellValue = *(u4*)(ayDataByte);
            ayDataByte += C_4B;
            ayDstByte += C_8B;
            break;
            
          case 0x5: // MC_NUMERIC 5B
            *aiCellValue = 0;
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            *ayDstByte++ = *ayDataByte++; // 3
            *ayDstByte++ = *ayDataByte++; // 4
            ayDstByte += C_3B;
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
            *aiCellValue = *(i8*)(ayDataByte);
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
        
        aEdt = (Edt*)aiCellValue;
        aDte = (Dte*)aiCellValue;
        
        aDte->m_uYearDelta = aEdt->m_uYearInEra + C_ERA_2;
        aDte->m_uMonth     = aEdt->m_uMonth;
        aDte->m_uDay       = aEdt->m_uDay;
        break;
        
      case MC_STRING_x1: // short string 0..15B (no null) - -- -- -- -- -- -- --
        //nStrSize = STRx1(uLow);
        
        if (SI_CELLARRAY_ADR == (eSI)ieStrInflateRule) {
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = (char*)ayDataByte;
          ayDstByte += 8;
          
          ayDataByte += uLow;
        }
        else
          if (SI_STRAREA_ADR == (eSI)ieStrInflateRule) {
          memcpy(acStrArea,ayDataByte,(size_t)uLow);
          ayDataByte += uLow;
          
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = acStrArea;
          ayDstByte += 8;
          acStrArea += uLow;
        }
        else
        if (SI_CELLARRAY_PST == (eSI)ieStrInflateRule) {
          memcpy(ayDstByte,ayDataByte,(size_t)uLow);
          ayDataByte += uLow;
          
          //vTrace("SI_CELLARRAY_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
          
          ayDstByte += (((uLow) / 8) * 8) + 16;
          aiCellValue = (i8*)ayDstByte;
          *aiCellValue = uLow - 1;
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
        nSizeUnits = (i8)aHeadLine->w8_1B(C_HEADLINE_OFS_WORDS);
        if (nSizeUnits >= 128) nSizeUnits -= 128;
        nHeadBytes = nSizeUnits * (i8)C_BYTES_PER_WORD;
        i_Over_rel = (i8)aHeadLine->w8_2B(nHeadBytes - C_2B);
        
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
        
        if (SI_CELLARRAY_ADR == (eSI)ieStrInflateRule) {
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = (char*)ayDataByte;
          ayDstByte += 8;
          
          ayDataByte += nStrSize;
        }
        else
        if (SI_STRAREA_ADR == (eSI)ieStrInflateRule) {
          
          memcpy(acStrArea,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;
          
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = acStrArea;
          ayDstByte += 8;
          acStrArea += nStrSize;
        }
        else
        if (SI_CELLARRAY_PST == (eSI)ieStrInflateRule) {
          azDst = (char*)ayDstByte;
          memcpy(azDst,ayDataByte,(size_t)nStrSize);
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
        
      case MC_STRING_x4: // string x 4 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrSize = STRx4(uLow);
        
        if (SI_CELLARRAY_ADR == (eSI)ieStrInflateRule) {
          aacCellValue = (char**)ayDataByte;
          *aacCellValue = (char*)ayDataByte;
          ayDstByte += 8;
          
          ayDataByte += nStrSize;
        }
        else
        if (SI_STRAREA_ADR == (eSI)ieStrInflateRule) {
          
          memcpy(acStrArea,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;
          
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = acStrArea;
          ayDstByte += 8;
          acStrArea += nStrSize;
        }
        else
        if (SI_CELLARRAY_PST == (eSI)ieStrInflateRule) {
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
        
      case MC_STRING_x8: // string x 8 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrSize = STRx8(uLow);
        
        if (SI_CELLARRAY_ADR == (eSI)ieStrInflateRule) {
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = (char*)ayDataByte;
          ayDstByte += 8;
          
          ayDataByte += nStrSize;
        }
        else
        if (SI_STRAREA_ADR == (eSI)ieStrInflateRule) {
          
          memcpy(acStrArea,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;
          
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = acStrArea;
          ayDstByte += 8;
          acStrArea += nStrSize;
        }
        else
        if (SI_CELLARRAY_PST == (eSI)ieStrInflateRule) {
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
        
      case MC_STRING_TAIL: // string in tail -- -- -- -- -- -- -- -- -- -- -- --
        u_Tail_rel = *(u2*)ayDataByte;
        aTailLine = aHeadLine->aGetTailRel(u_Tail_rel);
        
        nStrSize = (i8)aTailLine->w8_1B(+ C_U_1B);
        
        if (SI_CELLARRAY_ADR == (eSI)ieStrInflateRule) {
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = ((char*)aTailLine) + C_U_1B + C_L_1B;
          ayDstByte += 8;
          ayDataByte += 2;
        }
        else
        if (SI_STRAREA_ADR == (eSI)ieStrInflateRule) {
          
          memcpy(acStrArea,
                 ( (char*)aTailLine + C_U_1B + C_L_1B ),
                 (size_t)nStrSize);
          
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = acStrArea;
          ayDstByte += 8;
          ayDataByte += 2;
          acStrArea += nStrSize;
        }
        else {
          vTrace("ieStrInflateRule : %lld\n"_Stz,(i8)ieStrInflateRule);
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
        
        *ayDstByte++ = uLow; // 0
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
        *ayDstByte++ = uLow; // 0
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
    // ////////////////////////////////////////////////////////////////////// //
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // handle inflate into key
    
    if (! pIndexDef)
      goto L_NextColumn;
    
    pIdxColumn = pIndexDef->pGetColumn(i_ColSet);
    eDatum = pIdxColumn->eDatum();
    if (DM_VCH == eDatum) {
      RASSERT(0);
    }
    else {
      ASSERT(KE_INT == pIndexDef->eKeyKind());
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move to next column to retrieve
  L_NextColumn:
    
    i_ColToRetrieve = iaColSet[++i_ColSet];
    if (-1 == i_ColToRetrieve)
      break;
  }
  
  // end for each column
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // update callers aiDstk
  
  aiDstk_lcl = (i8*)ayDstByte;
  
  *aaElementArray = --aiDstk_lcl;

  }
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
