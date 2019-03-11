# include "netxpto.h"
# include "overlap_save_20180208.h"

/////////////////////////////////////////////////////////////
//////////////////////// FIR_Filter ///////////////////////// TIME DOMAIN
/////////////////////////////////////////////////////////////
class FIR_Filter : public Block {

	// State Variable
	vector<t_real> delayLine;

	bool saveImpulseResponse{ true };
	string impulseResponseFilename{ "impulse_response.imp" };

	// Input Parameters 
	bool seeBeginningOfImpulseResponse{ true };

public:

	// State Variable 
	vector<t_real> impulseResponse;

	// Input Parameters
	int impulseResponseLength;				   


	// Methods 
	FIR_Filter() {};
	FIR_Filter(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initializeFIR_Filter(void);
	bool runBlock(void);
	void terminate(void) {};

	void setSaveImpulseResponse(bool sImpulseResponse) { saveImpulseResponse = sImpulseResponse; };
	bool getSaveImpulseResponse(void) { return saveImpulseResponse; };

	void setImpulseResponseLength(int iResponseLength) { impulseResponseLength = iResponseLength; };
	int const getImpulseResponseLength() { return impulseResponseLength; }

	void setSeeBeginningOfImpulseResponse(bool sBeginning) { seeBeginningOfImpulseResponse = sBeginning; };
	bool const getSeeBeginningOfImpulseResponse() { return seeBeginningOfImpulseResponse; };
};

/////////////////////////////////////////////////////////////
//////////////////////// FIR_Filter ///////////////////////// FREQUENCY DOMAIN
/////////////////////////////////////////////////////////////
class FD_Filter : public Block {

public:
	// State Variable
	vector<t_complex> transferFunction;

	// Input parameter
	int transferFunctionLength;

	/* Methods */
	FD_Filter() {};
	FD_Filter(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initializeFD_Filter(void);
	bool runBlock(void);
	void terminate(void) {};

	void setSaveTransferFunction(bool sTransferFunction) { saveTransferFunction = sTransferFunction; };				
	bool getSaveTransferFunction(void) { return saveTransferFunction; };

	void setSeeBeginningOfTransferFunction(bool sBeginning) { seeBeginningOfTransferFunction = sBeginning; };		
	bool const getSeeBeginningOfTransferFunction() { return seeBeginningOfTransferFunction; };

	void setTransferFunctionLength(int iTransferFunctionLength) { transferFunctionLength = iTransferFunctionLength; }; 
	int const getTransferFunctionLength() { return transferFunctionLength; };

private:
	// State Variable
	vector<t_complex> previousCopy;
	int K{ 0 };
	bool saveTransferFunction{ true };
	string transferFunctionFilename{ "transfer_function.tfn" };

	// Input Parameters 
	bool seeBeginningOfTransferFunction{ true };
};



class FD_Filter_New : public Block {

public:
	// State Variable
	vector<t_complex> transferFunction;

	// Input parameter
	int transferFunctionLength;

	/* Methods */
	FD_Filter_New() {};
	FD_Filter_New(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initializeFD_Filter_New(void);
	bool runBlock(void);
	void terminate(void) {};

	void setTransferFunctionFrequencyLength(int transFunctionLength) { transferFunctionFrequencyLength = transFunctionLength; };
	int const getTransferFunctionFrequencyLength(void) { return transferFunctionFrequencyLength; };

	void setSaveTransferFunction(bool sTransferFunction) { saveTransferFunction = sTransferFunction; };				    
	bool getSaveTransferFunction(void) { return saveTransferFunction; };

	void setSeeBeginningOfTransferFunction(bool sBeginning) { seeBeginningOfTransferFunction = sBeginning; };			
	bool const getSeeBeginningOfTransferFunction() { return seeBeginningOfTransferFunction; };

	void setTransferFunctionLength(int iTransferFunctionLength) { transferFunctionLength = iTransferFunctionLength; };  
	int const getTransferFunctionLength() { return transferFunctionLength; };


private:
	// State Variable
	vector<t_complex> previousCopy;
	int K{ 0 };
	bool saveTransferFunction{ true };
	string transferFunctionFilename{ "transfer_function.tfn" };

	// Input Parameters 
	bool seeBeginningOfTransferFunction{ true };
	int transferFunctionFrequencyLength{ 16 };


};


#pragma once
