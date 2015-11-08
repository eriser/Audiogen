#pragma once

#include "Oscillator.h"

namespace SoundSynthesis {
	namespace Helpers
	{
		class SawtoothOscillator : public Oscillator
		{
		public:
			SawtoothOscillator(double baseFrequency, double octaveRange, unsigned int samplingRate, unsigned int channelsNumber)
			:	Oscillator(baseFrequency, octaveRange, samplingRate, channelsNumber)
			{
			}

		protected:
			float ProduceSample(double time) const override
			{
				double sample;
				time = ::remainder(time, M_2PI);

				if (time < 0.0)
					time += M_2PI;

				if (time <= M_PI_2)
				{
					sample = time / M_PI_2;
				}
				else if (time <= M_PI_2 * 3.0)
				{
					sample = 1.0 - (time - M_PI_2) / M_PI_2;
				}
				else
				{
					sample = (time - M_PI_2 * 3.0) / M_PI_2 - 1.0;
				}

				return static_cast<float>(sample);
			}
		};
	}
}
