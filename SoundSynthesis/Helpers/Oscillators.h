#pragma once

namespace SoundSynthesis
{
	namespace Helpers
	{
		typedef double (WINAPI *PFOSCILLATOR)(double phase);

		extern double WINAPI SineWaveOscillator(double phase);
		extern double WINAPI SawtoothOscillator(double phase);
		extern double WINAPI StringOscillator(double phase);
		extern double WINAPI LadderOscillator(double phase);
	}
}
