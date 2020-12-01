/*============================================================================*/
/* RowBuffer.hpp -- Row Buffer                                                */
/*============================================================================*/
#pragma once

namespace DataBuffers
{
  typedef       HV <Record,StreamDef>        HV_Rec;
  
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
      
    Row* aAllotCopyRow(RowSingle* pRowSingle);
    
    Row* aFirstRow(void);
    Row* aLastRow(void);
    Row* aNextRow(Row* aPriorRow);
    void vPrintRows(StreamDef* pStreamDef);
    void vShow(StreamDef* pStreamDef);
    
  private:
    i8   iToOffset(Stz* szStr);
  };
  
  RowBuffer* _pRowBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
