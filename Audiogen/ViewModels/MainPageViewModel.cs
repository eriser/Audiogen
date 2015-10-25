namespace Audiogen.ViewModels
{
    using Audiogen.Models;
    using System;

    sealed class MainPageViewModel : ViewModelBase
    {
        private IDispatcher _dispatcher;
        private ISynthesizer _synthesizer;
        private bool _isInitializing;
        private bool _isReady;
        private bool _isFailed;

        public MainPageViewModel()
        {
            _isInitializing = true;
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
    }
}
