/*============================================================================*/
/* ToLglPtnCrw.hpp -- Convert lglptn nst row buff to lglptn crw buff          */
/*============================================================================*/
#pragma once

#include "HashSet.hpp"

namespace STORE
{
  class ToLglPtnCrw : public Object
  {
  private:
  
    Table*     m_pTable;
    StreamDef* m_pStreamDef;
    CrwBuffer* m_pCrwBuffer;

    HashSet    m_oHashSet;
  
    RowCompactor* m_paRowCompactor[C_MAX_LGLPTNS]; // row compactors

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ToLglPtnCrw(StreamDef* pStreamDef);
   ~ToLglPtnCrw(void) noexcept(false);

    static void vRepairHashSet(Object* pObject);

    void vConvert(RowBuffer* pRowBuffer);
  
    CrwBuffer* pCompleted(i8 i_Ptn);
    CrwBuffer* pDangling(i8 i_Ptn);
  
  private:
    void vAddNst(RowBuffer* pRowBuffer);
    void vAddRowsToBuffer(PtnId tPtnId, RowBuffer* pRowBuffer);
    CrwBuffer* pNewCrwBuffer(PtnId tPtnId);
    CrwBuffer* pNewCrwBuffer(PtnId tPtnId, i8 iBkt);

    Footer* aFooter(void);
  };

  ToLglPtnCrw* _pToLglPtnCrw(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
