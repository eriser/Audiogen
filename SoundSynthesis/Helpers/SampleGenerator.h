#pragma once

namespace SoundSynthesis { namespace Helpers
{
	const double M_2PI = 2.0 * M_PI;

	class SampleGenerator
	{
		CRITICAL_SECTION m_guard;
		const double m_baseFrequency;
		const double m_octaveRange;
		const unsigned int m_samplingRate;
		const unsigned int m_channelsNumber;
		double m_phaseOffset;
		bool m_generating;
		double m_frequency;

	protected:
		SampleGenerator(double baseFrequency, double octaveRange, unsigned int samplingRate, unsigned int channelsNumber);

	public:
		virtual ~SampleGenerator();

		void GenerateSamples(_In_ size_t samplesNumber, _Out_bytecap_c_(capacity) BYTE *buffer, _In_ size_t capacity);
		void Begin(double position, double effect);
		void Change(double position, double effect);
		void End();

	protected:
		virtual float ProduceSample(double phase) const = 0;

	private:
		static double NormalizePhase(double phase);
	};
}}