#include "pch.h"
#include "XAudioCompositor.h"

using namespace SoundSynthesis::XAudioSynthesis;
using namespace SoundSynthesis::Helpers;

_Check_return_
IAudioCompositor *XAudioCompositor::Create() noexcept
{
	XAudioCompositor *compositor = new(std::nothrow) XAudioCompositor();

	if (compositor && !compositor->SetUp())
	{
		compositor->Release();
		compositor = nullptr;
	}

	return compositor;
}

XAudioCompositor::XAudioCompositor() noexcept
:	m_xaudio2(nullptr),
	m_masteringVoice(nullptr)
{
	SecureZeroMemory(&m_waveFormat, sizeof(m_waveFormat));
	m_waveFormat.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
	m_waveFormat.wBitsPerSample = sizeof(float) * 8;
}

XAudioCompositor::~XAudioCompositor() noexcept
{
	_ASSERTE(nullptr == m_xaudio2);
	_ASSERTE(nullptr == m_masteringVoice);
}

_Check_return_
IAudioSource *XAudioCompositor::GetOscillatingSource(_In_ PFOSCILLATOR oscillator) noexcept
{
	((void)oscillator);

	return nullptr;
}

_Check_return_
bool XAudioCompositor::SetUp() noexcept
{
	bool succeeded = false;

	if (SUCCEEDED(::XAudio2Create(&m_xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
	{
		if (SUCCEEDED(m_xaudio2->CreateMasteringVoice(&m_masteringVoice)))
		{
			XAUDIO2_VOICE_DETAILS voiceDetails;

			m_masteringVoice->GetVoiceDetails(&voiceDetails);

			m_waveFormat.nSamplesPerSec = voiceDetails.InputSampleRate;
			m_waveFormat.nChannels = static_cast<WORD>(voiceDetails.InputChannels);
			m_waveFormat.nBlockAlign = (m_waveFormat.nChannels * m_waveFormat.wBitsPerSample) / 8;
			m_waveFormat.nAvgBytesPerSec = m_waveFormat.nBlockAlign * m_waveFormat.nSamplesPerSec;

			succeeded = true;
		}

		if (!succeeded)
		{
			_ASSERTE(nullptr == m_masteringVoice);
			m_xaudio2->Release();
			m_xaudio2 = nullptr;
		}
	}

	return succeeded;
}

void XAudioCompositor::FinalRelease() noexcept
{
	if (nullptr != m_xaudio2)
	{
		_ASSERTE(nullptr != m_masteringVoice);
		m_masteringVoice->DestroyVoice();
		m_masteringVoice = nullptr;

		m_xaudio2->Release();
		m_xaudio2 = nullptr;
	}

	_ASSERTE(nullptr == m_masteringVoice);
	_ASSERTE(nullptr == m_xaudio2);
}
