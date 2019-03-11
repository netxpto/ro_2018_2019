# include "filter_20180306.h"

/////////////////////////////////////////////////////////////
//////////////////////// FIR_Filter ///////////////////////// TIME DOMAIN
/////////////////////////////////////////////////////////////

void FIR_Filter::initializeFIR_Filter(void) {

	outputSignals[0]->symbolPeriod = inputSignals[0]->symbolPeriod;
	outputSignals[0]->samplingPeriod = inputSignals[0]->samplingPeriod;
	outputSignals[0]->samplesPerSymbol = inputSignals[0]->samplesPerSymbol;

	if (!getSeeBeginningOfImpulseResponse()) {
		int aux = (int)(((double)impulseResponseLength) / 2) + 1;
		outputSignals[0]->setFirstValueToBeSaved(aux);
	}

	delayLine.resize(impulseResponseLength, 0);

	if (saveImpulseResponse) {
		ofstream fileHandler("./signals/" + impulseResponseFilename, ios::out);
		fileHandler << "// ### HEADER TERMINATOR ###\n";

		t_real t;
		double samplingPeriod = inputSignals[0]->samplingPeriod;
		for (int i = 0; i < impulseResponseLength; i++) {
			t = -impulseResponseLength / 2 * samplingPeriod + i * samplingPeriod;
			fileHandler << t << " " << impulseResponse[i] << "\n";
		}
		fileHandler.close();
	}

};


bool FIR_Filter::runBlock(void) {
	
	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();
	int process = min(ready, space);
	if (process == 0) return false;

	for (int i = 0; i < process; i++) {
		t_real val;
		(inputSignals[0])->bufferGet(&val);
		if (val != 0) {
			vector<t_real> aux(impulseResponseLength, 0.0);
			transform(impulseResponse.begin(), impulseResponse.end(), aux.begin(), bind1st(multiplies<t_real>(), val));
			transform(aux.begin(), aux.end(), delayLine.begin(), delayLine.begin(), plus<t_real>());
		}
		outputSignals[0]->bufferPut((t_real)(delayLine[0]));
		rotate(delayLine.begin(), delayLine.begin() + 1, delayLine.end());
		delayLine[impulseResponseLength - 1] = 0.0;
	}

	return true;
};


/////////////////////////////////////////////////////////////
///////////////////////// FD_Filter ///////////////////////// FREQUENCY DOMAIN
/////////////////////////////////////////////////////////////
void FD_Filter::initializeFD_Filter(void)
{
	outputSignals[0]->symbolPeriod = inputSignals[0]->symbolPeriod;
	outputSignals[0]->samplingPeriod = inputSignals[0]->samplingPeriod;
	outputSignals[0]->samplesPerSymbol = inputSignals[0]->samplesPerSymbol;

	if (!getSeeBeginningOfTransferFunction()) {
		int aux = (int)(((double)transferFunctionLength) / 2) + 1;
		outputSignals[0]->setFirstValueToBeSaved(aux);
	}

	if (saveTransferFunction)
	{
		ofstream fileHandler("./signals/" + transferFunctionFilename, ios::out);
		fileHandler << "// ### HEADER TERMINATOR ###\n";

		double samplingPeriod = inputSignals[0]->samplingPeriod;
		t_real fWindow = 1 / samplingPeriod;
		t_real df = fWindow / transferFunction.size();

		t_real f;
		for (int k = 0; k < transferFunction.size(); k++)
		{
			f = -fWindow / 2 + k * df;
			fileHandler << f << " " << transferFunction[k] << "\n";
		}
		fileHandler.close();
	}
}

bool FD_Filter::runBlock(void)
{
	bool alive{ false };

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();
	int process = min(ready, space);
	if (process == 0) return false;
	
	/////////////////////// previousCopy & currentCopy /////////////////////
	////////////////////////////////////////////////////////////////////////
	vector<double> re(process); // Get the Input signal
	t_real input;
	for (int i = 0; i < process; i++){
		inputSignals[0]->bufferGet(&input);
		re.at(i) = input;
	}

	vector<t_real> im(process);
	vector<t_complex> currentCopyAux = reImVect2ComplexVector(re, im);

	vector<t_complex> pcInitialize(process);	// For the first data block only
	if (K == 0){ previousCopy = pcInitialize; }
					
	// size modification of currentCopyAux to currentCopy.
	vector<t_complex> currentCopy(previousCopy.size());
	for (unsigned int i = 0; i < currentCopyAux.size(); i++){
		currentCopy[i] = currentCopyAux[i];
	}

	/////////////////////////// Filter Data "hn" //////////////////////////
	///////////////////////////////////////////////////////////////////////
	vector<t_complex> impulseResponse;
	impulseResponse = transferFunctionToImpulseResponse(transferFunction);
	vector<t_complex> hn = impulseResponse;

	////////////////////// OverlapSave in Realtime ///////////////////////
	//////////////////////////////////////////////////////////////////////
	vector<t_complex> OUTaux = overlapSave(currentCopy, previousCopy, hn);

	previousCopy = currentCopy;
	K = K + 1;

	// Remove the size modified data (opppsite to "currentCopyAux to currentCopy")
	vector<t_complex> OUT;
	for (int i = 0; i < process; i++){
		OUT.push_back(OUTaux[previousCopy.size() + i]);
	}

	// Bufferput
	for (int i = 0; i < process; i++){
		t_real val;
		val = OUT[i].real();
		outputSignals[0]->bufferPut((t_real)(val));
	}

	return true;
}


// FD_Filter_New
void FD_Filter_New::initializeFD_Filter_New(void) {


	double samplingPeriod = inputSignals[0]->samplingPeriod;
	double symbolPeriod = inputSignals[0]->symbolPeriod;

	outputSignals[0]->symbolPeriod = inputSignals[0]->symbolPeriod;
	outputSignals[0]->samplingPeriod = inputSignals[0]->samplingPeriod;

	transferFunctionLength = (int)floor(transferFunctionFrequencyLength * symbolPeriod / samplingPeriod);

	if (!getSeeBeginningOfTransferFunction()) {
		int aux = (int)(((double)transferFunctionLength) / 2) + 1;
		outputSignals[0]->setFirstValueToBeSaved(1);
	}
}


bool FD_Filter_New::runBlock()
{
	bool alive{ false };

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();
	int process = min(ready, space);
	if (process == 0) return false;

	/////////////////////// previousCopy & currentCopy /////////////////////
	////////////////////////////////////////////////////////////////////////
	vector<double> re(process); // Get the Input signal
	t_real input;
	for (int i = 0; i < process; i++) {
		inputSignals[0]->bufferGet(&input);
		re.at(i) = input;
	}

	vector<t_real> im(process);
	vector<t_complex> currentCopyAux = reImVect2ComplexVector(re, im);

	vector<t_complex> pcInitialize(process);	// For the first data block only
	if (K == 0) { previousCopy = pcInitialize; }

	// size modification of currentCopyAux to currentCopy.
	vector<t_complex> currentCopy(previousCopy.size());
	for (unsigned int i = 0; i < currentCopyAux.size(); i++) {
		currentCopy[i] = currentCopyAux[i];
	}


	////////////////////// OverlapSave in Realtime ///////////////////////
	//////////////////////////////////////////////////////////////////////
	vector<t_complex> transferFunction1 = transferFunction;
	transferFunction1 = ifftshift(transferFunction1);
	vector<t_complex> OUT = overlapSaveWithTransferFunction(currentCopy, previousCopy, transferFunction1);

	ofstream OutData("OutData.txt");
	for (size_t i = 0; i < OUT.size(); i++)
	{
		OutData << OUT[i].real() << "\n";
	}
	OutData.close();

	previousCopy = currentCopy;
	K = K + 1;


	// Bufferput
	for (int i = 0; i < process; i++) {
		t_real val;
		val = OUT[i].real();
		outputSignals[0]->bufferPut((t_real)(val));
	}

	return true;
}