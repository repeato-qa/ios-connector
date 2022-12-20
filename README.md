# iOS Connector for Repeato Studio and Repeato CLI
This library allows remote controlling iOS devices / simulators from an automation host. 
This connector library needs to be embedded into your app and takes care of transmitting and receiving data from [Repeato Studio](https://www.repeato.app) or [Repeato CLI](https://www.npmjs.com/package/@repeato/cli-testrunner).


# Installation
## 1. Make sure requirements are fullfilled

- Operating system: Mac OS
- Xcode installed
- Xcrun installed
- Simctl installed
- Source code of your app needs to be available on your machine. If you don't have access to the source code, ask the developer to integrate the Repeato plugin for you
- Optional (only for physical device control): [idb](https://fbidb.io/) installed

[Repeato Studio](https://www.repeato.app) provides a step by step tutorial and assists with setting things up. Just click "Troubles with connecting"...

![Repeato Studio](/docs/assets/repeato-studio-help1.png "Repeato Studio")

...and the setup wizard is started:
![Repeato Studio](/docs/assets/repeato-studio-help2.png "Repeato Studio setup wizard")

But for the sake of completenes we provide the installation instructions also here (in case you don't have Repeato Studio installed and you are integrating the plugin into the app for someone else). 

👉 Please make sure your app project is opened in Xcode before proceeding. 👈

## 2. Add library to "Swift packages" in Xcode

In the screenshot below you can see how we locate the swift packages in the app "MovieSwift" and then click the "+" button:

![Locate the swift package section](/docs/assets/connect-ios1-xcode13.png "Swift package section in Xcode")
        
Next, paste following URL to the input field: `https://github.com/repeato-qa/ios-connector`

![Paste URL into the search field](/docs/assets/connect-ios2-xcode13.png "")

Select branch "main" for "Dependency rule", then click "Add Package":

![Paste URL into the search field](/docs/assets/connect-ios3-xcode13.png "")

In the next step just click "Add Package" once more.

## 3. Build an launch app
That's it. Just build and run your project via Xcode on the simulator. Repeato connector will find your Repeato instance, and connect to it.


# Release cocoa pod

1. Validate local code via `pod lib lint`
2. Set version in Repeato.podspec
3. Sync protocolVersion with tag version
4. Commit, Tag version and push it
5. Validate repo via `pod spec lint Repeato.podspec` (Take care: This command is not validating local code it seems. It checks the version in the remote repo)
