
# A C++ Dependency Injection framework

This repository contains:
- Source code: **src** and **inc** folders
- Small example: **example** folder

After build the project ([see: Building the library and running the example](#Building-the-library-and-running-the-example)) there will be two more folders:
- compiled library : **lib** folder
- compiled example : **bin** folder

Limitations:
- It can only instantiate transient objects
- Registered services cannot have more than 1 dependency
- Registered services cannot have any input parameter in the constructor

## Usage:

1. Service interface classes must derive from `ServiceBase`.

2. To register services:

    If the service has no dependencies:

        IocContainer::GetSingleton().RegisterService<ServiceInterface, ServiceImplementation>();

    If it has one dependency:

        IocContainer::GetSingleton().RegisterService<ServiceInterface, ServiceImplementation, ServiceDependency>();

3. Instantiate objects:

        IocContainer::GetSingleton().GetService<ServiceInterface>();

4. To remove services:

        IocContainer::GetSingleton().RemoveService<ServiceInterface>();

## Example: VideoPlayer

In this example, an hypothetical VideoPlayer uses 2 services to accomplish the task of presenting frames in the screen: a Decoder to decode frames, and a Logger to record all ongoing actions. VideoPlayer depends on the Decoder, and the Decoder depends on the Logger.

The chain of dependencies is: *VideoPlayer->Decoder->Logger*

In the first part of main function:
- These 3 services are registered in the IocContainer.
- VideoPlayer and it's dependencies are instantiated and VideoPlayer::PresentFrame() is successfully called.

In the second part, cyclic dependency case is presented:
- A similar environment than before, but a new Logger implementation has a dependency on the VideoPlayer, making the circular dependency *VideoPlayer->Decoder->Logger->VideoPlayer*
- An exception is raised when trying to instantiate the VideoPlayer object

*(this example don't include any real implementation apart of the Dependency Injection Container)*

## Building the library and running the example
Clone the repo

    git clone -b ioc_container https://github.com/SCVready/cpp_service_locator ioc_container;
    cd ioc_container;

### Build/running locally (Linux/Ubuntu20.04):

Install dependencies:

    sudo apt install -y build-essential libgtest-dev libgmock-dev cmake;

Run and build:

    ./build.sh;

### Build/running in Docker (Linux):

    docker run -it -v $(pwd):/service_locator --workdir /service_locator --user 1000:1000 scvready/ubuntu-gtest:latest ./build.sh;