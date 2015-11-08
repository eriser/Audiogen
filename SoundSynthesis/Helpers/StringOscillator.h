#pragma once

#include "Oscillator.h"

namespace SoundSynthesis {
	namespace Helpers
	{
		class StringOscillator : public Oscillator
		{
		public:
			StringOscillator(unsigned int samplingRate, unsigned int channelsNumber)
				: Oscillator(samplingRate, channelsNumber)
			{
			}

		protected:
			float ProduceSample(double time) const override
			{
				double sample;
				time = ::remainder(time, M_2PI);

				if (time < 0.0)
					time += M_2PI;

				sample = 1.0 - time / M_PI;

				return static_cast<float>(sample);
			}
		};
	}
}
