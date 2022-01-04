/*============================================================================*/
/* RowSingle.hpp -- Computable form of a single Row                           */
/*============================================================================*/
#pragma once

#include "RowBox_t.hpp"

namespace STORE
{
  static const i8 C_ROWSINGLE_WSP  = C_MAX_RECORD;

  class RowSingle :
                    public Object,
                    public RowBox_t<C_ROWSINGLE_WSP>

  {
  public:
    // - - - - - - - - - - - - - - - - - -
    // meta data
    union {
      StreamDef *m_pStreamDef;     // stream def
      RowPix *m_pRowPix;        // row pix
    };
    
    bool       m_abGetCol[C_MAX_COLUMNS];

  private:
   
    // footer -- must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);

    RowSingle(void);
    RowSingle(StreamDef* pStreamDef);
    ~RowSingle(void) noexcept(false);

    static void* aGetCellArray(RowSingle* pRowSingle, char** aazStrArea);
    static void  vPrint(RowSingle* pRowSingle);
    static void  vRepair(Object* pObject);

    byte* ayCellArray(void);
    Dtm* aColDtm(i8 i_Col);
    Dtm  wColDtm(i8 i_Col);
    char* azColVal(i8 i_Col);
    i8   iColVal(i8 i_Col);
    d8   dColVal(i8 i_Col);
    Stz* szColVal(i8 i_Col);
    void vCopyRow(Row* pRow);
    bool bIsColNull(i8 i_Col);
    void vPrintRow(void);
    void vResetRow(void);
    void vResetValues(void);
    void vSetColNull(i8 i_Col);
    i8   nSumVchSizes(void);
    Stz* szStzAlottable(void);
    void vStzBytesTaken(i8 nBytesTaken);

  private:
    Footer* aFooter(void);
  };
  
  RowSingle* _pRowSingle(void* aAddr);
  void  vRowSingle_Print(void* pRowSingle);

  typedef HV<RowSingle,StreamDef> HV_RowSingle;

  typedef ListExo_t<CN_ROWSINGLELIST, RowSingle> RowSingleList;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
