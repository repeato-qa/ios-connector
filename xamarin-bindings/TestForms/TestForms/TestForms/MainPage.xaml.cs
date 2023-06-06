using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TestForms.Services;
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;

namespace TestForms
{
    public partial class MainPage : ContentPage
    {
        public MainPage()
        {
            InitializeComponent();
            On<iOS>().SetUseSafeArea(true);
        }

        protected override void OnAppearing()
        {
            base.OnAppearing();

            DependencyService.Get<IRepeatoCaptureService>().Load();
        }

        private async void Button_Clicked(System.Object sender, System.EventArgs e)
        {
            TestLabel.Text = "Test button clicked";
        }

        void OnColorsRadioButtonCheckedChanged(System.Object sender, Xamarin.Forms.CheckedChangedEventArgs e)
        {
            if( sender is RadioButton radioButton)
            {
                RedioButtonResultLabel.Text = $"you selected {radioButton.Content}";
            }
        }
    }
}

