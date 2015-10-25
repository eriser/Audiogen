namespace Audiogen.ViewModels
{
    using Helpers;
    using Audiogen.Models;
    using System;
    using System.Windows.Input;

    sealed class MainPageViewModel : ViewModelBase
    {
        private readonly DelegateCommand _start;
        private readonly DelegateCommand _stop;
        private IDispatcher _dispatcher;
        private ISynthesizer _synthesizer;
        private bool _isInitializing;
        private bool _isReady;
        private bool _isFailed;
        private bool _isRunning;

        public MainPageViewModel()
        {
            _start = new DelegateCommand(this.ExecuteStart, this.CanExecuteStart);
            _stop = new DelegateCommand(this.ExecuteStop, this.CanExecuteStop);
            _isInitializing = true;
            _isReady = false;
            _isFailed = false;
            _isRunning = false;
        }

        public IDispatcher Dispatcher
        {
            get { return _dispatcher; }
            set
            {
                if(this.SetProperty(ref _dispatcher, value))
                {
                    SetUpSynthesizerIfFullyComposed();
                }
            }
        }

        public ISynthesizer Synthesizer
        {
            get { return _synthesizer; }
            set
            {
                ISynthesizer oldSynthesizer = _synthesizer;

                if (this.SetProperty(ref _synthesizer, value))
                {
                    if (null != oldSynthesizer)
                    {
                        oldSynthesizer.Ready -= this.OnSynthesizerReady;
                        oldSynthesizer.Failed -= this.OnSynthesizerFailed;
                    }

                    if (null != _synthesizer)
                    {
                        _synthesizer.Ready += this.OnSynthesizerReady;
                        _synthesizer.Failed += this.OnSynthesizerFailed;
                    }

                    SetUpSynthesizerIfFullyComposed();
                }
            }
        }

        public bool IsInitializing
        {
            get { return _isInitializing; }
            private set { this.SetProperty(ref _isInitializing, value); }
        }

        public bool IsReady
        {
            get { return _isReady; }
            private set
            {
                if (this.SetProperty(ref _isReady, value))
                    this.IsInitializing = !(_isReady || _isFailed);
            }
        }

        public bool IsFailed
        {
            get { return _isFailed; }
            private set
            {
                if(this.SetProperty(ref _isFailed, value))
                    this.IsInitializing = !(_isReady || _isFailed);
            }
        }

        public ICommand Start
        {
            get { return _start; }
        }

        public ICommand Stop
        {
            get { return _stop; }
        }

        private void SetUpSynthesizerIfFullyComposed()
        {
            if(null != _synthesizer && null != _dispatcher)
            {
                _synthesizer.SetUp();
            }
        }

        private void OnSynthesizerReady(object sender, EventArgs e)
        {
            this.Dispatcher.Dispatch(() => this.IsReady = true);
        }

        private void OnSynthesizerFailed(object sender, EventArgs e)
        {
            this.Dispatcher.Dispatch(() => this.IsFailed = true);
        }

        private void ExecuteStart(object parameter)
        {
            _synthesizer.Start();
            _isRunning = true;
            _start.EmitCanExecuteChanged();
            _stop.EmitCanExecuteChanged();
        }

        private bool CanExecuteStart(object parameter)
        {
            return !_isRunning;
        }

        private void ExecuteStop(object parameter)
        {
            _synthesizer.Stop();
            _isRunning = false;
            _start.EmitCanExecuteChanged();
            _stop.EmitCanExecuteChanged();
        }

        private bool CanExecuteStop(object parameter)
        {
            return _isRunning;
        }
    }
}
