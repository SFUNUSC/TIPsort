# TIPsort

Sort codes used by the saviours of Western Civilization who steadfastly refuse to move to GRSISort.

Various parts contributed by K. Starosta, P. Voss, A. Chester, J. Williams, and F. Wu

## Installation

The install script `install.sh` is provided in an attempt to automate the compilation and installation process.  It also attempts to set up the proper environment variables in the `.bashrc` file.  Use at your own risk, I am not responsible for messing up your computer.

### Manual Installation

At the very least, ROOT (tested with v5.x) must be installed with environment variables set up properly.  Environment variable setup can be done by adding to your `.bashrc` (and then reloading the terminal):

```
#ROOT configuration in .bashrc
export ROOT5SYS=/path/to/root
export ROOT5INC=$ROOT5SYS/include
export ROOT5LIB=$ROOT5SYS/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROOT5LIB
```

Once this is done, run `make` from the `current` directory.  If everything works (you will see a bazillion compiler warnings), programs should be placed in the `bin` directory (which needs to exist along with subdirectories `midas` and `sfu`), which you can then add to your path:

```
#TIPsort configuration in .bashrc
export TIPsort=/path/to/TIPsort
export PATH=$PATH:$TIPsort/bin
export PATH=$PATH:$TIPsort/bin/midas
export PATH=$PATH:$TIPsort/bin/sfu
```

Congratulations, your suffering has only begun.

## Backwards Compatibility

In general, we try to maintain backwards compatibility by not changing the .sfu file format (allowing previously sorted files to be analyzed with newer revisions of the code).  However, occasionally things need to be adjusted to take new detector arrays and experimental peculiarities into account.  Major revisions affecting backwards compatibility which require re-conversion of data are listed here.

|**Date**|**Changes Made**|
|:---:|:---:|
| Nov 21, 2017 | CsI hit patterns extended so that they can include up to 256 detectors.  To acheive this, the hit patterns are now arrays of 64-bit variables rather than single 64-bit variables. |
| Oct 27, 2017 | Increased number of available CsI detector positions from 40 to 128. |
| Sep 14, 2016 | Increased number of available CsI detector positions from 30 to 40. |

## Version History

v9.0: Moved to git for version control.  Use 'git log' in the source repository for version notes.

v8.6: Various changes by Jonathan for analysis of 28Mg.
     -Pileup rejection implemented in waveform analyzer.  Switch for this functionality is implemented in the map file.
     -TTCal separator codes (Tigress, CsIArray, TigressCsIArray) modified to only keep hits which fall within a time gate after the initial hit.  Gate length is specified in the Tigress and CsIArray calibration parameter files linked to in the master file.

v8.4: Waveform analyzer changes to reflect weak sources with gamma background.  Used for analysis of 207Bi and 209Po sources for coincidence trigger tests.  Fits 2 components, fast only, slow only, gamma on PIN diode and selects lowest chi square. Previously assumed 2 component fit function based on optimization and development for use with in-beam data.  Includes CsI ring calibration from S1467 experiment at TRIUMF.

v8.3: Recovery from accidental deletion of source code. Correct implementation of risetimes in waveform analyzer.

v8.2: BGO detector type added. New method for calculating standard deviation for suppression maps. TIGRESS and BGO times are independet in calibrator.  Program for creating background subtracted spectra added (preliminary at this stage).

v8.0: HPGe/BGO time difference implemented in calibrator for suppression scheme.

v7.9: New HPGe/BGO suppression scheme from HPGe/BGO hit map. A new parameter file (the TIGRESS_suppression_map.par) must be used in conjunction with the new calibrator. The suppression map can be made with TigressBGO_TTHP_Map.

v7.7: Alpha particle energy calibration included in calibrator from S1232 alpha calibration in April 2014.

v7.3: Updated to include Kris' changes to the TIGRESS calibrator to allow selection for timing either from fits or from the DAQ values. The map must be changed to reflect this. A parameter file is included in /Analysis/Nov2013/ParFiles called TIGRESS_par_7.3.dat which should be used with this code.
          
v7.2: Original version of sort codes on alphadon.
      Includes functionality for CsI detectors and PID from PIN diodes
      Timing for HPGe detectors is a work in progress

Earlier versions: i dunno lol
