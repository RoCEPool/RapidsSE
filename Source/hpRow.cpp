/*============================================================================*/
/* hpRow.cpp - Row sprint helper                                              */
/*============================================================================*/

#include "Transform.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
hpRow::hpRow(void)
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
hpRow::~hpRow(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
void hpRow::vRow(Row* pRow)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  char* szCrs = &m_caRow[0];
  
  for (i8 i_Col = 0; i_Col < pRow->m_nColumns; i_Col++) {
  
    Column* pCol = pRow->m_pColVec->pGet(i_Col);
    eDM eDatum   = pCol->eDatumType();
    Stz* szDatum = (Stz*)aDM[eDatum];

    StrLib::vAppendString(&szCrs,(char*)"  "_Stz);

    StrLib::vAppendString(&szCrs,(char*)pCol->szColNm());
    StrLib::vAppendString(&szCrs,(char*)" "_Stz);
    StrLib::vAppendString(&szCrs,(char*)szDatum);
    StrLib::vAppendString(&szCrs,(char*)" "_Stz);

    switch (eDatum) {
      case DM_BCD: {
        spBCD spBcd;
        StrLib::vAppendString(&szCrs,(char*)spBcd.sz(pRow->m_aiValue[i_Col]));
        break;
      }

      case DM_BOL: {
        spBOL spBol;
        StrLib::vAppendString(&szCrs,(char*)spBol.sz(pRow->m_aiValue[i_Col]));
        break;
      }
        
      case DM_DTE: {
        spDTE spDte;
        StrLib::vAppendString(&szCrs,(char*)spDte.sz(pRow->m_aiValue[i_Col]));
        break;
      }
        
      case DM_FLT: {
        spFLT spFlt;
        StrLib::vAppendString(&szCrs,(char*)spFlt.sz(*(d8*)&pRow->m_aiValue[i_Col]));
        break;
      }
        
      case DM_INT: {
        spINT spInt;
        StrLib::vAppendString(&szCrs,(char*)spInt.sz(pRow->m_aiValue[i_Col]));
        break;
      }
        
      case DM_IVL: {
        spIVL spIvl;
        StrLib::vAppendString(&szCrs,(char*)spIvl.sz(pRow->m_aiValue[i_Col]));
        break;
      }
        
      case DM_TME: {
        spTME spTme;
        StrLib::vAppendString(&szCrs,(char*)spTme.sz(pRow->m_aiValue[i_Col]));
        break;
      }
        
      case DM_TSP: {
        spTSP spTsp;
        StrLib::vAppendString(&szCrs,(char*)spTsp.sz(pRow->m_aiValue[i_Col]));
        break;
      }
        
      case DM_VCH: {
        spVCH spVch;
        StrLib::vAppendString(&szCrs,(char*)spVch.sz((char*)pRow->m_aiValue[i_Col]));
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
Stz* hpRow::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caRow[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* hpRow::sz(Row* pRow)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vRow(pRow);
  
  return (Stz*) &m_caRow[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* hpRow::aFooter(void)
{
  _precondition_(_MAGIC_,CN_ROW);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
