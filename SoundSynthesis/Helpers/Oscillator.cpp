#include "pch.h"
#include "Oscillator.h"

namespace SoundSynthesis {
	namespace Helpers
	{
		Oscillator::Oscillator(unsigned int samplingRate, unsigned int channelsNumber)
		:	m_samplingRate(samplingRate),
			m_channelsNumber(channelsNumber),
			m_samplesGenerated( 0 ),
			m_frequency( 880.0 )
		{
		}

		void Oscillator::GenerateSamples(_In_ size_t samplesNumber, _Out_bytecap_c_(capacity) BYTE *buffer, _In_ size_t capacity)
		{
			size_t sampleCapacity = capacity / (m_channelsNumber * sizeof(float));
			if (samplesNumber > sampleCapacity)
				samplesNumber = sampleCapacity;

			float *samples = reinterpret_cast<float*>(buffer);

			for (size_t sample = 0; sample < samplesNumber; ++sample)
			{
				// sin(sample# / m_samplingRate * 2 Pi * frequency)
				double t = 2.0 * 3.1415926 * m_frequency * m_samplesGenerated / m_samplingRate;
				float fSample = static_cast<float>(sin(t));

				for (unsigned int c = 0; c < m_channelsNumber; ++c)
					*(samples++) = fSample;
				++m_samplesGenerated;
				if (m_samplesGenerated > m_samplingRate)
					m_samplesGenerated = 0;
			}
		}
	}
}
