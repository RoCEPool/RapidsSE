/*============================================================================*/
/* TypeMap1D.hpp - A class for mapping input to output types for expressions  */
/*============================================================================*/
#pragma once

#include "TypeId.hpp"

namespace Parser
{
  class TypeMap1D : Object
  {
    private:
    i8  m_iSize;
    i1* m_aiTypeArray;

    // must be last
    Footer m_xFooter;

    public:
    void *operator new(size_t);
    void operator delete(void *);

    TypeMap1D(i8 iInvalidTypeId);
    ~TypeMap1D();

    // Add a mapping of one input operand type to one output type
    TypeMap1D* pAddMapping(TypeId xArg, TypeId xResult);

    // Lookup a mapping of one input operand type to one output type
    TypeId     xLookup(TypeId xArg);

    Footer *aFooter(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
