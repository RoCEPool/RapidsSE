/*============================================================================*/
/* SP_Encoding.cpp - Row Encoding sprint helper                               */
/*============================================================================*/

#include "Data.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Encoding::SP_Encoding(void)
: Sprint(CN_ROW)
{
  _precondition_(_MAGIC_,CN_ROW,_FOOTER_);
  
  // set default enclosing char
  vFormat(F_VCH_enclose);

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caEncoding,sizeof(m_caEncoding),CN_ROW);
#else
  m_caRow[0] = '\0';
#endif
  
  _checkInitialised_(CN_ROW);
  
  _postcondition_(_MAGIC_,CN_ROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Encoding::~SP_Encoding(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
void SP_Encoding::vEncoding(Encoding* pEncoding)
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

  char caLine[256];
  for (i8 i_Col = 0; i_Col < pEncoding->m_nColumns; i_Col++) {
  
    char* szCrs = &caLine[0];
    
    Column* pCol = pEncoding->m_pColVec->pGet(i_Col);
    eDM eDatum   = pCol->eDatumType();
    Stz* szDatum = (Stz*)aDM[eDatum];
    
    StrLib::vAppendString(&szCrs,(char*)" "_Stz);

    StrLib::vAppendPad(&szCrs,caLine,2);
    StrLib::vAppendChar(&szCrs,'[');
    StrLib::vAppendDecimal(&szCrs,i_Col);
    //StrLib::vAppendDecimal(&szCrs,pEncoding->xaColEnc[i_Col].m_i_Offset);
    StrLib::vAppendChar(&szCrs,']');
    StrLib::vAppendChar(&szCrs,pEncoding->xaColEnc[i_Col].m_bOverflow ? '*' : ' ');

    StrLib::vAppendPad(&szCrs,caLine,8);
    StrLib::vAppendString(&szCrs,(char*)pCol->szColNm());
    
    StrLib::vAppendPad(&szCrs,caLine,25);
    StrLib::vAppendString(&szCrs,(char*)szDatum);
    
    StrLib::vAppendPad(&szCrs,caLine,30);
    StrLib::vAppendString(&szCrs,(char*)aMC[pEncoding->xaColEnc[i_Col].m_eMetaCode]);
    
    StrLib::vAppendPad(&szCrs,caLine,45);
    StrLib::vAppendString(&szCrs,(char*)"C_"_Stz);
    StrLib::vAppendDecimal(&szCrs,pEncoding->xaColEnc[i_Col].m_nDataSize);
    StrLib::vAppendString(&szCrs,(char*)"B"_Stz);

    StrLib::vAppendPad(&szCrs,caLine,52);
    
    if (pEncoding->xaColEnc[i_Col].m_i_Till) {
      StrLib::vAppendDecimal(&szCrs,pEncoding->xaColEnc[i_Col].m_i_Offset);
      StrLib::vAppendString(&szCrs,(char*)".."_Stz);
      StrLib::vAppendDecimal(&szCrs,pEncoding->xaColEnc[i_Col].m_i_Till);
    }
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    
    StrLib::vAppendPad(&szCrs,caLine,60);
    
   if (C_DATUM_NULL == pEncoding->xaColEnc[i_Col].m_wDataCode.m_iInt) {
      StrLib::vAppendString(&szCrs,(char*)"null"_Stz);
      continue;
    }
      
    switch (eDatum) {
      case DM_BCD: {
        SP_Bcd SP_Bcd;
        StrLib::vAppendString(&szCrs,
          (char*)SP_Bcd.sz(pEncoding->xaColEnc[i_Col].m_wDataCode.m_iBcd));
        break;
      }

      case DM_BOL: {
        SP_Bol SP_Bol;
        StrLib::vAppendString(&szCrs,
          (char*)SP_Bol.sz(pEncoding->xaColEnc[i_Col].m_wDataCode.m_iBol));
        break;
      }
        
      case DM_DTE: {
        SP_Dte SP_Dte;
        StrLib::vAppendString(&szCrs,
          (char*)SP_Dte.sz(pEncoding->xaColEnc[i_Col].m_wDataCode.m_iDte));
        break;
      }
        
      case DM_FLT: {
        SP_Flt SP_Flt;
        StrLib::vAppendString(&szCrs,
          (char*)SP_Flt.sz(*(d8*)&pEncoding->xaColEnc[i_Col].m_wDataCode.m_iInt));
        break;
      }
        
      case DM_INT: {
        SP_Int spInt;
        StrLib::vAppendString(&szCrs,
          (char*)spInt.sz(pEncoding->xaColEnc[i_Col].m_wDataCode.m_iInt));
        break;
      }
        
      case DM_IVL: {
        SP_Ivl spIvl;
        StrLib::vAppendString(&szCrs,
          (char*)spIvl.sz(pEncoding->xaColEnc[i_Col].m_wDataCode.m_iIvl));
        break;
      }
        
      case DM_TME: {
        SP_Tme SP_Tme;
        StrLib::vAppendString(&szCrs,
          (char*)SP_Tme.sz(pEncoding->xaColEnc[i_Col].m_wDataCode.m_iTme));
        break;
      }
        
      case DM_TSP: {
        SP_Tsp SP_Tsp;
        StrLib::vAppendString(&szCrs,
          (char*)SP_Tsp.sz(pEncoding->xaColEnc[i_Col].m_wDataCode.m_iTsp));
        break;
      }
        
      case DM_VCH: {
        SP_Vch SP_Vch;
        StrLib::vAppendString(&szCrs,
          (char*)SP_Vch.sz((char*)pEncoding->xaColEnc[i_Col].m_wDataCode.m_iInt));
        break;
      }
        
      default:
        NEVER_REACH;
    }

    StrLib::vAppendString(&szCrs,(char*)"\n"_Stz);
    
    StrLib::vConcat(m_caEncoding,caLine);
  }
}

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
Stz* SP_Encoding::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caEncoding[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Encoding::sz(Encoding* pEncoding)
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
Footer* SP_Encoding::aFooter(void)
{
  _precondition_(_MAGIC_,CN_ROW);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
