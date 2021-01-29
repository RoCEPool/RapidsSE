/*============================================================================*/
/* RowPix.hpp -- Row Pix                                                      */
/*============================================================================*/
#pragma once

#include "eDM.h"

#define C_ROWPIX_TEST_I8 512
#define C_ROWPIX_TEST_B1 4096

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
    i8 m_nNameOffset;  // 6)
    i8 m_nTotalChars;  // 7)

    //ColPix m_xaColPix[C_MAX_DEFCOLUMNS];        // column pix array
  union {
    ColPix m_xaColPix[1];          // column pix array
    //byte   m_baPixArea[C_8KB];   // pix and names
  
    //i8 m__i8Test[C_ROWPIX_TEST_I8]; // see if this reproduces problem on Linux
    byte m__b1Test[C_ROWPIX_TEST_B1]; // see if this reproduces problem on Linux
  };
  
    // must be last
    Footer m_xFooter;

  public:
  
    void* operator new(size_t);
    void* operator new(size_t, RowPix*);
    void operator delete(void*);

    RowPix(void);
   ~RowPix(void) noexcept(false);

    ColPix* axColPix(i8 i_Col);
    ColPix* axaColPixArray(void);
  
    i8   nColumns(void);
    void vColumns(i8 nColumns);
    void vCopyRowPix(RowPix* pRowPixSrc);
    Stz* szPixNm(void);
    void vPixNm(Stz* szPixNm);
    void vPrint(void);
    void vRemovePtnHash(void);

    Footer* aFooter(void);
  };

  RowPix* _pRowPix(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
