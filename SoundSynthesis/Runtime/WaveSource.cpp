#include "pch.h"
#include "WaveSource.h"

using namespace SoundSynthesis::Runtime;
using namespace Platform;

WaveSource::WaveSource(unsigned int samplingRate, unsigned int channelsNumber)
:	m_oscillator( samplingRate, channelsNumber )
{
}

void WaveSource::GenerateWave(
	Windows::Foundation::IMemoryBufferReference ^bufferReference,
	int samplesNumber)
{
	using namespace Windows::Foundation;
	IMemoryBufferByteAccess *byteAccess;
	if (S_OK == reinterpret_cast<IUnknown*>(bufferReference)->QueryInterface(&byteAccess))
	{
		BYTE	*bufferBytes;
		UINT32	bufferCapacity;

		if (S_OK == byteAccess->GetBuffer(&bufferBytes, &bufferCapacity))
		{
			// TODO: Ask the oscillator to produce wave data.
			m_oscillator.GenerateSamples(bufferBytes, bufferCapacity);
		}

		byteAccess->Release();
	}
}
