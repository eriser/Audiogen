#pragma once
#include "Oscillators.h"

namespace SoundSynthesis { namespace Helpers
{
	const double M_2PI = 2.0 * M_PI;

	class SampleGenerator
	{
		bool m_initialized;
		CRITICAL_SECTION m_guard;
		const PFOSCILLATOR m_oscillator;
		const double m_baseFrequency;
		const double m_octaveRange;
		const unsigned int m_samplingRate;
		const unsigned int m_channelsNumber;
		double m_phaseOffset;
		bool m_generating;
		double m_frequency;

	public:
		SampleGenerator(_In_ PFOSCILLATOR oscillator,
			double baseFrequency,
			double octaveRange,
			unsigned int samplingRate,
			unsigned int channelsNumber);
		virtual ~SampleGenerator();

		void GenerateSamples(_In_ size_t samplesNumber, _Out_bytecap_c_(capacity) BYTE *buffer, _In_ size_t capacity);
		void Begin(double position, double effect);
		void Change(double position, double effect);
		void End();

	private:
		static double NormalizePhase(double phase);
	};
}}
