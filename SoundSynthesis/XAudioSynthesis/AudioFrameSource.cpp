#include "pch.h"
#include "AudioFrameSource.h"

using namespace SoundSynthesis::XAudioSynthesis;

AudioFrameSource::AudioFrameSource() noexcept
{
}

AudioFrameSource::~AudioFrameSource() noexcept
{
}

_Check_return_
bool AudioFrameSource::Submit(_In_ XAUDIO2_BUFFER *frame, _In_ IXAudio2SourceVoice *sourceVoice) noexcept
{
	return SUCCEEDED(sourceVoice->SubmitSourceBuffer(frame));
}

_Check_return_
XAUDIO2_BUFFER *AudioFrameSource::Allocate(UINT32 flags, UINT32 audioBytes) noexcept
{
	UINT32		dataLength	= ((audioBytes + sizeof(float) - 1) / sizeof(float)) * sizeof(float);
	size_t		fullSize	= sizeof(XAudioFrame) - sizeof(float) + dataLength;
	XAudioFrame	*frame		= reinterpret_cast<XAudioFrame*>(calloc(1, fullSize));

	if (frame)
	{
		frame->Flags		= flags;
		frame->AudioBytes	= dataLength;
		frame->pAudioData	= reinterpret_cast<const BYTE*>(frame->audioData);
		frame->pContext		= frame;
	}

	return frame;
}

void AudioFrameSource::Release(_In_ XAUDIO2_BUFFER *buffer) noexcept
{
	free(buffer);
}
