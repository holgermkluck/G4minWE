**G4minWE** is a _minimal working example_ of a Gean4 simulation. It is intended for educational purposes: to show the basic structure of a minimal but valid Geant4 program. It can also serve as a starting point for more elaborated programs.

To demonstrate the development from the most basic code to slightly elaborated code, this repository features certain _development stages_ as dedicated branches:
  - **stage_0** the code consists just of a main file and the CMake files needed to compile it. The produced binary can be executed but does not make anything.
  
## Build and install G4minWE

G4minWE was successfully built, installed and run under CentOS7. It may also work on other linux(-like) systems but this is not guaranteed and can not be supported.

To build G4minWE from source, one needs [Geant4](https://geant4.cern.ch) version 10.6.3 with the following options: `-DCMAKE_BUILD_TYPE="RelWithDebInfo" -DGEANT4_BUILD_MULTITHREADED=OFF -DGEANT4_BUILD_CXXSTD=c++14 -DGEANT4_USE_GDML=ON -DGEANT4_USE_QT=ON -DGEANT4_USE_OPENGL_X11=ON -DGEANT4_USE_RAYTRACER_X11=ON -DGEANT4_USE_XM=ON -DGEANT4_INSTALL_DATA=ON -Dfail-on-missing=ON -DGEANT4_USE_G3TOG4=OFF -DGEANT4_USE_INVENTOR=OFF -DGEANT4_USE_SYSTEM_CLHEP=OFF -DGEANT4_USE_SYSTEM_EXPAT=ON -DGEANT4_USE_SYSTEM_ZLIB=ON -DBUILD_SHARED_LIBS=ON -DGEANT4_BUILD_STORE_TRAJECTORY=ON -DGEANT4_BUILD_VERBOSE_CODE=ON -DGEANT4_ENABLE_TESTING=OFF -DGEANT4_USE_NETWORKDAWN=OFF -DGEANT4_USE_NETWORKVRML=OFF -DGEANT4_USE_FREETYPE=ON -DGEANT4_USE_HDF5=OFF -DGEANT4_USE_USOLIDS=OFF -DGEANT4_USE_TIMEMORY=OFF` for Geant4's prerequisites see [here](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/BackupVersions/V10.6c/html/gettingstarted.html#os-software-prerequisites).

The source code of this program is maintained on [G4minWE's Git repository](https://gitlab.tuwien.ac.at/holger.kluck/g4minwe). To clone it into the directory `<source>`, do:

	git clone https://gitlab.cern.ch/cresst_cosinus/ImpCRESST.git <source>
	cd <source>
	git checkout -b <branch>

and check out the branch `<branch>` e.g. `stage_0`.

The following assumes that G4minWE should be installed under `<install>`. One will also need a temporary build directory, e.g. `<build>`; it can be deleted after the installation. Then execute the following shell commands:

    mkdir <build>
    cd <build>
    cmake3 -DCMAKE_INSTALL_PREFIX=<install> <source>
    cmake3 --build . --target install -j<number of cores>
    export LD_LIBRARY_PATH=<install>/lib:$LD_LIBRARY_PATH
    cd ..
    rm -r <build>

Here, `<number of cores>` is the number of CPU cores assigned to the compilation process. One may add the line starting with `export LD_LIBRARY_PATH` to one's shell configuration files (e.g. `.bashrc`, `.zshrc`, etc.), otherwise one has to repeat it in each new shell instance.

## Contact and support

If further information or support is needed, one may contact the maintainers of G4minWE by writing to [holger.kluck@oeaw.ac.at](mailto:holger.kluck@oeaw.ac.at).

## Acknowledgement

G4minWE is based on [Geant4](https://geant4.cern.ch):

>J. Allison et al., Nucl. Instrum. Methods. Phys. Res. A835 (2016) 186-225, [DOI:10.1016/j.nima.2016.06.125](https://doi.org/10.1016/j.nima.2016.06.125)

>J. Allison et al., IEEE Trans. Nucl. Phys. 53.1 (2006) 270-278, [DOI:10.1109/TNS.2006.869826](https://doi.org/10.1109/TNS.2006.869826)

>S. Agostinelli et al., Nucl. Instrum. Methods. Phys. Res. A506.3 (2003) 250-303, [DOI:10.1016/S0168-9002(03)01368-8](https://doi.org/10.1016/S0168-9002(03)01368-8)

## Copyright and licence

(c) Copyright 2024 Holger Kluck

This software is distributed under the terms of the GNU General Public
Licence version 3 (GPL Version 3), copied verbatim in the file "LICENCE.md".
