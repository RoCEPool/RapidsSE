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
  
    bool (*m_fpOptFun)(Optimiser* pOptimiser);
 
    Stz* m_szDescription; // description of the command

  public:
    OptimiserFun(Stz* szName,
                 Stz* szCommand1,
                 Stz* szCommand2,
                 bool (*fpOptFun)(Optimiser* pOptimiser),
                 Stz* szDescription
                 );
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
