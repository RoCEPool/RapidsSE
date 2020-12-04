/*============================================================================*/
/* RowSingle.hpp -- Computable form of a single Row                           */
/*============================================================================*/
#pragma once

namespace Rows
{
  class RowSingle :
#ifdef _ROWBOX_BASED
                    public Object,
                    public RowBox_t<C_ROWSINGLE_WSP>
#else
                    public RowWsp<C_ROWSINGLE_WSP>
#endif
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

    // - - - - - - - - - - - - - - - - - -
    // data
  
  private:
 
#ifndef _ROWBOX_BASED
    Dtm* m_awaColVal; // dynamically allotted column value array
#endif

  
    // - - - - - - - - - - - - - - - - - -
    // footer -- must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);

    RowSingle(StreamDef* pStreamDef, i8 iSubSet);
    ~RowSingle(void) noexcept(false);
  
#ifndef _ROWBOX_BASED
    void vAllotSetVarchar(i8 i_Col,Stz* szVal);
#endif
  
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
