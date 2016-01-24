#include "pch.h"
#include "ActiveVoices.h"
#include "OscillatingActiveVoice.h"

using namespace SoundSynthesis::XAudioSynthesis;
using namespace SoundSynthesis::Helpers;

ActiveVoices::ActiveVoices() noexcept
{
}

ActiveVoices::~ActiveVoices() noexcept
{
	if (m_initialized)
	{
		::DeleteCriticalSection(&m_monitor);
	}
}

bool ActiveVoices::Initialize() noexcept
{
	_ASSERTE(!m_initialized);
	m_initialized = 0 !=
#ifdef _DEBUG
		::InitializeCriticalSectionEx(&m_monitor, 4000, 0);
#else
		::InitializeCriticalSectionEx(&m_monitor, 4000, CRITICAL_SECTION_NO_DEBUG_INFO);
#endif

	return m_initialized;
}

_Check_return_
ActiveVoice *ActiveVoices::CreateOscillatingVoice(_In_ const WAVEFORMATEX *waveFormat, _In_ PFOSCILLATOR oscillator) noexcept
{
	_ASSERTE(m_initialized);

	OscillatingActiveVoice *voice = new(std::nothrow) OscillatingActiveVoice(waveFormat, oscillator);

	if (!AddVoice(voice) && nullptr != voice)
	{
		voice->Release();
		voice = nullptr;
	}

	return voice;
}

_Check_return_
bool ActiveVoices::AddVoice(_In_ ActiveVoice *voice) noexcept
{
	bool added = false;

	if (nullptr != voice)
	{
		::EnterCriticalSection(&m_monitor);
		try
		{
			added = m_voices.insert(voice).second;
		}
		catch (...)
		{
		}
		::LeaveCriticalSection(&m_monitor);
	}

	return added;
}
