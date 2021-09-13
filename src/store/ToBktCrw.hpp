/*============================================================================*/
/* ToBktCrw.hpp -- Convert ptn nst row buffers to ptn crw buffers             */
/*============================================================================*/
#pragma once

#include "HashSet.hpp"

namespace STORE
{
  class ToBktCrw : public Object
  {
  private:
  
    Table*     m_pTable;
    StreamDef* m_pStreamDef;
    CrwBuffer* m_pCrwBuffer;

    HashSet    m_oHashSet;
  
    RowCompactor* m_paRowCompactor[C_MAX_PTNSINGRID]; // row compactors

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ToBktCrw(StreamDef* pStreamDef);
   ~ToBktCrw(void) noexcept(false);

    static void vRepairHashSet(Object* pObject);

    void vConvert(RowBuffer* pRowBuffer);
  
    CrwBuffer* pCompleted(i8 i_Ptn);
    CrwBuffer* pDangling(i8 i_Ptn);
  
  private:
    void vAddNst(RowBuffer* pRowBuffer);
    void vAddRowsToBuffer(i8 i_Ptn, RowBuffer* pRowBuffer);
    CrwBuffer* pNewCrwBuffer(i8 i_Ptn);

    Footer* aFooter(void);
  };

  ToBktCrw* _pToBktCrw(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
