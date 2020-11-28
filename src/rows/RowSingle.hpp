/*============================================================================*/
/* RowSingle.hpp -- Computable form of a single Row                           */
/*============================================================================*/
#pragma once

namespace Rows
{
  class RowSingle : public RowWsp<C_ROWSOLO_WSP>
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
  
    t8  m_tPerspective;   // transaction perspective
    i8  m_iHash;          // hash of partition column values
    Dtm m_waColVal[C_MAX_COLUMNS]; // column value array
 
    SingleWord m__PadToCacheLine[6]; // padding to cache line

  private:

    // - - - - - - - - - - - - - - - - - -
    // footer -- must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);

    RowSingle(StreamDef* pStreamDef, i8 iSubSet);
    ~RowSingle(void) noexcept(false);
  
    void vAdjustAddresses(RowSingle* pCopiedFromRow);
    void vAllotSetVarchar(i8 i_Col,Stz* szVal);
    void vCopy(Row* pRow);
    void vCopy(Row* pRow, i8 iFromCol, i8 iIntoCol);
    void vDontDecodeVch(void);
    i8   iHashColumn(Column* pCol);
    void vResetValues(void);
    void vSet(i8 iArgsBegin, ...);
    void vSetValue(Datum* pDatum, Column* pCol);
    i8   nSizeBytes(void);
    i8   nSizeRow(void);
    i8   nSizeVch(void);

    Stz* szStzArea(void);
  private:
    Footer* aFooter(void);
  };
  
  RowSingle* _pRowSingle(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
