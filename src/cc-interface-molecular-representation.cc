

#ifdef USE_MOLECULES_TO_TRIANGLES

#ifdef USE_PYTHON
#include <Python.h>  // before system includes to stop "POSIX_C_SOURCE" redefined problems
#endif

#include <cstddef>

// #include "globjects.h" //includes gtk/gtk.h
#include "graphics-info.h"
#include "c-interface.h"
#include "cc-interface-molecular-representation.hh"

#ifdef USE_PYTHON

// Martin's Triangles

// e.g. 0, "//C", "RampChainsScheme", "Ribbon"
int add_molecular_representation_py(int imol, PyObject *atom_selection_py, PyObject *ColorScheme_py, PyObject *style_py) {

   int status = -1;
   if (is_valid_model_molecule(imol)) {
#ifdef USE_MOLECULES_TO_TRIANGLES
      // check that these are strings
      std::string atom_selection = PyString_AsString(atom_selection_py);
      std::string ColorScheme    = PyString_AsString(ColorScheme_py);
      std::string style          = PyString_AsString(style_py);
      status = graphics_info_t::molecules[imol].add_molecular_representation(atom_selection, ColorScheme, style);
#endif
   }
   return status;

}

#ifdef USE_GUILE
// e.g. 0, "//C", "RampChainsScheme", "Ribbon"
int add_molecular_representation_scm(int imol, SCM atom_selection_scm, SCM ColorScheme_scm, SCM style_scm) {

   int status = -1;
   if (is_valid_model_molecule(imol)) {
#ifdef USE_MOLECULES_TO_TRIANGLES
      // check that these are strings
      std::string atom_selection = scm_to_locale_string(atom_selection_scm);
      std::string ColorScheme    = scm_to_locale_string(ColorScheme_scm);
      std::string style          = scm_to_locale_string(style_scm);
      std::cout << "Calling add_molecular_representation with " << atom_selection << " " << ColorScheme << "  " << style << std::endl;
      status = graphics_info_t::molecules[imol].add_molecular_representation(atom_selection, ColorScheme, style);
      graphics_draw();
#endif
   }
   return status;
}
#endif // USE_GUILE

void remove_molecular_represenation(int imol, int rep_no) {

}
#endif // USE_PYTHON

#endif // USE_MOLECULES_TO_TRIANGLES
