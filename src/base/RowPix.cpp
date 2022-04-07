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
  vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
         "fatal: RowPix class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowPix::operator new(size_t, RowPix* aInfoPix)
{
  return aInfoPix;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RowPix::operator delete(void* aAddress)
{
  UNUSED(aAddress);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowPix::RowPix(void)
: Object(CN_ROWPIX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
  
  m_bInitialised = false;
  m_bShortForm   = false;
  m_nColumns     = 0;
  m_nExtraCols   = 0;
  m_bPtnHash     = false;
  m_nMaxRowSize  = 256;  // FOR TESTING PURPOSES
  m_nNameOffset  = C_UNKNOWN;
  m_nTotalChars  = 0;

  
  for (i8 i_Col = 0; i_Col < C_ROWPIX_COLUMNS; i_Col++)
    m_xaColPix[i_Col].m_uAsU8 = 0;
  
  MemLib::vZero(m_baPixArea,sizeof(m_baPixArea),CN_ROWPIX);
  
  
  _checkInitialised_(CN_ROWPIX);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowPix::~RowPix(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set number of columns                                                      */
/*----------------------------------------------------------------------------*/
void RowPix::vAssertRowPix(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
  
}

/*----------------------------------------------------------------------------*/
/* get number of columns                                                      */
/*----------------------------------------------------------------------------*/
i8 RowPix::nColumns(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
  
  return m_nColumns;
}

/*----------------------------------------------------------------------------*/
/* set number of columns                                                      */
/*----------------------------------------------------------------------------*/
void RowPix::vColumns(i8 nColumns)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
  
  m_nColumns = nColumns;
}

/*----------------------------------------------------------------------------*/
/* get col pic array                                                          */
/*----------------------------------------------------------------------------*/
ColPix* RowPix::axColPix(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
  
  return & m_xaColPix[i_Col];
}

/*----------------------------------------------------------------------------*/
/* get col pic array                                                          */
/*----------------------------------------------------------------------------*/
ColPix* RowPix::axaColPixArray(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
  
  return m_xaColPix;
}

/*----------------------------------------------------------------------------*/
/* copy row pix                                                               */
/*----------------------------------------------------------------------------*/
//void RowPix::vCopyRowPix(RowPix* pRowPixSrc)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
//  MemLib::vCopy(this,pRowPixSrc,sizeof(RowPix),CN_ROWPIX);
//
//  
//  //m_nRowSizeProper = Row::nRowProper(pRowPixSrc);
//}

/*----------------------------------------------------------------------------*/
/* get dynamic size of this RowPix                                            */
/*----------------------------------------------------------------------------*/
i8 RowPix::nDynamicSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
  
  ASSERT(m_bInitialised);
  ASSERT(m_nColumns);

  
  i8 nNilSize = sizeof(RowPix)
                  - sizeof(m_xaColPix) - sizeof(m_baPixArea);
  
  i8 nSizeColPix = m_nColumns * sizeof(ColPix);
  
  i8 nSize = nNilSize + nSizeColPix + m_nTotalChars + 16;
  
  //return sizeof(RowPix);
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* get i_keyHsh                                                               */
/*----------------------------------------------------------------------------*/
i8 RowPix::i_KeyHsh(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
  
  ASSERT(m_bInitialised);
  ASSERT(m_nColumns);

  i8 i_KeyHsh = m_nColumns - 1;
  
  return i_KeyHsh;
}

/*----------------------------------------------------------------------------*/
/* get pix name                                                               */
/*----------------------------------------------------------------------------*/
Stz* RowPix::szPixNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
  
  return m_sPixNm;
}

/*----------------------------------------------------------------------------*/
/* set pix name                                                               */
/*----------------------------------------------------------------------------*/
void RowPix::vPixNm(Stz* szPixNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
  
  m_sPixNm = szPixNm;
}
  
/*----------------------------------------------------------------------------*/
/* print this set pix                                                         */
/*----------------------------------------------------------------------------*/
void RowPix::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
  
  SP_ColPix spColPix;
  
  vTrace("RowPix: %s\n"_Stz,szPixNm());
  vTrace("  %lld columns (%lld extra)\n"_Stz,nColumns(),m_nExtraCols);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
  
  //vPrint();
  
  ASSERT(m_bPtnHash);
  ASSERT(m_nExtraCols);

  m_nColumns--;
  m_nExtraCols--;
  m_bPtnHash = false;
  
  //vPrint();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
//__aFooterFunction(RowPix);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
