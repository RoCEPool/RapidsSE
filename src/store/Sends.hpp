/*============================================================================*/
/* Sends.hpp -- consume send counters                                         */
/*============================================================================*/
#pragma once

namespace STORE
{
  class Sends : public Object
  {
  private:
    i8             m_iSends;   // consume Sends
    
    i8             m_iCancelMarkers;  // cancel markers received by consumeOp
    i8             m_iDestroyMarkers; // destroy markers received by consumeOp
    i8             m_iExitMarkers;    // exit markers received by consumeOp
    i8             m_iEosMarkers;     // eos / error markers received by consumeOp
    i8             m_iDataPulses;     // data pulses received by consumeOp
    i8             m_iPulsedMarkers;  // pulsed markers received by consumeOp
    i8             m_iStartMarkers;   // start markers received by consumeOp
    i8             m_iStreamDefs;     // stream def's received by consumeOp
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Sends(i8 iSends);
    ~Sends(void) noexcept(false);
    
    bool bCancelDone(void);
    bool bDataPulseDone(void);
    bool bDestroyDone(void);
    bool bEosDone(void);
    bool bErrorDone(void);
    bool bExitDone(void);
    bool bPulsedDone(void);
    bool bStartDone(void);
    bool bStreamDefDone(void);
    
    void vIncCancel(void);
    void vIncDataPulse(void);
    void vIncDestroy(void);
    void vIncEos(void);
    void vIncError(void);
    void vIncExit(void);
    void vIncPulsed(void);
    void vIncStart(void);
    void vIncStreamDef(void);
    
    Footer* aFooter(void);
    
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Sends* _pSends(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
