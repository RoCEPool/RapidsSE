/*============================================================================*/
/* FocalObjects.hpp -- Lattice of Focal Objects                               */
/*============================================================================*/
#pragma once

namespace StorageEngine
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
  
    void vAdd(Focal* pFocal);
    void vRemove(Focal* pFocal);
  
    // -- -- -- -- -- --
  
    void vMri(TokenVec* pTknVec);
    void vMriAll(void);
    void vMriClass(Stz* szClassNm);
    void vMriFocal(i8 i_Focal);

  private:
    
    Footer* aFooter(void);
  };
  
  FocalObjects* _pFocalObjects(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
