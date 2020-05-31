/*============================================================================*/
/* SP_RowPix.cpp - Record sprint helper                                       */
/*============================================================================*/

#include "Load.h"

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
Stz* SP_RowPix::sz(i8* aiaRowCells)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vPutRow(aiaRowCells);
  
  return (Stz*) m_caRecord;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendBcd(ColPix xColPix, i8 iVal)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);

  SP_Bcd spBcd;
  StrLib::vAppendString(&m_azCursor,(char*)spBcd.sz(iVal,xColPix.m_uScale));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendBol(ColPix xColPix, i8 iVal)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);

  SP_Bol spBol;
  StrLib::vAppendString(&m_azCursor,(char*)spBol.sz(iVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendDte(ColPix xColPix, i8 iVal)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);

  SP_Dte spDte;
  StrLib::vAppendString(&m_azCursor,(char*)spDte.sz(iVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendFlt(ColPix xColPix, i8 iVal)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);

  SP_Flt spFlt;
  StrLib::vAppendString(&m_azCursor,(char*)spFlt.sz(iVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendFlt(i8* aiaRowCells, i8 i_Cell)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);

  d8* adaRowCells = (d8*)aiaRowCells;
  d8 dVal = adaRowCells[- i_Cell];
  
  SP_Flt spFlt;
  StrLib::vAppendString(&m_azCursor,(char*)spFlt.sz(dVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendInt(ColPix xColPix, i8 iVal)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  SP_Int spInt;
  StrLib::vAppendString(&m_azCursor,(char*)spInt.sz(iVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendIvl(ColPix xColPix, i8 iVal)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  SP_Ivl spIvl;
  StrLib::vAppendString(&m_azCursor,(char*)spIvl.sz(iVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendTme(ColPix xColPix, i8 iVal)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  SP_Tme spTme;
  StrLib::vAppendString(&m_azCursor,(char*)spTme.sz(iVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendTsp(ColPix xColPix, i8 iVal)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  SP_Tsp spTsp;
  StrLib::vAppendString(&m_azCursor,(char*)spTsp.sz(iVal));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SP_RowPix::vAppendVch(ColPix xColPix, i8 iVal)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  SP_Vch spVch;
  StrLib::vAppendString(&m_azCursor,(char*)spVch.sz((char*)iVal));
}

/*----------------------------------------------------------------------------*/
/* put row into record                                                        */
/*----------------------------------------------------------------------------*/
void SP_RowPix::vPutRow(i8* aiaRowCells)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
    
  vReset();
  
  for (i8 i_Col = 0; i_Col < m_nCols; i_Col++) {
    
    if (i_Col)
      StrLib::vAppendString(&m_azCursor,",");

    i8 iVal = aiaRowCells[- i_Col];
    
    ColPix xColPix = m_axaColPix[i_Col];
    eDM eDatum = (eDM)xColPix.m_uType;
    
    switch (eDatum) {
      case DM_BCD: vAppendBcd(xColPix,iVal); break;
      case DM_BOL: vAppendBol(xColPix,iVal); break;
      case DM_DTE: vAppendDte(xColPix,iVal); break;
      case DM_FLT: vAppendFlt(aiaRowCells,i_Col); break;
      case DM_INT: vAppendInt(xColPix,iVal); break;
      case DM_IVL: vAppendIvl(xColPix,iVal); break;
      case DM_TME: vAppendTme(xColPix,iVal); break;
      case DM_TSP: vAppendTsp(xColPix,iVal); break;
      case DM_VCH: vAppendVch(xColPix,iVal); break;
      default:
        NEVER_REACH;
    }
  }
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
