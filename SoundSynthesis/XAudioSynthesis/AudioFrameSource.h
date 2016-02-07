#pragma once

#include "SampleWriter.h"

namespace SoundSynthesis { namespace XAudioSynthesis {

	class AudioFrameSource sealed
	{
	public:
		AudioFrameSource() noexcept;
		~AudioFrameSource() noexcept;

		static XAUDIO2_BUFFER *FromContext(_In_ void *context) noexcept
		{
			XAudioFrame *frame = reinterpret_cast<XAudioFrame*>(context);
			_ASSERTE(frame == frame->pContext);
			return frame;
		}

		static SampleWriter GetSampleWriter(_In_ XAUDIO2_BUFFER *frame, _Out_ size_t *blockCount) noexcept
		{
			*blockCount = frame->AudioBytes / sizeof(float);
			return SampleWriter(static_cast<XAudioFrame*>(frame)->audioData, frame->AudioBytes / sizeof(float));
		}

		static bool Submit(_In_ XAUDIO2_BUFFER *frame, _In_ IXAudio2SourceVoice *sourceVoice) noexcept;
		XAUDIO2_BUFFER *Allocate(UINT32 flags, UINT32 audioBytes) noexcept;
		void Release(_In_ XAUDIO2_BUFFER *buffer) noexcept;

	private:
		struct XAudioFrame sealed : XAUDIO2_BUFFER
		{
			float audioData[1];
		};
	};

}}
