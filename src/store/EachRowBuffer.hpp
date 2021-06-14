/*============================================================================*/
/* EachRowBuffer.hpp -- Each Row Buffer                                       */
/*============================================================================*/
#pragma once

#include "RowSingle.hpp"

namespace STORE
{

  static const i8 C_WSP_EACHROWBUFFER = C_SIZE_MEDIUM_OBJECT - C_32KB; // ref NmlStr
  
  class EachRowBuffer : public DataBuffer,
                        public RowBox_t<C_WSP_EACHROWBUFFER>
  {
  protected:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    EachRowBuffer(void);
    EachRowBuffer(eCN eClassNo);
    EachRowBuffer(eCN eClassNo, StreamDef* pStreamDef);
    EachRowBuffer(eCN eClassNo, RowPix* pRowPix);
    ~EachRowBuffer(void) noexcept(false);

    static EachRowBuffer* pToEachRowBuffer(void* aAddr);

    static void       vPostRecv(void* aEachRowBuffer);

    Row* pAllotRow(void);
    Row* pAllotRow(StreamDef* pStreamDef, i8 iVargsBegin, ...);

    i8   iDecile(void);
    bool bDepositColumns(Row* pRow, va_list xVarArgs);
    void vDepositRow(Row* pToRow, Row* pFromRow);

    EachRowBuffer* pNextEachRowBuffer(void);
    void       vNextEachRowBuffer(EachRowBuffer* pNextBuffer);
    EachRowBuffer* pPrevEachRowBuffer(void);
    void       vPrevEachRowBuffer(EachRowBuffer* pPrevBuffer);

    void vPrintRows(void);
    void vPrintRows(StreamDef* pStreamDef);

    void vRestorePointers(void);

    void vShow(StreamDef* pStreamDef);

    char* azStrArea(void);

    CsvBuffer* pToCsvBuffer(void);
    void vTruncate(void);

    bool bVerify(void);


  private:
  
    Footer* aFooter(void);

  };
  
  EachRowBuffer* _pEachRowBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
