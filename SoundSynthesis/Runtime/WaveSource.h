#pragma once
#include "..\Helpers\SineWaveOscillator.h"
#include "..\Helpers\SawtoothOscillator.h"

namespace SoundSynthesis { namespace Runtime
{
	public ref class WaveSource sealed
	{
	private:
		Helpers::SineWaveOscillator m_oscillator;

	public:
		WaveSource(unsigned int samplingRate, unsigned int channelsNumber);
		virtual ~WaveSource();

		void GenerateWave(
			Windows::Foundation::IMemoryBufferReference ^bufferReference,
			int samplesNumber);

		void BeginWave(double position, double effect);

		void ChangeWave(double position, double effect);

		void EndWave();
	};
}}
