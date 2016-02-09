#pragma once

#include "AudioFrameSource.h"

namespace SoundSynthesis { namespace XAudioSynthesis
{

	class WhiteNoise sealed : IXAudio2VoiceCallback
	{
	public:
		WhiteNoise() noexcept;
		~WhiteNoise() noexcept;

		void Start() noexcept;
		void Stop() noexcept;

	private:
		STDMETHODIMP_(void) OnVoiceProcessingPassStart(UINT32 BytesRequired);
		STDMETHODIMP_(void) OnVoiceProcessingPassEnd();
		STDMETHODIMP_(void) OnStreamEnd();
		STDMETHODIMP_(void) OnBufferStart(THIS_ void* pBufferContext);
		STDMETHODIMP_(void) OnBufferEnd(THIS_ void* pBufferContext);
		STDMETHODIMP_(void) OnLoopEnd(THIS_ void* pBufferContext);
		STDMETHODIMP_(void) OnVoiceError(THIS_ void* pBufferContext, HRESULT Error);

	private:
		AudioFrameSource		m_frameSource;
		WORD					m_channels;
		IXAudio2				*m_xaudio2;
		IXAudio2MasteringVoice	*m_masteringVoice;
		IXAudio2SourceVoice		*m_sourceVoice;
	};
}}
