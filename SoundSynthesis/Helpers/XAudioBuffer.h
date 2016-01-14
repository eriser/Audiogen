#pragma once

namespace SoundSynthesis { namespace Helpers
{
	class XAudioBuffer
	{
	private:
		XAUDIO2_BUFFER	m_buffer;
		float			*m_samples;

	public:
		XAudioBuffer();
		~XAudioBuffer();

		_Check_return_
		const XAUDIO2_BUFFER *GetBuffer() const { return &m_buffer; }

		_Check_return_
		float *LockBuffer(UINT32 framesNumber, UINT32 channelsNumber);
	};

}}