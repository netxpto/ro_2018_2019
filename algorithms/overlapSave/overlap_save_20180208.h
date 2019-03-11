#pragma once
# ifndef PROGRAM_INCLUDE_OVERLAP_AND_SAVE_H_
# define PROGRAM_INCLUDE_OVERLAP_AND_SAVE_H_

# include <vector>
# include <complex>
#include <algorithm>
# include "fft_20180208.h"

using namespace std;

//########################################################################################################################################################
//############################################################### FFT MODULE #############################################################################
//########################################################################################################################################################

std::vector<std::complex<double>> overlapSave(std::vector<std::complex<double> > &inTimeDomainComplex, std::vector<std::complex<double> > &inTimeDomainFilterComplex);
std::vector<std::complex<double>> overlapSave(std::vector<std::complex<double> > &inTimeDomainComplex1, std::vector<std::complex<double> > &inTimeDomainComplex2, std::vector<std::complex<double> > &inTimeDomainFilterComplex);
std::vector<std::complex<double>> overlapSaveWithTransferFunction(std::vector<std::complex<double> > &inTimeDomainComplex1, std::vector<std::complex<double> > &inTimeDomainComplex2, std::vector<std::complex<double> > &inTimeDomainFilterComplex);
std::vector<std::complex<double>> overlapSaveTFN(std::vector<std::complex<double> > &inTimeDomainComplex, std::vector<std::complex<double> > &inTransferFunctionComplex);

std::vector<std::complex<double>> conv(std::vector<std::complex<double>> &inTimeDomainComplex1, std::vector<std::complex<double>> &inTimeDomainComplex2); // Convolution
std::vector<std::complex<double>> circularConv(std::vector<std::complex<double>> &inTimeDomainComplex1, std::vector<std::complex<double>> &inTimeDomainComplex2); // Circular convolution


std::vector<std::complex<double>> transferFunctionToImpulseResponse(std::vector<std::complex<double>> &transferFunction);    // transfer function to impulse response
std::vector<std::complex<double>> transferFunctionToImpulseResponseNEW(std::vector<std::complex<double>> &transferFunction); // transfer function to impulse response for FD_Filter 

# endif
