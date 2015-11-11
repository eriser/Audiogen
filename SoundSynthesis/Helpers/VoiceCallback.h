#pragma once

namespace SoundSynthesis { namespace Helpers
{

	class VoiceCallback : public IXAudio2VoiceCallback
	{
	private:
		WAVEFORMATEX		m_waveFormat;
		IXAudio2SourceVoice	*m_sourceVoice;

	public:
		VoiceCallback();
		void SetUp(_In_ const XAUDIO2_VOICE_DETAILS *mastringVoiceDetails);
		void SetSourceVoice(_In_ IXAudio2SourceVoice *sourceVoice) { m_sourceVoice = sourceVoice; }

		const WAVEFORMATEX *GetWaveFormat() const { return &m_waveFormat; }

	private:
		STDMETHODIMP_(void) OnVoiceProcessingPassStart(UINT32 BytesRequired) override;
		STDMETHODIMP_(void) OnVoiceProcessingPassEnd() override;
		STDMETHODIMP_(void) OnStreamEnd(THIS) override;
		STDMETHODIMP_(void) OnBufferStart(void* pBufferContext) override;
		STDMETHODIMP_(void) OnBufferEnd(void* pBufferContext) override;
		STDMETHODIMP_(void) OnLoopEnd(void* pBufferContext) override;
		STDMETHODIMP_(void) OnVoiceError(void* pBufferContext, HRESULT Error) override;
	};

}}