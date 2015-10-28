namespace Audiogen.Controls
{
    using Audiogen.ViewModels;
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

        private bool _pointerDown;
        private uint _pointerId;
        public Touchboard()
        {
            this.InitializeComponent();
            _pointerDown = false;
        }


        public IPointerHandler PointerHandler
        {
            get { return (IPointerHandler)GetValue(PointerHandlerProperty); }
            set { SetValue(PointerHandlerProperty, value); }
        }

        protected override void OnPointerPressed(PointerRoutedEventArgs e)
        {
            base.OnPointerPressed(e);

            if (!_pointerDown)
            {
                PointerPoint point = e.GetCurrentPoint(this);

                _pointerDown = true;
                _pointerId = e.Pointer.PointerId;

                this.PointerHandler?.Down(new PointerPosition(
                    point.Position.X / this.ActualWidth,
                    point.Position.Y / this.ActualHeight));
            }
        }

        protected override void OnPointerReleased(PointerRoutedEventArgs e)
        {
            base.OnPointerReleased(e);

            if (IsTrackedPointer(e.Pointer))
            {
                _pointerDown = false;
                this.PointerHandler?.Up();
            }
        }

        protected override void OnPointerCanceled(PointerRoutedEventArgs e)
        {
            base.OnPointerCanceled(e);

            if (IsTrackedPointer(e.Pointer))
            {
                _pointerDown = false;
                this.PointerHandler?.Up();
            }
        }

        protected override void OnPointerMoved(PointerRoutedEventArgs e)
        {
            base.OnPointerMoved(e);

            if (IsTrackedPointer(e.Pointer))
            {
                PointerPoint point = e.GetCurrentPoint(this);

                this.PointerHandler?.Move(new PointerPosition(
                    point.Position.X / this.ActualWidth,
                    point.Position.Y / this.ActualHeight));
            }
        }

        protected override void OnPointerExited(PointerRoutedEventArgs e)
        {
            base.OnPointerExited(e);

            if (IsTrackedPointer(e.Pointer))
            {
                _pointerDown = false;
                this.PointerHandler?.Up();
            }
        }

        protected override void OnPointerEntered(PointerRoutedEventArgs e)
        {
            base.OnPointerEntered(e);

            if(!_pointerDown && e.Pointer.IsInContact)
            {
                PointerPoint point = e.GetCurrentPoint(this);

                _pointerDown = true;
                _pointerId = e.Pointer.PointerId;

                this.PointerHandler?.Down(new PointerPosition(
                    point.Position.X / this.ActualWidth,
                    point.Position.Y / this.ActualHeight));
            }
        }

        private void OnPointerHandlerChanged(DependencyPropertyChangedEventArgs e)
        {
        }

        private bool IsTrackedPointer(Pointer pointer)
        {
            return _pointerDown && pointer.PointerId == _pointerId;
        }
    }
}
