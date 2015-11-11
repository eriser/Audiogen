#pragma once

#include "SampleGenerator.h"

namespace SoundSynthesis {
	namespace Helpers
	{
		class SineWaveOscillator : public Oscillator
		{
		public:
			SineWaveOscillator(double baseFrequency, double octaveRange, unsigned int samplingRate, unsigned int channelsNumber)
			:	Oscillator(baseFrequency, octaveRange, samplingRate, channelsNumber)
			{
			}

		protected:
			float ProduceSample(double phase) const override
			{
				return static_cast<float>(::sin(phase));
			}
		};
	}
}
