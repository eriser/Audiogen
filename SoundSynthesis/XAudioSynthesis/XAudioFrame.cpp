#include "pch.h"
#include "XAudioFrame.h"

using namespace SoundSynthesis::XAudioSynthesis;

XAudioFrame::XAudioFrame(UINT32 flags, UINT32 audioBytes) noexcept
{
	//
	// Operator new allocates the object with extra data and zeroes out the allocated memory;
	// it is safe to simply assign the fields of the buffer structure.
	//
	m_buffer.Flags = flags;
	m_buffer.AudioBytes = audioBytes;
	m_buffer.pContext = this;
	m_buffer.pAudioData = reinterpret_cast<const BYTE *>(m_data);
}

void *XAudioFrame::operator new(size_t headerSize, UINT32 audioDataLength) noexcept
{
	return calloc(1, headerSize + audioDataLength - sizeof(m_data[0]));
}

void XAudioFrame::operator delete(void *instance) noexcept
{
	free(instance);
}

_Check_return_
XAudioFrame *XAudioFrame::Create(UINT32 flags, UINT32 bytesLength) noexcept
{
	return new(bytesLength) XAudioFrame(flags, bytesLength);
}

_Check_return_
bool XAudioFrame::Submit(_In_ IXAudio2SourceVoice *sourceVoice) noexcept
{
	return SUCCEEDED(sourceVoice->SubmitSourceBuffer(&m_buffer));
}
