/*============================================================================*/
/* Datums.cpp -- datums (single units of data)                                */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void Datums::vCheckAssumptions(void)
{
#ifdef _DEBUG
  vTrace("LLONG_MAX = %lld\n"_Stz,LLONG_MAX);
  vTrace("LLONG_MAX = %p\n"_Stz,LLONG_MAX);
  vTrace("LLONG_MIN = %lld\n"_Stz,LLONG_MIN);
  vTrace("LLONG_MIN = %p\n"_Stz,LLONG_MIN);

  vTrace("DBL_MAX %%e = %e\n"_Stz,DBL_MAX);
  vTrace("DBL_MIN %%e = %e\n"_Stz,DBL_MIN);
  vTrace("DBL_MAX %%f = %f\n"_Stz,DBL_MAX);
  vTrace("DBL_MIN %%f = %f\n"_Stz,DBL_MIN);
  vTrace("DBL_MAX %%g = %g\n"_Stz,DBL_MAX);
  vTrace("DBL_MIN %%g = %g\n"_Stz,DBL_MIN);
  
  vTrace("C_I8_MAX = %lld\n"_Stz,C_I8_MAX);
  vTrace("(u8)C_I8_MAX = %llu\n"_Stz,(u8)C_I8_MAX);
  
  vInfo("C_NULL_FLT %e\n"_Stz,  C_NULL_FLT);
  vInfo("C_NULL_INT %lld\n"_Stz,C_NULL_INT);
  vInfo("C_NULL_VCH %lld\n"_Stz,C_NULL_VCH);
#endif
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
