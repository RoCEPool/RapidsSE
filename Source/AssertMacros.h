/*============================================================================*/
/* AssertMacros.hpp -- Assert Macro Definitions                               */
/*============================================================================*/
#pragma once


/*----------------------------------------------------------------------------*/
/* check the object is within the pool                                        */
/*----------------------------------------------------------------------------*/
#ifdef WITH_ADDRESS_CHECKING
  #define _poolAddress_(pArg) Correctness::vCheckAddress(pArg)
#else
  #define _poolAddress_(pArg)
#endif

/*----------------------------------------------------------------------------*/
/* ASSERT macro                                                               */
/*----------------------------------------------------------------------------*/
#ifdef WITH_ASSERT_MACRO
  #define ASSERT(condition) \
    if (! (condition)) \
      Correctness::vThrow(FK_ASSERT,__func__,__FILE__,__LINE__,(Stz*)#condition);
#else
  #define ASSERT(condition)
#endif

/*----------------------------------------------------------------------------*/
/* RASSERT macro                                                              */
/*----------------------------------------------------------------------------*/
#define RASSERT(condition) \
  if (! (condition)) \
    Correctness::vThrow(FK_ASSERT,__func__,__FILE__,__LINE__,(Stz*)#condition);

/*----------------------------------------------------------------------------*/
/* RASSERT_TYPE macro                                                         */
/*----------------------------------------------------------------------------*/
#define RASSERT_TYPE(pARG,eCLASS) \
  if (pARG && eCLASS != pARG->eClass()) \
    Correctness::vTypeFatal(pARG,eCLASS); 

/*----------------------------------------------------------------------------*/
/* THROW macro                                                                */
/*----------------------------------------------------------------------------*/
#define THROW(szDescription) \
  Correctness::vThrow(FK_FAILED,__func__,__FILE__,__LINE__,szDescription);

/*----------------------------------------------------------------------------*/
/* NEVER_REACH macro (point in code which should never be reached             */
/*----------------------------------------------------------------------------*/
#define NEVER_REACH \
  Correctness::vThrow(FK_REACHED,__func__,__FILE__,__LINE__, \
                      (Stz*)"NEVER_REACH point reached");


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
