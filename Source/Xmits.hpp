/*============================================================================*/
/* Xmits.hpp -- consume xmit counters                                  */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class Xmits : public Focal
  {
  private:
    i8             m_iXmits;   // consume xmits
    
    i8             m_iCancelMarkers;  // cancel markers received by consumeOp
    i8             m_iDestroyMarkers; // destroy markers received by consumeOp
    i8             m_iExitMarkers;    // exit markers received by consumeOp
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
  
    Xmits(i8 iXmits);
    ~Xmits(void) noexcept(false);
    
    bool bCancelDone(void);
    bool bDestroyDone(void);
    bool bEosDone(void);
    bool bErrorDone(void);
    bool bExitDone(void);
    bool bPulseDone(void);
    bool bPulsedDone(void);
    bool bStartDone(void);
    bool bStreamDefDone(void);
    
    void vIncCancel(void);
    void vIncDestroy(void);
    void vIncEos(void);
    void vIncError(void);
    void vIncExit(void);
    void vIncPulse(void);
    void vIncPulsed(void);
    void vIncStart(void);
    void vIncStreamDef(void);
    
    Footer* aFooter(void);
    
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Xmits* _pXmits(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
