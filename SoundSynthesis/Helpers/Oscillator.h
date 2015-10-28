#pragma once

namespace SoundSynthesis { namespace Helpers
{
	class Oscillator
	{
		CRITICAL_SECTION m_guard;
		const unsigned int m_samplingRate;
		const unsigned int m_channelsNumber;
		unsigned long m_samplesGenerated;
		bool m_generating;
		double m_frequency;

	public:
		Oscillator(unsigned int samplingRate, unsigned int channelsNumber);
		~Oscillator();

		void GenerateSamples(_In_ size_t samplesNumber, _Out_bytecap_c_(capacity) BYTE *buffer, _In_ size_t capacity);
		void Begin(double position, double effect);
		void Change(double position, double effect);
		void End();
	};
}}
