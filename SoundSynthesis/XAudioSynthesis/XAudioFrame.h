#pragma once

#include "SampleWriter.h"

namespace SoundSynthesis { namespace XAudioSynthesis
{
	//
	// Audio frame object allocated on the heap as one contiguous memory block.
	//
	class XAudioFrame
	{
	private:
		//
		// Make the constructor private so only the factory method can create instances of the class.
		//
		XAudioFrame(UINT32 audioBytes) noexcept;
		//
		// Make the destructor private so no one outside of the class can delete it.
		//
		~XAudioFrame() noexcept {}
		//
		// Override the new operator to allocate XAudioFrame with extra room for audio data and zero out allocated memory.
		//
		void *operator new(size_t headerSize, UINT32 audioDataLength) noexcept;
		void operator delete(void *instance) noexcept;

	public:

		_Check_return_
		static XAudioFrame *Create(UINT32 bytesLength) noexcept;

		_Check_return_
		static XAudioFrame *FromContext(_In_ void *context) noexcept
		{
			_ASSERTE(nullptr != context);
			XAudioFrame *frame = reinterpret_cast<XAudioFrame*>(context);
			_ASSERTE(frame->m_buffer.pContext == frame);
			return frame;
		}
		//
		// Get the lightweight writer object that will be used immediately to fill up the buffer with samples.
		//
		_Check_return_
		SampleWriter GetSampleWriter(_Out_ size_t *capacity) noexcept
		{
			size_t cap = m_buffer.AudioBytes / sizeof(float);
			*capacity = cap;
			return SampleWriter(m_data, cap);
		}

		_Check_return_
		bool Submit(_In_ IXAudio2SourceVoice *sourceVoice) noexcept;

		void Destroy() noexcept
		{
			//
			// Use the private overridden operator delete.
			//
			delete this;
		}

	private:
		XAUDIO2_BUFFER	m_buffer;
		float			m_data[1];
	};
}}
