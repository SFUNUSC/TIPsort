# TIPsort

Sort codes used by the TIP guys.

Various part contributed by K. Starosta, P. Voss, A. Chester, and J. Williams


## Version History

v7.2: Original version of sort codes on alphadon.
      Includes functionality for CsI detectors and PID from PIN diodes
      Timing for HPGe detectors is a work in progress

v7.3 Updated to include Kris' changes to the TIGRESS calibrator to allow 
     selection for timing either from fits or from the DAQ values. The 
     map must be changed to reflect this. A parameter file is included
     in /Analysis/Nov2013/ParFiles called TIGRESS_par_7.3.dat which
     should be used with this code.

v7.7 Alpha particle energy calibration included in calibrator from S1232
     alpha calibration in April 2014.

v7.9 New HPGe/BGO suppression scheme from HPGe/BGO hit map. A new
     parameter file (the TIGRESS_suppression_map.par) must be used in
     conjunction with the new calibrator. The suppression map can be made
     with TigressBGO_TTHP_Map.

v8.0 HPGe/BGO time difference implemented in calibrator for suppression scheme.

v8.2 BGO detector type added. New method for calculating standard deviation for	     suppression maps. TIGRESS and BGO times are independet in calibrator. 
     Program for creating background subtracted spectra added (preliminary
     at this stage).

v8.3 Recovery from accidental deletion of source code. Correct implementation
     of risetimes in waveform analyzer.

v8.4 Waveform analyzer changes to reflect weak sources with gamma background.
     Used for analysis of 207Bi and 209Po sources for coincidence trigger tests.
     Fits 2 components, fast only, slow only, gamma on PIN diode and selects 
     lowest chi square. Previously assumed 2 component fit function based on
     optimization and development for use with in-beam data. Includes CsI ring calibration from S1467 experiment at TRIUMF.

v8.6 Various changes by Jonathan for analysis of 28Mg.
     -Pileup rejection implemented in waveform analyzer.  Switch for this 
      functionality is implemented in the map file.
     -TTCal separator codes (Tigress, CsIArray, TigressCsIArray) modified 
      to only keep hits which fall within a time gate after the initial 
      hit.  Gate length is specified in the Tigress and CsIArray calibration 
      parameter files linked to in the master file.

v9.0 Moved to git for version control.  Use 'git log' in the source repository 
     for version notes.
