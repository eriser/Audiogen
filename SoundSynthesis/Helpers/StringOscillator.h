#pragma once

#include "Oscillator.h"

namespace SoundSynthesis {
	namespace Helpers
	{
		class StringOscillator : public Oscillator
		{
		public:
			StringOscillator(double baseFrequency, double octaveRange, unsigned int samplingRate, unsigned int channelsNumber)
			:	Oscillator(baseFrequency, octaveRange, samplingRate, channelsNumber)
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
