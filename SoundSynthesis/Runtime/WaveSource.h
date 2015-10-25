#pragma once
#include "..\Helpers\Oscillator.h"

namespace SoundSynthesis { namespace Runtime
{
	public ref class WaveSource sealed
	{
	private:
		Helpers::Oscillator m_oscillator;

	public:
		WaveSource(unsigned int samplingRate, unsigned int channelsNumber);

		void GenerateWave(
			Windows::Foundation::IMemoryBufferReference ^bufferReference,
			int samplesNumber);
	};
}}
