#include "pch.h"
#include "Oscillator.h"

namespace SoundSynthesis {
	namespace Helpers
	{
		Oscillator::Oscillator(unsigned int samplingRate, unsigned int channelsNumber)
		:	m_samplingRate(samplingRate),
			m_channelsNumber(channelsNumber)
		{

		}

		void Oscillator::GenerateSamples(_Out_bytecap_c_(capacity) BYTE *buffer, _In_ size_t capacity)
		{
		}
	}
}
