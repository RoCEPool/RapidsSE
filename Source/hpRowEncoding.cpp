/*============================================================================*/
/* hpRowEncoding.cpp - Row sprint helper                                      */
/*============================================================================*/

#include "Data.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
hpRowEncoding::hpRowEncoding(void)
: Sprint(CN_ROW)
{
  _precondition_(_MAGIC_,CN_ROW,_FOOTER_);
  
  // set default enclosing char
  vFormat(F_VCH_enclose);

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caRow,sizeof(m_caRow),CN_ROW);
#else
  m_caRow[0] = '\0';
#endif
  
  _checkInitialised_(CN_ROW);
  
  _postcondition_(_MAGIC_,CN_ROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
hpRowEncoding::~hpRowEncoding(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
void hpRowEncoding::vRowEncoding(RowEncoding* pEncRow)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_caRow[0] = '\0';
  
  char caLine[256];

  for (i8 i_Col = 0; i_Col < pEncRow->m_nColumns; i_Col++) {
  
    char* szCrs = &caLine[0];
    
    Column* pCol = pEncRow->m_pColVec->pGet(i_Col);
    eDM eDatum   = pCol->eDatumType();
    Stz* szDatum = (Stz*)aDM[eDatum];
    
    StrLib::vAppendString(&szCrs,(char*)" "_Stz);

    StrLib::vAppendPad(&szCrs,caLine,2);
    StrLib::vAppendChar(&szCrs,'[');
    StrLib::vAppendDecimal(&szCrs,i_Col);
    StrLib::vAppendChar(&szCrs,pEncRow->xEncode[i_Col].m_bSplit ? '*' : ' ');
    StrLib::vAppendDecimal(&szCrs,pEncRow->xEncode[i_Col].m_nOffset);
    StrLib::vAppendChar(&szCrs,']');

    StrLib::vAppendPad(&szCrs,caLine,10);
    StrLib::vAppendString(&szCrs,(char*)pCol->szColNm());
    
    StrLib::vAppendPad(&szCrs,caLine,30);
    StrLib::vAppendString(&szCrs,(char*)szDatum);
    
    StrLib::vAppendPad(&szCrs,caLine,35);
    StrLib::vAppendString(&szCrs,(char*)aMC[pEncRow->xEncode[i_Col].m_eMetaCode]);
    
    StrLib::vAppendPad(&szCrs,caLine,50);
    StrLib::vAppendString(&szCrs,(char*)"C_"_Stz);
    StrLib::vAppendDecimal(&szCrs,pEncRow->xEncode[i_Col].m_nDataSize);
    StrLib::vAppendString(&szCrs,(char*)"B"_Stz);
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    
    StrLib::vAppendPad(&szCrs,caLine,58);
    
   if (C_DATUM_NULL == pEncRow->xEncode[i_Col].m_wDataCode.m_iInt) {
      StrLib::vAppendString(&szCrs,(char*)"null"_Stz);
      continue;
    }
      
    switch (eDatum) {
      case DM_BCD: {
        spBCD spBcd;
        StrLib::vAppendString(&szCrs,
          (char*)spBcd.sz(pEncRow->xEncode[i_Col].m_wDataCode.m_iBcd));
        break;
      }

      case DM_BOL: {
        spBOL spBol;
        StrLib::vAppendString(&szCrs,
          (char*)spBol.sz(pEncRow->xEncode[i_Col].m_wDataCode.m_iBol));
        break;
      }
        
      case DM_DTE: {
        spDTE spDte;
        StrLib::vAppendString(&szCrs,
          (char*)spDte.sz(pEncRow->xEncode[i_Col].m_wDataCode.m_iDte));
        break;
      }
        
      case DM_FLT: {
        spFLT spFlt;
        StrLib::vAppendString(&szCrs,
          (char*)spFlt.sz(*(d8*)&pEncRow->xEncode[i_Col].m_wDataCode.m_iInt));
        break;
      }
        
      case DM_INT: {
        spINT spInt;
        StrLib::vAppendString(&szCrs,
          (char*)spInt.sz(pEncRow->xEncode[i_Col].m_wDataCode.m_iInt));
        break;
      }
        
      case DM_IVL: {
        spIVL spIvl;
        StrLib::vAppendString(&szCrs,
          (char*)spIvl.sz(pEncRow->xEncode[i_Col].m_wDataCode.m_iIvl));
        break;
      }
        
      case DM_TME: {
        spTME spTme;
        StrLib::vAppendString(&szCrs,
          (char*)spTme.sz(pEncRow->xEncode[i_Col].m_wDataCode.m_iTme));
        break;
      }
        
      case DM_TSP: {
        spTSP spTsp;
        StrLib::vAppendString(&szCrs,
          (char*)spTsp.sz(pEncRow->xEncode[i_Col].m_wDataCode.m_iTsp));
        break;
      }
        
      case DM_VCH: {
        spVCH spVch;
        StrLib::vAppendString(&szCrs,
          (char*)spVch.sz((char*)pEncRow->xEncode[i_Col].m_wDataCode.m_iInt));
        break;
      }
        
      default:
        NEVER_REACH;
    }

    StrLib::vAppendString(&szCrs,(char*)"\n"_Stz);
    
    StrLib::vConcat(m_caRow,caLine);
    
  }
  
}

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
Stz* hpRowEncoding::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caRow[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* hpRowEncoding::sz(RowEncoding* pRowEncoding)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vRowEncoding(pRowEncoding);
  
  return (Stz*) &m_caRow[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* hpRowEncoding::aFooter(void)
{
  _precondition_(_MAGIC_,CN_ROW);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
