#include "pch.h"
#include "Oscillator.h"

namespace SoundSynthesis {
	namespace Helpers
	{
		Oscillator::Oscillator(double baseFrequency, double octaveRange, unsigned int samplingRate, unsigned int channelsNumber)
		:	m_baseFrequency(baseFrequency),
			m_octaveRange(octaveRange),
			m_samplingRate(samplingRate),
			m_channelsNumber(channelsNumber),
			m_phaseOffset(0.0),
			m_generating(false)
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
				double	sampleAngle = M_2PI * frequency / m_samplingRate;
				double	phase = m_phaseOffset;
				float	s;

				for (size_t sample = 0; sample < samplesNumber; ++sample)
				{
					phase += sampleAngle;
					s = 0.85f * ProduceSample(phase);

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

		void Oscillator::Begin(double position, double effect)
		{
			::EnterCriticalSection(&m_guard);
			m_generating = true;
			m_frequency = m_baseFrequency * ::pow(2.0, m_octaveRange * position);
			m_phaseOffset = 0.0;
			::LeaveCriticalSection(&m_guard);
		}

		void Oscillator::Change(double position, double effect)
		{
			::EnterCriticalSection(&m_guard);
			m_frequency = m_baseFrequency * ::pow(2.0, m_octaveRange * position);
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
