/*============================================================================*/
/* Marker.hpp -- Marker                                                       */
/*============================================================================*/
#pragma once

#include "eOM.h"
#include "eMG.h"

namespace STORE { class MarkerMgr; }

namespace STORE
{
  class Marker : public Operand
  {
  protected:
    eOM  m_eOpMarker;
    eMG  m_eMarker;

    Marker* m_pNextItem;
  
    b8  m_bDeletable;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t, eMG eMarker);
    void* operator new(size_t, Job* pJob);
    //void* operator new(size_t, MarkerMgr* pMarkerMgr);
    void operator delete(void*);
  

    Marker(eOM eOpMarker);
   ~Marker(void) noexcept(false);
    
    static bool bInspect(Object* pObject);

    static bool bObjRecv(Socket* pSocket, Object* pObjBase, SysCall& roSysCall,
                         Object** ppObject, void* pvAuxBuf, i8 iAuxSize);
    static void vPostRecv(void* aMarker);

    bool bDeletable(void);
    void vDeletable(bool bDeletable);
  
    eMG eMarker(void);
    void vMarker(eMG eMarker);

    Marker* pNextItem(void);
    void vNextItem(Marker* pNext);

    eOM eOpMarker(void);
    void vOpMarker(eOM eOpMarker);

  private:
    Footer* aFooter(void);
  };

  Marker* _pMarker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
