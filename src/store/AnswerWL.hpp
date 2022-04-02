/*============================================================================*/
/* AnswerWL.hpp -- Wire Line Answer                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "ApiHelper.hpp"

namespace STORE
{
  class AnswerWL : public Object
      , public ApiHelper
      {
      private:

        // must be last
        Footer m_xFooter;

      public:
        void* operator new(size_t);
        void  operator delete(void*);

        AnswerWL(void);
        ~AnswerWL(void) noexcept(false);

        static void vRepair(Object* pObject);

        void vProcessEventA(void);
        void vAction1(void);
        void vAction2(void);

      private:

        Footer* aFooter(void);

      };

  AnswerWL* _pAnswerWL(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
