/*============================================================================*/
/* RowPix.cpp -- Row Pix                                                      */
/*============================================================================*/

#include "BASE.h"

#include "SP_ColPix.hpp"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowPix* BASE::_pRowPix(void* aAddr)
{
  RowPix* pRowPix = reinterpret_cast<RowPix*>(aAddr);
  
  return pRowPix;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowPix::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: RowPix class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* RowPix::operator new(size_t, RowPix* aInfoPix)
{
  return aInfoPix;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void RowPix::operator delete(void* aAddress)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowPix::RowPix(void)
: Object(CN_ROWPIX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX,_FOOTER_);
  
  m_bInitialised = false;
  m_bShortForm   = false;
  m_nColumns     = 0;
  m_nExtraCols   = 0;
  m_bPtnHash     = false;
  m_nMaxRowSize  = 256;  // FOR TESTING PURPOSES
  m_nNameOffset  = C_UNKNOWN;
  m_nTotalChars  = 0;

  for (i8 i_Col = 0; i_Col < C_MAX_DEFCOLUMNS; i_Col++) {
    m_xaColPix[i_Col].m_uAsU8 = 0;
  }

  //m_xaColPix[0].m_uAsU8 = 0;
  //MemLib::vZero(m_baPixArea,sizeof(m_baPixArea),CN_ROWPIX);

  _checkInitialised_(CN_ROWPIX);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowPix::~RowPix(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get number of columns                                                      */
/*----------------------------------------------------------------------------*/
i8 RowPix::nColumns(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX,_FOOTER_);
  
  return m_nColumns;
}

/*----------------------------------------------------------------------------*/
/* set number of columns                                                      */
/*----------------------------------------------------------------------------*/
void RowPix::vColumns(i8 nColumns)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX,_FOOTER_);
  
  m_nColumns = nColumns;
}

/*----------------------------------------------------------------------------*/
/* get col pic array                                                          */
/*----------------------------------------------------------------------------*/
ColPix* RowPix::axColPix(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX,_FOOTER_);
  
  return & m_xaColPix[i_Col];
}

/*----------------------------------------------------------------------------*/
/* get col pic array                                                          */
/*----------------------------------------------------------------------------*/
ColPix* RowPix::axaColPixArray(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX,_FOOTER_);
  
  return m_xaColPix;
}

/*----------------------------------------------------------------------------*/
/* copy row pix                                                               */
/*----------------------------------------------------------------------------*/
void RowPix::vCopyRowPix(RowPix* pRowPixSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX,_FOOTER_);
  MemLib::vCopy(this,pRowPixSrc,sizeof(RowPix),CN_ROWPIX);

  //m_nRowSizeProper = Row::nRowProper(pRowPixSrc);
}

/*----------------------------------------------------------------------------*/
/* get pix name                                                               */
/*----------------------------------------------------------------------------*/
Stz* RowPix::szPixNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX,_FOOTER_);
  
  return m_sPixNm;
}

/*----------------------------------------------------------------------------*/
/* set pix name                                                               */
/*----------------------------------------------------------------------------*/
void RowPix::vPixNm(Stz* szPixNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX,_FOOTER_);
  
  m_sPixNm = szPixNm;
}
  
/*----------------------------------------------------------------------------*/
/* print this set pix                                                         */
/*----------------------------------------------------------------------------*/
void RowPix::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX,_FOOTER_);
  
  SP_ColPix spColPix;
  
  vTrace("RowPix: %s\n"_Stz,szPixNm());
  vTrace("  %lld columns\n"_Stz,nColumns());
  
  for (i8 i_Col = 0; i_Col < nColumns(); i_Col++) {
    ColPix* axColPix = this->axColPix(i_Col);
    vTrace("  %s\n"_Stz,spColPix.sz(axColPix));
  }
}

/*----------------------------------------------------------------------------*/
/* remove partition metadata column                                           */
/*----------------------------------------------------------------------------*/
void RowPix::vRemovePtnHash(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX,_FOOTER_);
  ASSERT(m_bPtnHash);
  ASSERT(m_nExtraCols);

  m_nColumns--;
  m_nExtraCols--;
  m_bPtnHash = false;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(RowPix);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
