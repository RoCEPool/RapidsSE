/*============================================================================*/
/* RowPix.cpp -- Row Pix                                                      */
/*============================================================================*/

#include "BASE.h"
#include "eCX.h"
#include "SP_ColPix.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,RowPix,CN_ROWPIX)
__aNewFunction(RowPix,CN_ROWPIX)
__vDeleteFunction(RowPix,CN_ROWPIX)

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowPix::operator new(size_t, RowPix* aInfoPix)
{
  return aInfoPix;
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
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowPix::RowPix(RowPix* pRowPix, bool bAddPtnHash)
    : Object(CN_ROWPIX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);

  i8 nNameSize;
  i8 nStzOfs;

  m_sPixNm       = ""_Stz;
  m_bInitialised = pRowPix->m_bInitialised;
  m_bShortForm   = pRowPix->m_bShortForm;
  m_nColumns     = pRowPix->m_nColumns;
  m_nExtraCols   = pRowPix->m_nExtraCols;
  m_bPtnHash     = pRowPix->m_bPtnHash;
  m_nMaxRowSize  = pRowPix->m_nMaxRowSize;
  m_nTotalChars  = 0;

  m_nNameOffset = sizeof(ColPix)
                           * ( m_nColumns + (bAddPtnHash ? 1 : 0) );

  i8 iBufSpaceRemaining = (i8) (sizeof(m_xaColPix) + sizeof(m_baPixArea) - m_nNameOffset);

  MemLib::vZero(m_baPixArea,sizeof(m_baPixArea),CN_ROWPIX);

  ColPix* axaColPixDst = m_xaColPix;

  char* azNames = aAdd<char*>(axaColPixDst,m_nNameOffset);
  char* azColNmDst = azNames;
  i8 i_Col;

  for (i_Col = 0; i_Col < pRowPix->m_nColumns; i_Col++) {

    m_xaColPix[i_Col].m_uAsU8 = 0;
    ColPix* axColPixDst = &axaColPixDst[i_Col];

    axColPixDst->m_uNumber   = (u8)i_Col;
    // name
    axColPixDst->m_uKind     = (u8)CX_USERCOL;
    axColPixDst->m_uTypeId    = (u8)pRowPix->m_xaColPix[i_Col].m_uTypeId;
    axColPixDst->m_uDatum    = (u8)pRowPix->m_xaColPix[i_Col].m_uDatum;
    axColPixDst->m_uLength   = (u8)pRowPix->m_xaColPix[i_Col].m_uLength;
    axColPixDst->m_uScale    = (u8)pRowPix->m_xaColPix[i_Col].m_uScale;
    axColPixDst->m_bNullable = (u8)pRowPix->m_xaColPix[i_Col].m_bNullable;
    axColPixDst->m_bPtnKey   = (u8)pRowPix->m_xaColPix[i_Col].m_bPtnKey;
    axColPixDst->m_uSpare    = (u8)0;

    ColPix* pColPix = pRowPix->axColPix(i_Col);

    StrLib::vCopy(azColNmDst, iBufSpaceRemaining, aAdd<char*>(pColPix,pColPix->m_uName));
    nNameSize = StrLib::nSize(azColNmDst);
    iBufSpaceRemaining -= nNameSize;

    nStzOfs = nDifference(axColPixDst,azColNmDst);
    axColPixDst->m_uName = nStzOfs;

    azColNmDst += nNameSize;
    m_nTotalChars += nNameSize;
  }

  // i_Col falls thru

  if (bAddPtnHash) {

    //pRowPixDst->vPrint();
    ASSERT(! m_nExtraCols);

    m_nColumns++;
    m_nExtraCols = 1;
    m_bPtnHash = true;

    ColPix* axColPixDst = &axaColPixDst[i_Col];

    axColPixDst->m_uNumber   = (u8)i_Col;
    // name
    axColPixDst->m_uKind     = (u8)CX_PTNHASH;
    axColPixDst->m_uTypeId   = (u8)CT_INTEGER;
    axColPixDst->m_uDatum    = (u8)DM_INT;
    axColPixDst->m_uLength   = (u8)C_MAX_INT_PRECISION;
    axColPixDst->m_uScale    = (u8)0;
    axColPixDst->m_bNullable = (u8)false;
    axColPixDst->m_bPtnKey   = (u8)false;
    axColPixDst->m_uSpare    = (u8)0;

    StrLib::vCopy(azColNmDst, iBufSpaceRemaining, "__keyHash");
    nNameSize = StrLib::nSize(azColNmDst);
    iBufSpaceRemaining -= nNameSize;

    nStzOfs = nDifference(axColPixDst,azColNmDst);
    axColPixDst->m_uName = nStzOfs;

    azColNmDst += nNameSize;
    m_nTotalChars += nNameSize;

    //pRowPixDst->vPrint();
  }

  m_nTotalChars++;
  * azColNmDst = '\0';

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
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
RowPix* RowPix::pRowPixNew(i8 nNumCols, ColumnInfo* pColumnInfo) {

	_static_

  i8 nNameSize;
  i8 nStzOfs;
  bool bAddPtnHash = 0;

  RowPix* pRowPix = new RowPix;

  pRowPix->m_sPixNm       = ""_Stz;
  pRowPix->m_bInitialised = true;
  pRowPix->m_bShortForm   = false;
  pRowPix->m_nColumns = nNumCols;
  pRowPix->m_nExtraCols   = 0;
  pRowPix->m_bPtnHash     = false;
  pRowPix->m_nMaxRowSize  = 256;
  pRowPix->m_nTotalChars = 0;

  pRowPix->m_nNameOffset = sizeof(ColPix)
                              * ( nNumCols + (bAddPtnHash ? 1 : 0) );
  i8 iBufSpaceRemaining = (i8) (sizeof(m_xaColPix) + sizeof(m_baPixArea) - pRowPix->m_nNameOffset);

  MemLib::vZero(pRowPix->m_baPixArea,sizeof(pRowPix->m_baPixArea),CN_ROWPIX);

  ColPix* axaColPixDst = pRowPix->m_xaColPix;

  char* azNames = aAdd<char*>(axaColPixDst,pRowPix->m_nNameOffset);
  char* azColNmDst = azNames;

  for (i8 i_Col = 0; i_Col < nNumCols; i_Col++) {

    pRowPix->m_xaColPix[i_Col].m_uAsU8 = 0;
    ColPix* axColPixDst = &axaColPixDst[i_Col];

    axColPixDst->m_uNumber   = (u8)i_Col;
    // name
    axColPixDst->m_uKind     = (u8)CX_USERCOL;
    axColPixDst->m_uTypeId   = (u8)pColumnInfo[i_Col].m_iColumnTypeId;
    axColPixDst->m_uDatum    = (u8)Column::eDatum((eCT)((TypeId)pColumnInfo[i_Col].m_iColumnTypeId).eCTVal());
    axColPixDst->m_uLength   = (u8)pColumnInfo[i_Col].m_iColumnLength;
    axColPixDst->m_uScale    = (u8)pColumnInfo[i_Col].m_iColumnScale;
    axColPixDst->m_bNullable = (u8)pColumnInfo[i_Col].m_bIsNullable;
    axColPixDst->m_bPtnKey   = (u8)pColumnInfo[i_Col].m_bPtnKey;
    axColPixDst->m_uSpare    = (u8)0;

    StrLib::vCopy(azColNmDst, iBufSpaceRemaining, &pColumnInfo[i_Col].m_cColumnNm[0]);
    nNameSize = StrLib::nSize(azColNmDst);
    iBufSpaceRemaining -= nNameSize;

    nStzOfs = nDifference(axColPixDst,azColNmDst);
    axColPixDst->m_uName = nStzOfs;

    azColNmDst += nNameSize;
    pRowPix->m_nTotalChars += nNameSize;
  }

  pRowPix->m_nTotalChars++;
  * azColNmDst = '\0';

  return pRowPix;
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
/* Copy                                                                       */
/*----------------------------------------------------------------------------*/
void RowPix::vCopyRowPix(RowPix* pSrcRowPix)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);

  bool bAddPtnHash = pSrcRowPix->m_bPtnHash;
  i8 nNameSize;
  i8 nStzOfs;

  m_sPixNm = ""_Stz;
  m_bInitialised = pSrcRowPix->m_bInitialised;
  m_bShortForm   = pSrcRowPix->m_bShortForm;
  m_nColumns     = pSrcRowPix->m_nColumns;
  m_nExtraCols   = pSrcRowPix->m_nExtraCols;
  m_bPtnHash     = pSrcRowPix->m_bPtnHash;
  m_nMaxRowSize  = pSrcRowPix->m_nMaxRowSize;
  m_nTotalChars  = 0;

  m_nNameOffset = sizeof(ColPix)
                  * ( m_nColumns + (bAddPtnHash ? 1 : 0) );

  i8 iBufSpaceRemaining = (i8) (sizeof(m_xaColPix) + sizeof(m_baPixArea) - m_nNameOffset);

  MemLib::vZero(m_baPixArea,sizeof(m_baPixArea),CN_ROWPIX);

  ColPix* axaColPixDst = m_xaColPix;

  char* azNames = aAdd<char*>(axaColPixDst,m_nNameOffset);
  char* azColNmDst = azNames;
  i8 i_Col;

  for (i_Col = 0; i_Col < pSrcRowPix->m_nColumns; i_Col++) {

    m_xaColPix[i_Col].m_uAsU8 = 0;
    ColPix* axColPixDst = &axaColPixDst[i_Col];

    axColPixDst->m_uNumber   = (u8)i_Col;
    // name
    axColPixDst->m_uKind     = (u8)CX_USERCOL;
    axColPixDst->m_uTypeId   = (u8)pSrcRowPix->m_xaColPix[i_Col].m_uTypeId;
    axColPixDst->m_uDatum    = (u8)pSrcRowPix->m_xaColPix[i_Col].m_uDatum;
    axColPixDst->m_uLength   = (u8)pSrcRowPix->m_xaColPix[i_Col].m_uLength;
    axColPixDst->m_uScale    = (u8)pSrcRowPix->m_xaColPix[i_Col].m_uScale;
    axColPixDst->m_bNullable = (u8)pSrcRowPix->m_xaColPix[i_Col].m_bNullable;
    axColPixDst->m_bPtnKey   = (u8)pSrcRowPix->m_xaColPix[i_Col].m_bPtnKey;
    axColPixDst->m_uSpare    = (u8)0;

    ColPix* pColPix = pSrcRowPix->axColPix(i_Col);

    StrLib::vCopy(azColNmDst, iBufSpaceRemaining, aAdd<char*>(pColPix,pColPix->m_uName));
    nNameSize = StrLib::nSize(azColNmDst);
    iBufSpaceRemaining -= nNameSize;

    nStzOfs = nDifference(axColPixDst,azColNmDst);
    axColPixDst->m_uName = nStzOfs;

    azColNmDst += nNameSize;
    m_nTotalChars += nNameSize;

  }

  // i_Col falls thru

//  if (bAddPtnHash) {
//
//    //pRowPixDst->vPrint();
//    ASSERT(! m_nExtraCols);
//
//    m_nColumns++;
//    m_nExtraCols = 1;
//    m_bPtnHash = true;
//
//    ColPix* axColPixDst = &axaColPixDst[i_Col];
//
//    axColPixDst->m_uNumber   = (u8)i_Col;
//    // name
//    axColPixDst->m_uKind     = (u8)CX_PTNHASH;
//    axColPixDst->m_uTypeId   = (u8)CT_INTEGER;
//    axColPixDst->m_uDatum    = (u8)DM_INT;
//    axColPixDst->m_uLength   = (u8)C_MAX_INT_PRECISION;
//    axColPixDst->m_uScale    = (u8)0;
//    axColPixDst->m_bNullable = (u8)false;
//    axColPixDst->m_bPtnKey   = (u8)false;
//    axColPixDst->m_uSpare    = (u8)0;
//
//    StrLib::vCopy(azColNmDst, iBufSpaceRemaining, "__keyHash");
//    nNameSize = StrLib::nSize(azColNmDst);
//    iBufSpaceRemaining -= nNameSize;
//
//    nStzOfs = nDifference(axColPixDst,azColNmDst);
//    axColPixDst->m_uName = nStzOfs;
//
//    azColNmDst += nNameSize;
//    m_nTotalChars += nNameSize;
//
//    //pRowPixDst->vPrint();
//  }

  m_nTotalChars++;
  * azColNmDst = '\0';

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
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
