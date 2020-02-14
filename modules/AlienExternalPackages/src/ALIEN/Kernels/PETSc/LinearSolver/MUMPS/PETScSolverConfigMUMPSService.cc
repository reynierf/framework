/* Author : havep at Mon Jun 30 16:04:32 2008
 * Generated by createNew
 */



#include <ALIEN/Kernels/PETSc/LinearSolver/MUMPS/PETScSolverConfigMUMPSService.h>
#include <ALIEN/axl/PETScSolverConfigMUMPS_StrongOptions.h>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace Alien {
/** Constructeur de la classe */
#ifdef ALIEN_USE_ARCANE
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
PETScSolverConfigMUMPSService::PETScSolverConfigMUMPSService(
    const Arcane::ServiceBuildInfo& sbi)
: ArcanePETScSolverConfigMUMPSObject(sbi)
, PETScConfig(sbi.subDomain()->parallelMng()->isParallel())
{
  ;
}
#endif
PETScSolverConfigMUMPSService::
PETScSolverConfigMUMPSService(IParallelMng* parallel_mng, std::shared_ptr<IOptionsPETScSolverConfigMUMPS> options)
  : ArcanePETScSolverConfigMUMPSObject(options)
  , PETScConfig(parallel_mng->isParallel())
{}


void
PETScSolverConfigMUMPSService::configure(KSP & ksp,
                const ISpace& space,
                const MatrixDistribution& distribution)
{
        alien_debug([&] {
                cout() << "configure PETSc mumps solver";
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


        PCFactorSetMatSolverPackage(pc,MATSOLVERMUMPS);

        KSPSetUp(ksp);

#ifdef TEST
        Mat F;
        PCFactorGetMatrix(pc,&F);

        /* sequential ordering */
        PetscInt  ival,icntl;
        icntl = 7; ival = 2;
        MatMumpsSetIcntl(F,icntl,ival);

        /* threshhold for row pivot detection */
        MatMumpsSetIcntl(F,24,1);
        PetscReal  val = 1.e-6;
        icntl = 3;
        MatMumpsSetCntl(F,icntl,val);
#endif

}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_REGISTER_SERVICE_PETSCSOLVERCONFIGMUMPS(MUMPS,PETScSolverConfigMUMPSService);

} // namespace Alien

REGISTER_STRONG_OPTIONS_PETSCSOLVERCONFIGMUMPS();
