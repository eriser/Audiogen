#include "pch.h"
#include "SampleProducer.h"

using namespace SoundSynthesis::XAudioSynthesis;

SampleProducer::SampleProducer(_In_ PFOSCILLATOR oscillator, _In_ const WAVEFORMATEX *waveFormat) noexcept
:	m_oscillator(oscillator),
	m_sampleAngleFactor(2.0 * M_PI / static_cast<double>(waveFormat->nSamplesPerSec)),
	m_frequency(0.0),
	m_phase(0.0)
{
}

SampleProducer::~SampleProducer() noexcept
{
}

float SampleProducer::GetSample() noexcept
{
	float sample = static_cast<float>((*m_oscillator)(m_phase));

	m_phase += m_sampleAngleFactor * m_frequency;
	if (m_phase >= M_PI * 2.0)
		m_phase -= M_PI * 2.0;

	return sample;
}
