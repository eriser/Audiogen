namespace Audiogen.ViewModels
{
    /// <summary>
    /// Relative coordinates of a pointer device in its viewport.
    ///  Both coordinates are 0.0 to 1.0 relative to the size of the viewport.
    /// </summary>
    public sealed class PointerPosition
    {
        private readonly double _x, _y;

        public PointerPosition(double x, double y)
        {
            _x = x < 0.0 ? 0.0 : x > 1.0 ? 1.0 : x;
            _y = y < 0.0 ? 0.0 : y > 1.0 ? 1.0 : y;
        }

        public double X { get { return _x; } }

        public double Y { get { return _y; } }
    }
}
