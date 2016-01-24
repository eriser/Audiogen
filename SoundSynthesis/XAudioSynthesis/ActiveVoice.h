#pragma once

#include "XAudioFrame.h"

namespace SoundSynthesis { namespace XAudioSynthesis
{
	class ActiveVoice : public IXAudio2VoiceCallback
	{
	public:
		void Retain() noexcept;
		void Release() noexcept;

	protected:
		ActiveVoice(_In_ const WAVEFORMATEX *waveFormat) noexcept;
		virtual ~ActiveVoice() noexcept;

		virtual XAudioFrame *MakeAudioFrame(UINT32 bytesRequired, _In_ const WAVEFORMATEX *waveFormat) noexcept = 0;

	private:
		STDMETHODIMP_(void) OnVoiceProcessingPassStart(UINT32 BytesRequired) noexcept override;
		STDMETHODIMP_(void) OnVoiceProcessingPassEnd() noexcept override;
		STDMETHODIMP_(void) OnStreamEnd() noexcept override;
		STDMETHODIMP_(void) OnBufferStart(void *pBufferContext) noexcept override;
		STDMETHODIMP_(void) OnBufferEnd(void *pBufferContext) noexcept override;
		STDMETHODIMP_(void) OnLoopEnd(void *pBufferContext) noexcept override;
		STDMETHODIMP_(void) OnVoiceError(void *pBufferContext, HRESULT Error) noexcept override;

	private:
		volatile LONG				m_refs;
		const WAVEFORMATEX * const	m_waveFormat;
		IXAudio2SourceVoice			*m_sourceVoice;
	};
}}
