/*============================================================================*/
/* DevRSB.hpp -- RSB Device                                                   */
/*============================================================================*/
#pragma once

namespace Expressions
{
  class DevRSB : public Device
  {
  public:
  
    s256       m_sTableNm;
    i8         m_i_Row;
    i8         m_nRows;
    Rsb*       m_aRsb;
    RowSlab*   m_pRowSlab;
    StreamDef* m_pStreamDef;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    DevRSB(void);
   ~DevRSB(void) noexcept(false);
  
    void  v_ENTWRK_APPENDELM(LogicUnit* pLogicUnit);
    void  v_ENTWRK_APPENDELMI(LogicUnit* pLogicUnit, i8 iTop);
    void  v_ENTWRK_APPENDELMS(LogicUnit* pLogicUnit, Stz* azTop);


  private:
    Footer* aFooter(void);
  };
  
  DevRSB* _pDevRSB(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
