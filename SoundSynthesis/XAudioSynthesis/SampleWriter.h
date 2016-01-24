#pragma once

namespace SoundSynthesis { namespace XAudioSynthesis
{
	class SampleWriter
	{
	public:
		SampleWriter(_Writable_elements_(count) float *samples, size_t count) noexcept
		:	m_currentSample(samples),
			m_remainingSamples(count)
		{
		}

		SampleWriter(_In_ const SampleWriter &writer) noexcept
		:	m_currentSample(writer.m_currentSample),
			m_remainingSamples(writer.m_remainingSamples)
		{
		}

		void Write(float sample, size_t count) noexcept
		{
			_ASSERTE(count <= m_remainingSamples);
			
			while (count-- > 0)
			{
				*(m_currentSample++) = sample;
			}
			m_remainingSamples -= count;
		}

	private:
		float	*m_currentSample;
		size_t	m_remainingSamples;
	};
}}
