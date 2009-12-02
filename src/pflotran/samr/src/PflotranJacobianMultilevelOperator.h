#ifndef included_PflotranJacobianMultilevelOperator
#define included_PflotranJacobianMultilevelOperator


#ifndef included_Array
#define included_Array
#include "tbox/Array.h"
#endif

#ifdef DEBUG_CHECK_ASSERTIONS
extern "C"{
#include "assert.h"
}
#endif

#include <vector>
#include "RefineOperator.h"
#include "CoarsenOperator.h"
#include "SAMRAIVectorReal.h" 
#include "MultilevelOperator.h"
#include "PflotranJacobianLevelOperator.h"
#include "BoundaryConditionStrategy.h"
#include "CCellVariable.h"
#include "CSideVariable.h"
#include "HierarchyDataOpsReal.h"
#include "CoarsenSchedule.h"

extern "C" {
#include "petscmat.h"
}

namespace SAMRAI{

class PflotranJacobianMultilevelOperator: public SAMRSolvers::MultilevelOperator
{
public:

  PflotranJacobianMultilevelOperator(SAMRSolvers::MultilevelOperatorParameters *parameters);

   ~PflotranJacobianMultilevelOperator();

   /**
    * Compute forward apply operation on a level, with the default arguments for a and b
    * the residual, r=f-Au, should be calculated, else b*f+a*A*u is calculated.
    *  \param ln
    *            level number for which apply() should be done
    * \param f_id
    *            array of integer id's for right hand side
    * \param u_id
    *            array of integer id's for solution variables
    * \param r_id
    *            array of integer id's for residual
    * \param f_idx
    *            Array of integer id's for components of right hand side.
    *            These must correspond to the integer id's supplied in f_id
    * \param u_idx
    *            Array of integer id's for components of solution variables.
    *            These must correspond to the integer id's supplied in u_id
    * \param r_idx
    *            Array of integer id's for components of residual.
    *            These must correspond to the integer id's supplied in r_id
    * \param a
    *            Double scaling factor for A*u, default of -1.0
    * \param b
    *            Double scaling factor for rhs, default of 1.0
    */
   void apply(const int ln,
              const int *f_id,
              const int *u_id, 
              const int *r_id,
              const int *f_idx=NULL,
              const int *u_idx=NULL,
              const int *r_idx=NULL,
              const double a = -1.0,
              const double b = 1.0);

   /**
    * Compute forward apply operation on a range of levels, with the default arguments for a and b
    * the residual, r=f-Au, should be calculated, else b*f+a*A*u is calculated.
    *  \param coarse_ln
    *            lower bound on levels for which apply() should be done
    *  \param fine_ln
    *            upper bound on levels for which apply() should be done
    * \param f_id
    *            array of integer id's for right hand side
    * \param u_id
    *            array of integer id's for solution variables
    * \param r_id
    *            array of integer id's for residual
    * \param f_idx
    *            Array of integer id's for components of right hand side.
    *            These must correspond to the integer id's supplied in f_id
    * \param u_idx
    *            Array of integer id's for components of solution variables.
    *            These must correspond to the integer id's supplied in u_id
    * \param r_idx
    *            Array of integer id's for components of residual.
    *            These must correspond to the integer id's supplied in r_id
    * \param a
    *            Double scaling factor for A*u, default of -1.0
    * \param b
    *            Double scaling factor for rhs, default of 1.0
    */
   void apply(const int coarse_ln,
              const int fine_ln,
              const int *f_id,
              const int *u_id, 
              const int *r_id,
              const int *f_idx=NULL,
              const int *u_idx=NULL,
              const int *r_idx=NULL,
              const double a = -1.0,
              const double b = 1.0);

   /**
   * Apply a boundary condition for a subset of variable, index pairs. Periodic boundaries
   * will be automatically detected and set.
   * \pre Initialization of the boundary condition types for the different variables
   *      should be done when the constructor is called.
   * \param var_id
   *         Variable descriptor index 
   * \param var_idx
   *         Component index
   * \param var_components
   *         We assume that the order of the descriptor indices supplied in var_id and
   *         the corresponding component indices supplied in var_idx need not correspond 
   *         with the order of the variables assumed within the discrete operator. For example
   *         the discrete equations may be ax+by=f, cx+dy=g. Within the operator, x is considered
   *         to be the first variable and y the second variable. However, var_id[0] may correspond
   *         y and var_id[1] to x. In which case var_components[0]=1 and var_components[1]=0
   *         var_components specifies the mapping from (var_id, var_idx) to the order assumed
   *         within the discrete operator.
   * \param number_of_variables
   *         For scalars the default is always true, for systems, in cases where it is desirable
   *         to call applyBoundaryCondition() for a subset of the variables, this specifies the
   *         number of variables for which we are calling applyBoundaryCondition().
   * \param reset_ghost_values
   *          boolean specifying whether interpolation from coarser levels and caching
   *          internally of the data is required before applying boundary conditions, default
   *          is false to minimize interpolations
   */
   void applyBoundaryCondition(const int ln,
                               const int *var_id,
                               const int *var_idx=NULL,
                               const int *var_components=NULL,
                               const int number_of_variables=-1,
                               const bool reset_ghost_values = false);
  
   /**
   * Interpolate source data on a coarser level to destination data on finer level
   * using scratch locations for intermediate calculations. This function is part
   * of the linear operator class because interpolations can require data that is
   * available only to the discrete linear operator, eg, boundary condition data.
   * \param flevel
   *        reference to pointer for fine level
   * \param clevel
   *        reference to pointer for coarse level
   * \param dst_id
   *        pointer to array of descriptor indices for destination data
   * \param src_id
   *        pointer to array of descriptor indices for source data
   * \param scratch_id
   *        pointer to array of descriptor indices for scratch data
   * \param refine_op
   *        name of refine operator to use
   */
   virtual void interpolate(const tbox::Pointer<hier::PatchLevel<NDIM> > &flevel,
                            const tbox::Pointer<hier::PatchLevel<NDIM> > &clevel,
                            const int *dst_id,
                            const int *src_id,
                            const int *scratch_id,
                            std::string &refine_op);

   /**
   * Returns the number of primitive variables for the discretization
   */
   const int getNumberOfVariables(void){ return d_ndof; }
   
   static PetscErrorCode wrapperMatMult(Mat mat,Vec x,Vec y);

   PetscErrorCode MatMult(Mat mat,Vec x,Vec y);

   static PetscErrorCode wrapperMatZeroEntries(Mat mat);

   PetscErrorCode MatZeroEntries(Mat mat);

   static PetscErrorCode wrapperMatSetValuesLocal(Mat mat,
                                                  PetscInt nrow,const PetscInt irow[],
                                                  PetscInt ncol,const PetscInt icol[],
                                                  const PetscScalar y[],InsertMode addv);
      
   PetscErrorCode MatSetValuesLocal(Mat mat,
                                    PetscInt nrow,const PetscInt irow[],
                                    PetscInt ncol,const PetscInt icol[],
                                    const PetscScalar y[],InsertMode addv);
      
   static PetscErrorCode wrapperMatSetValuesBlockedLocal(Mat mat,
                                                         PetscInt nrow,const PetscInt irow[],
                                                         PetscInt ncol,const PetscInt icol[],
                                                         const PetscScalar y[],InsertMode addv);
      
   PetscErrorCode MatSetValuesBlockedLocal(Mat mat,
                                           PetscInt nrow,const PetscInt irow[],
                                           PetscInt ncol,const PetscInt icol[],
                                           const PetscScalar y[],InsertMode addv);
      
   static PetscErrorCode wrapperMatDiagonalScaleLocal(Mat mat,Vec diag);

   PetscErrorCode MatDiagonalScaleLocal(Mat mat,Vec diag);

   static PetscErrorCode wrapperMatDiagonalScale(Mat mat, Vec l, Vec r);  

   PetscErrorCode MatDiagonalScale(Mat mat, Vec l, Vec r);

   tbox::Pointer<hier::PatchHierarchy<NDIM> > getHierarchy(void){ return d_hierarchy; }

   SAMRSolvers::LevelOperator *getLevelOperator(const int ln){ return d_level_operators[ln];}

   void setCurrentPatch(hier::Patch <NDIM> *patch){d_patch = patch;}

   int getVariableIndex(std::string &name, 
                        tbox::Pointer<hier::VariableContext> &context,
                        tbox::Pointer<hier::Variable<NDIM> > &var,
                        hier::IntVector<NDIM> nghosts = hier::IntVector<NDIM>(0),
                        int depth=1,
                        bool bOverride=false,
                        std::string centering="");

   void setSourceValueOnPatch(SAMRAI::hier::Patch<NDIM> **patch, int *index, double *val);

   void setSrcCoefficientsOnPatch(SAMRAI::hier::Patch<NDIM> **patch);

protected:

   void initializePetscMatInterface(void);
   /**
   * Allocates space for the internally used face centered flux
   */
   void initializeInternalVariableData(void);

   void getFromInput(tbox::Pointer<tbox::Database> db);

   /**
   * Routine used to set up internal transfer schedules
   */
   void setupTransferSchedules(void);

   void initializeBoundaryConditionStrategy(tbox::Pointer<tbox::Database> &db);

   Vec getScratchVector(void){ return d_scratch_vector; }

   void initializeScratchVector( Vec x);

   /**
   * Compute fluxes.
   *  \param coarse_ln
   *            lower bound on levels for which setFlux() should be called
   *  \param fine_ln
   *            upper bound on levels for which setFlux() should be called
   * \param u_id
   *        descriptor index for cell centered variable to calculate flux from
   * \param u_idx
   *        component index for cell centered variable to calculate flux from
   */

   void setFlux(const int coarse_ln,
                const int fine_ln,
                const int *u_id,
                const int *u_idx=NULL);

   /**
   * Coarsen a souce term like variable from level ln+1 to level ln
   * currently this routine is identical to coarsenVariable but that may
   * change
   * \param ln
   *        level number
   * \param u_id
   *        descriptor index of solution to coarsen
   * \param src_id
   *        descriptor index of rhs to coarsen
   * \param b
   *        boolean to determine if b should be coarsened or not
   */
   void coarsenSolutionAndSourceTerm(const int ln, 
                                     const int u_id,
                                     const int src_id,
                                     const bool coarsen_soln,
                                     const bool coarsen_rhs);



private:

   PflotranJacobianMultilevelOperator();


   bool d_adjust_cf_coefficients;
   bool d_coarsen_diffusive_fluxes;
   bool d_schedules_initialized;
   bool d_use_cf_interpolant;
   bool d_variable_order_interpolation;
   bool d_reset_ghost_values;

   std::string d_face_coarsen_op_str;
   std::string d_face_refine_op_str;
   std::string d_cell_soln_coarsen_op_str;
   std::string d_cell_src_coarsen_op_str;
   std::string d_cell_refine_op_str;

   int d_flux_id;
   int d_srcsink_id;

   int d_bdry_types[2*NDIM];

   int d_ndof;

   int d_stencilSize;

   /*
    * Variables.
    */
   tbox::Pointer< pdat::CCellVariable<NDIM,double> > d_scratch_variable;
   tbox::Pointer< pdat::CCellVariable<NDIM,double> > d_srcsink;
   tbox::Pointer< pdat::CSideVariable<NDIM,double> > d_flux;

   hier::Patch <NDIM> *d_patch;

   tbox::Pointer<xfer::RefineOperator<NDIM> >  d_soln_refine_op;
   tbox::Pointer<xfer::CoarsenOperator<NDIM> > d_soln_coarsen_op;
   tbox::Pointer<xfer::CoarsenOperator<NDIM> > d_src_coarsen_op;

   tbox::Array< tbox::Pointer< xfer::RefineSchedule<NDIM> > > d_GlobalToLocalRefineSchedule;
   tbox::Array< tbox::Pointer<xfer::RefineSchedule<NDIM> > > d_interpolate_schedule;

   tbox::Array< tbox::Pointer< xfer::CoarsenSchedule<NDIM> > > d_src_coarsen_schedule;
   tbox::Array< tbox::Pointer< xfer::CoarsenSchedule<NDIM> > > d_soln_coarsen_schedule;
   tbox::Array< tbox::Pointer< xfer::CoarsenSchedule<NDIM> > > d_flux_coarsen_schedule;

   RefinementBoundaryInterpolation::InterpolationScheme d_tangential_interp_scheme;
   RefinementBoundaryInterpolation::InterpolationScheme d_normal_interp_scheme;

   BoundaryConditionStrategy  *d_refine_patch_strategy;

   tbox::Array<PflotranJacobianLevelOperator*> d_level_operators;

   Mat *d_pMatrix;

   Vec d_scratch_vector;

   tbox::Pointer< math::HierarchyDataOpsReal< NDIM, double > > d_math_op;

};


}

#endif //included_PflotranJacobianMultilevelOperator
