/*============================================================================*/
/* SP_Encoding_v2.cpp - Row Encoding_v2 sprint helper                         */
/*============================================================================*/

#include "CompactBuffers.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Encoding_v2::SP_Encoding_v2(void)
: Sprint(CN_ROW)
{
  _precondition_(_MAGIC_,CN_ROW,_FOOTER_);
  
  // set default enclosing char
  vFormat(F_VCH_enclose);

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caEncoding,sizeof(m_caEncoding),CN_ROW);
#else
  m_caEncoding[0] = '\0';
#endif
  
  _checkInitialised_(CN_ROW);
  
  _postcondition_(_MAGIC_,CN_ROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Encoding_v2::~SP_Encoding_v2(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
void SP_Encoding_v2::vEncoding(Encoding_v2* pEncoding)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_caEncoding[0] = '\0';
  StrLib::vSprint(m_caEncoding,
                  "HeadSize %lldB <W=%lld> MetaSize %lldB "
                  "BytesNeeded %lldB ShiftOver %lldB\n"_Stz,
                  pEncoding->m_nHeadSize,
                  MemLib::nQuantaRequired(pEncoding->m_nHeadSize,C_BYTES_PER_WORD),
                  pEncoding->m_nMetaSize,
                  pEncoding->m_nBytesNeeded,
                  pEncoding->m_nShiftOver
                  );

  char caLine[C_MAX_RECORD];
  for (i8 i_Col = 0; i_Col < pEncoding->m_nColumns; i_Col++) {
  
    char* szCrs = &caLine[0];
    
    _ColEnc_v2* aColEnc = & pEncoding->xaColEnc[i_Col];
    
    Column* pCol = pEncoding->m_pStreamDef->pGetColumn(i_Col);
    eDM eDatum   = pCol->eDatum();
    Stz* szDatum = (Stz*)aDM[eDatum];
    
    StrLib::vAppendString(&szCrs,(char*)" "_Stz);

    StrLib::vAppendPad(&szCrs,caLine,2);
    StrLib::vAppendChar(&szCrs,'[');
    StrLib::vAppendDecimal(&szCrs,i_Col);
    StrLib::vAppendChar(&szCrs,']');
    StrLib::vAppendChar(&szCrs,aColEnc->m_bSplit ? '*' : ' ');

    StrLib::vAppendPad(&szCrs,caLine,8);
    StrLib::vAppendString(&szCrs,(char*)pCol->szColNm());
    
    StrLib::vAppendPad(&szCrs,caLine,25);
    StrLib::vAppendString(&szCrs,(char*)szDatum);
    
    StrLib::vAppendPad(&szCrs,caLine,30);
    StrLib::vAppendString(&szCrs,(char*)aMC[aColEnc->m_eMetaCode]);
    
    StrLib::vAppendPad(&szCrs,caLine,45);
    StrLib::vAppendHex(&szCrs,aColEnc->m_uMetaClue);

    StrLib::vAppendPad(&szCrs,caLine,55);
    StrLib::vAppendString(&szCrs,(char*)"C_"_Stz);
    StrLib::vAppendDecimal(&szCrs,aColEnc->m_nDataSize);
    StrLib::vAppendString(&szCrs,(char*)"B"_Stz);

    StrLib::vAppendPad(&szCrs,caLine,62);
    
    if (pEncoding->xaColEnc[i_Col].m_i_Till) {
      StrLib::vAppendDecimal(&szCrs,aColEnc->m_i_Offset);
      StrLib::vAppendString(&szCrs,(char*)".."_Stz);
      StrLib::vAppendDecimal(&szCrs,aColEnc->m_i_Till);
    }
    
    StrLib::vAppendPad(&szCrs,caLine,70);
    StrLib::vAppendDecimal(&szCrs,aColEnc->m_i_Tail);

    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    
    StrLib::vAppendPad(&szCrs,caLine,77);
    
    if (aColEnc->m_wDataCode.bIsNull()) {
      StrLib::vAppendString(&szCrs,(char*)C_NULL);
    }
    else {
      switch (eDatum) {
        case DM_BCD: {
          SP_Bcd SP_Bcd;
          StrLib::vAppendString(&szCrs,
                                (char*)SP_Bcd.sz(aColEnc->m_wDataCode));
          break;
        }

        case DM_BOL: {
          SP_Bol SP_Bol;
          StrLib::vAppendString(&szCrs,
                                (char*)SP_Bol.sz(aColEnc->m_wDataCode));
          break;
        }
          
        case DM_DTE: {
          SP_Dte SP_Dte;
          StrLib::vAppendString(&szCrs,
                                (char*)SP_Dte.sz(aColEnc->m_wDataCode));
          break;
        }
          
        case DM_FLT: {
          SP_Flt SP_Flt;
          StrLib::vAppendString(&szCrs,
                                (char*)SP_Flt.sz(*(d8*)&aColEnc->m_wDataCode));
          break;
        }
          
        case DM_INT: {
          SP_Int spInt;
          StrLib::vAppendString(&szCrs,
                                (char*)spInt.sz(aColEnc->m_wDataCode));
          break;
        }
          
        case DM_IVL: {
          SP_Ivl spIvl;
          StrLib::vAppendString(&szCrs,
                                (char*)spIvl.sz(aColEnc->m_wDataCode));
          break;
        }
          
        case DM_TME: {
          SP_Tme SP_Tme;
          StrLib::vAppendString(&szCrs,
                                (char*)SP_Tme.sz(aColEnc->m_wDataCode));
          break;
        }
          
        case DM_TSP: {
          SP_Tsp SP_Tsp;
          StrLib::vAppendString(&szCrs,
                                (char*)SP_Tsp.sz(aColEnc->m_wDataCode));
          break;
        }
          
        case DM_VCH: {
          SP_Vch SP_Vch;
          if (MC_STR_FIT_1L == aColEnc->m_eMetaCode ||
              MC_STR_FIT_2L == aColEnc->m_eMetaCode) {
            StrLib::vAppendString(&szCrs,
                                  aColEnc->m_wDataCode.m_azStr);
          }
          else if (MC_STR_EMPTY == aColEnc->m_eMetaCode) {
            StrLib::vAppendString(&szCrs,(char*)""_Stz);
          }
          else {
            StrLib::vAppendString(&szCrs,
                                  (char*)aColEnc->m_aTailLine->sz());
          }
          break;
        }
          
        default:
          NEVER_REACH;
      }
    }
    
    StrLib::vAppendString(&szCrs,(char*)"\n"_Stz);
    
    StrLib::vConcat(m_caEncoding,caLine);
  }
}

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
Stz* SP_Encoding_v2::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caEncoding[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Encoding_v2::sz(Encoding_v2* pEncoding)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vEncoding(pEncoding);
  
  return (Stz*) &m_caEncoding[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SP_Encoding_v2::aFooter(void)
{
  _precondition_(_MAGIC_,CN_ROW);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
