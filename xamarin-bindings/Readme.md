How to create Bindings Library.
1. Install the latest version of sharpie: https://docs.microsoft.com/en-us/xamarin/cross-platform/macios/binding/objective-sharpie/get-started#installing
2. Download Library and rename RepeatoImpl.m to RepeatoCapture.h
3. Make Fat library (.a) with next Make file
```
XBUILD=/Applications/Xcode.app/Contents/Developer/usr/bin/xcodebuild
PROJECT_ROOT=./RepeatoCapture
PROJECT=$(PROJECT_ROOT)/RepeatoCapture.xcodeproj
TARGET=RepeatoCapture


all: lib$(TARGET).a

lib$(TARGET)-x86_64.a:
	$(XBUILD) -project $(PROJECT) -target $(TARGET) -sdk iphonesimulator -configuration Release clean build
	-mv $(PROJECT_ROOT)/build/Release-iphonesimulator/lib$(TARGET).a $@

lib$(TARGET)-arm64.a:
	$(XBUILD) -project $(PROJECT) -target $(TARGET) -sdk iphoneos -arch arm64 -configuration Release clean build
	-mv $(PROJECT_ROOT)/build/Release-iphoneos/lib$(TARGET).a $@

lib$(TARGET).a: lib$(TARGET)-x86_64.a lib$(TARGET)-arm64.a
	xcrun -sdk iphoneos lipo -create -output $@ $^

clean:
	-rm -f *.a *.dll

```
4. Call sharpie command that would gererate ApiDefinition.cs anв Structs.cs
```
sharpie bind -output RepeatoCaptureLibrary -namespace  RepeatoCapture -sdk iphoneos16.2 RepeatoCapture/build/Release-iphoneos/include/RepeatoCapture/RepeatoCapture.h  -scope RepeatoCapture/build/Release-iphoneos/include/RepeatoCapture 
```

//iphoneos16.2 - change to sdk that XCode supports

5. Copy generated code into Xamarin Bindings Lib project
6. Include/Replace Fat Lib into project as well
7. Call Build command for the solution
8. Copy gererated lib from bin/iPhone and add as reference into Xamarin.iOS project
