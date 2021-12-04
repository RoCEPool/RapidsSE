/*============================================================================*/
/* RowBuffer.hpp -- Row Buffer                                                */
/*============================================================================*/
#pragma once

#include "RowSingle.hpp"
#include "EachRowBuffer.hpp"
#include "Tub.hpp"

namespace STORE
{
  static const i8 C_WSP_ROWBUFFER = C_SIZE_LARGE_OBJECT - C_10KB; // ref NmlStr


  class RowBuffer : public DataBuffer,
                    public RowBox_t<C_WSP_ROWBUFFER>
  {
  protected:
  
    bool    m_bDeletable;
    Decider m_oDecider;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    RowBuffer(void);
    RowBuffer(eCN eClassNo);
    RowBuffer(eCN eClassNo, StreamDef* pStreamDef);
   ~RowBuffer(void) noexcept(false);

    static Stz*       szGetStrArea(RowBuffer* pRowBuffer);
    static void*      aGetCellArray(RowBuffer* pRowBuffer, i8 iRow);
    static RowBuffer* pToRowBuffer(void* aAddr);
    static void       vRepairList(Object* pObject);
    static void       vPostRecv(void* aRowBuffer);

    Row* pAllotCopyRow(RowSingle* pRowSingle);
  
    Row* pAllotRow(void);
    Row* pAllotRow(i8 iVargsBegin, ...);
    Row* pAllotRow(StreamDef* pStreamDef, i8 iVargsBegin, ...);
    Row* pAllotRow(Row* pRow);

    i8   iDecile(void);
    bool bDepositColumns(Row* pRow, va_list xVarArgs);
    void vDepositRow(Row* pRow, i8 iVargsBegin, ...);

    RowBuffer* pNextRowBuffer(void);
    void       vNextRowBuffer(RowBuffer* pNextBuffer);
    RowBuffer* pPrevRowBuffer(void);
    void       vPrevRowBuffer(RowBuffer* pPrevBuffer);

    void vPrintRows(Stz* szLabel=nullptr);
    void vPrintRows(StreamDef* pStreamDef);
  
    void vRestorePointers(void);
  
    void vShow(StreamDef* pStreamDef);
  
    char* azStrArea(void);
  
    void vToCsv(Tub* pTub);
    CsvBuffer* pToCsvBuffer(void);
    EachRowBuffer* pToEachRowBuffer(i8 i_Row);
    void vTruncate(void);

    bool bVerify(void);
  
  private:

    i8   iToOffset(Stz* szStr);
  
    Footer* aFooter(void);

  };
  
  RowBuffer* _pRowBuffer(void* aAddr);
  void* aRowBuffer_GetCellArray(void* pRowBuffer, i8 iRow);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
