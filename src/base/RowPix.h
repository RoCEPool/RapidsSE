/*============================================================================*/
/* RowPix.h Stream Pix                                                        */
/*============================================================================*/
#pragma once

#include "eDM.h"

namespace BASE
{
  class RowPix // condensed stream def
  {
  public:
  
    // IF YOU ADD A MEMBER CHANGE CALC IN nBytes()
  
    b8 m_bInitialised; // has this stream pix been initialised
    i8 m_nColumns;     // number of columns
    i8 m_nExtraCols;   //   of which there are nExtra metadata columns
    b8 m_bPtnHash;     // is there a partition hash ?
    i8 m_nMaxRowSize;  //

    ColPix m_xaColPix[C_MAX_DEFCOLUMNS]; // col pix (C++ flexible member array must be last)
    
    // constructor
    RowPix(void)
    {
      m_bInitialised = false;
      m_nColumns        = 0;
      m_nExtraCols   = 0;
      m_bPtnHash     = false;
      m_nMaxRowSize  = 256;  // FOR TESTING PURPOSES
      
      for (i8 i_Col = 0; i_Col < C_MAX_DEFCOLUMNS; i_Col++) {
        m_xaColPix[i_Col].m_uAsU8 = 0;
      }
    }
  
    void vCopyRowPix(RowPix* pRowPixSrc)
    {
      MemLib::vCopy(this,pRowPixSrc,sizeof(RowPix),CN_ROWBUFFER);

      //m_nRowSizeProper = Row::nRowProper(pRowPixSrc);
    }

  
  
    // calc number of bytes (this is weird)
    i8 nBytes(i8 nCols)
    {
      // there are 5 scalar 8B members in RowPix + 2 arrays
      i8 nBytes = (  (    5 * (i8)sizeof(i8))
                   + (nCols * (i8)sizeof(bool))
                   + (nCols * (i8)sizeof(ColPix))
                   ) ;
    
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
      i8 nSize = nBytes(m_nColumns);
    
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
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
