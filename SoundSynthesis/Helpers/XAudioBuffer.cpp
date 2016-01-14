#include "pch.h"
#include "XAudioBuffer.h"

using namespace SoundSynthesis::Helpers;

XAudioBuffer::XAudioBuffer()
:	m_samples(nullptr)
{
	SecureZeroMemory(&m_buffer, sizeof(m_buffer));

	m_buffer.pContext = this;
}

XAudioBuffer::~XAudioBuffer()
{
	m_buffer.pAudioData = nullptr;
	delete[] m_samples;
}

_Check_return_
float *XAudioBuffer::LockBuffer(UINT32 framesNumber, UINT32 channelsNumber)
{
	float	*data = nullptr;
	UINT32	byteLength = framesNumber * channelsNumber * sizeof(float);

	if (byteLength <= XAUDIO2_MAX_BUFFER_BYTES)
	{
		if (m_buffer.AudioBytes < byteLength)
		{
			float *newBuffer = new(std::nothrow) float[framesNumber * channelsNumber];

			if (newBuffer)
			{
				delete[] m_samples;
				m_samples = newBuffer;
				data = m_samples;
				m_buffer.AudioBytes = byteLength;
			}
		}
		else
		{
			data = m_samples;
		}

		if (data)
			m_buffer.PlayLength = byteLength;
	}

	return data;
}
