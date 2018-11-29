
#include "clipper/core/coords.h"

#include "geometry/residue-and-atom-specs.hh"
#include "helix-like.hh"


std::vector<clipper::Coord_orth>
coot::alpha_helical_reference_positions() {

      std::vector<clipper::Coord_orth> ref_pos;
      ref_pos.push_back(clipper::Coord_orth(0.370,   3.598,   2.351));
      ref_pos.push_back(clipper::Coord_orth(1.039,   3.821,   3.641));
      ref_pos.push_back(clipper::Coord_orth(2.491,   3.342,   3.566));
      ref_pos.push_back(clipper::Coord_orth(3.418,   4.049,   3.988));
      ref_pos.push_back(clipper::Coord_orth(2.634,   2.146,   3.027));
      ref_pos.push_back(clipper::Coord_orth(3.942,   1.496,   2.858));
      ref_pos.push_back(clipper::Coord_orth(4.862,   2.382,   2.015));
      ref_pos.push_back(clipper::Coord_orth(6.030,   2.606,   2.366));
      ref_pos.push_back(clipper::Coord_orth(4.295,   2.858,   0.922));
      ref_pos.push_back(clipper::Coord_orth(4.999,   3.730,  -0.029));
      ref_pos.push_back(clipper::Coord_orth(5.506,   4.984,   0.686));
      ref_pos.push_back(clipper::Coord_orth(6.667,   5.386,   0.524));
      ref_pos.push_back(clipper::Coord_orth(4.606,   5.561,   1.460));
      ref_pos.push_back(clipper::Coord_orth(4.881,   6.777,   2.238));
      ref_pos.push_back(clipper::Coord_orth(6.063,   6.539,   3.181));
      ref_pos.push_back(clipper::Coord_orth(6.987,   7.362,   3.267));
      ref_pos.push_back(clipper::Coord_orth(5.989,   5.409,   3.859));
      ref_pos.push_back(clipper::Coord_orth(7.017,   4.983,   4.820));
      ref_pos.push_back(clipper::Coord_orth(8.378,   4.895,   4.125));
      ref_pos.push_back(clipper::Coord_orth(9.391,   5.393,   4.639));
      return ref_pos;
}

coot::helical_results_t
coot::compare_to_helix(const std::vector<mmdb::Residue *> &helical_residues) {

   helical_results_t r;
   std::vector<clipper::Coord_orth> ref_pos = alpha_helical_reference_positions();
   if (helical_residues.size() == 5) {
      r = compare_to_helix(helical_residues, ref_pos);
   }
   return r;

}

void
coot::like_a_helix(mmdb::Manager *mol, int residue_selection_handle) {

   // return a vector of residues that have *forward* restraints i.e. O(n_this) - N(n_this+4)
   // 
   std::vector<mmdb::Residue *> helical_residues;

   mmdb::PResidue *SelResidues = 0; 
   int nSelResidues;
   mol->GetSelIndex(residue_selection_handle, SelResidues, nSelResidues);

   // ideal helix atom positions
   // N CA C O

   if (nSelResidues > 4) {

      std::vector<clipper::Coord_orth> ref_pos = alpha_helical_reference_positions();

      for (int istart=0; istart<(nSelResidues-5); istart++) {

	 std::vector<mmdb::Residue *> test_residues;
	 if ((istart+5) < nSelResidues) {
	    for (int i_5=istart; i_5<(istart+5); i_5++) {
               // std::cout << "pushing back " << residue_spec_t(SelResidues[i_5]) << std::endl;
	       test_residues.push_back(SelResidues[i_5]);
	    }

	    if (test_residues.size() == 5) {
	       coot::helical_results_t helicals = compare_to_helix(test_residues, ref_pos);
               std::cout << "Here with helicals.is_alpha_helix_like " << helicals.is_alpha_helix_like << std::endl;
	    }
	 }
      }
   }
}

// Compare this set of helical residue to the reference helical postions
// (jut one test)
//
coot::helical_results_t
coot::compare_to_helix(const std::vector<mmdb::Residue *> &helical_residues,
                       const std::vector<clipper::Coord_orth> &alpha_ref_positions) {

   double sum_delta_lim = 3.0; // beyond this we don't have a helix (sum delta limit).
                              // 2.5 is quite strict.

   helical_results_t r;
   std::vector<clipper::Coord_orth> match_set(20); // 5 x 4

   if (helical_residues.size() == 5) {
      int n_found = 0;
      for (unsigned int i=0; i<5; i++) {
         mmdb::PPAtom residue_atoms = NULL;
         int n_residue_atoms;
         mmdb::Residue *residue_p = helical_residues[i];
         residue_p->GetAtomTable(residue_atoms, n_residue_atoms);
         for (int iat=0; iat<n_residue_atoms; iat++) {
             int idx = -1;
             mmdb::Atom *at = residue_atoms[iat];
             std::string atom_name(at->name);
             if (atom_name == " N  ") idx = 0;
             if (atom_name == " CA ") idx = 1;
             if (atom_name == " C  ") idx = 2;
             if (atom_name == " O  ") idx = 3;
             if (idx != -1) {
                int idx_match_set = i*4 + idx;
                clipper::Coord_orth co(at->x, at->y, at->z);
                match_set[idx_match_set] = co;
                n_found++;
                // also count n_found_this (for this residue)
                //
             }
         }
      }
      if (n_found == 20) {
         clipper::RTop_orth rtop(alpha_ref_positions, match_set);
         double sum_delta = 0.0;
         for (unsigned int ii=0; ii<20; ii++) {
            clipper::Coord_orth moved_pos = rtop * alpha_ref_positions[ii];
            double dd = clipper::Coord_orth(match_set[ii]-moved_pos).lengthsq();
            sum_delta += sqrt(dd);
         }
         if (sum_delta < sum_delta_lim) {
            r.is_alpha_helix_like = true;
         }
      }
   }
   return r;
}
