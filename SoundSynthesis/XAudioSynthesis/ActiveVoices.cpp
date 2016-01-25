#include "pch.h"
#include "ActiveVoices.h"
#include "OscillatingActiveVoice.h"

using namespace SoundSynthesis::XAudioSynthesis;
using namespace SoundSynthesis::Helpers;

ActiveVoices::ActiveVoices() noexcept
:	m_cleanupThread(NULL)
{
}

ActiveVoices::~ActiveVoices() noexcept
{
	_ASSERTE(!m_cleanupThread);
}

bool ActiveVoices::Initialize() noexcept
{
	_ASSERTE(!m_cleanupThread);

	if (
#ifdef _DEBUG
		::InitializeCriticalSectionEx(&m_monitor, 4000, 0)
#else
		::InitializeCriticalSectionEx(&m_monitor, 4000, CRITICAL_SECTION_NO_DEBUG_INFO)
#endif
	)
	{
		m_stopCleanupThread = ::CreateEvent(NULL, TRUE, FALSE, NULL);

		if (m_stopCleanupThread)
		{
			m_cleanupThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, CleanupThreadStub, this, 0, NULL));

			if (!m_cleanupThread)
			{
				::CloseHandle(m_stopCleanupThread);
				m_stopCleanupThread = NULL;
			}
		}

		if (!m_cleanupThread)
		{
			::DeleteCriticalSection(&m_monitor);
		}
	}

	return NULL != m_cleanupThread;
}

void ActiveVoices::TearDown() noexcept
{
	if (m_cleanupThread)
	{
		_ASSERTE(NULL != m_stopCleanupThread);
		::SetEvent(m_stopCleanupThread);
		::WaitForSingleObject(m_cleanupThread, INFINITE);
		::CloseHandle(m_cleanupThread);
		m_cleanupThread = NULL;
		::CloseHandle(m_stopCleanupThread);
		m_stopCleanupThread = NULL;
	}
}

_Check_return_
ActiveVoice *ActiveVoices::CreateOscillatingVoice(_In_ IXAudio2 *audio,
	_In_ IXAudio2Voice *receiver,
	_In_ const WAVEFORMATEX *waveFormat,
	_In_ PFOSCILLATOR oscillator) noexcept
{
	_ASSERTE(NULL != m_cleanupThread);

	OscillatingActiveVoice *voice = new(std::nothrow) OscillatingActiveVoice(this, waveFormat, oscillator);

	if (nullptr != voice)
	{
		if (!voice->CreateSourceVoice(audio, receiver) || !AddVoice(voice))
		{
			voice->Release();
			voice = nullptr;
		}
	}

	return voice;
}

void ActiveVoices::CleanupAsync() noexcept
{
	_ASSERTE(m_cleanupThread);
	::QueueUserAPC(CleanupAPC, m_cleanupThread, reinterpret_cast<ULONG_PTR>(this));
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
			voice->Retain();
			added = m_voices.insert(voice).second;
			if(!added)
				voice->Release();
		}
		catch (...)
		{
		}
		::LeaveCriticalSection(&m_monitor);
	}

	return added;
}

unsigned int ActiveVoices::CleanupThreadStub(void *arg) noexcept
{
	reinterpret_cast<ActiveVoices*>(arg)->CleanupThreadBody();
	_endthreadex(0);
	return 0;
}

void ActiveVoices::CleanupThreadBody() noexcept
{
	_ASSERTE(m_stopCleanupThread);

	while (WAIT_OBJECT_0 != ::WaitForSingleObjectEx(m_stopCleanupThread, INFINITE, TRUE))
	{
		//
		// Do nothing; APCs have been executed.
		//
	}
}

VOID NTAPI ActiveVoices::CleanupAPC(_In_ ULONG_PTR parameter) noexcept
{
	reinterpret_cast<ActiveVoices*>(parameter)->CleanupBody();
}

void ActiveVoices::CleanupBody() noexcept
{
	::EnterCriticalSection(&m_monitor);
	//
	// Remove all deactivated active voices from the collection
	//
	for (auto itr = m_voices.begin(); itr != m_voices.end(); )
	{
		if ((*itr)->IsActive())
		{
			++itr;
		}
		else
		{
			ActiveVoice *voice = *itr;

			itr = m_voices.erase(itr);
			voice->Recycle();
			voice->Release();
		}
	}

	::LeaveCriticalSection(&m_monitor);
}
