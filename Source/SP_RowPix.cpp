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
Stz* SP_RowPix::sz(Rsb* aRsb, bool bNewLine)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vPutRow(aRsb,bNewLine);
  
  return (Stz*) m_caRecord;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendBcd(Rsb* aRsb, i8 i_Col, ColPix xColPix)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);

  i8 i_Cell_ntv = aRsb->i_ColToCell_ntv(i_Col);
  i8 iVal = aRsb->iVal_ntv(i_Cell_ntv);

  SP_Bcd spBcd;
  StrLib::vAppendString(&m_azCursor,(char*)spBcd.sz(iVal,xColPix.m_uScale));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendBol(Rsb* aRsb, i8 i_Col, ColPix xColPix)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);

  i8 i_Cell_ntv = aRsb->i_ColToCell_ntv(i_Col);
  i8 iVal = aRsb->iVal_ntv(i_Cell_ntv);
  
  SP_Bol spBol;
  StrLib::vAppendString(&m_azCursor,(char*)spBol.sz(iVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendDte(Rsb* aRsb, i8 i_Col, ColPix xColPix)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);

  i8 i_Cell_ntv = aRsb->i_ColToCell_ntv(i_Col);
  i8 iVal = aRsb->iVal_ntv(i_Cell_ntv);
  
  SP_Dte spDte;
  StrLib::vAppendString(&m_azCursor,(char*)spDte.sz(iVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendFlt(Rsb* aRsb, i8 i_Col, ColPix xColPix)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);

  i8 i_Cell_ntv = aRsb->i_ColToCell_ntv(i_Col);
  d8 dVal = aRsb->dVal_ntv(i_Cell_ntv);
  
  SP_Flt spFlt;
  StrLib::vAppendString(&m_azCursor,(char*)spFlt.sz(dVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendInt(Rsb* aRsb, i8 i_Col, ColPix xColPix)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  i8 i_Cell_ntv = aRsb->i_ColToCell_ntv(i_Col);
  i8 iVal = aRsb->iVal_ntv(i_Cell_ntv);
  
  if (CX_PTNHASH == xColPix.m_uKind)
    StrLib::vAppendChar(&m_azCursor,'#');
  
  SP_Int spInt;
  StrLib::vAppendString(&m_azCursor,(char*)spInt.sz(iVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendIvl(Rsb* aRsb, i8 i_Col, ColPix xColPix)
{
  i8 i_Cell_ntv = aRsb->i_ColToCell_ntv(i_Col);
  i8 iVal = aRsb->iVal_ntv(i_Cell_ntv);
  
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  SP_Ivl spIvl;
  StrLib::vAppendString(&m_azCursor,(char*)spIvl.sz(iVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendTme(Rsb* aRsb, i8 i_Col, ColPix xColPix)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  i8 i_Cell_ntv = aRsb->i_ColToCell_ntv(i_Col);
  i8 iVal = aRsb->iVal_ntv(i_Cell_ntv);
  
  SP_Tme spTme;
  StrLib::vAppendString(&m_azCursor,(char*)spTme.sz(iVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendTsp(Rsb* aRsb, i8 i_Col, ColPix xColPix)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  i8 i_Cell_ntv = aRsb->i_ColToCell_ntv(i_Col);
  i8 iVal = aRsb->iVal_ntv(i_Cell_ntv);
  
  SP_Tsp spTsp;
  StrLib::vAppendString(&m_azCursor,(char*)spTsp.sz(iVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendVch(Rsb* aRsb, i8 i_Col, ColPix xColPix)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  i8 i_Cell_ntv = aRsb->i_ColToCell_ntv(i_Col);
  Stz* szVal = aRsb->szVal_ntv(i_Cell_ntv);
  
  SP_Vch spVch;
  StrLib::vAppendString(&m_azCursor,(char*)szVal);
}

/*----------------------------------------------------------------------------*/
/* put row into record                                                        */
/*----------------------------------------------------------------------------*/
void SP_RowPix::vPutRow(Rsb* aRsb, bool bNewLine)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
    
  vReset();
  
  for (i8 i_Col = 0; i_Col < m_nCols; i_Col++) {
    
    if (i_Col)
      StrLib::vAppendChar(&m_azCursor,',');

    i8 i_Cell_ntv = aRsb->i_ColToCell_ntv(i_Col);
    
    i8 iVal = aRsb->iVal_ntv(i_Cell_ntv);
    
    ColPix xColPix = m_axaColPix[i_Col];
    eDM eDatum = (eDM)xColPix.m_uDatum;
    
    switch (eDatum) {
      case DM_BCD: vAppendBcd(aRsb,i_Col,xColPix); break;
      case DM_BOL: vAppendBol(aRsb,i_Col,xColPix); break;
      case DM_DTE: vAppendDte(aRsb,i_Col,xColPix); break;
      case DM_FLT: vAppendFlt(aRsb,i_Col,xColPix); break;
      case DM_INT: vAppendInt(aRsb,i_Col,xColPix); break;
      case DM_IVL: vAppendIvl(aRsb,i_Col,xColPix); break;
      case DM_TME: vAppendTme(aRsb,i_Col,xColPix); break;
      case DM_TSP: vAppendTsp(aRsb,i_Col,xColPix); break;
      case DM_VCH: vAppendVch(aRsb,i_Col,xColPix); break;
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
