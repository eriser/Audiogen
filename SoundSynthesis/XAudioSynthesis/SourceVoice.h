#pragma once

namespace SoundSynthesis { namespace XAudioSynthesis
{
	class AudioEngine;
	class AudioFrameSource;
	class SampleProducer;

	class SourceVoice sealed : IXAudio2VoiceCallback
	{
	public:
		SourceVoice(_In_ AudioEngine *engine, _In_ AudioFrameSource *frameSource) noexcept;
		~SourceVoice() noexcept;

		_Check_return_
		bool CreateResources(_In_ IXAudio2 *xaudio, _In_ IXAudio2Voice *output, const WAVEFORMATEX *waveFormat) noexcept;
		void TearDown() noexcept;
		_Check_return_
		bool Start(double x, double y) noexcept;
		void Move(double x, double y) noexcept;
		void Stop() noexcept;

	private:
		//
		// Inherited via IXAudio2VoiceCallback
		//
		void OnVoiceProcessingPassStart(UINT32 BytesRequired) noexcept override;
		void OnVoiceProcessingPassEnd() noexcept override;
		void OnStreamEnd() noexcept override;
		void OnBufferStart(void *pBufferContext) noexcept override;
		void OnBufferEnd(void *pBufferContext) noexcept override;
		void OnLoopEnd(void *pBufferContext) noexcept override;
		void OnVoiceError(void *pBufferContext, HRESULT error) noexcept override;

	private:
		volatile INT16				m_started;
		AudioEngine * const			m_engine;
		AudioFrameSource * const	m_frameSource;
		IXAudio2SourceVoice			*m_voice;
		size_t						m_channelsNumber;
		bool						m_eofSent;
		WORD						m_eofBytes;
		SampleProducer				*m_sampleProducer;
	};

}}
