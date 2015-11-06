#include "pch.h"
#include "Oscillator.h"

namespace SoundSynthesis {
	namespace Helpers
	{
		Oscillator::Oscillator(unsigned int samplingRate, unsigned int channelsNumber)
		:	m_samplingRate(samplingRate),
			m_channelsNumber(channelsNumber),
			m_phaseOffset(0.0),
			m_generating(false),
			m_frequency( 880.0 )
		{
			::InitializeCriticalSectionAndSpinCount(&m_guard, 4000);
		}

		Oscillator::~Oscillator()
		{
			::DeleteCriticalSection(&m_guard);
		}

		void Oscillator::GenerateSamples(_In_ size_t samplesNumber, _Out_bytecap_c_(capacity) BYTE *buffer, _In_ size_t capacity)
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
				double	freqToRate = frequency / m_samplingRate;
				double	samplePhase = M_2_PI * freqToRate;
				double	t;
				float	s;

				for (size_t sample = 0; sample < samplesNumber; ++sample)
				{
					t = m_phaseOffset + sample * samplePhase;
					s = 0.85f * ProduceSample(t);

					for (unsigned int c = 0; c < m_channelsNumber; ++c)
						*(samples++) = s;
				}

				m_phaseOffset = ::remainder(t, M_2PI);
			}
			else
			{
				SecureZeroMemory(buffer, capacity);
			}
		}

		void Oscillator::Begin(double position, double effect)
		{
			::EnterCriticalSection(&m_guard);
			m_generating = true;
			m_frequency = 220.0 + (880.0 - 220.0) * position;
			m_phaseOffset = 0.0;
			::LeaveCriticalSection(&m_guard);
		}

		void Oscillator::Change(double position, double effect)
		{
			::EnterCriticalSection(&m_guard);
			m_frequency = 220.0 + (880.0 - 220.0) * position;
			::LeaveCriticalSection(&m_guard);
		}

		void Oscillator::End()
		{
			::EnterCriticalSection(&m_guard);
			m_generating = false;
			::LeaveCriticalSection(&m_guard);
		}
	}
}
