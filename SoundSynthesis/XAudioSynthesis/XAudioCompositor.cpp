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
:	m_refs(1),
	m_xaudio2(nullptr),
	m_masteringVoice(nullptr)
{
}

XAudioCompositor::~XAudioCompositor() noexcept
{
	_ASSERTE(0 == m_refs);
	_ASSERTE(nullptr == m_xaudio2);
	_ASSERTE(nullptr == m_masteringVoice);
}

void XAudioCompositor::Retain() noexcept
{
	_InterlockedIncrement(&m_refs);
}

void XAudioCompositor::Release() noexcept
{
	if (0 == _InterlockedDecrement(&m_refs))
	{
		TearDown();
		delete this;
	}
}

_Check_return_
IAudioSource *XAudioCompositor::GetOscillatingSource(_In_ PFOSCILLATOR oscillator) noexcept
{
	((void)oscillator);

	return nullptr;
}

bool XAudioCompositor::SetUp() noexcept
{
	bool succeeded = false;

	if (SUCCEEDED(::XAudio2Create(&m_xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
	{
		if (SUCCEEDED(m_xaudio2->CreateMasteringVoice(&m_masteringVoice)))
		{
			m_masteringVoice->GetVoiceDetails(&m_voiceDetails);
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

void XAudioCompositor::TearDown() noexcept
{
	if (nullptr != m_xaudio2)
	{
		m_xaudio2->Release();
	}
}
