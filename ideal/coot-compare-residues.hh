

namespace coot {
   
   bool
   compare_residue_torsions(int imol_1, mmdb::Manager *mol1, mmdb::Residue *res_1,
			    int imol_2, mmdb::Manager *mol2, mmdb::Residue *res_2,
			    double tollerance, // in degrees
			    coot::protein_geometry *geom_p);

   // return true if the atom names of the torsions match
   // 
   // if there are no torsionable bonds, return true
   bool compare_residue_torsion_atom_names(const std::vector<torsion_atom_quad> &tqv_1,
					   const std::vector<torsion_atom_quad> &tqv_2);
   
   
}

