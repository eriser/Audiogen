namespace Audiogen.Helpers
{
    using System;
    using Windows.UI.Xaml;
    using Windows.UI.Xaml.Data;

    sealed class BoleanToVisibilityConverter : IValueConverter
    {
        object IValueConverter.Convert(object value, Type targetType, object parameter, string language)
        {
            return Convert.ToBoolean(value) ? Visibility.Visible : Visibility.Collapsed;
        }

        object IValueConverter.ConvertBack(object value, Type targetType, object parameter, string language)
        {
            throw new NotImplementedException();
        }
    }
}
