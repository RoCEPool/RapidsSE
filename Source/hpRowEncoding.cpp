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
  
  char* szCrs = &m_caRow[0];
  
  for (i8 i_Col = 0; i_Col < pEncRow->m_nColumns; i_Col++) {
  
    Column* pCol = pEncRow->m_pColVec->pGet(i_Col);
    eDM eDatum   = pCol->eDatumType();
    Stz* szDatum = (Stz*)aDM[eDatum];

    StrLib::vAppendString(&szCrs,(char*)"  "_Stz);

    StrLib::vAppendString(&szCrs,(char*)pCol->szColNm());
    StrLib::vAppendString(&szCrs,(char*)" "_Stz);
    StrLib::vAppendString(&szCrs,(char*)szDatum);
    StrLib::vAppendString(&szCrs,(char*)" "_Stz);
 
    StrLib::vAppendString(&szCrs,(char*)aMC[pEncRow->xEncode[i_Col].m_eEncoding]);
    StrLib::vAppendString(&szCrs,(char*)" "_Stz);
    
    StrLib::vAppendDecimal(&szCrs,pEncRow->xEncode[i_Col].m_nEncodedBytes);
    StrLib::vAppendString(&szCrs,(char*)" "_Stz);

    if (C_DATUM_NULL == pEncRow->m_waColVal[i_Col].m_iInt) {
      StrLib::vAppendString(&szCrs,(char*)"null"_Stz);
      return;
    }
      
    switch (eDatum) {
      case DM_BCD: {
        spBCD spBcd;
        StrLib::vAppendString(&szCrs,
                              (char*)spBcd.sz(pEncRow->m_waColVal[i_Col].m_iBcd));
        break;
      }

      case DM_BOL: {
        spBOL spBol;
        StrLib::vAppendString(&szCrs,
                              (char*)spBol.sz(pEncRow->m_waColVal[i_Col].m_iBol));
        break;
      }
        
      case DM_DTE: {
        spDTE spDte;
        StrLib::vAppendString(&szCrs,
                              (char*)spDte.sz(pEncRow->m_waColVal[i_Col].m_iDte));
        break;
      }
        
      case DM_FLT: {
        spFLT spFlt;
        StrLib::vAppendString(&szCrs,
                              (char*)spFlt.sz(*(d8*)&pEncRow->m_waColVal[i_Col].m_iInt));
        break;
      }
        
      case DM_INT: {
        spINT spInt;
        StrLib::vAppendString(&szCrs,
                              (char*)spInt.sz(pEncRow->m_waColVal[i_Col].m_iInt));
        break;
      }
        
      case DM_IVL: {
        spIVL spIvl;
        StrLib::vAppendString(&szCrs,
                              (char*)spIvl.sz(pEncRow->m_waColVal[i_Col].m_iIvl));
        break;
      }
        
      case DM_TME: {
        spTME spTme;
        StrLib::vAppendString(&szCrs,
                              (char*)spTme.sz(pEncRow->m_waColVal[i_Col].m_iTme));
        break;
      }
        
      case DM_TSP: {
        spTSP spTsp;
        StrLib::vAppendString(&szCrs,
                              (char*)spTsp.sz(pEncRow->m_waColVal[i_Col].m_iTsp));
        break;
      }
        
      case DM_VCH: {
        spVCH spVch;
        StrLib::vAppendString(&szCrs,
                              (char*)spVch.sz((char*)pEncRow->m_waColVal[i_Col].m_iInt));
        break;
      }
        
      default:
        NEVER_REACH;
    }

    StrLib::vAppendString(&szCrs,(char*)"\n"_Stz);
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
