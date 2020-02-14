/* Author : havep at Mon Jun 30 17:55:25 2008
 * Generated by createNew
 */


#include <ALIEN/Kernels/PETSc/LinearSolver/MUMPS/PETScPrecConfigMUMPSService.h>
#include <ALIEN/axl/PETScPrecConfigMUMPS_StrongOptions.h>

/* Pour debugger le ILU, utiliser l'option:
 * <cmd-line-param>-mat_superlu_printstat 1 </cmd-line-param>
 */

/*---------------------------------------------------------------------------*/
#if ((PETSC_VERSION_MAJOR == 3 && PETSC_VERSION_MINOR >= 3) || (PETSC_VERSION_MAJOR > 3))
/*---------------------------------------------------------------------------*/

namespace Alien {

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/** Constructeur de la classe */
#ifdef ALIEN_USE_ARCANE
PETScPrecConfigMUMPSService::PETScPrecConfigMUMPSService(
    const Arcane::ServiceBuildInfo& sbi)
: ArcanePETScPrecConfigMUMPSObject(sbi)
, PETScConfig(sbi.subDomain()->parallelMng()->isParallel())
{
  ;
}
#endif

PETScPrecConfigMUMPSService::
PETScPrecConfigMUMPSService(IParallelMng* parallel_mng, std::shared_ptr<IOptionsPETScPrecConfigMUMPS> options)
  : ArcanePETScPrecConfigMUMPSObject(options)
  , PETScConfig(parallel_mng->isParallel())
{
  ;
}

//! Initialisation
void
PETScPrecConfigMUMPSService::configure(PC & pc,
                                         const ISpace& space,
                                         const MatrixDistribution& distribution)
{
  alien_debug([&] {
      cout() << "configure PETSc mumps preconditioner";
    });
  checkError("Set preconditioner",PCSetType(pc,PCLU));

  checkError("Set mumps solver package",PCFactorSetMatSolverPackage(pc,MATSOLVERMUMPS));

  // checkError("Set fill factor",  PCFactorSetUpMatSolverPackage(pc));
  double fill_factor = options()->fillFactor();
  if (fill_factor < 1.0)
    PETScConfig::traceMng()->fatal() << "Bad Fill Factor: cannot be less than 1.0";

  checkError("Set fill factor",PCFactorSetFill(pc, fill_factor));
  // checkError("Set shift type",PCFactorSetShiftType(pc,PETSC_DECIDE));
  checkError("Set shift amount",PCFactorSetShiftAmount(pc, PETSC_DECIDE));

  checkError("Preconditioner setup",PCSetUp(pc));
}

/*---------------------------------------------------------------------------*/

ARCANE_REGISTER_SERVICE_PETSCPRECCONFIGMUMPS(MUMPS,PETScPrecConfigMUMPSService);

} // namespace Alien

REGISTER_STRONG_OPTIONS_PETSCPRECCONFIGMUMPS();
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#endif /* PETSC_VERSION */


