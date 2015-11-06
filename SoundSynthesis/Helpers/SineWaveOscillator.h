#pragma once

#include "Oscillator.h"

namespace SoundSynthesis {
	namespace Helpers
	{
		class SineWaveOscillator : public Oscillator
		{
		public:
			SineWaveOscillator(unsigned int samplingRate, unsigned int channelsNumber)
			:	Oscillator(samplingRate, channelsNumber)
			{
			}

		protected:
			float ProduceSample(double time) const override
			{
				return static_cast<float>(::sin(time));
			}
		};
	}
}
