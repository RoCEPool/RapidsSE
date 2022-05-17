/*============================================================================*/
/* Output.hpp -- Output                                                       */
/*============================================================================*/
#pragma once

#include "eOM.h"
#include "eRS.h"

#include "Definition.hpp"
#include "StreamDef.hpp"

namespace STORE
{
  class Output : public Object
  {

  public:
    Queue*     m_pOutputQueue;     // queue of buffers being the job output

    StreamDef* m_pStreamDef_result;
    bool       m_bToExit;
  
    Atomic     m_ixRowsProcessed; // rows processed
    Atomic     m_ixRowsInvalid;   // invalid rows detected
    Atomic     m_ixRowsSkipped;   // rows skipped

    bool       m_bFileOut;
    File*      m_pFileOut;
    s256       m_sFilePath;

  private:

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Output(void);
   ~Output(void) noexcept(false);
  
    void vAssertEmptyQueue(void);
    void vBeginProcOneCmd(void);
    void vDrainBufferQueue(void);
    void vEndProcOneCmd(void);
    void vFileOut(bool bFileOut);
    void vFilePath(Stz* szFilePath);
    void vProcessBufferQueue(void);

    void vPrintResult(void);
  
//  private:
    Operand* pPopBufferQueue_blocking(void);
    bool bProcessOperand(Operand* pOperand);
        
    Footer* aFooter(void);
  };
  
  Output* _pOutput(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
