using Foundation;
using System;
using UIKit;
using RepeatoCapture;
using CoreGraphics;

namespace Test
{
    public partial class ViewController : UIViewController
    {
        private UILabel testLabel;

        public ViewController(IntPtr handle) : base(handle)
        {

        }

        public override void ViewDidLoad()
        {
            base.ViewDidLoad();

            // Create a test label
            testLabel = new UILabel();
            testLabel.TranslatesAutoresizingMaskIntoConstraints = false;
            testLabel.Text = "Test Label";
            testLabel.TextColor = UIColor.Black;
            testLabel.TextAlignment = UITextAlignment.Center;
            View.AddSubview(testLabel);

            testLabel.CenterXAnchor.ConstraintEqualTo(View.CenterXAnchor).Active = true;
            testLabel.CenterYAnchor.ConstraintEqualTo(View.CenterYAnchor).Active = true;

            RepeatoCapture.RepeatoCapture.Load();
        }

        public override void DidReceiveMemoryWarning()
        {
            base.DidReceiveMemoryWarning();
        }
    }


}
