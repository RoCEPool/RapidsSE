/*============================================================================*/
/* IndexStream.hpp -- Index Stream                                            */
/*============================================================================*/
#pragma once

#include "eRA.h"
#include "eRO.h"

#include "IndexDef.hpp"

#include "Pay.hpp"
#include "Key.hpp"
#include "Ent.hpp"

namespace STORE
{
  class IndexStream : public Object
  {
  private:
    TablePtn*  m_pTablePtn;
    Index*     m_pIndex;
    EntBuffer* m_pEntBuffer;
    Ent*       m_aLowerEntry;
    Ent*       m_aUpperEntry;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IndexStream(TablePtn* pTablePtn, Index* pIndex);
   ~IndexStream(void) noexcept(false);

    static void vRepair(Object* pObject, eRF eRepair);

    void vClose(void);
    bool bDelete(void);
    bool bOpen(void);
    CrwBuffer* pRead_idxstm(RowOnce* pRowOnce);
    void vReset(void);
    void vSeek(eRO eLowerRelnOp, Stz* szLowerKey, eRO eUpperRelnOp, Stz* szUpperKey);

  //private:
    Footer* aFooter(void);
  };
  
  IndexStream* _pIndexStream(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
