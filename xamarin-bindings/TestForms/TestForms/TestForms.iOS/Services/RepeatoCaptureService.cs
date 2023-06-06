using System;
using TestForms.Services;
using RepeatoCapture;
using Xamarin.Forms;
using TestForms.iOS.Services;

namespace TestForms.iOS.Services
{
	public class RepeatoCaptureService : IRepeatoCaptureService
    {
		public void Load()
		{
			RepeatoCapture.RepeatoCapture.Load();
		}
	}
}

