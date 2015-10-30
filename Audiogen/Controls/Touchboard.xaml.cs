namespace Audiogen.Controls
{
    using Audiogen.ViewModels;
    using System.Collections.Generic;
    using System.Diagnostics.Contracts;
    using Windows.UI.Input;
    using Windows.UI.Xaml;
    using Windows.UI.Xaml.Controls;
    using Windows.UI.Xaml.Input;

    public sealed partial class Touchboard : UserControl
    {
        public static readonly DependencyProperty PointerHandlerProperty = DependencyProperty.Register(
            "PointerHandler",
            typeof(IPointerHandler),
            typeof(Touchboard),
            new PropertyMetadata(null, (sender, e) =>
            {
                Touchboard touchboard = (Touchboard)sender;
                touchboard.OnPointerHandlerChanged(e);
            }));

        private readonly IDictionary<uint, IPointerTracker> _pointerTrackers;

        public Touchboard()
        {
            this.InitializeComponent();
            _pointerTrackers = new SortedDictionary<uint, IPointerTracker>();
        }


        public IPointerHandler PointerHandler
        {
            get { return (IPointerHandler)GetValue(PointerHandlerProperty); }
            set { SetValue(PointerHandlerProperty, value); }
        }

        protected override void OnPointerPressed(PointerRoutedEventArgs e)
        {
            base.OnPointerPressed(e);

            PointerPoint point = e.GetCurrentPoint(this);

            IPointerTracker newTracker = this.PointerHandler.TrackPointer(new PointerPosition(
                                            point.Position.X / this.ActualWidth,
                                            point.Position.Y / this.ActualHeight));

            if(null != newTracker)
            {
                Contract.Assert(!_pointerTrackers.ContainsKey(e.Pointer.PointerId));
                _pointerTrackers.Add(e.Pointer.PointerId, newTracker);
            }
        }

        protected override void OnPointerReleased(PointerRoutedEventArgs e)
        {
            base.OnPointerReleased(e);

            RemoveTracker(e.Pointer.PointerId);
        }

        protected override void OnPointerCanceled(PointerRoutedEventArgs e)
        {
            base.OnPointerCanceled(e);

            RemoveTracker(e.Pointer.PointerId);
        }

        protected override void OnPointerMoved(PointerRoutedEventArgs e)
        {
            base.OnPointerMoved(e);

            IPointerTracker tracker;

            if(_pointerTrackers.TryGetValue(e.Pointer.PointerId, out tracker))
            {
                PointerPoint point = e.GetCurrentPoint(this);

                tracker.Move(new PointerPosition(
                    point.Position.X / this.ActualWidth,
                    point.Position.Y / this.ActualHeight));
            }
        }

        protected override void OnPointerExited(PointerRoutedEventArgs e)
        {
            base.OnPointerExited(e);

            RemoveTracker(e.Pointer.PointerId);
        }

        protected override void OnPointerEntered(PointerRoutedEventArgs e)
        {
            base.OnPointerEntered(e);

            IPointerTracker tracker;

            if (!_pointerTrackers.TryGetValue(e.Pointer.PointerId, out tracker) && e.Pointer.IsInContact)
            {
                PointerPoint point = e.GetCurrentPoint(this);

                tracker = this.PointerHandler.TrackPointer(new PointerPosition(
                                    point.Position.X / this.ActualWidth,
                                    point.Position.Y / this.ActualHeight));

                if (null != tracker)
                {
                    Contract.Assert(!_pointerTrackers.ContainsKey(e.Pointer.PointerId));
                    _pointerTrackers.Add(e.Pointer.PointerId, tracker);
                }
            }
        }

        private void RemoveTracker(uint pointerId)
        {
            IPointerTracker tracker;

            if (_pointerTrackers.TryGetValue(pointerId, out tracker))
            {
                _pointerTrackers.Remove(pointerId);
                tracker.Dispose();
            }
        }

        private void OnPointerHandlerChanged(DependencyPropertyChangedEventArgs e)
        {
        }
    }
}
