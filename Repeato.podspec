Pod::Spec.new do |s|

  s.name         = "Repeato"
  s.version      = "1.1.9"
  s.summary      = "Mobile app test automation: Well-known problems and Repeato's solutions"

  s.description  = "The mobile app test automation tool for UI testing without coding
Create robust UI tests for iOS and Android apps. Automate tedious mobile app testing using physical devices. Simple, speedy, and even without coding skills."

  s.homepage     = "https://www.repeato.app"
  s.license      = "MIT"
  s.author             = "Repeato"
  s.social_media_url   = "https://twitter.com/repeato_testing"
  s.platform     = :ios, "12.0"
  s.source       = { :git => "https://github.com/repeato-qa/ios-connector.git", :tag => "#{s.version}" }
  s.source_files = "Sources/RepeatoCapture/include/**/*.{swift,h,m,xcdatamodeld,xib,storyboard,xcassets,json}"
  s.framework =  "UIKit"
  s.framework =  "Foundation"
end
