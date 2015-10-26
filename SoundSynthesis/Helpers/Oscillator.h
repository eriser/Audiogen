#pragma once

namespace SoundSynthesis { namespace Helpers
{
	class Oscillator
	{
		const unsigned int m_samplingRate;
		const unsigned int m_channelsNumber;
		unsigned long m_samplesGenerated;
		double m_frequency;

	public:
		Oscillator(unsigned int samplingRate, unsigned int channelsNumber);

		void GenerateSamples(_In_ size_t samplesNumber, _Out_bytecap_c_(capacity) BYTE *buffer, _In_ size_t capacity);
	};
}}
