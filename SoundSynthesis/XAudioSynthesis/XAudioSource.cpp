#include "pch.h"
#include "XAudioSource.h"

using namespace SoundSynthesis::XAudioSynthesis;
using namespace SoundSynthesis::Helpers;

XAudioSource::XAudioSource(IAudioCompositor *compositor) noexcept
:	m_compositor(compositor)
{
	_ASSERTE(nullptr != m_compositor);
	m_compositor->Retain();
}

XAudioSource::~XAudioSource() noexcept
{
	m_compositor->Release();
}

void XAudioSource::FinalRelease() noexcept
{
}
