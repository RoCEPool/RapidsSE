/*============================================================================*/
/* RowBox.hpp -- Container of a single Row                                    */
/*============================================================================*/
#pragma once

namespace Rows
{
  class RowBox : public RowWsp<C_ROWSOLO_WSP>
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
 
    Dtm* m_awaColVal; // dynamically allotted column value array
  
    // - - - - - - - - - - - - - - - - - -
    // footer -- must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);

    RowBox(StreamDef* pStreamDef, i8 iSubSet);
    ~RowBox(void) noexcept(false);
  
    void vAdjustAddresses(RowBox* pCopiedFromRow);
    void vAllotSetVarchar(i8 i_Col,Stz* szVal);
    Dtm* aColDtm(i8 i_Col);
    char* azColVal(i8 i_Col);
    i8   iColVal(i8 i_Col);
    d8   dColVal(i8 i_Col);
    Stz* szColVal(i8 i_Col);
    Dtm  wColVal(i8 i_Col);
    void vCopy(Row* pRow);
    void vDontDecodeVch(void);
    byte* ayDstByte(void);
    bool bIsColNull(i8 i_Col);
    void vResetValues(void);
    void vSet(i8 iArgsBegin, ...);
    void vSetNull(i8 i_Col);
    void vSetValue(Datum* pDatum, Column* pCol);
    i8   nSizeBytes(void);
    i8   nSizeRow(void);
    i8   nSizeVch(void);

    Stz* szStzArea(void);
  private:
    Footer* aFooter(void);
  };
  
  RowBox* _pRowBox(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
