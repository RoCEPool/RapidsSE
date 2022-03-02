/*============================================================================*/
/* OptimiserFun.hpp -- description of an optimiser function                   */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE { class Optimiser; }

namespace STORE
{
  class OptimiserFun
  {
  public:
    Stz* m_szName;        // name of the function
    Stz* m_szCommand1;    // first word of command
    Stz* m_szCommand2;    // optional second word of command
  
    bool (*m_fpOptFun)(Optimiser* pOptimiser,
                       TokenVec*  pTknVec,
                       i8&        riAction);
  
    i8   m_iSpare0;
    i8   m_iSpare1;
    Stz* m_szDescription; // description of the command

  public:
    OptimiserFun(Stz* szName,
                 Stz* szCommand1,
                 Stz* szCommand2,
                 bool (*fpOptFun)(Optimiser* pOptimiser,
                                  TokenVec*  pTokenVec,
                                  i8&        riAction),
                 i8   iSpare0,
                 i8   iSpare1,
                 Stz* szDescription
                 );
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
