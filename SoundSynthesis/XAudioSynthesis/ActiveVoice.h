#pragma once

#include "AudioFrameSource.h"

namespace SoundSynthesis { namespace XAudioSynthesis
{
	class ActiveVoices;

	class ActiveVoice : public IXAudio2VoiceCallback
	{
	public:
		void Retain() noexcept;
		void Release() noexcept;

		bool IsActive() const noexcept
		{
			return 0 != m_isActive;
		}

		void Start(float x, float y) noexcept;
		void Move(float x, float y) noexcept;
		void Stop() noexcept;

		virtual void Recycle() noexcept;

		_Check_return_
		virtual bool CreateSourceVoice(_In_ IXAudio2 *audio, _In_ IXAudio2Voice *receiver) noexcept;
		virtual void DestroySourceVoice() noexcept;

	protected:
		ActiveVoice(_In_ ActiveVoices *container, _In_ const WAVEFORMATEX *waveFormat) noexcept;
		virtual ~ActiveVoice() noexcept;

		AudioFrameSource *FrameSource() const noexcept;
		virtual XAUDIO2_BUFFER *MakeAudioFrame(UINT32 bytesRequired, _In_ const WAVEFORMATEX *waveFormat) noexcept = 0;
		XAUDIO2_BUFFER *MakeEndOfStreamFrame(_In_ const WAVEFORMATEX *waveFormat) noexcept;

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
		volatile INT16				m_isActive;
		ActiveVoices * const		m_container;
		const WAVEFORMATEX * const	m_waveFormat;
		IXAudio2SourceVoice			*m_sourceVoice;
		IXAudio2Voice				*m_receiverVoice;
	};
}}
