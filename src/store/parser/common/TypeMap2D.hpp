/*============================================================================*/
/* TypeMap2D.hpp - A class for mapping input to output types for expressions  */
/*============================================================================*/
#pragma once

#include "TypeId.hpp"

namespace Parser
{
  class TypeMap2D : Object
  {
    private:
    i8  m_iSize;
    i1* m_aiTypeArray;

    // must be last
    Footer m_xFooter;

    public:
    void *operator new(size_t);
    void operator delete(void *);

    TypeMap2D(i8 iInvalidTypeId);
    ~TypeMap2D();

    // Add a mapping of two input operand types to one output type
    TypeMap2D* pAddMapping(TypeId xArg1, TypeId xArg2, TypeId xResult);

    // Lookup a mapping of two input operand types to one output type
    TypeId     xLookup(TypeId xArg1, TypeId xArg2);

    Footer *aFooter(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
