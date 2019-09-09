/*============================================================================*/
/* ProjectOp.hpp -- Project                                                   */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class ProjectOp : public Op
  {
  private:
  
    IrwBuffer* m_pIrwBuffer;
    StreamDef* m_pOriginalStreamDef;
    i8         m_iWhichVec[C_MAX_COLUMNS];
    i8         m_iWhichCol[C_MAX_COLUMNS];
        
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ProjectOp(OpSpec* pOpSpec, Digest* pDigest, Stz* szOpLabel, OpDefn* aOpDefn, bool bAppR);
   ~ProjectOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Digest* pDigest, Stz* szLabel,
                    OpDefn* aOpDefn, bool bAppR,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR eOpLoop(void);
    eOR eDrainLoop(void);
    
  private:
    void vUpdateStreamDef(void);
    Footer* aFooter(void);
  };

  ProjectOp* _pProjectOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
