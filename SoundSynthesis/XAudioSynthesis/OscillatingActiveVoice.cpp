#include "pch.h"
#include "OscillatingActiveVoice.h"
#include "XAudioFrame.h"

using namespace SoundSynthesis::XAudioSynthesis;
using namespace SoundSynthesis::Helpers;

OscillatingActiveVoice::OscillatingActiveVoice(_In_ ActiveVoices *container, _In_ const WAVEFORMATEX *waveFormat, _In_ PFOSCILLATOR oscillator) noexcept
:	ActiveVoice(container, waveFormat),
	m_oscillator(oscillator)
{
	_ASSERTE(nullptr != m_oscillator);
}

OscillatingActiveVoice::~OscillatingActiveVoice() noexcept
{
}

_Check_return_
XAudioFrame *OscillatingActiveVoice::MakeAudioFrame(UINT32 bytesRequired, _In_ const WAVEFORMATEX *waveFormat) noexcept
{
	XAudioFrame *frame = XAudioFrame::Create(0, bytesRequired);

	if (frame)
	{
		size_t			sampleBlocks;
		SampleWriter	writer = frame->GetSampleWriter(&sampleBlocks);

		sampleBlocks /= waveFormat->nChannels;

		for (size_t i = 0; i < sampleBlocks; ++i)
		{
			float sample = GetNextSample();
			writer.Write(sample, waveFormat->nChannels);
		}
	}

	return frame;
}

float OscillatingActiveVoice::GetNextSample() noexcept
{
	return (float)(rand() % 100 - 50) / 100.f;
}
