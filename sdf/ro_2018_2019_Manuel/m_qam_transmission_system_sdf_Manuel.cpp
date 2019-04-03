# include "../../include/netxpto_20180815.h"
# include "../../include/binary_source_20180815.h"
# include "../../include/laser_20180815.h"
# include "../../include/m_qam_transmitter_20180815.h"
# include "../../include/m_qam_receiver_20180815.h"
# include "../../include/bit_error_rate_20180815.h"
# include "../../include/sink_20180815.h"


// #####################################################################################################
// ################################### System Input Parameters #########################################
// #####################################################################################################

int numberOfBits{ 1000 };
int constellationCardinality{ 4 };	
double symbolPeriod_s{ 2e-11 };	
int samplesPerSymbol{ 8 };
double txLocalOscillatorPower_dBm{ 0 };
pulse_shapper_filter_type txPulseShapperType{ pulse_shapper_filter_type::RootRaisedCosine};
double raisedCosineRollOffFactor{ 0.1 };
int txPulseShaperLength_symbolPeriods{ 20 };
double rxLocalOscillatorPower_dBm{ 6 };
double rxTiAmplifierInputNoisePowerSpectralDensity{ 5e-17 }; //gama de interesse: 1e-17 - 1e-13 
int rxTiAmplifierImpulseResponseTimeLength_symbolPeriods{ 20 };
double rxTiAmplifierBandwidth{1.5*50.0e9 }; // gama de interesse : 0.1 a 0.8
//double rxThermalNoiseSpectralDensity{  }; // gama de interesse : 1e-14 - 1e-12
double rxThermalNoisePower{ 0 };//rxThermalNoiseSpectralDensity/symbolPeriod_s }; // gama de interesse : 1e-5 a 1e-3
pulse_shapper_filter_type rxElectricalFilterImpulseResponseType{ pulse_shapper_filter_type::RootRaisedCosine };
int rxSamplerSamplesToSkip{ (txPulseShaperLength_symbolPeriods - 1)*samplesPerSymbol + (rxTiAmplifierImpulseResponseTimeLength_symbolPeriods *samplesPerSymbol/2)};


int main() {

	// #####################################################################################################
	// ########################### Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	Binary BinarySourceOut_0{ "S0_BinarySourceOut_0.sgn" };

	Binary BinarySourceOut_1{ "S1_BinarySourceOut_0.sgn" };

	OpticalSignal TxLocalOscillatorOut{ "S2_TxLocalOscillatorOut.sgn" };
	
	OpticalSignal MQamTransmitterOut{ "S3_MQamTransmitterOut.sgn" };
	
	OpticalSignal RxLocalOscillatorOut{ "S4_RxLocalOscillatorOut.sgn" };

	Binary MQamReceiverOut{ "S5_MQamReceiverOut.sgn" };

	Binary BitErrorRateOut{ "S6_BitErrorRateOut.sgn" };

		
	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################


	BinarySource BinarySource_{ {}, { &BinarySourceOut_0, &BinarySourceOut_1 } };
	BinarySource_.setBitPeriod(symbolPeriod_s / log2(constellationCardinality));

	Laser TxLocalOscillator_{ {},{ &TxLocalOscillatorOut } };
	TxLocalOscillator_.setSymbolPeriod(symbolPeriod_s);
	TxLocalOscillator_.setSamplingPeriod(symbolPeriod_s / samplesPerSymbol);
	TxLocalOscillator_.setOpticalPower_dBm(txLocalOscillatorPower_dBm);

	MQamTransmitter MQamTransmitter_{ {&BinarySourceOut_1, &TxLocalOscillatorOut}, {&MQamTransmitterOut} };
	MQamTransmitter_.setNumberOfSamplesPerSymbol(samplesPerSymbol);
	MQamTransmitter_.setImpulseResponseTimeLength_symbolPeriods(txPulseShaperLength_symbolPeriods);
	MQamTransmitter_.setFilterType(txPulseShapperType);
	MQamTransmitter_.setRollOffFactor(raisedCosineRollOffFactor);
		
	Laser RxLocalOscillator_{ {},{ &RxLocalOscillatorOut } };
	RxLocalOscillator_.setSymbolPeriod(symbolPeriod_s);
	RxLocalOscillator_.setSamplingPeriod(symbolPeriod_s / samplesPerSymbol);
	RxLocalOscillator_.setOpticalPower_dBm(rxLocalOscillatorPower_dBm);

	MQamReceiver MQamReceiver_{ {&MQamTransmitterOut, &RxLocalOscillatorOut} , {&MQamReceiverOut} };
	MQamReceiver_.setNoiseSamplingPeriod(symbolPeriod_s / samplesPerSymbol);
	MQamReceiver_.setNoiseSymbolPeriod(symbolPeriod_s);
	MQamReceiver_.setPhotodiodesResponsivity(1.0);
	MQamReceiver_.setGain(1.0);
	MQamReceiver_.setTiAmplifierFilterType(LowPass);
	MQamReceiver_.setTiAmplifierCutoffFrequency(rxTiAmplifierBandwidth);
	MQamReceiver_.setTiAmplifierImpulseResponseTimeLength_symbolPeriods(rxTiAmplifierImpulseResponseTimeLength_symbolPeriods);
	MQamReceiver_.setAmplifierInputNoisePowerSpectralDensity(rxTiAmplifierInputNoisePowerSpectralDensity);
	MQamReceiver_.setThermalNoisePower(rxThermalNoisePower);
	MQamReceiver_.setSamplesToSkip(rxSamplerSamplesToSkip);
	MQamReceiver_.setFilterType(rxElectricalFilterImpulseResponseType);
	MQamReceiver_.setRollOffFactor(raisedCosineRollOffFactor);
	

	BitErrorRate BitErrorRate_{ {&MQamReceiverOut, &BinarySourceOut_0},{ &BitErrorRateOut} };
	
	Sink Sink_{ {&BitErrorRateOut},{} };
	
	// #####################################################################################################
	// ########################### System Declaration and Inicialization ###################################
	// #####################################################################################################

	//MQamTransmitter_.setIqAmplitudes({ { -3, -3 }, { -3,-1 }, { -3,3 }, { -3,1 },{ -1, -3 }, { -1,-1 }, { -1,3 }, { -1,1 },{ 3, -3 }, { 3,-1 }, { 3,3 }, { 3,1 },{ 1, -3 }, { 1,-1 }, { 1,3 }, { 1,1 } }); //Grey encoding 16-QAM
	//MQamReceiver_.setIqAmplitudes({ { -3, -3 }, { -3,-1 }, { -3,3 }, { -3,1 },{ -1, -3 }, { -1,-1 }, { -1,3 }, { -1,1 },{ 3, -3 }, { 3,-1 }, { 3,3 }, { 3,1 },{ 1, -3 }, { 1,-1 }, { 1,3 }, { 1,1 } }); //Grey encoding 16-QAM

	//MQamTransmitter_.setIqAmplitudes({ { -3, 3 }, { -1,1 }, { 1,3 }, { 3,1 },{ 3, 3 }, { 1,1 }, { -1,3 }, { -3,1 },{ 1, -1 }, { 3,-3 }, { -3,-1 }, { -1,-3 },{ -1, -1 }, { -3,-3 }, { 3,-1 }, { 1,-3 } }); //Not grey encoding 16-QAM
	//MQamReceiver_.setIqAmplitudes({ { -3, 3 }, { -1,1 }, { 1,3 }, { 3,1 },{ 3, 3 }, { 1,1 }, { -1,3 }, { -3,1 },{ 1, -1 }, { 3,-3 }, { -3,-1 }, { -1,-3 },{ -1, -1 }, { -3,-3 }, { 3,-1 }, { 1,-3 } }); //Not grey encoding 16-QAM


	//MQamTransmitter_.setIqAmplitudes({ { 1, 1 }, { 1,-1 }, { -1,-1 }, { -1,1 } }); //Grey encoding QPSK
	//MQamReceiver_.setIqAmplitudes({ { 1, 1 }, { 1,-1 }, { -1,-1 }, { -1,1 } }); //Grey encoding QPSK
	BinarySource_.setNumberOfBits(numberOfBits);
	BitErrorRate_.setMidReportSize(100);
	

	System MainSystem{ 
		{ 
			&BinarySource_,
			&TxLocalOscillator_, 
			&MQamTransmitter_, 
			&RxLocalOscillator_, 
			&MQamReceiver_,
			&BitErrorRate_,
			&Sink_
		}
	};

	// #####################################################################################################
	// #################################### System Run #####################################################
	// #####################################################################################################

	MainSystem.run();
	MainSystem.terminate();

	//system("pause");
	return 0;

}