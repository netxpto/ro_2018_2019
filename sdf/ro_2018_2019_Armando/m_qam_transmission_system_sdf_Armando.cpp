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

int constellationCardinality{ 4 };	
double symbolPeriod_s{ 2e-11 };	
int samplesPerSymbol{ 8 };
double txLocalOscillatorPower_dBm{ 0 };
pulse_shapper_filter_type pulseShapperType{ pulse_shapper_filter_type::RaisedCosine };
double raisedCosineRollOffFactor{ 0.1 };
int pulseShaperLength_symbolPeriods{ 20 };
double rxLocalOscillatorPower_dBm{ 5 };


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
	MQamTransmitter_.setImpulseResponseTimeLength_symbolPeriods(pulseShaperLength_symbolPeriods);
	MQamTransmitter_.setFilterType(pulseShapperType);
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
	MQamReceiver_.setElectricalFilterType(LowPass);
	MQamReceiver_.setCutoffFrequency(10 / symbolPeriod_s);

	BitErrorRate BitErrorRate_{ {&MQamReceiverOut, &BinarySourceOut_0},{ &BitErrorRateOut} };
	
	Sink Sink_{ {&BitErrorRateOut},{} };
	
	// #####################################################################################################
	// ########################### System Declaration and Inicialization ###################################
	// #####################################################################################################

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

	system("pause");
	return 0;

}