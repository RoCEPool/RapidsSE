/*============================================================================*/
/* RowBuffer.hpp -- Row Buffer                                                */
/*============================================================================*/
#pragma once

#include "RowSingle.hpp"

namespace STORE
{
  static const i8 C_WSP_ROWBUFFER = C_SIZE_LARGE_OBJECT - C_32KB; // ref NmlStr


  class RowBuffer : public DataBuffer,
                    public RowBox_t<C_WSP_ROWBUFFER>
  {
  protected:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    RowBuffer(void);
    RowBuffer(eCN eClassNo);
    RowBuffer(eCN eClassNo, StreamDef* pStreamDef);
   ~RowBuffer(void) noexcept(false);
      
    static RowBuffer* pToRowBuffer(void* aAddr);

    Row* pAllotCopyRow(RowSingle* pRowSingle);
  
    Row* pAllotRow(void);
    Row* pAllotRow(i8 iVargsBegin, ...);
    Row* pAllotRow(StreamDef* pStreamDef, i8 iVargsBegin, ...);

    void vDepositRow(Row* pRow, i8 iVargsBegin, ...);
  
    void vPrintRows(void);
    void vPrintRows(StreamDef* pStreamDef);
  
    void vRestorePointers(void);
  
    void vShow(StreamDef* pStreamDef);
  
    char* azStrArea(void);
  
    CsvBuffer* pToCsvBuffer(void);
  
  private:
    bool bDepositColumns(Row* pRow, va_list xVarArgs);

    i8   iToOffset(Stz* szStr);
  
    Footer* aFooter(void);

  };
  
  RowBuffer* _pRowBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
