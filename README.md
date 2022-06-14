# Jester

A core component of _Project Dawson_, Jester is an Unreal Engine 4 plugin that integrates Kinect for Windows v2 functionality via the official SDK. It comprises two parts: A third-party module, KinectSDK, providing the Microsoft Kinect for Windows Software Development Kit v2.0-1409; and JesterDK, which exposes Kinect functionality to the Blueprint developer.

I developed Jester DK during my time at the University of Lincoln, whilst researching wheelchair-based interaction paradigms for virtual reality experiences, and it's now ready for prime-time. I hope you can derive some value from this work.

With the deprecation of Opaque Media's Kinect4Unreal toolkit, I have endeavoured to make Jester as compatible as possible for those who may wish to transition from Kinect4Unreal.

## Unreal Engine Support

Jester is built for Unreal Engine 4.27.x. Other versions may work but have not been tested and are not supported. When new versions become available, this section will be updated to record compatibility.

## Developing Jester DK

This repository contains a complete Unreal Engine project. You can simply open `Jester.uproject` and generate the project files to get started. Per the [Open Source License 3.0](LICENSE), I ask that you submit any improvements you make to me so that I can incorporate them into this repository.

## Using This Plugin in Your Own Projects

The simplest option is to copy the contents of the `Plugins` directory into your own project structure.

# Licensing

Use of the Microsoft Kinect for Windows Software Development Kit is governed by the Microsoft Kinect for Windows Software Development Kit (SDK) 2.0 End User License Agreement, a copy of which is included and can be found in the `KinectSDK` directory. Use of Jester is governed by the Open Source License 3.0. Summarily, you may translate, adapt, alter, transform, modify or arrange Jester in any way you wish, and may distribute copies of Jester and your derivative works, on the proviso that any such work is licensed under the Open Source License 3.0.
