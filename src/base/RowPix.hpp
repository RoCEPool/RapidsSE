/*============================================================================*/
/* RowPix.hpp -- Row Pix                                                      */
/*============================================================================*/
#pragma once

#include "eDM.h"

namespace BASE
{
  class RowPix : public Object
  {
  public:
  
    // IF YOU ADD A MEMBER CHANGE CALC IN nBytes()
    s256 m_sPixNm;     // pix name

    b8 m_bInitialised; // 0) has this row pix been initialised?
    b8 m_bShortForm;   // 1) has been compacted into short form?
    i8 m_nColumns;     // 2) number of columns
    i8 m_nExtraCols;   // 3)   of which there are nExtra non-user columns
    b8 m_bPtnHash;     // 4) is there a partition hash ?
    i8 m_nMaxRowSize;  // 5)
    i8 m_nTotalChars;  // 6)

    ColPix m_xaColPix[C_MAX_DEFCOLUMNS]; // column pix
    s32    m_saColNm[C_MAX_DEFCOLUMNS];  // column names

  public:
  
    void* operator new(size_t);
    void* operator new(size_t, RowPix*);
    void operator delete(void*);

    RowPix(void);
   ~RowPix(void) noexcept(false);

    void vCopyRowPix(RowPix* pRowPixSrc);
    i8   nRowPixBytes(i8 nCols);
    void vRemovePtnHash(void);
    i8   nSize(void);
    void vPrint(void);

/*
    // constructor
    RowPix(void)
    : Object(CN_ROWPIX)
    {
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
    }
  
  
    // copy row pix
    void vCopyRowPix(RowPix* pRowPixSrc)
    {
      MemLib::vCopy(this,pRowPixSrc,sizeof(RowPix),CN_ROWBUFFER);

      //m_nRowSizeProper = Row::nRowProper(pRowPixSrc);
    }

  
    // calc number of bytes (this is weird)
    i8 nRowPixBytes(i8 nCols)
    {
      i8 nBytes =    (    1 * (i8)sizeof(m_sPixNm))
                   + (    7 * (i8)sizeof(i8))
                   + (nCols * (i8)sizeof(ColPix))
                   ;
    
      return nBytes;
    }
  
          
    // remove partition metadata column
    void vRemovePtnHash(void)
    {
      ASSERT(m_bPtnHash);
      ASSERT(m_nExtraCols);

      m_nColumns--;
      m_nExtraCols--;
      m_bPtnHash = false;
    }
  
    
    // calc size of this RowPix
    i8 nSize(void)
    {
      i8 nSize = nRowPixBytes(m_nColumns);
    
      return nSize;
    }
  
  
    // print
    void vPrint(void)
    {
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
  */
  };

  RowPix* _pRowPix(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
