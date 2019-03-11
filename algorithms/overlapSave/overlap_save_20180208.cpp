# include "overlap_save_20180208.h"
# include "fft_20180208.h"

#ifndef M_PI
#define M_PI  3.14159265358979323846
#endif


//########################################################################################################################################################
//############################################################### FFT Module #############################################################################
//########################################################################################################################################################

////////////
//// 13 ////
////////////
std::vector<std::complex<double>> overlapSave(std::vector<std::complex<double> > &inTimeDomainComplex, std::vector<std::complex<double> > &inTimeDomainFilterComplex)
{
	size_t impulseResponseLength = inTimeDomainFilterComplex.size();  // Initial size of filter
	size_t N;									  // FFT size

	if ((impulseResponseLength & (impulseResponseLength - 1)) == 0)						  // Is power of 2
	{
		N = impulseResponseLength * 2;
	}
	else
	{
		size_t m = 1;
		while (m <= impulseResponseLength)					// This calculates next value of power of 2
		{
			m *= 2;
		}

		N = m;
	}

	// Now, check if the value of (N-impulseResponseLength) is less than or equal to the 25% of N,
	// then double the size of the N.

	size_t L = N - impulseResponseLength + 1;			// Size of data block (Here, we used fix 50% overlap)
	size_t overlap = N - L;		// size of overlap

	size_t Dl = inTimeDomainComplex.size();
	int extraZero = 0;
	if (Dl % L == 0)
	{
		inTimeDomainComplex = inTimeDomainComplex;
	}
	else
	{
		// This changes the length of data if it's not interger multiple of L
		size_t Dlnew = inTimeDomainComplex.size();
		while (Dlnew % L != 0)
		{
			inTimeDomainComplex.push_back((0, 0));
			Dlnew = inTimeDomainComplex.size();
			extraZero += 1;
		}
	}



	vector <complex<double>>  filterTD(N); // Time domain filter
	vector <complex<double>>  filterFD(N); // Freq domain filter

	for (unsigned int i = 0; i < inTimeDomainFilterComplex.size(); i++)
	{
		filterTD[i] = inTimeDomainFilterComplex[i];
		filterTD[i] = filterTD[i] * (sqrt(filterTD.size())); //Scaling ny sqrt(N)
	}
	filterFD = fft(filterTD);



	//////////////////////////////// Section 4 /////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// Create Matrix ///////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////


	size_t nr = inTimeDomainComplex.size() / L;

	vector < vector<complex<double>> > matrix;
	vector <complex<double>> tempVector(L); // Without overlap
	vector <complex<double>> tempVectorA(N); // With overlap


	size_t position = 0;

	for (unsigned int i = 0; i < nr; i++)
	{

		for (unsigned int j = 0; j < L; j++)
		{
			tempVector[j] = inTimeDomainComplex[position + j]; // Take data withot overlap
		}


		if (i == 0)
		{
			for (unsigned int k = 0; k < tempVector.size(); k++)
			{
				tempVectorA[overlap + k] = tempVector[k];  // For first block, overlap is zero
			}
		}
		else
		{
			vector <complex<double>> tempVectorM = matrix[i - 1];
			for (unsigned int m = 0; m < overlap; m++)
			{
				tempVectorA[m] = tempVectorM[L + m];
			}
			for (unsigned int n = 0; n < L; n++)
			{
				tempVectorA[overlap + n] = tempVector[n];
			}
		}


		matrix.push_back(tempVectorA);
		position = position + L;
	}


	//////////////////////////////// Section 4 /////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// Convolution ////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	for (unsigned int p = 0; p < matrix.size(); p++)
	{
		vector <complex<double>> dataBlock = matrix[p];
		vector <complex<double>> dataBlockTD = dataBlock;

		dataBlockTD = fft(dataBlockTD);	 // Here to very with MATLAB, multiply this data with SQRT(N)

		for (unsigned int q = 0; q < dataBlockTD.size(); q++)
		{
			dataBlockTD[q] = dataBlockTD[q] * filterFD[q]; // Multiplication with filter data
		}

		dataBlockTD = ifft(dataBlockTD);				  // ifft to get filtered data

		matrix[p] = dataBlockTD;						  // save data to matrix
	}

	// Next, create vector from a matrix by disarding the overlap 
	vector <complex<double>> y_aux(inTimeDomainComplex.size());
	size_t startPosition = 0;

	for (unsigned int r = 0; r < matrix.size(); r++)
	{

		vector <complex<double>> tempVectorB = matrix[r];

		for (unsigned int s = 0; s < L; s++)
		{
			y_aux[startPosition + s] = tempVectorB[overlap + s];
		}

		startPosition = startPosition + L;
	}

	// Next, discard last "extrazero" samples. 
	vector <complex<double>> y(inTimeDomainComplex.size() - extraZero);

	for (unsigned int t = 0; t < y_aux.size() - extraZero; t++)
	{
		y[t] = y_aux[t];
	}

	//////////////////////////////// Section 6 ///////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// Display Section //////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	/*
	// Display length of data
	cout << "\nTotal length of of data,     Dl = " << count << endl;
	cout << "\nTotal length of of Filter,	M = " << countFilter << endl;
	cout << "\nTotal length of of Filter after zero padding = " << filterFD.size() << endl;
	cout << "\nTotal length of of data after zero padding,    Dlnew = " << inTimeDomainComplex.size() << " (This is because to make the data length of itnger multiple of L)" << endl;

	cout << endl << endl;

	cout << "length of data block size,  L = " << L << endl;
	cout << "length of FFT size,         N = " << N << endl;
	cout << "Length of overlap,    overlap = " << overlap << endl;*/

	return y;

}

////////////
//// 14 ////
////////////
std::vector<std::complex<double>> overlapSave(std::vector<std::complex<double> > &inTimeDomainComplex1, std::vector<std::complex<double> > &inTimeDomainComplex2, std::vector<std::complex<double> > &inTimeDomainFilterComplex)
{
	vector<complex<double>> currentCopy = inTimeDomainComplex1;
	vector<complex<double>> previousCopy = inTimeDomainComplex2;
	vector<complex<double>> dataBlock = inTimeDomainComplex2;
	vector<complex<double>> filter = inTimeDomainFilterComplex;
	size_t overlap = previousCopy.size();

	for (unsigned int i = 0; i < currentCopy.size(); i++)
	{
		dataBlock.push_back(currentCopy[i]);
	}

	size_t N = dataBlock.size();

	if ((N & (N - 1)) == 0)
	{
		N = N;
	}
	else
	{
		size_t m = 1;
		while (m <= N)					// This calculates next value of power of 2
		{
			m *= 2;
		}

		N = m;
	}

	vector<complex<double>> dataBlockModified(N);
	for (unsigned int i = 0; i < dataBlock.size(); i++)
	{
		dataBlockModified[i] = dataBlock[i];
	}


	vector<complex<double>> y(N);

	y = overlapSave(dataBlock, filter);

	return y;
}

////////////
//// 15 ////
////////////

std::vector<std::complex<double>> overlapSaveWithTransferFunction(std::vector<std::complex<double> > &inTimeDomainComplex1, std::vector<std::complex<double> > &inTimeDomainComplex2, std::vector<std::complex<double> > &inFilterTransferFunctionComplex)
{
	vector<complex<double>> currentCopy = inTimeDomainComplex1;
	vector<complex<double>> previousCopy = inTimeDomainComplex2;
	vector<complex<double>> filter = inFilterTransferFunctionComplex;

	size_t N = filter.size();
	size_t L = N / 2;
	size_t overlapSize = N / 2;

	vector<complex<double>> overlap;
	for(size_t i= previousCopy.size()-overlapSize; i<previousCopy.size(); i++)
	{ overlap.push_back(previousCopy[i]); }

	vector<complex<double>> dataBlock; // previousCopy1, previousCopy2 and currentCopy concotanation
	dataBlock.reserve(overlap.size() + currentCopy.size());
	dataBlock.insert(dataBlock.end(), overlap.begin(), overlap.end());
	dataBlock.insert(dataBlock.end(), currentCopy.begin(), currentCopy.end());

	// Make dataBlock to be integer multiple of N

	size_t Dl = dataBlock.size();
	unsigned int extraZero = 0;
	if (Dl % L == 0)
	{
		dataBlock = dataBlock;
	}
	else
	{
		// This changes the length of data if it's not interger multiple of L
		size_t Dlnew = dataBlock.size();
		while (Dlnew % L != 0)
		{
			dataBlock.push_back((0, 0));
			Dlnew = dataBlock.size();
			extraZero += 1;
		}
	}


	//////////////////////////////// Section 4 /////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// Create Matrix ///////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	size_t nr = dataBlock.size() / L;

	vector < vector<complex<double>> > matrix;
	vector <complex<double>> tempVector(N); // With overlap
	size_t position = 0;

	for (unsigned int i = 0; i < nr-1; i++)
	{

		for (unsigned int j = 0; j < N; j++)
		{
			tempVector[j] = dataBlock[position + j]; // Take data withot overlap
		}

		/*{
			vector <complex<double>> tempVectorM = matrix[i - 1];
			for (unsigned int m = 0; m < overlapSize; m++)
			{
				tempVectorA[m] = tempVectorM[L + m];
			}
			for (unsigned int n = 0; n < L; n++)
			{
				tempVectorA[overlapSize + n] = tempVector[n];
			}
		}*/

		matrix.push_back(tempVector);
		position = position + L;
	}


	//////////////////////////////// Section 4 /////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// Convolution ////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	for (unsigned int p = 0; p < matrix.size(); p++)
	{
		vector <complex<double>> dataBlockSegment = matrix[p];
		vector <complex<double>> dataBlockTD = dataBlockSegment;

		dataBlockTD = fft(dataBlockTD);	 // Here to very with MATLAB, multiply this data with SQRT(N)

		for (unsigned int q = 0; q < dataBlockTD.size(); q++)
		{
			dataBlockTD[q] = dataBlockTD[q] * filter[q]; // Multiplication with filter data
		}

		dataBlockTD = ifft(dataBlockTD);				  // ifft to get filtered data

		matrix[p] = dataBlockTD;						  // save data to matrix
	}

	// Next, create vector from a matrix by disarding the overlap 
	vector <complex<double>> y_aux(dataBlock.size()-overlapSize);
	size_t startPosition = 0;

	for (unsigned int r = 0; r < matrix.size(); r++)
	{

		vector <complex<double>> tempVectorB = matrix[r];

		for (unsigned int s = 0; s < L; s++)
		{
			y_aux[startPosition + s] = tempVectorB[overlapSize + s];
		}

		startPosition = startPosition + L;
	}

	// Next, discard last "extrazero" samples. 
	vector <complex<double>> y(y_aux.size() -  extraZero);

	for (unsigned int t = 0; t < y_aux.size() - extraZero; t++)
	{
		y[t] = y_aux[t];
	}

	return y;
}

std::vector<std::complex<double>> overlapSaveTFN(std::vector<std::complex<double> > &inTimeDomainComplex, std::vector<std::complex<double> > &inTransferFunctionComplex)
{
	vector<complex<double>>dataBlockFrequencyDomain; // FFT of dataBlock
	dataBlockFrequencyDomain = fft(inTimeDomainComplex);

	vector<complex<double>> y;
	y = complexVectorMultiplication(dataBlockFrequencyDomain, inTransferFunctionComplex); // Vector multiplication
	y = ifft(y);

	return y;



}

////////////
//// 16 ////
////////////
std::vector<std::complex<double>> conv(std::vector<std::complex<double>> &inTimeDomainComplex1, std::vector<std::complex<double>> &inTimeDomainComplex2)
{
	// Here we will calculate the length of the linear convolution L = M+N-1
	size_t M = inTimeDomainComplex1.size();
	size_t N = inTimeDomainComplex2.size();

	size_t L = M + N - 1;

	vector<std::complex<double>> xn(L);
	vector<std::complex<double>> hn(L);

	// Change length of both vectr by zero padding
	for (unsigned int i = 0; i < L; i++)
	{
		xn[i] = inTimeDomainComplex1[i];
		hn[i] = inTimeDomainComplex2[i];
	}

	// Calculate linear convolution using Circular convolution conversion
	vector<std::complex<double>> XN(L);
	vector<std::complex<double>> HN(L);
	vector<std::complex<double>> y(L);

	XN = fft(xn);
	HN = fft(hn);
	y = complexVectorMultiplication(XN, HN);
	y = ifft(y);

	return y;
}

////////////
//// 17 ////
////////////
std::vector<std::complex<double>> circularConv(std::vector<std::complex<double>> &inTimeDomainComplex1, std::vector<std::complex<double>> &inTimeDomainComplex2) 

{
	// Here we will calculate the length of the linear convolution L = min(impulseResponseLength,N)
	size_t M = inTimeDomainComplex1.size();
	size_t N = inTimeDomainComplex2.size();

	size_t L = min(M, N);

	vector<std::complex<double>> xn(L);
	vector<std::complex<double>> hn(L);

	// Change length of both vectr by zero padding
	for (unsigned int i = 0; i < L; i++)
	{
		xn[i] = inTimeDomainComplex1[i];
		hn[i] = inTimeDomainComplex2[i];
	}

	// Calculate linear convolution using Circular convolution conversion
	vector<std::complex<double>> XN(L);
	vector<std::complex<double>> HN(L);
	vector<std::complex<double>> y(L);

	XN = fft(xn);
	HN = fft(hn);
	y = complexVectorMultiplication(XN, HN);
	y = ifft(y);

	return y;

}

////////////
//// 18 ////
////////////
std::vector<std::complex<double>> transferFunctionToImpulseResponse(std::vector<std::complex<double>> &transferFunction)
{
	// Calculate the impulse response of the filter
	vector<complex<double>> transferFunctionComplex = transferFunction;
	vector<complex<double>> impulseResponseComplex(transferFunctionComplex.size());
	vector<complex<double>> impulseResponse(transferFunctionComplex.size());

	transferFunctionComplex = ifftshift(transferFunctionComplex); // ifftshift 
	impulseResponseComplex = ifft(transferFunctionComplex);		  // ifft	   
	impulseResponseComplex = fftshift(impulseResponseComplex);	  // fftshift 

	for (unsigned int i = 0; i < impulseResponseComplex.size(); i++) {
		impulseResponseComplex[i] = impulseResponseComplex[i] / (sqrt((impulseResponseComplex.size())));
	}

	vector<double> realImpulseResponse(impulseResponseComplex.size());
	for (int i = 0; i < impulseResponseComplex.size(); i++) { realImpulseResponse[i] = (impulseResponseComplex[i].real()); }
	double maxValue = *std::max_element(realImpulseResponse.begin(), realImpulseResponse.end());

	for (unsigned int i = 0; i < transferFunctionComplex.size(); i++) {
		impulseResponse[i] = static_cast<complex<double>>((1 / maxValue)*realImpulseResponse[i]);
	}

	return impulseResponse;
}

std::vector<std::complex<double>> transferFunctionToImpulseResponseNEW(std::vector<std::complex<double>> &transferFunction)
{
	// Calculate the impulse response of the filter
	vector<complex<double>> transferFunctionComplex = transferFunction;
	vector<complex<double>> impulseResponseComplex(transferFunctionComplex.size());
	vector<complex<double>> impulseResponse(transferFunctionComplex.size());

	transferFunctionComplex = ifftshift(transferFunctionComplex); // ifftshift
	impulseResponseComplex = ifft(transferFunctionComplex);		  // ifft	   
	impulseResponseComplex = fftshift(impulseResponseComplex);	  // fftshift

	for (unsigned int i = 0; i < impulseResponseComplex.size(); i++) {
		impulseResponseComplex[i] = impulseResponseComplex[i] / (sqrt((impulseResponseComplex.size())));
	}

	for (unsigned int i = 0; i < transferFunctionComplex.size(); i++) {
		impulseResponse[i] = static_cast<complex<double>>(impulseResponseComplex[i].real());
	}

	return impulseResponse;
}
