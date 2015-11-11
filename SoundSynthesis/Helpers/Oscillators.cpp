#include "pch.h"
#include "Oscillators.h"

namespace SoundSynthesis
{
	namespace Helpers
	{
		double WINAPI SineWaveOscillator(double phase)
		{
			return ::sin(phase);
		}

		double WINAPI SawtoothOscillator(double phase)
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

			return sample;
		}

		double WINAPI StringOscillator(double phase)
		{
			return 1.0 - phase / M_PI;
		}
	}
}
