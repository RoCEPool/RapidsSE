/*============================================================================*/
/* SP_RowPix.cpp - Record sprint helper                                       */
/*============================================================================*/

#include "Rows.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_RowPix::SP_RowPix(i8 nCols, ColPix* axaColPix)
: Sprint(CN_SPID)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  // set default enclosing char
  vFormat(F_VCH_enclose);

  m_nCols     = nCols;
  m_axaColPix = axaColPix;
  m_azCursor  = azRecord();

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caRecord,sizeof(m_caRecord),CN_SPID);
#else
  m_caRecord[0] = '\0';
#endif
  
  //! FIX RAY
  //_checkInitialised_(CN_SPID);
  
  _postcondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_RowPix::~SP_RowPix(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
Stz* SP_RowPix::sz(Row* aRow, bool bNewLine)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vPutRow(aRow,bNewLine);
  
  return (Stz*) m_caRecord;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendBcd(Row* aRow, i8 i_Col, ColPix xColPix)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);

  i8 iVal = aRow->iVal_rsb(i_Col);

  SP_Bcd spBcd;
  StrLib::vAppendString(&m_azCursor,(char*)spBcd.sz(iVal,xColPix.m_uScale));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendBol(Row* aRow, i8 i_Col, ColPix xColPix)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);

  i8 iVal = aRow->iVal_rsb(i_Col);
  
  SP_Bol spBol;
  StrLib::vAppendString(&m_azCursor,(char*)spBol.sz(iVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendDte(Row* aRow, i8 i_Col, ColPix xColPix)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);

  i8 iVal = aRow->iVal_rsb(i_Col);
    
  SP_Dte spDte;
  StrLib::vAppendString(&m_azCursor,(char*)spDte.sz(iVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendFlt(Row* aRow, i8 i_Col, ColPix xColPix)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);

  d8 dVal = aRow->dColVal_flt(i_Col);
  
  SP_Flt spFlt;
  StrLib::vAppendString(&m_azCursor,(char*)spFlt.sz(dVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendInt(Row* aRow, i8 i_Col, ColPix xColPix)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  i8 iVal = aRow->iVal_rsb(i_Col);
  
  if (CX_PTNHASH == xColPix.m_uKind)
    StrLib::vAppendChar(&m_azCursor,'#');
  
  SP_Int spInt;
  StrLib::vAppendString(&m_azCursor,(char*)spInt.sz(iVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendIvl(Row* aRow, i8 i_Col, ColPix xColPix)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  i8 iVal = aRow->iVal_rsb(i_Col);
  
  SP_Ivl spIvl;
  StrLib::vAppendString(&m_azCursor,(char*)spIvl.sz(iVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendTme(Row* aRow, i8 i_Col, ColPix xColPix)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  i8 iVal = aRow->iVal_rsb(i_Col);
  
  SP_Tme spTme;
  StrLib::vAppendString(&m_azCursor,(char*)spTme.sz(iVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendTsp(Row* aRow, i8 i_Col, ColPix xColPix)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  i8 iVal = aRow->iVal_rsb(i_Col);
  
  SP_Tsp spTsp;
  StrLib::vAppendString(&m_azCursor,(char*)spTsp.sz(iVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendVch(Row* aRow, i8 i_Col, ColPix xColPix)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  Stz* szVal = aRow->szVal_rsb(i_Col);
  
  SP_Vch spVch;
  StrLib::vAppendChar(&m_azCursor,'\'');
  StrLib::vAppendString(&m_azCursor,(char*)szVal);
  StrLib::vAppendChar(&m_azCursor,'\'');
}

/*----------------------------------------------------------------------------*/
/* put row into record                                                        */
/*----------------------------------------------------------------------------*/
void SP_RowPix::vPutRow(Row* aRow, bool bNewLine)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
    
  vReset();
  
  for (i8 i_Col = 0; i_Col < m_nCols; i_Col++) {
    
    if (i_Col)
      StrLib::vAppendChar(&m_azCursor,',');

    if (aRow->bIsColNull(i_Col)) {
      StrLib::vAppendString(&m_azCursor,(char*)"NULL"_Stz);
      continue;
    }
    
    i8 iVal = aRow->iVal_rsb(i_Col);
    
    ColPix xColPix = m_axaColPix[i_Col];
    eDM eDatum = (eDM)xColPix.m_uDatum;
    
    switch (eDatum) {
      case DM_BCD: vAppendBcd(aRow,i_Col,xColPix); break;
      case DM_BOL: vAppendBol(aRow,i_Col,xColPix); break;
      case DM_DTE: vAppendDte(aRow,i_Col,xColPix); break;
      case DM_FLT: vAppendFlt(aRow,i_Col,xColPix); break;
      case DM_INT: vAppendInt(aRow,i_Col,xColPix); break;
      case DM_IVL: vAppendIvl(aRow,i_Col,xColPix); break;
      case DM_TME: vAppendTme(aRow,i_Col,xColPix); break;
      case DM_TSP: vAppendTsp(aRow,i_Col,xColPix); break;
      case DM_VCH: vAppendVch(aRow,i_Col,xColPix); break;
      default:
        NEVER_REACH;
    }
  }
  
  if (bNewLine)
    StrLib::vAppendChar(&m_azCursor,'\n');
}

/*----------------------------------------------------------------------------*/
/* return char* of result                                                     */
/*----------------------------------------------------------------------------*/
char* SP_RowPix::azRecord(void)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);

  return & m_caRecord[0];
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void SP_RowPix::vReset(void)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);

  m_caRecord[0] = '\0';
  
  m_azCursor = azRecord();
}

/*----------------------------------------------------------------------------*/
/*-- Private ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_RowPix)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
