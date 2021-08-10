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
    
    i8             m_iCancellingMarkers; // cancel markers received by consumeOp
    i8             m_iCleanupMarkers;    // cleanup markers received by consumeOp
    i8             m_iDataPulses;        // data pulses received by consumeOp
    i8             m_iDestroyMarkers;    // destroy markers received by consumeOp
    i8             m_iEndMarkers;        // end markers received by consumeOp
    i8             m_iEndingMarkers;     // ending markers received by consumeOp
    i8             m_iErroringMarkers;   // erroring markers received by consumeOp
    i8             m_iExitMarkers;       // exit markers received by consumeOp
    i8             m_iEosMarkers;        // eos / error markers received by consumeOp
    i8             m_iLeavingMarkers;    // leaving markers received by consumeOp
    i8             m_iStartMarkers;      // start markers received by consumeOp
    i8             m_iStreamDefs;        // stream def's received by consumeOp
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Sends(i8 iSends);
    ~Sends(void) noexcept(false);
    
    bool bCancellingDone(void);
    bool bCleanupDone(void);
    bool bDataPulseDone(void);
    bool bDestroyDone(void);
    bool bEndDone(void);
    bool bEndingDone(void);
    bool bEosDone(void);
    bool bErrorDone(void);
    bool bErroringDone(void);
    bool bExitDone(void);
    bool bLeavingDone(void);
    bool bStartDone(void);
    bool bStreamDefDone(void);
    
    void vIncCancelling(void);
    void vIncCleanup(void);
    void vIncDataPulse(void);
    void vIncDestroy(void);
    void vIncEnd(void);
    void vIncEnding(void);
    void vIncEos(void);
    void vIncError(void);
    void vIncErroring(void);
    void vIncExit(void);
    void vIncLeaving(void);
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
