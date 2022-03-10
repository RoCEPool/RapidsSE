/*============================================================================*/
/* OptimiserFun.cpp -- description of an optimiser function                   */
/*============================================================================*/

#include "OptimiserFun.hpp"
#include "OptimiserCatalog.hpp"

extern OptimiserCatalog gxOptimiserCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OptimiserFun::OptimiserFun(Stz* szName,
                           Stz* szCommand1,
                           Stz* szCommand2,
                           bool (*fpOptFun)(Optimiser* pOptimiser,
                                            TokenVec*  pTokenVec,
                                            i8&        riAction),
                           i8   iSpare0,
                           i8   iSpare1,
                           Stz* szDescription,
                           Stz* szUsage
                           )
{
  m_szName        = szName;
  m_szCommand1    = szCommand1;
  m_szCommand2    = szCommand2;
  m_fpOptFun      = fpOptFun;
  m_iSpare0       = iSpare0;
  m_iSpare1       = iSpare1;
  m_szDescription = szDescription;
  m_szUsage       = szUsage;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // add the function to the catalog
  i8 i_Fun = gxOptimiserCatalog.m_nFunctions++;
  gxOptimiserCatalog.m_aaFun[i_Fun] = this;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
