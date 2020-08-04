/*============================================================================*/
/* DevIXT.hpp -- IXT Device                                                   */
/*============================================================================*/
#pragma once

namespace Expressions
{
  class DevIXT : public Device
  {
  public:
  
    IndexTree* m_pIdxTree;
  
    Ent*       m_aEnt;
    Ent*       m_aUpperEnt;
    
    EntBuffer* m_pEntBuffer;

    Crw*       m_aCrw;
    CrwBuffer* m_pCrwBuffer;

    i8         m_i_Row;
    i8         m_nRows;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    DevIXT(void);
   ~DevIXT(void) noexcept(false);
  
    eKC   e_ENT_COMPARE(LogicUnit* pLogicUnit);
  
    i8    i_ENT_NEXT(LogicUnit* pLogicUnit);
    void  v_ENT_POSPRIOR(LogicUnit* pLogicUnit);
    void* a_ENT_PUSH(LogicUnit* pLogicUnit);
    i8    i_ENT_PUSHELMI0(LogicUnit* pLogicUnit);
    Stz*  sz_ENT_PUSHELMS0(LogicUnit* pLogicUnit);
    i8    i_ENT_PUSHOFS(LogicUnit* pLogicUnit);
  
    void  v_ENTWRK_APPENDELM(LogicUnit* pLogicUnit);
    void  v_ENTWRK_APPENDELMI(LogicUnit* pLogicUnit, i8 iTop);
    void  v_ENTWRK_APPENDELMS(LogicUnit* pLogicUnit, Stz* azTop);

    void  v_IXT_DEL_ENT(LogicUnit* pLogicUnit);
    void  v_IXT_DUMP(LogicUnit* pLogicUnit);
    void  v_IXT_INS_ENT(LogicUnit* pLogicUnit);
    i8    i_IXT_TMP_ENTRIES(LogicUnit* pLogicUnit);
    i8    i_IXT_TRV_BEGIN(LogicUnit* pLogicUnit);
    i8    i_IXT_TRV_NEXT(LogicUnit* pLogicUnit);
  
    i8    i_PAY_FETCH_COLI(LogicUnit* pLogicUnit, i8 iArg);
    Stz*  sz_PAY_FETCH_COLS(LogicUnit* pLogicUnit, i8 iArg);

    void  v_PAY_STORE_COLI(LogicUnit* pLogicUnit, i8 iArg, i8 iTop);
    void  v_PAY_STORE_COLS(LogicUnit* pLogicUnit, i8 iArg, Stz* azTop);

  private:
    Footer* aFooter(void);
  };
  
  DevIXT* _pDevIXT(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
