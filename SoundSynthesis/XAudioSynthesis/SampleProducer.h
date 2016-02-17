#pragma once

#include "../Helpers/Oscillators.h"

namespace SoundSynthesis { namespace XAudioSynthesis
{
	using namespace SoundSynthesis::Helpers;

	class SampleProducer sealed
	{
	public:
		SampleProducer(_In_ PFOSCILLATOR oscillator, _In_ const WAVEFORMATEX *waveFormat) noexcept;
		~SampleProducer() noexcept;

		void ResetPhase() noexcept { m_phase = 0.0; }
		void SetFrequency(double frequency) noexcept { m_frequency = frequency; }
		float GetSample() noexcept;

	private:
		const PFOSCILLATOR	m_oscillator;
		const double		m_sampleAngleFactor;	// product of this and the frequency is the phase shift (angle) of one sample
		double				m_frequency;
		double				m_phase;
	};

}}
