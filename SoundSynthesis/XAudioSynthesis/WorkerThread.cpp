#include "pch.h"
#include "WorkerThread.h"

using namespace SoundSynthesis::XAudioSynthesis;

WorkerThread::WorkerThread() noexcept
:	m_thread(NULL),
	m_stop(NULL)
{
}

WorkerThread::~WorkerThread() noexcept
{
	_ASSERTE(NULL == m_thread);
	_ASSERTE(NULL == m_stop);
}

_Check_return_
bool WorkerThread::Start() noexcept
{
	bool started = false;

	if (NULL == m_thread)
	{
		_ASSERTE(NULL == m_stop);

		m_stop = ::CreateEvent(NULL, TRUE, FALSE, NULL);

		if (NULL != m_stop)
		{
			m_thread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, ThreadStub, this, 0, NULL));

			if (NULL == m_thread)
			{
				::CloseHandle(m_stop);
				m_stop = NULL;
			}
			else
			{
				started = true;
			}
		}
	}

	return started;
}

void WorkerThread::Stop() noexcept
{
	if (NULL != m_stop)
	{
		_ASSERTE(NULL != m_thread);
		::SetEvent(m_stop);
		::WaitForSingleObject(m_thread, INFINITE);
		::CloseHandle(m_thread);
		m_thread = NULL;
		::CloseHandle(m_stop);
		m_stop = NULL;
	}
}

void WorkerThread::Queue(_In_ PAPCFUNC function, ULONG_PTR parameter) noexcept
{
	_ASSERTE(NULL != m_thread);
	::QueueUserAPC(function, m_thread, parameter);
}

unsigned int WorkerThread::ThreadStub(void *arg) noexcept
{
	reinterpret_cast<const WorkerThread*>(arg)->ThreadProc();
	_endthreadex(0);
	return 0;
}

void WorkerThread::ThreadProc() const noexcept
{
	while (WAIT_OBJECT_0 != ::WaitForSingleObjectEx(m_stop, INFINITE, TRUE))
	{
		//
		// Do nothing.
		//
	}
}
