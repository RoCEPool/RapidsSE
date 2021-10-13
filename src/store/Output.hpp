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
    Queue* m_pOutputQueue;     // queue of buffers being the job output

    StreamDef* m_pStreamDef_result;
    bool       m_bExitMarker;
    bool       m_bExitEarlyMarker;

    bool       m_bCmdErr;
    bool       m_bExecErr;
    bool       m_bPrintEnabled;
  
    Atomic  m_ixRowsProcessed; // rows processed
    Atomic  m_ixRowsInvalid;   // invalid rows detected
    Atomic  m_ixRowsSkipped;   // rows skipped

  private:

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Output(void);
   ~Output(void) noexcept(false);
  
    void vAssertEmptyQueue(void);
    void vDrainBufferQueue(Session* pSession);
    void vProcessBufferQueue(Session* pSession);
    bool bPrintEnabled(void);
    void vPrintEnabled(bool bPrintEnabled);

    void vPrintResult(void);
  
//  private:
    Operand* pPopBufferQueue_blocking(Session* pSession);
    bool bProcessOperand(Session* pSession, Operand* pOperand);
        
    Footer* aFooter(void);
  };
  
  Output* _pOutput(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
