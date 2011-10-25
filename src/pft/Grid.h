#ifndef GRID_H_
#define GRID_H_

#include "include/petsc.h"
#include "include/petscvec.h"
#include "include/petscmat.h"

#include "BoundaryCondition.h"
#include "Globals.h"
#include "GridCell.h"
#include "GridConnection.h"
#include "Source.h"
#include "StructuredGrid.h"
#include "SaturationFunction.h"

class Grid {
  
public:

  Grid();
  Grid(int nx, int ny, int nz, int fdof, int tdof);
  Grid(int nx, int ny, int nz, double dx, double dy, double dz, int fdof, int tdof);
  virtual ~Grid();

  void nullifyArrays();
  void setUp(int nx, int ny, int nz, double dx, double dy, double dz, 
             int fdof, int tdof);
  void setUp(int nx, int ny, int nz, int fdof, int tdof);
  void setUpConnectivity();
  void setUpMapping();
  void setUpCells();
  void printISLocalToGlobalMapping();
  void printAO();
  void printConnectivity();
  void printCells();
  void test(int **array);
  void test2(int ***array);
  void addBoundaryCondition(int is, int ie, int js, int je, int ks, int ke, 
                            char *face, char *type, double scalar);
  void setInitialHydrostaticPressure(double reference_pressure, 
                                     double datum);
  void addSource(int is, int ie, int js, int je, int ks, int ke, 
                 char *type, double scalar);

  void get1dofVectorNatural(Vec *v);
  void get1dofVectorLocal(Vec *v);
  void get1dofVectorGlobal(Vec *v);
  void getFdofMatrix(Mat *m, MatType mtype);
  void getFdofVectorGlobal(Vec *v);
  void getFdofVectorLocal(Vec *v);
  void getTdofVectorGlobal(Vec *v);
  void getTdofVectorLocal(Vec *v);
  void globalToNatural(Vec global, Vec natural);
  BoundaryCondition *getBoundaryConditions();
  Source *getSources();

  void initSaturationFunction(double theta_res, double theta_sat,
                              double alpha, double n);
  void updateRelativePermeability(double *pressure);

  int getNx();
  int getNy();
  int getNz();
  int getN();
  double getDx(int i);
  double getDy(int j);
  double getDz(int k);
  void zeroFlux();
  
  int num_cells;
  int num_nodes_local;
  int num_nodes_ghosted;
  int num_connections;
  int *mapping_local_to_ghosted;
  int *mapping_ghosted_to_local;
  GridCell *cells;
  GridConnection *connections;
  
  Vec pressure_vec;
  Vec density_vec;
//  Vec porosity_vec;
//  Vec saturation_vec;

private:
  PetscErrorCode ierr;
  StructuredGrid *structuredGrid;

};

#endif /*GRID_H_*/
