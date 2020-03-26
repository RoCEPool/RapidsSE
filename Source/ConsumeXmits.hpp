/*============================================================================*/
/* ConsumeXmits.hpp -- consume xmit counters                                  */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class ConsumeXmits : public Focal
  {
  private:
    i8             m_iConsumeXmits;   // consume xmits
    
    i8             m_iExitMarkers;    // exit / cancel markers received by consumeOp
    i8             m_iEosMarkers;     // eos / error markers received by consumeOp
    i8             m_iPulseMarkers;   // pulse markers received by consumeOp
    i8             m_iPulsedMarkers;  // pulsed markers received by consumeOp
    i8             m_iStartMarkers;   // start markers received by consumeOp
    i8             m_iStreamDefs;     // stream def's received by consumeOp
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    ConsumeXmits(i8 iConsumeXmits);
    ~ConsumeXmits(void) noexcept(false);
    
    bool bCancelDone(void);
    bool bEosDone(void);
    bool bErrorDone(void);
    bool bExitDone(void);
    bool bPulseDone(void);
    bool bPulsedDone(void);
    
    void vIncCancel(void);
    void vIncEos(void);
    void vIncError(void);
    void vIncExit(void);
    void vIncPulse(void);
    void vIncPulsed(void);
    
    Footer* aFooter(void);
    
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ConsumeXmits* _pConsumeXmits(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
