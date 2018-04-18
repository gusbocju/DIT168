![build status on 'master'](https://travis-ci.org/gusbocju/DIT168.svg?branch=master)

# DIT168 V18 Project: Industrial IT and Embedded Systems

During the spring term of 2018 - and as part of DIT168 V18 at the University of Gothenburg, this repository will be used to design and implement software for a mini car to drive autonomously under stable conditions. The car should be able to, in addition to self-driving, follow a line, do safe handling of intersections, pass other vehicles and park. To accomplish this the car is equipped with sensors. The project will be done in C/C++ and built upon [OpenDaVINCI](http://opendavinci.cse.chalmers.se/www/) - with a high focus on quality assurance, effective design and testing throughout. 

## GIT LAYOUT

Please keep individual branches per feature in mind; they’ll be merged using pull requests that have to approved by at least one other contributor/ group member. There shall be no direct pushes to the master branch; prototypes will only evolve by pulling completed features into the release branch. In addition to the feature branches, each contributor creates a separate branch from the feature branch for which they push their code to.\
\
Following the original folder structure, please make sure to place all generated code and/ or supporting files within the right directory - which, in general means: 
Within the `src/` folder for anything code related; within additional sub-directories (keeping precise naming in mind) per feature (e.g. `src/networking/` for anything network related); and within supporting `test/` and `demo/` sub-directories for inherent demos and tests accordingly.

### CLONING
Cloning requires nothing more than a local Git setup and the following command:

`$ git clone https://github.com/gusbocju/DIT168.git`

## BUILD AND DEPLOY
In order to keep build and deploy processes as platform independent and straightforward as possible, we strongly encourage the use of [Docker](https://www.docker.com/community-edition) and the supplied [Dockerfile](Dockerfile). To build the project on your local machine, simply clone the repository (as described above) and run the following command from it's root directory:

`$ docker build -t dit168.marble -f Dockerfile .`

Successful builds originating *master* will be pushed to `gusbocju/dit168.marble:latest` (x86_64) and `gusbocju/dit168.marble:latest-armhf` (for ARM based platforms) on Docker Hub using Travis CI.\
\
Don't forget to add new CMake configurations and (if necessary) extensions to the existing (respectively a new) Dockerfile to include new features/ modules. Refer to [https://docs.docker.com](https://docs.docker.com) to resolve any uncertainties.

### PULL, SAVE AND RESTORE
Thanks to Travis CI, the latest Docker image (originating *master*) can be fetched directly from Docker Hub - no manual building required: 

`$ docker pull gusbocju/dit168.marble:latest` <sub><sup>(Alpine 3.7 x86_64, including dit168-signal-viewer)</sup></sub>\
`$ docker pull gusbocju/dit168.marble:latest-armhf` <sub><sup>(Alpine 3.7 ARM, including dit168-signal-viewer)</sup></sub>\
`$ docker pull gusbocju/dit168.marble:latest-ubuntu` <sub><sup>(Ubuntu 16.04 x86_64, recommended for MARBLE.DS4)</sup></sub>\
`$ docker pull gusbocju/dit168.marble:latest-ubuntu-armhf` <sub><sup>(Ubuntu 16.04 ARM, recommended for MARBLE.DS4)</sup></sub>

Use the following snippets to save and/ or restore a local Docker image whenever needed:

`$ docker save dit168.marble > dit168.marble.tar`\
`$ cat dit168.marble.tar | docker load`

### RUN
Running different modules/ features from any of our Docker images can be done using the `$ docker run`, the following is just an excerpt of currently available executables and standalone features: 

##### MODULES AND FEATURES
`$ docker run --rm -ti --net=host dit168.marble /opt/MARBLE.DS4`\
`$ docker run --rm -ti --net=host dit168.marble /opt/MARBLE.V2V`

##### DEMOS
`$ docker run --rm -ti --net=host dit168.marble /opt/MARBLE.DS4.Demo`\
`$ docker run --rm -ti --net=host dit168.marble /opt/MARBLE.SafetyCheck.Main`\
`$ docker run --rm -ti --net=host dit168.marble /opt/MARBLE.SafetyCheck.RemoteControl`\
`$ docker run --rm -ti --net=host dit168.marble /opt/MARBLE.Networking.Demo.Sender`\
`$ docker run --rm -ti --net=host dit168.marble /opt/MARBLE.Networking.Demo.Receiver`

\
The above processes can be terminated using `$ docker ps` and `$ docker stop <container id>`.

## CONTRIBUTING
Please read the [CONTRIBUTING.md](CONTRIBUTING.md) on how to contribute to this project.

## LICENSE
This project is licensed under GPL 3.0, see [LICENSE.md](LICENSE.md) for more details.





