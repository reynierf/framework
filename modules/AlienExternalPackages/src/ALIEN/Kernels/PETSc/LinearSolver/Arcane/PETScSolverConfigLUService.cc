/* Author : desrozis at Mon Jan 14 10:00:00 2019
 * Generated by createNew
 */

#include <ALIEN/Kernels/PETSc/LinearSolver/Arcane/PETScSolverConfigLUService.h>
#include <ALIEN/axl/PETScSolverConfigLU_StrongOptions.h>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Alien {

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/** Constructeur de la classe */

#ifdef ALIEN_USE_ARCANE
PETScSolverConfigLUService::PETScSolverConfigLUService(
    const Arcane::ServiceBuildInfo& sbi)
: ArcanePETScSolverConfigLUObject(sbi)
, PETScConfig(sbi.subDomain()->parallelMng()->isParallel())
{}
#endif

PETScSolverConfigLUService::PETScSolverConfigLUService(
    Arccore::MessagePassing::IMessagePassingMng* parallel_mng,
    std::shared_ptr<IOptionsPETScSolverConfigLU> options)
: ArcanePETScSolverConfigLUObject(options)
, PETScConfig(parallel_mng->commSize() > 1)
{}

//! Initialisation
void
PETScSolverConfigLUService::
configure(KSP & ksp,
	  const ISpace& space,
	  const MatrixDistribution& distribution)
{
  alien_debug([&] {
    cout() << "configure PETSc lu solver";
  });
 
  checkError("Set solver tolerances",KSPSetTolerances(ksp,
						      1e-9,
						      1e-15,
						      PETSC_DEFAULT,
						      2));

  checkError("Solver set type",KSPSetType(ksp,KSPPREONLY));
  
  PC pc;
  checkError("Get preconditioner",KSPGetPC(ksp,&pc));
  checkError("Preconditioner set type",PCSetType(pc,PCLU));
  
  if (isParallel()) {
    alien_fatal([&] {
      cout() << "PETSc LU is not available in parallel";
    });
  }

  checkError("Set lu solver package", PCFactorSetMatSolverType(pc, MATSOLVERPETSC));

  KSPSetUp(ksp);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_REGISTER_SERVICE_PETSCSOLVERCONFIGLU(LU,PETScSolverConfigLUService);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Alien

REGISTER_STRONG_OPTIONS_PETSCSOLVERCONFIGLU();

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
