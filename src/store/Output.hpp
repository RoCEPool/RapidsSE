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
    Session*   m_pSession;
    Queue*     m_pOutputQueue;     // queue of buffers being the job output

    StreamDef* m_pStreamDef_result;
    bool       m_bExitMarker;
    bool       m_bExitEarlyMarker;

    bool       m_bCmdErr;
    bool       m_bExecErr;
    bool       m_bPrintEnabled;
  
    Atomic     m_ixRowsProcessed; // rows processed
    Atomic     m_ixRowsInvalid;   // invalid rows detected
    Atomic     m_ixRowsSkipped;   // rows skipped
  
    i8 m_iOutputState;

  private:

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Output(Session* pSession);
   ~Output(void) noexcept(false);
  
    void vAssertEmptyQueue(void);
    void vBeginProcOneCmd(void);
    void vDrainBufferQueue(void);
    void vEndProcOneCmd(void);
    void vProcessBufferQueue(void);
    bool bPrintEnabled(void);
    void vPrintEnabled(bool bPrintEnabled);

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
