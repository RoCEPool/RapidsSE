/*============================================================================*/
/* FocalObjects.hpp -- Lattice of Focal Objects                               */
/*============================================================================*/
#pragma once

namespace Logging
{
  class FocalObjects : public LatticeNL_t<CN_FOCALOBJECTS,Focal,C_64KB>
  {
  private:
    Lock m_okLock;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    FocalObjects(void);
   ~FocalObjects(void) noexcept(false);
    
    Footer* aFooter(void);
  };
  
  FocalObjects* _pFocalObjects(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
