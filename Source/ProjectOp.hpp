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
    i8         m_iWhichVec[C_MAX_COLUMNVECS];
    i8         m_iWhichCol[C_MAX_COLUMNS];
        
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ProjectOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR, OpSpec* pOpSpec=nullptr);
   ~ProjectOp(void) noexcept(false);
  
    static Op* pNew(Stz* szLabel, Digest* pDigest,
                    OpDefn* aOpDefn, bool bAppR, OpSpec* pOpSpec,
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
