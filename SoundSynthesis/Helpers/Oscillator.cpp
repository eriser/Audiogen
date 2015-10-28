#include "pch.h"
#include "Oscillator.h"

namespace SoundSynthesis {
	namespace Helpers
	{
		Oscillator::Oscillator(unsigned int samplingRate, unsigned int channelsNumber)
		:	m_samplingRate(samplingRate),
			m_channelsNumber(channelsNumber),
			m_samplesGenerated( 0 ),
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
				for (size_t sample = 0; sample < samplesNumber; ++sample)
				{
					// sin(sample# / m_samplingRate * 2 Pi * frequency)
					double t = 2.0 * 3.1415926 * frequency * m_samplesGenerated / m_samplingRate;
					float fSample = static_cast<float>(sin(t));

					for (unsigned int c = 0; c < m_channelsNumber; ++c)
						*(samples++) = fSample;
					++m_samplesGenerated;
					if (m_samplesGenerated > m_samplingRate)
						m_samplesGenerated = 0;
				}
			}
			else
			{
				SecureZeroMemory(buffer, capacity);
				m_samplesGenerated += samplesNumber;
				if (m_samplesGenerated > m_samplingRate)
					m_samplesGenerated = 0;
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
