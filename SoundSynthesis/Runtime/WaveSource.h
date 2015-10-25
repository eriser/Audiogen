#pragma once

namespace SoundSynthesis { namespace Runtime
{
	public ref class WaveSource sealed
	{
	public:
		WaveSource();

		void GenerateWave(
			Windows::Foundation::IMemoryBufferReference ^bufferReference,
			unsigned int channelsNumber,
			int samplesNumber);
	};
}}
