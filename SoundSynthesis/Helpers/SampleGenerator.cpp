#include "pch.h"
#include "SampleGenerator.h"

namespace SoundSynthesis {
	namespace Helpers
	{
		SampleGenerator::SampleGenerator(_In_ PFOSCILLATOR oscillator,
			double baseFrequency,
			double octaveRange,
			unsigned int samplingRate,
			unsigned int channelsNumber)
		:	m_oscillator(oscillator),
			m_baseFrequency(baseFrequency),
			m_octaveRange(octaveRange),
			m_samplingRate(samplingRate),
			m_channelsNumber(channelsNumber),
			m_phaseOffset(0.0),
			m_generating(false)
		{
			::InitializeCriticalSectionAndSpinCount(&m_guard, 4000);
		}

		SampleGenerator::~SampleGenerator()
		{
			::DeleteCriticalSection(&m_guard);
		}

		void SampleGenerator::GenerateSamples(_In_ size_t samplesNumber, _Out_bytecap_c_(capacity) BYTE *buffer, _In_ size_t capacity)
		{
			bool	generating;
			double	frequency;

			::EnterCriticalSection(&m_guard);
			generating = m_generating;
			frequency = m_frequency;
			::LeaveCriticalSection(&m_guard);

			size_t sampleCapacity = capacity / (m_channelsNumber * sizeof(float));
			if (samplesNumber > sampleCapacity)
				samplesNumber = sampleCapacity;

			float *samples = reinterpret_cast<float*>(buffer);

			if (generating)
			{
				double	sampleAngle = M_2PI * frequency / m_samplingRate;
				double	phase = m_phaseOffset;
				float	s;

				for (size_t sample = 0; sample < samplesNumber; ++sample)
				{
					phase += sampleAngle;
					s = static_cast<float>(0.85 * (*m_oscillator)(NormalizePhase(phase)));

					for (unsigned int c = 0; c < m_channelsNumber; ++c)
						*(samples++) = s;
				}

				m_phaseOffset = ::remainder(phase, M_2PI);
				if (m_phaseOffset < 0.0)
					m_phaseOffset += M_2PI;
			}
			else
			{
				SecureZeroMemory(buffer, capacity);
			}
		}

		void SampleGenerator::Begin(double position, double effect)
		{
			(void)effect;

			::EnterCriticalSection(&m_guard);
			m_generating = true;
			m_frequency = m_baseFrequency * ::pow(2.0, m_octaveRange * position);
			m_phaseOffset = 0.0;
			::LeaveCriticalSection(&m_guard);
		}

		void SampleGenerator::Change(double position, double effect)
		{
			(void)effect;

			::EnterCriticalSection(&m_guard);
			m_frequency = m_baseFrequency * ::pow(2.0, m_octaveRange * position);
			::LeaveCriticalSection(&m_guard);
		}

		void SampleGenerator::End()
		{
			::EnterCriticalSection(&m_guard);
			m_generating = false;
			::LeaveCriticalSection(&m_guard);
		}

		double SampleGenerator::NormalizePhase(double phase)
		{
			if (phase >= M_2PI || phase < 0.0)
			{
				phase = ::remainder(phase, M_2PI);
				if (phase < 0.0)
					phase += M_2PI;
			}

			return phase;
		}
	}
}
