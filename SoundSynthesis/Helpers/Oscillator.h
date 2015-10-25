#pragma once

namespace SoundSynthesis { namespace Helpers
{
	class Oscillator
	{
		const unsigned int m_samplingRate;
		const unsigned int m_channelsNumber;

	public:
		Oscillator(unsigned int samplingRate, unsigned int channelsNumber);

		void GenerateSamples(_Out_bytecap_c_(capacity) BYTE *buffer, _In_ size_t capacity);
	};
}}
