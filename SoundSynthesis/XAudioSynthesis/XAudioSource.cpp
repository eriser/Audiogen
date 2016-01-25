#include "pch.h"
#include "XAudioSource.h"

using namespace SoundSynthesis::XAudioSynthesis;
using namespace SoundSynthesis::Helpers;

XAudioSource::XAudioSource(_In_ IAudioCompositor *compositor, _In_ ActiveVoice *voice) noexcept
:	m_compositor(compositor),
	m_voice(voice)
{
	_ASSERTE(nullptr != m_compositor);
	_ASSERTE(nullptr != m_voice);
	m_compositor->Retain();
	m_voice->Retain();
}

XAudioSource::~XAudioSource() noexcept
{
	_ASSERTE(nullptr == m_voice);
	m_compositor->Release();
}

void XAudioSource::FinalRelease() noexcept
{
	m_voice->Stop();
	m_voice->Release();
	m_voice = nullptr;
}
