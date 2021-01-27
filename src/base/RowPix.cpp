/*============================================================================*/
/* RowPix.cpp -- Row Pix                                                      */
/*============================================================================*/
#include "BASE.h"


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
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
  
  m_bInitialised = false;
  m_bShortForm   = false;
  m_nColumns     = 0;
  m_nExtraCols   = 0;
  m_bPtnHash     = false;
  m_nMaxRowSize  = 256;  // FOR TESTING PURPOSES
  m_nTotalChars  = 0;

  for (i8 i_Col = 0; i_Col < C_MAX_DEFCOLUMNS; i_Col++) {
    m_xaColPix[i_Col].m_uAsU8 = 0;
  }
  
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
/* copy row pix                                                               */
/*----------------------------------------------------------------------------*/
void RowPix::vCopyRowPix(RowPix* pRowPixSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
  MemLib::vCopy(this,pRowPixSrc,sizeof(RowPix),CN_ROWBUFFER);

  //m_nRowSizeProper = Row::nRowProper(pRowPixSrc);
}

/*----------------------------------------------------------------------------*/
/* calc number of bytes (this is weird)                                       */
/*----------------------------------------------------------------------------*/
i8 RowPix::nRowPixBytes(i8 nCols)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
  i8 nBytes =    (    1 * (i8)sizeof(m_sPixNm))
               + (    7 * (i8)sizeof(i8))
               + (nCols * (i8)sizeof(ColPix))
               ;

  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* remove partition metadata column                                           */
/*----------------------------------------------------------------------------*/
void RowPix::vRemovePtnHash(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
  ASSERT(m_bPtnHash);
  ASSERT(m_nExtraCols);

  m_nColumns--;
  m_nExtraCols--;
  m_bPtnHash = false;
}

/*----------------------------------------------------------------------------*/
/* calc size of this RowPix                                                   */
/*----------------------------------------------------------------------------*/
i8 RowPix::nSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
  i8 nSize = nRowPixBytes(m_nColumns);

  return nSize;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void RowPix::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWPIX);
  vTrace("RowPix: %lld cols\n"_Stz,m_nColumns);
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    vTrace("[%02lld] %s(%lld,%lld)%s%s\n"_Stz,
           i_Col,
           aDM[m_xaColPix[i_Col].m_uDatum],
           (i8)m_xaColPix[i_Col].m_uLength,
           (i8)m_xaColPix[i_Col].m_uScale,
           m_xaColPix[i_Col].m_bPtnKey ? " !PtnKey" : "",
           i_Col + 1 == m_nColumns && m_nExtraCols ? " #PtnHsh" : ""
           );

  }
}
  
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
