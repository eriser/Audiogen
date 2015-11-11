#pragma once

#include "SampleGenerator.h"

namespace SoundSynthesis {
	namespace Helpers
	{
		class SawtoothOscillator : public SampleGenerator
		{
		public:
			SawtoothOscillator(double baseFrequency, double octaveRange, unsigned int samplingRate, unsigned int channelsNumber)
			:	SampleGenerator(baseFrequency, octaveRange, samplingRate, channelsNumber)
			{
			}

		protected:
			float ProduceSample(double phase) const override
			{
				double sample;

				if (phase <= M_PI_2)
				{
					sample = phase / M_PI_2;
				}
				else if (phase <= M_PI_2 * 3.0)
				{
					sample = 1.0 - (phase - M_PI_2) / M_PI_2;
				}
				else
				{
					sample = (phase - M_PI_2 * 3.0) / M_PI_2 - 1.0;
				}

				return static_cast<float>(sample);
			}
		};
	}
}
