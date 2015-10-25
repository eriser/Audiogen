namespace Audiogen.Helpers
{
    using System;
    using Audiogen.ViewModels;
    using Windows.UI.Xaml;
    using Windows.UI.Core;

    sealed class Dispatcher : DependencyObject, IDispatcher
    {
        void IDispatcher.Dispatch(Action action)
        {
            var ignored = this.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => action());
        }
    }
}
