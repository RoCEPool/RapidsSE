/*============================================================================*/
/* RowCounts.hpp -- A count of row changes, for reporting to clients          */
/*============================================================================*/
#pragma once

#include "BASE.h"


namespace STORE
{
  class RowCounts : public Object
  {
    private:
      i8 m_nRowsProcessed;
      i8 m_nRowsSkipped;
      i8 m_nRowsInvalid;
      i8 m_nRowsInserted;
      i8 m_nRowsUpdated;
      i8 m_nRowsDeleted;

      // must be last
      Footer m_xFooter;

    public:
      void* operator new(size_t);
      void  operator delete(void*);

      RowCounts(void);
      ~RowCounts(void) noexcept(false);

      void vAddRowsProcessed(i8 nProcessed);
      void vAddRowsSkipped(i8 nSkipped);
      void vAddRowsInvalid(i8 nInvalid);
      void vAddRowsInserted(i8 nInserted);
      void vAddRowsUpdated(i8 nUpdated);
      void vAddRowsDeleted(i8 nDeleted);

      i8 nRowsProcessed(void);
      i8 nRowsSkipped(void);
      i8 nRowsInvalid(void);
      i8 nRowsInserted(void);
      i8 nRowsUpdated(void);
      i8 nRowsDeleted(void);

      static void vRepair(Object* pObject);


    private:
      Footer* aFooter(void);

  };

  RowCounts* _pRowCounts(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
