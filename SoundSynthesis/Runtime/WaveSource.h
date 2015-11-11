#pragma once
#include "..\Helpers\SampleGenerator.h"

namespace SoundSynthesis { namespace Runtime
{
	public ref class WaveSource sealed
	{
	private:
		Helpers::SampleGenerator m_sampleGenerator;

	public:
		WaveSource(unsigned int samplingRate, unsigned int channelsNumber);
		virtual ~WaveSource();

		void GenerateWave(
			Windows::Foundation::IMemoryBufferReference ^bufferReference,
			int samplesNumber);

		void BeginWave(double position, double effect);

		void ChangeWave(double position, double effect);

		void EndWave();
	};
}}
