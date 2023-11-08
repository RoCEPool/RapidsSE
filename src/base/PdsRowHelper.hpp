/*============================================================================*/
/* PdsRowHelper.hpp -- Wrapper for Pds of Rows and string area                */
/*============================================================================*/
#pragma once

namespace BASE
{
  class PdsRowHelper : public Object
  {
  public:
    i8    m_iSynthKey;     // Synthetic key for the Pds

    RowPix* m_pRowPix;

    // temporary variables
    Row* m_aRowCur;     // current row
    i8   m_i_RowCrs;    // current logical row number
    i8   m_i_ColCrs;    // current working column
    void* m_aStzSet;     // current pointer to string set area
    i8   m_iCurStzSetSpace; // current space left for string set area

    Pds*  m_pPdsOfRows;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    PdsRowHelper(void);
    ~PdsRowHelper(void) noexcept(false);

    Row* pAllotRow(void);
    void* aAllotStzSet(void);
    void vCopyRows(RowBuffer* pRowBuffer);
    void vPrintRows(File* pFilePtr, Stz* szLabel);
    void vReset(void);

//private
    void vAdvanceCursor(void);
    void vDepositColVal(d8 dColVal);
    void vDepositColVal(i8 iColVal);
    void vDepositColVal(i8 i_Col, i8 iColVal);
    void vDepositColVal(Stz* szColVal, i8 iMaxStzLen);
//    void vDepositColVal(i8 i_Col, Stz* szColVal);

    String_d* azDepositString_begin(i8 nSize);
    void vDepositString_end(i8 nSize);


    Footer* aFooter(void);
  };

  PdsRowHelper* _pPdsRowHelper(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/