#pragma once

#include "SampleGenerator.h"

namespace SoundSynthesis {
	namespace Helpers
	{
		class StringOscillator : public SampleGenerator
		{
		public:
			StringOscillator(double baseFrequency, double octaveRange, unsigned int samplingRate, unsigned int channelsNumber)
			:	SampleGenerator(baseFrequency, octaveRange, samplingRate, channelsNumber)
			{
			}

		protected:
			float ProduceSample(double phase) const override
			{
				return static_cast<float>(1.0 - phase / M_PI);
			}
		};
	}
}
