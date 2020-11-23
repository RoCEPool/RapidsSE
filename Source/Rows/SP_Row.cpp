/*============================================================================*/
/* SP_Row.cpp - Row sprint helper                                             */
/*============================================================================*/

#include "Rows.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Row::SP_Row(void)
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
SP_Row::~SP_Row(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
void SP_Row::vRow(RowSingle* pRow)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  char* szCrs = &m_caRow[0];
  
  for (i8 i_Col = 0; i_Col < pRow->m_nColumns; i_Col++) {
  
    Column* pCol = pRow->m_pStreamDef->pGetColumn(i_Col);
    eDM eDatum   = pCol->eDatum();
    Stz* szDatum = (Stz*)aDM[eDatum];

    StrLib::vAppendString(&szCrs,(char*)"  "_Stz);

    StrLib::vAppendString(&szCrs,(char*)pCol->szColNm());
    StrLib::vAppendString(&szCrs,(char*)" "_Stz);
    StrLib::vAppendString(&szCrs,(char*)szDatum);
    StrLib::vAppendString(&szCrs,(char*)" "_Stz);
    
    if (pRow->m_waColVal[i_Col].bIsNull()) {
      StrLib::vAppendString(&szCrs,(char*)C_NULL);
      continue;
    }
      
    switch (eDatum) {
      case DM_BCD: {
        SP_Bcd SP_Bcd;
        StrLib::vAppendString(&szCrs,
                              (char*)SP_Bcd.sz(pRow->m_waColVal[i_Col]));
        break;
      }

      case DM_BOL: {
        SP_Bol SP_Bol;
        StrLib::vAppendString(&szCrs,
                              (char*)SP_Bol.sz(pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_DTE: {
        SP_Dte SP_Dte;
        StrLib::vAppendString(&szCrs,
                              (char*)SP_Dte.sz(pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_FLT: {
        SP_Flt SP_Flt;
        StrLib::vAppendString(&szCrs,
                              (char*)SP_Flt.sz(*(d8*)&pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_INT: {
        SP_Int spInt;
        StrLib::vAppendString(&szCrs,
                              (char*)spInt.sz(pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_IVL: {
        SP_Ivl spIvl;
        StrLib::vAppendString(&szCrs,
                              (char*)spIvl.sz(pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_TME: {
        SP_Tme SP_Tme;
        StrLib::vAppendString(&szCrs,
                              (char*)SP_Tme.sz(pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_TSP: {
        SP_Tsp SP_Tsp;
        StrLib::vAppendString(&szCrs,
                              (char*)SP_Tsp.sz(pRow->m_waColVal[i_Col]));
        break;
      }
        
      case DM_VCH: {
        SP_Vch SP_Vch;
        StrLib::vAppendString(&szCrs,
                              (char*)SP_Vch.sz(pRow->m_waColVal[i_Col].m_azStr));
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
Stz* SP_Row::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caRow[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Row::sz(RowSingle* pRow)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vRow(pRow);
  
  return (Stz*) &m_caRow[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_Row);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
