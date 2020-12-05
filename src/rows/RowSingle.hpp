/*============================================================================*/
/* RowSingle.hpp -- Computable form of a single Row                           */
/*============================================================================*/
#pragma once

namespace Rows
{
  static const i8 C_ROWSINGLE_WSP  = C_MAX_RECORD;

  class RowSingle :
                    public Object,
                    public RowBox_t<C_ROWSINGLE_WSP>

  {
  public:
    // - - - - - - - - - - - - - - - - - -
    // meta data
  
    StreamDef* m_pStreamDef;     // stream def
    i8         m_nColumns;       // no cols in stream def
    i8         m_iSubSet;        // stream def subset
    
    bool       m_bGetAll;        // get all crw columns?
  
    bool       m_abGetCol[C_MAX_COLUMNS];
    i8         m_iLastCol;

  private:
   
    // footer -- must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);

    RowSingle(StreamDef* pStreamDef, i8 iSubSet);
    ~RowSingle(void) noexcept(false);
    
    byte* ayCellArray(void);
    Dtm* aColDtm(i8 i_Col);
    Dtm  wColDtm(i8 i_Col);
    char* azColVal(i8 i_Col);
    i8   iColVal(i8 i_Col);
    d8   dColVal(i8 i_Col);
    Stz* szColVal(i8 i_Col);
    void vCopyRow(Row* pRow);
    bool bIsColNull(i8 i_Col);
    void vResetValues(void);
    void vSetColNull(i8 i_Col);
    void vSetValue(Datum* pDatum, Column* pCol);
    i8   nSumVchSizes(void);
    Stz* szStzAlottable(void);
    Stz* szStzArea(void);
    void vStzBytesTaken(i8 nBytesTaken);

  private:
    Footer* aFooter(void);
  };
  
  RowSingle* _pRowSingle(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
