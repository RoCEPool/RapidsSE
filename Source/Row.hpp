/*============================================================================*/
/* Row.hpp -- Computable form of Row                                          */
/*============================================================================*/
#pragma once

namespace Rows
{
  class RowSingle;
  
  class Row : public Object
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
    Wsp_t<C_MAX_RECORD> m_xWsp;    // sz workspace

    // - - - - - - - - - - - - - - - - - -
    // footer -- must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);

    Row(StreamDef* pStreamDef, i8 iSubSet);
    ~Row(void) noexcept(false);
  
    void vAdjustAddresses(RowSingle* pCopiedFromRow);
    void vCopy(Row* pRow);
    void vCopy(RowSingle* pRow);
    void vCopy(Row* pRow, i8 iFromCol, i8 iIntoCol);
    void vDontDecodeVch(void);
    i8   iHashColumn(Column* pCol);
    void vResetValues(void);
    void vSetValue(Datum* pDatum, Column* pCol);
    i8   nSizeBytes(void);

  private:
    Footer* aFooter(void);
  };
  
  Row* _pRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
