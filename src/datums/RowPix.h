/*============================================================================*/
/* RowPix.h Stream Pix                                                        */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RowPix // condensed stream def
  {
  public:
  
    // IF YOU ADD A MEMBER CHANGE CALC IN nBytes()
  
    b8 m_bInitialised; // has this stream pix been initialised
    i8 m_nCols;        // number of columns
    i8 m_nExtraCols;   //   of which there are nExtra metadata columns
    b8 m_bPtnHash;     // is there a partition hash ?
    i8 m_nMaxRowSize;  //

    bool   m_bProject[C_MAX_DEFCOLUMNS];
    ColPix m_xaColPix[C_MAX_DEFCOLUMNS]; // col pix (C++ flexible member array must be last)
    
    // constructor
    RowPix(void)
    {
      m_bInitialised = false;
      m_nCols        = 0;
      m_nExtraCols   = 0;
      m_bPtnHash     = false;
      m_nMaxRowSize  = 256;  // FOR TESTING PURPOSES
      
      for (i8 i_Col = 0; i_Col < C_MAX_DEFCOLUMNS; i_Col++) {
        m_xaColPix[i_Col].m_uAsI4 = 0;
        m_bProject[i_Col] = true;
      }
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
  
    // is column being projected
    inline i8 bIsProjected(i8 i_Col)
    {
      return m_bProject[i_Col];
    }
    
    // mark column to be not projected
    void vNotProject(i8 i_Col)
    {
      m_bProject[i_Col] = false;
    }
    
    // remove partition metadata column
    void vRemovePtnHash(void)
    {
      ASSERT(m_bPtnHash);
      ASSERT(m_nExtraCols);

      m_nCols--;
      m_nExtraCols--;
      m_bPtnHash = false;
    }
    
    // calc size of this RowPix
    i8 nSize(void)
    {
      i8 nSize = nBytes(m_nCols);
    
      return nSize;
    }
  
    // print
    void vPrint(void)
    {
      vTrace("RowPix: %lld cols\n"_Stz,m_nCols);
      for (i8 i_Col = 0; i_Col < m_nCols; i_Col++) {
        vTrace("[%02lld] P:%s %s(%lld,%lld)%s%s\n"_Stz,
               i_Col,
               bIsProjected(i_Col) ? "true"_Stz : "false"_Stz ,
               aDM[m_xaColPix[i_Col].m_uDatum],
               (i8)m_xaColPix[i_Col].m_uLength,
               (i8)m_xaColPix[i_Col].m_uScale,
               m_xaColPix[i_Col].m_bPtnKey ? " !PtnKey" : "",
               i_Col + 1 == m_nCols && m_nExtraCols ? " #PtnHsh" : ""
               );

      }
    }
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
