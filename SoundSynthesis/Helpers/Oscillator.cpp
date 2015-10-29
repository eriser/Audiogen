#include "pch.h"
#include "Oscillator.h"

namespace SoundSynthesis {
	namespace Helpers
	{
		Oscillator::Oscillator(unsigned int samplingRate, unsigned int channelsNumber)
		:	m_samplingRate(samplingRate),
			m_channelsNumber(channelsNumber),
			m_initialPhase(0.0),
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
			double generating, frequency;

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
				double freqToRate = frequency / m_samplingRate;
				double M_2PI = 2.0 * M_PI;
				double samplePhase = M_2PI * freqToRate;
				double t;

				for (size_t sample = 0; sample < samplesNumber; ++sample)
				{
					// sin(sample# / m_samplingRate * 2 Pi * frequency)
					t = m_initialPhase + sample * samplePhase;
					float fSample = static_cast<float>(sin(t));

					for (unsigned int c = 0; c < m_channelsNumber; ++c)
						*(samples++) = fSample;
				}

				m_initialPhase = remainder(t, M_2PI);
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
